//
// Copyright 2020 Electronic Arts Inc.
//
// The Command & Conquer Map Editor and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// The Command & Conquer Map Editor and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection
using MobiusEditor.Controls;
using MobiusEditor.Event;
using MobiusEditor.Interface;
using MobiusEditor.Model;
using MobiusEditor.Render;
using MobiusEditor.Utility;
using MobiusEditor.Widgets;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public abstract class ViewTool : ITool
    {
        protected readonly IGamePlugin plugin;
        protected readonly Map map;

        protected readonly MapPanel mapPanel;
        protected readonly ToolStripStatusLabel statusLbl;
        protected readonly UndoRedoList<UndoRedoEventArgs> url;
        protected readonly NavigationWidget navigationWidget;

        protected virtual Map RenderMap => map;

        private MapLayerFlag layers;
        public MapLayerFlag Layers
        {
            get => layers;
            set
            {
                if (layers != value)
                {
                    layers = value;
                    Invalidate();
                }
            }
        }

        public ViewTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
        {
            this.layers = layers;
            this.plugin = plugin;
            this.url = url;

            this.mapPanel = mapPanel;
            this.mapPanel.PreRender += MapPanel_PreRender;
            this.mapPanel.PostRender += MapPanel_PostRender;

            this.statusLbl = statusLbl;

            map = plugin.Map;
            map.BasicSection.PropertyChanged += BasicSection_PropertyChanged;

            navigationWidget = new NavigationWidget(mapPanel, map.Metrics, Globals.TileSize);
        }

        protected void Invalidate()
        {
            mapPanel.Invalidate(RenderMap);
        }

        private void BasicSection_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "BasePlayer":
                    {
                        foreach (var baseBuilding in map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0))
                        {
                            mapPanel.Invalidate(map, baseBuilding);
                        }
                    }
                    break;
            }
        }

        private void MapPanel_PreRender(object sender, RenderEventArgs e)
        {
            if ((e.Cells != null) && (e.Cells.Count == 0))
            {
                return;
            }

            PreRenderMap();

            using (var g = Graphics.FromImage(mapPanel.MapImage))
            {
                if (Properties.Settings.Default.Quality > 1)
                {
                    g.InterpolationMode = InterpolationMode.NearestNeighbor;
                    g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
                }

                MapRenderer.Render(plugin.GameType, RenderMap, g, e.Cells?.Where(p => map.Metrics.Contains(p)).ToHashSet(), Layers);
            }
        }

        private void MapPanel_PostRender(object sender, RenderEventArgs e)
        {
            PostRenderMap(e.Graphics);
            navigationWidget.Render(e.Graphics);
        }

        protected virtual void PreRenderMap() { }

        protected virtual void PostRenderMap(Graphics graphics)
        {
            if ((Layers & MapLayerFlag.Waypoints) != MapLayerFlag.None)
            {
                var waypointBackgroundBrush = new SolidBrush(Color.FromArgb(96, Color.Black));
                var waypointBrush = new SolidBrush(Color.FromArgb(128, Color.DarkOrange));
                var waypointPen = new Pen(Color.DarkOrange);

                foreach (var waypoint in map.Waypoints)
                {
                    if (waypoint.Cell.HasValue)
                    {
                        var x = waypoint.Cell.Value % map.Metrics.Width;
                        var y = waypoint.Cell.Value / map.Metrics.Width;

                        var location = new Point(x * Globals.TileWidth, y * Globals.TileHeight);
                        var textBounds = new Rectangle(location, Globals.TileSize);

                        graphics.FillRectangle(waypointBackgroundBrush, textBounds);
                        graphics.DrawRectangle(waypointPen, textBounds);

                        StringFormat stringFormat = new StringFormat
                        {
                            Alignment = StringAlignment.Center,
                            LineAlignment = StringAlignment.Center
                        };

                        var text = waypoint.Name.ToString();
                        var font = graphics.GetAdjustedFont(text, SystemFonts.DefaultFont, textBounds.Width, 24 / Globals.TileScale, 48 / Globals.TileScale, true);
                        graphics.DrawString(text.ToString(), font, waypointBrush, textBounds, stringFormat);
                    }
                }
            }

            if ((Layers & MapLayerFlag.TechnoTriggers) != MapLayerFlag.None)
            {
                var technoTriggerBackgroundBrush = new SolidBrush(Color.FromArgb(96, Color.Black));
                var technoTriggerBrush = new SolidBrush(Color.LimeGreen);
                var technoTriggerPen = new Pen(Color.LimeGreen);

                foreach (var (cell, techno) in map.Technos)
                {
                    var location = new Point(cell.X * Globals.TileWidth, cell.Y * Globals.TileHeight);

                    (string trigger, Rectangle bounds)[] triggers = null;
                    if (techno is Terrain terrain)
                    {
                        triggers = new (string, Rectangle)[] { (terrain.Trigger, new Rectangle(location, terrain.Type.RenderSize)) };
                    }
                    else if (techno is Building building)
                    {
                        var size = new Size(building.Type.Size.Width * Globals.TileWidth, building.Type.Size.Height * Globals.TileHeight);
                        triggers = new (string, Rectangle)[] { (building.Trigger, new Rectangle(location, size)) };
                    }
                    else if (techno is Unit unit)
                    {
                        triggers = new (string, Rectangle)[] { (unit.Trigger, new Rectangle(location, Globals.TileSize)) };
                    }
                    else if (techno is InfantryGroup infantryGroup)
                    {
                        List<(string, Rectangle)> infantryTriggers = new List<(string, Rectangle)>();
                        for (var i = 0; i < infantryGroup.Infantry.Length; ++i)
                        {
                            var infantry = infantryGroup.Infantry[i];
                            if (infantry == null)
                            {
                                continue;
                            }

                            var size = Globals.TileSize;
                            var offset = Size.Empty;
                            switch ((InfantryStoppingType)i)
                            {
                                case InfantryStoppingType.UpperLeft:
                                    offset.Width = -size.Width / 4;
                                    offset.Height = -size.Height / 4;
                                    break;
                                case InfantryStoppingType.UpperRight:
                                    offset.Width = size.Width / 4;
                                    offset.Height = -size.Height / 4;
                                    break;
                                case InfantryStoppingType.LowerLeft:
                                    offset.Width = -size.Width / 4;
                                    offset.Height = size.Height / 4;
                                    break;
                                case InfantryStoppingType.LowerRight:
                                    offset.Width = size.Width / 4;
                                    offset.Height = size.Height / 4;
                                    break;
                            }

                            var bounds = new Rectangle(location + offset, size);
                            infantryTriggers.Add((infantry.Trigger, bounds));
                        }

                        triggers = infantryTriggers.ToArray();
                    }

                    if (triggers != null)
                    {
                        StringFormat stringFormat = new StringFormat
                        {
                            Alignment = StringAlignment.Center,
                            LineAlignment = StringAlignment.Center
                        };

                        foreach (var (trigger, bounds) in triggers.Where(x => !x.trigger.Equals("None", StringComparison.OrdinalIgnoreCase)))
                        {
                            var font = graphics.GetAdjustedFont(trigger, SystemFonts.DefaultFont, bounds.Width, 12 / Globals.TileScale, 24 / Globals.TileScale, true);
                            var textBounds = graphics.MeasureString(trigger, font, bounds.Width, stringFormat);

                            var backgroundBounds = new RectangleF(bounds.Location, textBounds);
                            backgroundBounds.Offset((bounds.Width - textBounds.Width) / 2.0f, (bounds.Height - textBounds.Height) / 2.0f);
                            graphics.FillRectangle(technoTriggerBackgroundBrush, backgroundBounds);
                            graphics.DrawRectangle(technoTriggerPen, Rectangle.Round(backgroundBounds));

                            graphics.DrawString(trigger, font, technoTriggerBrush, bounds, stringFormat);
                        }
                    }
                }
            }

            if ((Layers & MapLayerFlag.CellTriggers) != MapLayerFlag.None)
            {
                var cellTriggersBackgroundBrush = new SolidBrush(Color.FromArgb(96, Color.Black));
                var cellTriggersBrush = new SolidBrush(Color.FromArgb(128, Color.White));
                var cellTriggerPen = new Pen(Color.White);

                foreach (var (cell, cellTrigger) in map.CellTriggers)
                {
                    var x = cell % map.Metrics.Width;
                    var y = cell / map.Metrics.Width;

                    var location = new Point(x * Globals.TileWidth, y * Globals.TileHeight);
                    var textBounds = new Rectangle(location, Globals.TileSize);

                    graphics.FillRectangle(cellTriggersBackgroundBrush, textBounds);
                    graphics.DrawRectangle(cellTriggerPen, textBounds);

                    StringFormat stringFormat = new StringFormat
                    {
                        Alignment = StringAlignment.Center,
                        LineAlignment = StringAlignment.Center
                    };

                    var text = cellTrigger.Trigger;
                    var font = graphics.GetAdjustedFont(text, SystemFonts.DefaultFont, textBounds.Width, 24 / Globals.TileScale, 48 / Globals.TileScale, true);
                    graphics.DrawString(text.ToString(), font, cellTriggersBrush, textBounds, stringFormat);
                }
            }

            if ((Layers & MapLayerFlag.Boundaries) != MapLayerFlag.None)
            {
                var boundsPen = new Pen(Color.Cyan, 8.0f);
                var bounds = Rectangle.FromLTRB(
                    map.Bounds.Left * Globals.TileWidth,
                    map.Bounds.Top * Globals.TileHeight,
                    map.Bounds.Right * Globals.TileWidth,
                    map.Bounds.Bottom * Globals.TileHeight
                );
                graphics.DrawRectangle(boundsPen, bounds);
            }
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    navigationWidget.Dispose();

                    mapPanel.PreRender -= MapPanel_PreRender;
                    mapPanel.PostRender -= MapPanel_PostRender;

                    map.BasicSection.PropertyChanged -= BasicSection_PropertyChanged;
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}
