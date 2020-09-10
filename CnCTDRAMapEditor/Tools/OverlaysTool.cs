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
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public class OverlaysTool : ViewTool
    {
        private readonly TypeComboBox overlayTypeComboBox;
        private readonly MapPanel overlayTypeMapPanel;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private OverlayType selectedOverlayType;
        private OverlayType SelectedOverlayType
        {
            get => selectedOverlayType;
            set
            {
                if (selectedOverlayType != value)
                {
                    if (placementMode && (selectedOverlayType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    selectedOverlayType = value;
                    overlayTypeComboBox.SelectedValue = selectedOverlayType;

                    RefreshMapPanel();
                }
            }
        }

        public OverlaysTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox overlayTypeComboBox, MapPanel overlayTypeMapPanel, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += OverlaysTool_KeyDown;
            (this.mapPanel as Control).KeyUp += OverlaysTool_KeyUp;

            this.overlayTypeComboBox = overlayTypeComboBox;
            this.overlayTypeComboBox.SelectedIndexChanged += OverlayTypeComboBox_SelectedIndexChanged;

            this.overlayTypeMapPanel = overlayTypeMapPanel;
            this.overlayTypeMapPanel.BackColor = Color.White;
            this.overlayTypeMapPanel.MaxZoom = 1;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedOverlayType = this.overlayTypeComboBox.Types.First() as OverlayType;

            UpdateStatus();
        }

        private void OverlayTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedOverlayType = overlayTypeComboBox.SelectedValue as OverlayType;
        }

        private void OverlaysTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void OverlaysTool_KeyUp(object sender, KeyEventArgs e)
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
                    AddOverlay(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveOverlay(navigationWidget.MouseCell);
                }
            }
            else if ((e.Button == MouseButtons.Left) || (e.Button == MouseButtons.Right))
            {
                PickOverlay(navigationWidget.MouseCell);
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
                if (SelectedOverlayType != null)
                {
                    mapPanel.Invalidate(map, new Rectangle(e.OldCell, new Size(1, 1)));
                    mapPanel.Invalidate(map, new Rectangle(e.NewCell, new Size(1, 1)));
                }
            }
        }

        private void AddOverlay(Point location)
        {
            if ((location.Y == 0) || (location.Y == (map.Metrics.Height - 1)))
            {
                return;
            }

            if (map.Overlay[location] == null)
            {
                if (SelectedOverlayType != null)
                {
                    var overlay = new Overlay
                    {
                        Type = SelectedOverlayType,
                        Icon = 0
                    };
                    map.Overlay[location] = overlay;
                    mapPanel.Invalidate(map, location);

                    void undoAction(UndoRedoEventArgs e)
                    {
                        e.MapPanel.Invalidate(e.Map, location);
                        e.Map.Overlay[location] = null;
                    }

                    void redoAction(UndoRedoEventArgs e)
                    {
                        e.Map.Overlay[location] = overlay;
                        e.MapPanel.Invalidate(e.Map, location);
                    }

                    url.Track(undoAction, redoAction);

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveOverlay(Point location)
        {
            if ((map.Overlay[location] is Overlay overlay) && overlay.Type.IsPlaceable)
            {
                map.Overlay[location] = null;
                mapPanel.Invalidate(map, location);

                void undoAction(UndoRedoEventArgs e)
                {
                    e.Map.Overlay[location] = overlay;
                    e.MapPanel.Invalidate(e.Map, location);
                }

                void redoAction(UndoRedoEventArgs e)
                {
                    e.MapPanel.Invalidate(e.Map, location);
                    e.Map.Overlay[location] = null;
                }

                url.Track(undoAction, redoAction);

                plugin.Dirty = true;
            }
        }

        private void EnterPlacementMode()
        {
            if (placementMode)
            {
                return;
            }

            placementMode = true;

            navigationWidget.MouseoverSize = Size.Empty;

            if (SelectedOverlayType != null)
            {
                mapPanel.Invalidate(map, navigationWidget.MouseCell);
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

            if (SelectedOverlayType != null)
            {
                mapPanel.Invalidate(map, navigationWidget.MouseCell);
            }

            UpdateStatus();
        }

        private void PickOverlay(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var overlay = map.Overlay[cell];
                if ((overlay != null) && !overlay.Type.IsWall)
                {
                    SelectedOverlayType = overlay.Type;
                }
            }
        }

        private void RefreshMapPanel()
        {
            overlayTypeMapPanel.MapImage = SelectedOverlayType?.Thumbnail;
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place overlay, Right-Click to remove overlay";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click or Right-Click to pick overlay";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedOverlayType != null)
                {
                    if (previewMap.Metrics.GetCell(location, out int cell))
                    {
                        if (previewMap.Overlay[cell] == null)
                        {
                            previewMap.Overlay[cell] = new Overlay
                            {
                                Type = SelectedOverlayType,
                                Icon = 0,
                                Tint = Color.FromArgb(128, Color.White)
                            };
                        }
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var overlayPen = new Pen(Color.Green, 4.0f);
            foreach (var (cell, overlay) in previewMap.Overlay.Where(x => x.Value.Type.IsPlaceable))
            {
                previewMap.Metrics.GetLocation(cell, out Point topLeft);
                var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                graphics.DrawRectangle(overlayPen, bounds);
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
                    overlayTypeComboBox.SelectedIndexChanged -= OverlayTypeComboBox_SelectedIndexChanged;

                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= OverlaysTool_KeyDown;
                    (mapPanel as Control).KeyUp -= OverlaysTool_KeyUp;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
