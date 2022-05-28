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
    public class WallsTool : ViewTool
    {
        private readonly TypeComboBox wallTypeComboBox;
        private readonly MapPanel wallTypeMapPanel;

        private readonly Dictionary<int, Overlay> undoOverlays = new Dictionary<int, Overlay>();
        private readonly Dictionary<int, Overlay> redoOverlays = new Dictionary<int, Overlay>();

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private OverlayType selectedWallType;
        private OverlayType SelectedWallType
        {
            get => selectedWallType;
            set
            {
                if (selectedWallType != value)
                {
                    if (placementMode && (selectedWallType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    selectedWallType = value;
                    wallTypeComboBox.SelectedValue = selectedWallType;

                    RefreshMapPanel();
                }
            }
        }

        public WallsTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox wallTypeComboBox, MapPanel wallTypeMapPanel, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += WallTool_KeyDown;
            (this.mapPanel as Control).KeyUp += WallTool_KeyUp;

            this.wallTypeComboBox = wallTypeComboBox;
            this.wallTypeComboBox.SelectedIndexChanged += WallTypeComboBox_SelectedIndexChanged;

            this.wallTypeMapPanel = wallTypeMapPanel;
            this.wallTypeMapPanel.BackColor = Color.White;
            this.wallTypeMapPanel.MaxZoom = 1;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedWallType = this.wallTypeComboBox.Types.First() as OverlayType;

            UpdateStatus();
        }

        private void WallTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedWallType = wallTypeComboBox.SelectedValue as OverlayType;
        }

        private void WallTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void WallTool_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                ExitPlacementMode();
            }
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (e.Button == MouseButtons.Left)
                {
                    AddWall(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveWall(navigationWidget.MouseCell);
                }
            }
            else if ((e.Button == MouseButtons.Left) || (e.Button == MouseButtons.Right))
            {
                PickWall(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if ((undoOverlays.Count > 0) || (redoOverlays.Count > 0))
            {
                CommitChange();
            }
        }

        private void MapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (!placementMode && (Control.ModifierKeys == Keys.Shift))
            {
                EnterPlacementMode();
            }
            else if (placementMode && (Control.ModifierKeys == Keys.None))
            {
                ExitPlacementMode();
            }
        }

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (Control.MouseButtons == MouseButtons.Left)
                {
                    AddWall(e.NewCell);
                }
                else if (Control.MouseButtons == MouseButtons.Right)
                {
                    RemoveWall(e.NewCell);
                }

                if (SelectedWallType != null)
                {
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.OldCell, new Size(1, 1)), 1, 1));
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.NewCell, new Size(1, 1)), 1, 1));
                }
            }
        }

        private void AddWall(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (SelectedWallType != null)
                {
                    var overlay = new Overlay { Type = SelectedWallType, Icon = 0 };
                    if (map.Technos.CanAdd(cell, overlay) && map.Buildings.CanAdd(cell, overlay))
                    {
                        if (!undoOverlays.ContainsKey(cell))
                        {
                            undoOverlays[cell] = map.Overlay[cell];
                        }

                        map.Overlay[cell] = overlay;
                        redoOverlays[cell] = overlay;

                        mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(location, new Size(1, 1)), 1, 1));

                        plugin.Dirty = true;
                    }
                }
            }
        }

        private void RemoveWall(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var overlay = map.Overlay[cell];
                if (overlay?.Type.IsWall ?? false)
                {
                    if (!undoOverlays.ContainsKey(cell))
                    {
                        undoOverlays[cell] = map.Overlay[cell];
                    }

                    map.Overlay[cell] = null;
                    redoOverlays[cell] = null;

                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(location, new Size(1, 1)), 1, 1));

                    plugin.Dirty = true;
                }
            }
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
                    return Rectangle.Inflate(new Rectangle(location, new Size(1, 1)), 1, 1);
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
                    return Rectangle.Inflate(new Rectangle(location, new Size(1, 1)), 1, 1);
                }));
            }

            undoOverlays.Clear();
            redoOverlays.Clear();

            url.Track(undoAction, redoAction);
        }

        private void EnterPlacementMode()
        {
            if (placementMode)
            {
                return;
            }

            placementMode = true;

            navigationWidget.MouseoverSize = Size.Empty;

            if (SelectedWallType != null)
            {
                mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
            }

            UpdateStatus();
        }

        private void ExitPlacementMode()
        {
            if (!placementMode)
            {
                return;
            }

            placementMode = false;

            navigationWidget.MouseoverSize = new Size(1, 1);

            if (SelectedWallType != null)
            {
                mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
            }

            UpdateStatus();
        }

        private void PickWall(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var overlay = map.Overlay[cell];
                if ((overlay != null) && overlay.Type.IsWall)
                {
                    SelectedWallType = overlay.Type;
                }
            }
        }

        private void RefreshMapPanel()
        {
            wallTypeMapPanel.MapImage = SelectedWallType?.Thumbnail;
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click drag to add walls, Right-Click drag to remove walls";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click or Right-Click to pick wall";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedWallType != null)
                {
                    if (previewMap.Metrics.GetCell(location, out int cell))
                    {
                        var overlay = new Overlay { Type = SelectedWallType, Icon = 0, Tint = Color.FromArgb(128, Color.White) };
                        if (previewMap.Technos.CanAdd(cell, overlay) && previewMap.Buildings.CanAdd(cell, overlay))
                        {
                            previewMap.Overlay[cell] = overlay;
                            mapPanel.Invalidate(previewMap, Rectangle.Inflate(new Rectangle(location, new Size(1, 1)), 1, 1));
                        }
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var wallPen = new Pen(Color.Green, 4.0f);
            foreach (var (cell, overlay) in previewMap.Overlay)
            {
                if (overlay.Type.IsWall)
                {
                    previewMap.Metrics.GetLocation(cell, out Point topLeft);
                    var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                    graphics.DrawRectangle(wallPen, bounds);
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
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= WallTool_KeyDown;
                    (mapPanel as Control).KeyUp -= WallTool_KeyUp;

                    wallTypeComboBox.SelectedIndexChanged -= WallTypeComboBox_SelectedIndexChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
