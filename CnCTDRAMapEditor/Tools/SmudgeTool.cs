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
    public class SmudgeTool : ViewTool
    {
        private readonly TypeComboBox smudgeTypeComboBox;
        private readonly MapPanel smudgeTypeMapPanel;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private SmudgeType selectedSmudgeType;
        private SmudgeType SelectedSmudgeType
        {
            get => selectedSmudgeType;
            set
            {
                if (selectedSmudgeType != value)
                {
                    if (placementMode && (selectedSmudgeType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    selectedSmudgeType = value;
                    smudgeTypeComboBox.SelectedValue = selectedSmudgeType;

                    if (placementMode && (selectedSmudgeType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    RefreshMapPanel();
                }
            }
        }

        public SmudgeTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox smudgeTypeComboBox, MapPanel smudgeTypeMapPanel, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += SmudgeTool_KeyDown;
            (this.mapPanel as Control).KeyUp += SmudgeTool_KeyUp;

            this.smudgeTypeComboBox = smudgeTypeComboBox;
            this.smudgeTypeComboBox.SelectedIndexChanged += SmudgeTypeComboBox_SelectedIndexChanged;

            this.smudgeTypeMapPanel = smudgeTypeMapPanel;
            this.smudgeTypeMapPanel.BackColor = Color.White;
            this.smudgeTypeMapPanel.MaxZoom = 1;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedSmudgeType = smudgeTypeComboBox.Types.First() as SmudgeType;

            UpdateStatus();
        }

        private void SmudgeTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedSmudgeType = smudgeTypeComboBox.SelectedValue as SmudgeType;
        }

        private void SmudgeTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void SmudgeTool_KeyUp(object sender, KeyEventArgs e)
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
                    AddSmudge(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveSmudge(navigationWidget.MouseCell);
                }
            }
            else if ((e.Button == MouseButtons.Left) || (e.Button == MouseButtons.Right))
            {
                PickSmudge(navigationWidget.MouseCell);
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
                if (SelectedSmudgeType != null)
                {
                    mapPanel.Invalidate(map, e.OldCell);
                    mapPanel.Invalidate(map, e.NewCell);
                }
            }
        }

        private void AddSmudge(Point location)
        {
            if (map.Smudge[location] == null)
            {
                if (SelectedSmudgeType != null)
                {
                    var smudge = new Smudge
                    {
                        Type = SelectedSmudgeType,
                        Icon = 0,
                        Data = 0
                    };
                    map.Smudge[location] = smudge;
                    mapPanel.Invalidate(map, location);

                    void undoAction(UndoRedoEventArgs e)
                    {
                        e.MapPanel.Invalidate(e.Map, location);
                        e.Map.Smudge[location] = null;
                    }

                    void redoAction(UndoRedoEventArgs e)
                    {
                        e.Map.Smudge[location] = smudge;
                        e.MapPanel.Invalidate(e.Map, location);
                    }

                    url.Track(undoAction, redoAction);

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveSmudge(Point location)
        {
            if ((map.Smudge[location] is Smudge smudge) && ((smudge.Type.Flag & SmudgeTypeFlag.Bib) == SmudgeTypeFlag.None))
            {
                map.Smudge[location] = null;
                mapPanel.Invalidate(map, location);

                void undoAction(UndoRedoEventArgs e)
                {
                    e.Map.Smudge[location] = smudge;
                    e.MapPanel.Invalidate(e.Map, location);
                }

                void redoAction(UndoRedoEventArgs e)
                {
                    e.MapPanel.Invalidate(e.Map, location);
                    e.Map.Smudge[location] = null;
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

            if (SelectedSmudgeType != null)
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

            if (SelectedSmudgeType != null)
            {
                mapPanel.Invalidate(map, navigationWidget.MouseCell);
            }

            UpdateStatus();
        }

        private void PickSmudge(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var smudge = map.Smudge[cell];
                if (smudge != null)
                {
                    SelectedSmudgeType = smudge.Type;
                }
            }
        }

        private void RefreshMapPanel()
        {
            smudgeTypeMapPanel.MapImage = SelectedSmudgeType?.Thumbnail;
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place smudge, Right-Click to remove smudge";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click or Right-Click to pick smudge";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedSmudgeType != null)
                {
                    if (previewMap.Metrics.GetCell(location, out int cell))
                    {
                        if (previewMap.Smudge[cell] == null)
                        {
                            previewMap.Smudge[cell] = new Smudge { Type = SelectedSmudgeType, Data = 0, Tint = Color.FromArgb(128, Color.White) };
                        }
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var smudgePen = new Pen(Color.Green, 4.0f);
            foreach (var (cell, smudge) in previewMap.Smudge.Where(x => (x.Value.Type.Flag & SmudgeTypeFlag.Bib) == SmudgeTypeFlag.None))
            {
                previewMap.Metrics.GetLocation(cell, out Point topLeft);
                var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                graphics.DrawRectangle(smudgePen, bounds);
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
                    smudgeTypeComboBox.SelectedIndexChanged -= SmudgeTypeComboBox_SelectedIndexChanged;

                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= SmudgeTool_KeyDown;
                    (mapPanel as Control).KeyUp -= SmudgeTool_KeyUp;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
