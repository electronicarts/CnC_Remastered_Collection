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
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public class BuildingTool : ViewTool
    {
        private readonly TypeComboBox buildingTypeComboBox;
        private readonly MapPanel buildingTypeMapPanel;
        private readonly ObjectProperties objectProperties;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private readonly Building mockBuilding;

        private Building selectedBuilding;
        private ObjectPropertiesPopup selectedObjectProperties;
        private Point selectedBuildingPivot;

        private BuildingType selectedBuildingType;
        private BuildingType SelectedBuildingType
        {
            get => selectedBuildingType;
            set
            {
                if (selectedBuildingType != value)
                {
                    if (placementMode && (selectedBuildingType != null))
                    {
                        mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedBuildingType.OverlapBounds.Size));
                    }

                    selectedBuildingType = value;
                    buildingTypeComboBox.SelectedValue = selectedBuildingType;

                    if (placementMode && (selectedBuildingType != null))
                    {
                        mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, selectedBuildingType.OverlapBounds.Size));
                    }

                    mockBuilding.Type = selectedBuildingType;

                    RefreshMapPanel();
                }
            }
        }

        public BuildingTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox buildingTypeComboBox, MapPanel buildingTypeMapPanel, ObjectProperties objectProperties, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            mockBuilding = new Building()
            {
                Type = buildingTypeComboBox.Types.First() as BuildingType,
                House = map.Houses.First().Type,
                Strength = 256,
                Direction = map.DirectionTypes.Where(d => d.Equals(FacingType.North)).First()
            };
            mockBuilding.PropertyChanged += MockBuilding_PropertyChanged;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseDoubleClick += MapPanel_MouseDoubleClick;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += UnitTool_KeyDown;
            (this.mapPanel as Control).KeyUp += UnitTool_KeyUp;

            this.buildingTypeComboBox = buildingTypeComboBox;
            this.buildingTypeComboBox.SelectedIndexChanged += UnitTypeComboBox_SelectedIndexChanged;

            this.buildingTypeMapPanel = buildingTypeMapPanel;
            this.buildingTypeMapPanel.BackColor = Color.White;
            this.buildingTypeMapPanel.MaxZoom = 1;

            this.objectProperties = objectProperties;
            this.objectProperties.Object = mockBuilding;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedBuildingType = mockBuilding.Type;

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
                if (map.Technos[cell] is Building building)
                {
                    selectedBuilding = null;
                    selectedBuildingPivot = Point.Empty;

                    selectedObjectProperties?.Close();
                    selectedObjectProperties = new ObjectPropertiesPopup(objectProperties.Plugin, building);
                    selectedObjectProperties.Closed += (cs, ce) =>
                    {
                        navigationWidget.Refresh();
                    };

                    building.PropertyChanged += SelectedBuilding_PropertyChanged;

                    selectedObjectProperties.Show(mapPanel, mapPanel.PointToClient(Control.MousePosition));

                    UpdateStatus();
                }
            }
        }

        private void MockBuilding_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if ((mockBuilding.Type == null) || !mockBuilding.Type.HasTurret)
            {
                mockBuilding.Direction = map.DirectionTypes.Where(d => d.Equals(FacingType.North)).First();
            }

            RefreshMapPanel();
        }

        private void SelectedBuilding_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            mapPanel.Invalidate(map, sender as Building);
        }

        private void UnitTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedBuildingType = buildingTypeComboBox.SelectedValue as BuildingType;
        }

        private void UnitTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void UnitTool_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                ExitPlacementMode();
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

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (e.Button == MouseButtons.Left)
                {
                    AddBuilding(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveBuilding(navigationWidget.MouseCell);
                }
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectBuilding(navigationWidget.MouseCell);
            }
            else if (e.Button == MouseButtons.Right)
            {
                PickBuilding(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (selectedBuilding != null)
            {
                selectedBuilding = null;
                selectedBuildingPivot = Point.Empty;

                UpdateStatus();
            }
        }

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (SelectedBuildingType != null)
                {
                    mapPanel.Invalidate(map, new Rectangle(e.OldCell, SelectedBuildingType.OverlapBounds.Size));
                    mapPanel.Invalidate(map, new Rectangle(e.NewCell, SelectedBuildingType.OverlapBounds.Size));
                }
            }
            else if (selectedBuilding != null)
            {
                var oldLocation = map.Technos[selectedBuilding].Value;
                var newLocation = new Point(Math.Max(0, e.NewCell.X - selectedBuildingPivot.X), Math.Max(0, e.NewCell.Y - selectedBuildingPivot.Y));
                mapPanel.Invalidate(map, selectedBuilding);
                map.Buildings.Remove(selectedBuilding);
                if (map.Technos.CanAdd(newLocation, selectedBuilding, selectedBuilding.Type.BaseOccupyMask) &&
                    map.Buildings.Add(newLocation, selectedBuilding))
                {
                    mapPanel.Invalidate(map, selectedBuilding);
                    plugin.Dirty = true;
                }
                else
                {
                    map.Buildings.Add(oldLocation, selectedBuilding);
                }
            }
        }

        private void AddBuilding(Point location)
        {
            if (SelectedBuildingType != null)
            {
                var building = mockBuilding.Clone();
                if (map.Technos.CanAdd(location, building, building.Type.BaseOccupyMask) && map.Buildings.Add(location, building))
                {
                    if (building.BasePriority >= 0)
                    {
                        foreach (var baseBuilding in map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0))
                        {
                            if ((building != baseBuilding) && (baseBuilding.BasePriority >= building.BasePriority))
                            {
                                baseBuilding.BasePriority++;
                            }
                        }

                        var baseBuildings = map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0).OrderBy(x => x.BasePriority).ToArray();
                        for (var i = 0; i < baseBuildings.Length; ++i)
                        {
                            baseBuildings[i].BasePriority = i;
                        }

                        foreach (var baseBuilding in map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0))
                        {
                            mapPanel.Invalidate(map, baseBuilding);
                        }
                    }

                    mapPanel.Invalidate(map, building);

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveBuilding(Point location)
        {
            if (map.Technos[location] is Building building)
            {
                mapPanel.Invalidate(map, building);
                map.Buildings.Remove(building);

                if (building.BasePriority >= 0)
                {
                    var baseBuildings = map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0).OrderBy(x => x.BasePriority).ToArray();
                    for (var i = 0; i < baseBuildings.Length; ++i)
                    {
                        baseBuildings[i].BasePriority = i;
                    }

                    foreach (var baseBuilding in map.Buildings.OfType<Building>().Select(x => x.Occupier).Where(x => x.BasePriority >= 0))
                    {
                        mapPanel.Invalidate(map, baseBuilding);
                    }
                }

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

            if (SelectedBuildingType != null)
            {
                mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, SelectedBuildingType.OverlapBounds.Size));
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

            if (SelectedBuildingType != null)
            {
                mapPanel.Invalidate(map, new Rectangle(navigationWidget.MouseCell, SelectedBuildingType.OverlapBounds.Size));
            }

            UpdateStatus();
        }

        private void PickBuilding(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.Technos[cell] is Building building)
                {
                    SelectedBuildingType = building.Type;
                    mockBuilding.House = building.House;
                    mockBuilding.Strength = building.Strength;
                    mockBuilding.Direction = building.Direction;
                    mockBuilding.Trigger = building.Trigger;
                    mockBuilding.BasePriority = building.BasePriority;
                    mockBuilding.IsPrebuilt = building.IsPrebuilt;
                    mockBuilding.Sellable = building.Sellable;
                    mockBuilding.Rebuild = building.Rebuild;
                }
            }
        }

        private void SelectBuilding(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                selectedBuilding = map.Technos[cell] as Building;
                selectedBuildingPivot = (selectedBuilding != null) ? (location - (Size)map.Technos[selectedBuilding].Value) : Point.Empty;
            }

            UpdateStatus();
        }

        private void RefreshMapPanel()
        {
            if (mockBuilding.Type != null)
            {
                var render = MapRenderer.Render(plugin.GameType, map.Theater, new Point(0, 0), Globals.TileSize, Globals.TileScale, mockBuilding);
                if (!render.Item1.IsEmpty)
                {
                    var buildingPreview = new Bitmap(render.Item1.Width, render.Item1.Height);
                    using (var g = Graphics.FromImage(buildingPreview))
                    {
                        render.Item2(g);
                    }
                    buildingTypeMapPanel.MapImage = buildingPreview;
                }
                else
                {
                    buildingTypeMapPanel.MapImage = null;
                }
            }
            else
            {
                buildingTypeMapPanel.MapImage = null;
            }
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place building, Right-Click to remove building";
            }
            else if (selectedBuilding != null)
            {
                statusLbl.Text = "Drag mouse to move building";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click drag to move building, Double-Click update building properties, Right-Click to pick building";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedBuildingType != null)
                {
                    var building = mockBuilding.Clone();
                    building.Tint = Color.FromArgb(128, Color.White);
                    if (previewMap.Technos.CanAdd(location, building, building.Type.BaseOccupyMask) && previewMap.Buildings.Add(location, building))
                    {
                        mapPanel.Invalidate(previewMap, building);
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var buildingPen = new Pen(Color.Green, 4.0f);
            var occupyPen = new Pen(Color.Red, 2.0f);
            foreach (var (topLeft, building) in map.Buildings.OfType<Building>())
            {
                var bounds = new Rectangle(
                    new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight),
                    new Size(building.Type.Size.Width * Globals.TileWidth, building.Type.Size.Height * Globals.TileHeight)
                );
                graphics.DrawRectangle(buildingPen, bounds);

                for (var y = 0; y < building.Type.BaseOccupyMask.GetLength(0); ++y)
                {
                    for (var x = 0; x < building.Type.BaseOccupyMask.GetLength(1); ++x)
                    {
                        if (building.Type.BaseOccupyMask[y, x])
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
                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseUp -= MapPanel_MouseUp;
                    mapPanel.MouseDoubleClick -= MapPanel_MouseDoubleClick;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= UnitTool_KeyDown;
                    (mapPanel as Control).KeyUp -= UnitTool_KeyUp;

                    buildingTypeComboBox.SelectedIndexChanged -= UnitTypeComboBox_SelectedIndexChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
