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
    public class InfantryTool : ViewTool
    {
        private readonly TypeComboBox infantryTypeComboBox;
        private readonly MapPanel infantryTypeMapPanel;
        private readonly ObjectProperties objectProperties;

        private Map previewMap;
        protected override Map RenderMap => previewMap;

        private bool placementMode;

        private readonly Infantry mockInfantry;

        private Infantry selectedInfantry;
        private ObjectPropertiesPopup selectedObjectProperties;

        private InfantryType selectedInfantryType;
        private InfantryType SelectedInfantryType
        {
            get => selectedInfantryType;
            set
            {
                if (selectedInfantryType != value)
                {
                    if (placementMode && (selectedInfantryType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    selectedInfantryType = value;
                    infantryTypeComboBox.SelectedValue = selectedInfantryType;

                    if (placementMode && (selectedInfantryType != null))
                    {
                        mapPanel.Invalidate(map, navigationWidget.MouseCell);
                    }

                    mockInfantry.Type = selectedInfantryType;

                    RefreshMapPanel();
                }
            }
        }

        public InfantryTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, TypeComboBox infantryTypeComboBox, MapPanel infantryTypeMapPanel, ObjectProperties objectProperties, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            previewMap = map;

            mockInfantry = new Infantry(null)
            {
                Type = infantryTypeComboBox.Types.First() as InfantryType,
                House = map.Houses.First().Type,
                Strength = 256,
                Direction = map.DirectionTypes.Where(d => d.Equals(FacingType.South)).First(),
                Mission = map.MissionTypes.Where(m => m.Equals("Guard")).FirstOrDefault() ?? map.MissionTypes.First()
            };
            mockInfantry.PropertyChanged += MockInfantry_PropertyChanged;

            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseDoubleClick += MapPanel_MouseDoubleClick;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += InfantryTool_KeyDown;
            (this.mapPanel as Control).KeyUp += InfantryTool_KeyUp;

            this.infantryTypeComboBox = infantryTypeComboBox;
            this.infantryTypeComboBox.SelectedIndexChanged += InfantryTypeComboBox_SelectedIndexChanged;

            this.infantryTypeMapPanel = infantryTypeMapPanel;
            this.infantryTypeMapPanel.BackColor = Color.White;
            this.infantryTypeMapPanel.MaxZoom = 1;

            this.objectProperties = objectProperties;
            this.objectProperties.Object = mockInfantry;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            SelectedInfantryType = this.infantryTypeComboBox.Types.First() as InfantryType;

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
                if (map.Technos[cell] is InfantryGroup infantryGroup)
                {
                    var i = InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>().First();
                    if (infantryGroup.Infantry[i] is Infantry infantry)
                    {
                        selectedInfantry = null;

                        selectedObjectProperties?.Close();
                        selectedObjectProperties = new ObjectPropertiesPopup(objectProperties.Plugin, infantry);
                        selectedObjectProperties.Closed += (cs, ce) =>
                        {
                            navigationWidget.Refresh();
                        };

                        infantry.PropertyChanged += SelectedInfantry_PropertyChanged;

                        selectedObjectProperties.Show(mapPanel, mapPanel.PointToClient(Control.MousePosition));

                        UpdateStatus();
                    }
                }
            }
        }

        private void MockInfantry_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            RefreshMapPanel();
        }

        private void SelectedInfantry_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            mapPanel.Invalidate(map, (sender as Infantry).InfantryGroup);
        }

        private void InfantryTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedInfantryType = infantryTypeComboBox.SelectedValue as InfantryType;
        }

        private void InfantryTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void InfantryTool_KeyUp(object sender, KeyEventArgs e)
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

            if (placementMode)
            {
                mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
            }
            else if (selectedInfantry != null)
            {
                var oldLocation = map.Technos[selectedInfantry.InfantryGroup].Value;
                var oldStop = Array.IndexOf(selectedInfantry.InfantryGroup.Infantry, selectedInfantry);

                InfantryGroup infantryGroup = null;
                var techno = map.Technos[navigationWidget.MouseCell];
                if (techno == null)
                {
                    infantryGroup = new InfantryGroup();
                    map.Technos.Add(navigationWidget.MouseCell, infantryGroup);
                }
                else if (techno is InfantryGroup)
                {
                    infantryGroup = techno as InfantryGroup;
                }

                if (infantryGroup != null)
                {
                    foreach (var i in InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>())
                    {
                        if (infantryGroup.Infantry[i] == null)
                        {
                            selectedInfantry.InfantryGroup.Infantry[oldStop] = null;
                            infantryGroup.Infantry[i] = selectedInfantry;
                            
                            if (infantryGroup != selectedInfantry.InfantryGroup)
                            {
                                mapPanel.Invalidate(map, selectedInfantry.InfantryGroup);
                                if (selectedInfantry.InfantryGroup.Infantry.All(x => x == null))
                                {
                                    map.Technos.Remove(selectedInfantry.InfantryGroup);
                                }
                            }
                            selectedInfantry.InfantryGroup = infantryGroup;

                            mapPanel.Invalidate(map, infantryGroup);

                            plugin.Dirty = true;
                        }

                        if (infantryGroup == selectedInfantry.InfantryGroup)
                        {
                            break;
                        }
                    }
                }
            }
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (e.Button == MouseButtons.Left)
                {
                    AddInfantry(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveInfantry(navigationWidget.MouseCell);
                }
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectInfantry(navigationWidget.MouseCell);
            }
            else if (e.Button == MouseButtons.Right)
            {
                PickInfantry(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (selectedInfantry != null)
            {
                selectedInfantry = null;
                UpdateStatus();
            }
        }

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (SelectedInfantryType != null)
                {
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.OldCell, new Size(1, 1)), 1, 1));
                    mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(e.NewCell, new Size(1, 1)), 1, 1));
                }
            }
        }

        private void AddInfantry(Point location)
        {
            if (SelectedInfantryType != null)
            {
                if (map.Metrics.GetCell(location, out int cell))
                {
                    InfantryGroup infantryGroup = null;

                    var techno = map.Technos[cell];
                    if (techno == null)
                    {
                        infantryGroup = new InfantryGroup();
                        map.Technos.Add(cell, infantryGroup);
                    }
                    else if (techno is InfantryGroup)
                    {
                        infantryGroup = techno as InfantryGroup;
                    }

                    if (infantryGroup != null)
                    {
                        foreach (var i in InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>())
                        {
                            if (infantryGroup.Infantry[i] == null)
                            {
                                var infantry = mockInfantry.Clone();
                                infantryGroup.Infantry[i] = infantry;
                                infantry.InfantryGroup = infantryGroup;
                                mapPanel.Invalidate(map, infantryGroup);
                                plugin.Dirty = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        private void RemoveInfantry(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.Technos[cell] is InfantryGroup infantryGroup)
                {
                    foreach (var i in InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>())
                    {
                        if (infantryGroup.Infantry[i] != null)
                        {
                            infantryGroup.Infantry[i] = null;
                            mapPanel.Invalidate(map, infantryGroup);
                            plugin.Dirty = true;
                            break;
                        }
                    }
                    if (infantryGroup.Infantry.All(i => i == null))
                    {
                        map.Technos.Remove(infantryGroup);
                    }
                }
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

            if (SelectedInfantryType != null)
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

            if (SelectedInfantryType != null)
            {
                mapPanel.Invalidate(map, Rectangle.Inflate(new Rectangle(navigationWidget.MouseCell, new Size(1, 1)), 1, 1));
            }

            UpdateStatus();
        }

        private void PickInfantry(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.Technos[cell] is InfantryGroup infantryGroup)
                {
                    var i = InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>().First();
                    if (infantryGroup.Infantry[i] is Infantry infantry)
                    {
                        SelectedInfantryType = infantry.Type;
                        mockInfantry.House = infantry.House;
                        mockInfantry.Strength = infantry.Strength;
                        mockInfantry.Direction = infantry.Direction;
                        mockInfantry.Mission = infantry.Mission;
                        mockInfantry.Trigger = infantry.Trigger;
                    }
                }
            }
        }

        private void SelectInfantry(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                selectedInfantry = null;
                if (map.Technos[cell] is InfantryGroup infantryGroup)
                {
                    var i = InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>().First();
                    if (infantryGroup.Infantry[i] is Infantry infantry)
                    {
                        selectedInfantry = infantry;
                    }
                }
            }

            UpdateStatus();
        }

        private void RefreshMapPanel()
        {
            if (mockInfantry.Type != null)
            {
                var infantryPreview = new Bitmap(Globals.TileWidth, Globals.TileHeight);
                using (var g = Graphics.FromImage(infantryPreview))
                {
                    MapRenderer.Render(map.Theater, Point.Empty, Globals.TileSize, mockInfantry, InfantryStoppingType.Center).Item2(g);
                }
                infantryTypeMapPanel.MapImage = infantryPreview;
            }
            else
            {
                infantryTypeMapPanel.MapImage = null;
            }
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to place infantry, Right-Click to remove infantry";
            }
            else if (selectedInfantry != null)
            {
                statusLbl.Text = "Drag mouse to move infantry";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click drag to move infantry, Double-Click update infantry properties, Right-Click to pick infantry";
            }
        }

        protected override void PreRenderMap()
        {
            base.PreRenderMap();

            previewMap = map.Clone();
            if (placementMode)
            {
                var location = navigationWidget.MouseCell;
                if (SelectedInfantryType != null)
                {
                    if (previewMap.Metrics.GetCell(location, out int cell))
                    {
                        InfantryGroup infantryGroup = null;

                        var techno = previewMap.Technos[cell];
                        if (techno == null)
                        {
                            infantryGroup = new InfantryGroup();
                            previewMap.Technos.Add(cell, infantryGroup);
                        }
                        else if (techno is InfantryGroup)
                        {
                            infantryGroup = techno as InfantryGroup;
                        }

                        if (infantryGroup != null)
                        {
                            foreach (var i in InfantryGroup.ClosestStoppingTypes(navigationWidget.MouseSubPixel).Cast<int>())
                            {
                                if (infantryGroup.Infantry[i] == null)
                                {
                                    var infantry = mockInfantry.Clone();
                                    infantry.Tint = Color.FromArgb(128, Color.White);
                                    infantryGroup.Infantry[i] = infantry;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        protected override void PostRenderMap(Graphics graphics)
        {
            base.PostRenderMap(graphics);

            var infantryPen = new Pen(Color.Green, 4.0f);
            foreach (var (topLeft, _) in map.Technos.OfType<InfantryGroup>())
            {
                var bounds = new Rectangle(new Point(topLeft.X * Globals.TileWidth, topLeft.Y * Globals.TileHeight), Globals.TileSize);
                graphics.DrawRectangle(infantryPen, bounds);
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
                    (mapPanel as Control).KeyDown -= InfantryTool_KeyDown;
                    (mapPanel as Control).KeyUp -= InfantryTool_KeyUp;

                    infantryTypeComboBox.SelectedIndexChanged -= InfantryTypeComboBox_SelectedIndexChanged;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
