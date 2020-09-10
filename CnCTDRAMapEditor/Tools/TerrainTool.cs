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
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public class TerrainTool : ViewTool
    {
        private readonly TypeComboBox terrainTypeComboBox;
        private readonly MapPanel terrainTypeMapPanel;
        private readonly TerrainProperties terrainProperties;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private readonly Terrain mockTerrain;

        private Terrain selectedTerrain;
        private Point selectedTerrainPivot;

        private TerrainType selectedTerrainType;
        private TerrainPropertiesPopup selectedTerrainProperties;
        private TerrainType SelectedTerrainType
        {
            get => selectedTerrainType;
            set
            {
                if (selectedTerrainType != value)
                {
                    if (placementMode && (selectedTerrainType != null))
                    {
                        mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedTerrainType.OverlapBounds.Size));
                    }

                    selectedTerrainType = value;
                    terrainTypeComboBox.SelectedValue = selectedTerrainType;

                    if (placementMode && (selectedTerrainType != null))
                    {
                        mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedTerrainType.OverlapBounds.Size));
                    }

                    mockTerrain.Type = selectedTerrainType;
                    mockTerrain.Icon = selectedTerrainType.IsTransformable ? 22 : 0;

                    RefreshMapPanel();
                }
            }
        }

        public TerrainTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox terrainTypeComboBox, MapPanel terrainTypeMapPanel, TerrainProperties terrainProperties, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            mockTerrain = new Terrain();
            mockTerrain.PropertyChanged += MockTerrain_PropertyChanged;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseDoubleClick += MapPanel_MouseDoubleClick;
            (this.mapPanel as Control).KeyDown += TerrainTool_KeyDown;
            (this.mapPanel as Control).KeyUp += TerrainTool_KeyUp;

            this.terrainTypeComboBox = terrainTypeComboBox;
            this.terrainTypeComboBox.SelectedIndexChanged += TerrainTypeCombo_SelectedIndexChanged;

            this.terrainTypeMapPanel = terrainTypeMapPanel;
            this.terrainTypeMapPanel.BackColor = Color.White;
            this.terrainTypeMapPanel.MaxZoom = 1;

            this.terrainProperties = terrainProperties;
            this.terrainProperties.Terrain = mockTerrain;
            this.terrainProperties.Visible = plugin.GameType == GameType.TiberianDawn;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedTerrainType = terrainTypeComboBox.Types.First() as TerrainType;

            UpdateStatus();
        }

        private void MapPanel_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (Control.ModifierKeys != Keys.None)
            {
                return;
            }

            if (map.Metrics.GetCell(navigationWidget.MouseCell, out int cell))
            {
                if (map.Technos[cell] is Terrain terrain)
                {
                    selectedTerrain = null;

                    selectedTerrainProperties?.Close();
                    selectedTerrainProperties = new TerrainPropertiesPopup(terrainProperties.Plugin, terrain);
                    selectedTerrainProperties.Closed += (cs, ce) =>
                    {
                        navigationWidget.Refresh();
                    };

                    selectedTerrainProperties.Show(mapPanel, mapPanel.PointToClient(Control.MousePosition));

                    UpdateStatus();
                }
            }
        }

        private void MockTerrain_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            RefreshMapPanel();
        }

        private void TerrainTypeCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedTerrainType = terrainTypeComboBox.SelectedValue as TerrainType;
        }

        private void TerrainTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void TerrainTool_KeyUp(object sender, KeyEventArgs e)
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
                    AddTerrain(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveTerrain(navigationWidget.MouseCell);
                }
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectTerrain(navigationWidget.MouseCell);
            }
            else if (e.Button == MouseButtons.Right)
            {
                PickTerrain(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (selectedTerrain != null)
            {
                selectedTerrain = null;
                selectedTerrainPivot = Point.Empty;

                UpdateStatus();
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
                if (SelectedTerrainType != null)
                {
                    mapPanel.Invalidate(map, new Rectangle(e.OldCell, SelectedTerrainType.OverlapBounds.Size));
                    mapPanel.Invalidate(map, new Rectangle(e.NewCell, SelectedTerrainType.OverlapBounds.Size));
                }
            }
            else if (selectedTerrain != null)
            {
                var oldLocation = map.Technos[selectedTerrain].Value;
                var newLocation = new Point(Math.Max(0, e.NewCell.X - selectedTerrainPivot.X), Math.Max(0, e.NewCell.Y - selectedTerrainPivot.Y));
                mapPanel.Invalidate(map, selectedTerrain);
                map.Technos.Remove(selectedTerrain);
                if (map.Technos.Add(newLocation, selectedTerrain))
                {
                    mapPanel.Invalidate(map, selectedTerrain);
                }
                else
                {
                    map.Technos.Add(oldLocation, selectedTerrain);
                }
            }
        }

        private void AddTerrain(Point location)
        {
            if (!map.Metrics.Contains(location))
            {
                return;
            }

            if (SelectedTerrainType != null)
            {
                var terrain = mockTerrain.Clone();
                if (map.Technos.Add(location, terrain))
                {
                    mapPanel.Invalidate(map, terrain);

                    void undoAction(UndoRedoEventArgs e)
                    {
                        e.MapPanel.Invalidate(e.Map, location);
                        e.Map.Technos.Remove(terrain);
                    }

                    void redoAction(UndoRedoEventArgs e)
                    {
                        e.Map.Technos.Add(location, terrain);
                        e.MapPanel.Invalidate(e.Map, location);
                    }

                    url.Track(undoAction, redoAction);

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveTerrain(Point location)
        {
            if (map.Technos[location] is Terrain terrain)
            {
                mapPanel.Invalidate(map, terrain);
                map.Technos.Remove(location);

                void undoAction(UndoRedoEventArgs e)
                {
                    e.Map.Technos.Add(location, terrain);
                    e.MapPanel.Invalidate(e.Map, location);
                }

                void redoAction(UndoRedoEventArgs e)
                {
                    e.MapPanel.Invalidate(e.Map, location);
                    e.Map.Technos.Remove(terrain);
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

            if (SelectedTerrainType != null)
            {
                mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedTerrainType.OverlapBounds.Size));
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

            if (SelectedTerrainType != null)
            {
                mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedTerrainType.OverlapBounds.Size));
            }

            UpdateStatus();
        }

        private void PickTerrain(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.Technos[cell] is Terrain terrain)
                {
                    SelectedTerrainType = terrain.Type;
                    mockTerrain.Trigger = terrain.Trigger;
                }
            }
        }

        private void SelectTerrain(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                selectedTerrain = map.Technos[cell] as Terrain;
                selectedTerrainPivot = (selectedTerrain != null) ? (location - (Size)map.Technos[selectedTerrain].Value) : Point.Empty;
            }

            UpdateStatus();
        }

        private void RefreshMapPanel()
        {
            terrainTypeMapPanel.MapImage = mockTerrain.Type.Thumbnail;
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place terrain, Right-Click to remove terrain";
            }
            else if (selectedTerrain != null)
            {
                statusLbl.Text = "Drag mouse to move terrain";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click drag to move terrain, Double-Click update terrain properties, Right-Click to pick terrain";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedTerrainType != null)
                {
                    if (previewMap.Metrics.Contains(location))
                    {
                        var terrain = new Terrain
                        {
                            Type = SelectedTerrainType,
                            Icon = SelectedTerrainType.IsTransformable ? 22 : 0,
                            Tint = Color.FromArgb(128, Color.White)
                        };
                        previewMap.Technos.Add(location, terrain);
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var terrainPen = new Pen(Color.Green, 4.0f);
            var occupyPen = new Pen(Color.Red, 2.0f);
            foreach (var (topLeft, terrain) in previewMap.Technos.OfType<Terrain>())
            {
                var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), terrain.Type.RenderSize);
                graphics.DrawRectangle(terrainPen, bounds);

                for (var y = 0; y < terrain.Type.OccupyMask.GetLength(0); ++y)
                {
                    for (var x = 0; x < terrain.Type.OccupyMask.GetLength(1); ++x)
                    {
                        if (terrain.Type.OccupyMask[y, x])
                        {
                            var occupyBounds = new Rectangle(
                                new Point((topLeft.X + x) * Globals.TileWidth, (topLeft.Y + y) * Globals.TileHeight),
                                Globals.TileSize
                            );
                            graphics.DrawRectangle(occupyPen, occupyBounds);
                        }
                    }
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
                    selectedTerrainProperties?.Close();

                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    mapPanel.MouseUp -= MapPanel_MouseUp;
                    mapPanel.MouseDoubleClick -= MapPanel_MouseDoubleClick;
                    (mapPanel as Control).KeyDown -= TerrainTool_KeyDown;
                    (mapPanel as Control).KeyUp -= TerrainTool_KeyUp;

                    terrainTypeComboBox.SelectedIndexChanged -= TerrainTypeCombo_SelectedIndexChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
