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
    public class UnitTool : ViewTool
    {
        private readonly TypeComboBox unitTypeComboBox;
        private readonly MapPanel unitTypeMapPanel;
        private readonly ObjectProperties objectProperties;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private readonly Unit mockUnit;

        private Unit selectedUnit;
        private ObjectPropertiesPopup selectedObjectProperties;

        private UnitType selectedUnitType;
        private UnitType SelectedUnitType
        {
            get => selectedUnitType;
            set
            {
                if (selectedUnitType != value)
                {
                    if (placementMode && (selectedUnitType != null))
                    {
                        mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
                    }

                    selectedUnitType = value;
                    unitTypeComboBox.SelectedValue = selectedUnitType;

                    if (placementMode && (selectedUnitType != null))
                    {
                        mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
                    }

                    mockUnit.Type = selectedUnitType;

                    RefreshMapPanel();
                }
            }
        }

        public UnitTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox unitTypeComboBox, MapPanel unitTypeMapPanel, ObjectProperties objectProperties, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            mockUnit = new Unit()
            {
                Type = unitTypeComboBox.Types.First() as UnitType,
                House = map.Houses.First().Type,
                Strength = 256,
                Direction = map.DirectionTypes.Where(d => d.Equals(FacingType.North)).First(),
                Mission = map.MissionTypes.Where(m => m.Equals("Guard")).FirstOrDefault() ?? map.MissionTypes.First()
            };
            mockUnit.PropertyChanged += MockUnit_PropertyChanged;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseDoubleClick += MapPanel_MouseDoubleClick;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += UnitTool_KeyDown;
            (this.mapPanel as Control).KeyUp += UnitTool_KeyUp;

            this.unitTypeComboBox = unitTypeComboBox;
            this.unitTypeComboBox.SelectedIndexChanged += UnitTypeComboBox_SelectedIndexChanged;

            this.unitTypeMapPanel = unitTypeMapPanel;
            this.unitTypeMapPanel.BackColor = Color.White;
            this.unitTypeMapPanel.MaxZoom = 1;

            this.objectProperties = objectProperties;
            this.objectProperties.Object = mockUnit;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedUnitType = mockUnit.Type;

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
                if (map.Technos[cell] is Unit unit)
                {
                    selectedUnit = null;

                    selectedObjectProperties?.Close();
                    selectedObjectProperties = new ObjectPropertiesPopup(objectProperties.Plugin, unit);
                    selectedObjectProperties.Closed += (cs, ce) =>
                    {
                        navigationWidget.Refresh();
                    };

                    unit.PropertyChanged += SelectedUnit_PropertyChanged;

                    selectedObjectProperties.Show(mapPanel, mapPanel.PointToClient(Control.MousePosition));

                    UpdateStatus();
                }
            }
        }

        private void MockUnit_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            RefreshMapPanel();
        }

        private void SelectedUnit_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            mapPanel.Invalidate(map, sender as Unit);
        }

        private void UnitTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedUnitType = unitTypeComboBox.SelectedValue as UnitType;
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
                    AddUnit(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveUnit(navigationWidget.MouseCell);
                }
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectUnit(navigationWidget.MouseCell);
            }
            else if (e.Button == MouseButtons.Right)
            {
                PickUnit(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (selectedUnit != null)
            {
                selectedUnit = null;

                UpdateStatus();
            }
        }

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (SelectedUnitType != null)
                {
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.OldCell, new Size(1, 1)), 1, 1));
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.NewCell, new Size(1, 1)), 1, 1));
                }
            }
            else if (selectedUnit != null)
            {
                var oldLocation = map.Technos[selectedUnit].Value;
                mapPanel.Invalidate(map, selectedUnit);
                map.Technos.Remove(selectedUnit);
                if (map.Technos.Add(e.NewCell, selectedUnit))
                {
                    mapPanel.Invalidate(map, selectedUnit);
                    plugin.Dirty = true;
                }
                else
                {
                    map.Technos.Add(oldLocation, selectedUnit);
                }
            }
        }

        private void AddUnit(Point location)
        {
            if (SelectedUnitType != null)
            {
                var unit = mockUnit.Clone();
                if (map.Technos.Add(location, unit))
                {
                    mapPanel.Invalidate(map, unit);
                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveUnit(Point location)
        {
            if (map.Technos[location] is Unit unit)
            {
                mapPanel.Invalidate(map, unit);
                map.Technos.Remove(unit);
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

            if (SelectedUnitType != null)
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

            if (SelectedUnitType != null)
            {
                mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
            }

            UpdateStatus();
        }

        private void PickUnit(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.Technos[cell] is Unit unit)
                {
                    SelectedUnitType = unit.Type;
                    mockUnit.House = unit.House;
                    mockUnit.Strength = unit.Strength;
                    mockUnit.Direction = unit.Direction;
                    mockUnit.Mission = unit.Mission;
                    mockUnit.Trigger = unit.Trigger;
                }
            }
        }

        private void SelectUnit(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                selectedUnit = map.Technos[cell] as Unit;
            }

            UpdateStatus();
        }

        private void RefreshMapPanel()
        {
            if (mockUnit.Type != null)
            {
                var unitPreview = new Bitmap(Globals.TileWidth * 3, Globals.TileHeight * 3);
                using (var g = Graphics.FromImage(unitPreview))
                {
                    MapRenderer.Render(plugin.GameType, map.Theater, new Point(1, 1), Globals.TileSize, mockUnit).Item2(g);
                }
                unitTypeMapPanel.MapImage = unitPreview;
            }
            else
            {
                unitTypeMapPanel.MapImage = null;
            }
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place unit, Right-Click to remove unit";
            }
            else if (selectedUnit != null)
            {
                statusLbl.Text = "Drag mouse to move unit";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click drag to move unit, Double-Click update unit properties, Right-Click to pick unit";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedUnitType != null)
                {
                    var unit = mockUnit.Clone();
                    unit.Tint = Color.FromArgb(128, Color.White);
                    if (previewMap.Technos.Add(location, unit))
                    {
                        mapPanel.Invalidate(previewMap, unit);
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var unitPen = new Pen(Color.Green, 4.0f);
            foreach (var (topLeft, _) in map.Technos.OfType<Unit>())
            {
                var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                graphics.DrawRectangle(unitPen, bounds);
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
                    selectedObjectProperties?.Close();

                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseUp -= MapPanel_MouseUp;
                    mapPanel.MouseDoubleClick -= MapPanel_MouseDoubleClick;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= UnitTool_KeyDown;
                    (mapPanel as Control).KeyUp -= UnitTool_KeyUp;

                    unitTypeComboBox.SelectedIndexChanged -= UnitTypeComboBox_SelectedIndexChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
