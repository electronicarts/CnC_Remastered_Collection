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
using MobiusEditor.Utility;
using MobiusEditor.Widgets;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public class ResourcesTool : ViewTool
    {
        private readonly Label totalResourcesLbl;
        private readonly NumericUpDown brushSizeNud;
        private readonly CheckBox gemsCheckBox;

        private bool placementMode;
        private bool additivePlacement;

        private readonly Dictionary<int, Overlay> undoOverlays = new Dictionary<int, Overlay>();
        private readonly Dictionary<int, Overlay> redoOverlays = new Dictionary<int, Overlay>();

        public ResourcesTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, Label totalResourcesLbl, NumericUpDown brushSizeNud, CheckBox gemsCheckBox, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            (this.mapPanel as Control).KeyDown += ResourceTool_KeyDown;

            this.totalResourcesLbl = totalResourcesLbl;
            this.brushSizeNud = brushSizeNud;
            this.gemsCheckBox = gemsCheckBox;

            this.brushSizeNud.ValueChanged += BrushSizeNud_ValueChanged;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;
            navigationWidget.MouseoverSize = new Size((int)brushSizeNud.Value, (int)brushSizeNud.Value);

            url.Undone += Url_UndoRedo;
            url.Redone += Url_UndoRedo;

            Update();

            UpdateStatus();
        }

        private void Url_UndoRedo(object sender, EventArgs e)
        {
            Update();
        }

        private void BrushSizeNud_ValueChanged(object sender, EventArgs e)
        {
            navigationWidget.MouseoverSize = new Size((int)brushSizeNud.Value, (int)brushSizeNud.Value);
        }

        private void ResourceTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.OemOpenBrackets)
            {
                brushSizeNud.DownButton();
                mapPanel.Invalidate();
            }
            else if (e.KeyCode == Keys.OemCloseBrackets)
            {
                brushSizeNud.UpButton();
                mapPanel.Invalidate();
            }
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (!placementMode)
                {
                    EnterPlacementMode(true);
                    AddResource(navigationWidget.MouseCell);
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (!placementMode)
                {
                    EnterPlacementMode(false);
                    RemoveResource(navigationWidget.MouseCell);
                }
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (((e.Button == MouseButtons.Left) && additivePlacement) ||
                    ((e.Button == MouseButtons.Right) && !additivePlacement))
                {
                    ExitPlacementMode();
                }
            }

            if ((undoOverlays.Count > 0) || (redoOverlays.Count > 0))
            {
                CommitChange();
            }
        }

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (additivePlacement)
                {
                    AddResource(e.NewCell);
                }
                else
                {
                    RemoveResource(e.NewCell);
                }
            }

            if (brushSizeNud.Value > 1)
            {
                foreach (var cell in new Point[] { e.OldCell, e.NewCell })
                {
                    mapPanel.Invalidate(mapPanel.MapToClient(new Rectangle(
                        new Point(cell.X - ((int)brushSizeNud.Value / 2), cell.Y - ((int)brushSizeNud.Value / 2)),
                        new Size((int)brushSizeNud.Value, (int)brushSizeNud.Value)
                    )));
                }
            }
        }

        private void AddResource(Point location)
        {
            Rectangle rectangle = new Rectangle(location, new Size(1, 1));
            rectangle.Inflate(navigationWidget.MouseoverSize.Width / 2, navigationWidget.MouseoverSize.Height / 2);
            foreach (var subLocation in rectangle.Points())
            {
                if ((subLocation.Y == 0) || (subLocation.Y == (map.Metrics.Height - 1)))
                {
                    continue;
                }

                if (map.Metrics.GetCell(subLocation, out int cell))
                {
                    if (map.Overlay[cell] == null)
                    {
                        var resourceType = gemsCheckBox.Checked ?
                            map.OverlayTypes.Where(t => t.IsGem).FirstOrDefault() :
                            map.OverlayTypes.Where(t => t.IsTiberiumOrGold).FirstOrDefault();
                        if (resourceType != null)
                        {
                            if (!undoOverlays.ContainsKey(cell))
                            {
                                undoOverlays[cell] = map.Overlay[cell];
                            }

                            var overlay = new Overlay { Type = resourceType, Icon = 0 };
                            map.Overlay[cell] = overlay;
                            redoOverlays[cell] = overlay;

                            plugin.Dirty = true;
                        }
                    }
                }
            }

            rectangle.Inflate(1, 1);
            mapPanel.Invalidate(map, rectangle);

            Update();
        }

        private void RemoveResource(Point location)
        {
            Rectangle rectangle = new Rectangle(location, new Size(1, 1));
            rectangle.Inflate(navigationWidget.MouseoverSize.Width / 2, navigationWidget.MouseoverSize.Height / 2);
            foreach (var subLocation in rectangle.Points())
            {
                if (map.Metrics.GetCell(subLocation, out int cell))
                {
                    if (map.Overlay[cell]?.Type.IsResource ?? false)
                    {
                        if (!undoOverlays.ContainsKey(cell))
                        {
                            undoOverlays[cell] = map.Overlay[cell];
                        }

                        map.Overlay[cell] = null;
                        redoOverlays[cell] = null;

                        plugin.Dirty = true;
                    }
                }
            }

            rectangle.Inflate(1, 1);
            mapPanel.Invalidate(map, rectangle);

            Update();
        }

        private void EnterPlacementMode(bool additive)
        {
            if (placementMode)
            {
                return;
            }

            placementMode = true;
            additivePlacement = additive;

            UpdateStatus();
        }

        private void ExitPlacementMode()
        {
            if (!placementMode)
            {
                return;
            }

            placementMode = false;

            UpdateStatus();
        }

        private void CommitChange()
        {
            var undoOverlays2 = new Dictionary<int, Overlay>(undoOverlays);
            void undoAction(UndoRedoEventArgs e)
            {
                foreach (var kv in undoOverlays2)
                {
                    e.Map.Overlay[kv.Key] = kv.Value;
                }
                e.MapPanel.Invalidate(e.Map, undoOverlays2.Keys.Select(k =>
                {
                    e.Map.Metrics.GetLocation(k, out Point location);
                    var rectangle = new Rectangle(location, new Size(1, 1));
                    rectangle.Inflate(1, 1);
                    return rectangle;
                }));
            }

            var redoOverlays2 = new Dictionary<int, Overlay>(redoOverlays);
            void redoAction(UndoRedoEventArgs e)
            {
                foreach (var kv in redoOverlays2)
                {
                    e.Map.Overlay[kv.Key] = kv.Value;
                }
                e.MapPanel.Invalidate(e.Map, redoOverlays2.Keys.Select(k =>
                {
                    e.Map.Metrics.GetLocation(k, out Point location);
                    var rectangle = new Rectangle(location, new Size(1, 1));
                    rectangle.Inflate(1, 1);
                    return rectangle;
                }));
            }

            undoOverlays.Clear();
            redoOverlays.Clear();

            url.Track(undoAction, redoAction);
        }

        private void Update()
        {
            totalResourcesLbl.Text = map.TotalResources.ToString();

            if (map.OverlayTypes.Any(t => t.IsGem))
            {
                gemsCheckBox.Visible = true;
            }
            else
            {
                gemsCheckBox.Visible = false;
                gemsCheckBox.Checked = false;
            }
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                if (additivePlacement)
                {
                    statusLbl.Text = "Drag mouse to add resources";
                }
                else
                {
                    statusLbl.Text = "Drag mouse to remove resources";
                }
            }
            else
            {
                statusLbl.Text = "Left-Click drag to add resources, Right-Click drag to remove resources";
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var resourcePen = new Pen(Color.Green, 4.0f);
            foreach (var (cell, overlay) in map.Overlay)
            {
                if (overlay.Type.IsResource)
                {
                    map.Metrics.GetLocation(cell, out Point topLeft);
                    var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                    graphics.DrawRectangle(resourcePen, bounds);
                }
            }
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected override void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseUp -= MapPanel_MouseUp;
                    (mapPanel as Control).KeyDown -= ResourceTool_KeyDown;

                    brushSizeNud.ValueChanged -= BrushSizeNud_ValueChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;

                    url.Undone -= Url_UndoRedo;
                    url.Redone -= Url_UndoRedo;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
