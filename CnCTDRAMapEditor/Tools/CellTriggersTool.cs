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
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace MobiusEditor.Tools
{
    public class CellTriggersTool : ViewTool
    {
        private readonly ComboBox triggerCombo;

        private readonly Dictionary<int, CellTrigger> undoCellTriggers = new Dictionary<int, CellTrigger>();
        private readonly Dictionary<int, CellTrigger> redoCellTriggers = new Dictionary<int, CellTrigger>();

        private bool placementMode;

        public CellTriggersTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, ComboBox triggerCombo, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseUp += MapPanel_MouseUp;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += WaypointsTool_KeyDown;
            (this.mapPanel as Control).KeyUp += WaypointsTool_KeyUp;

            this.triggerCombo = triggerCombo;

            navigationWidget.MouseCellChanged += MouseoverWidget_MouseCellChanged;

            UpdateStatus();
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (e.Button == MouseButtons.Left)
                {
                    SetCellTrigger(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveCellTrigger(navigationWidget.MouseCell);
                }
            }
            else if ((e.Button == MouseButtons.Left) || (e.Button == MouseButtons.Right))
            {
                PickCellTrigger(navigationWidget.MouseCell);
            }
        }

        private void MapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if ((undoCellTriggers.Count > 0) || (redoCellTriggers.Count > 0))
            {
                CommitChange();
            }
        }

        private void WaypointsTool_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ShiftKey)
            {
                EnterPlacementMode();
            }
        }

        private void WaypointsTool_KeyUp(object sender, KeyEventArgs e)
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

        private void MouseoverWidget_MouseCellChanged(object sender, MouseCellChangedEventArgs e)
        {
            if (placementMode)
            {
                if (Control.MouseButtons == MouseButtons.Left)
                {
                    SetCellTrigger(e.NewCell);
                }
                else if (Control.MouseButtons == MouseButtons.Right)
                {
                    RemoveCellTrigger(e.NewCell);
                }
            }
        }

        private void SetCellTrigger(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                if (map.CellTriggers[cell] == null)
                {
                    if (!undoCellTriggers.ContainsKey(cell))
                    {
                        undoCellTriggers[cell] = map.CellTriggers[cell];
                    }

                    var cellTrigger = new CellTrigger { Trigger = triggerCombo.SelectedItem as string };
                    map.CellTriggers[cell] = cellTrigger;
                    redoCellTriggers[cell] = cellTrigger;

                    mapPanel.Invalidate();

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveCellTrigger(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var cellTrigger = map.CellTriggers[cell];
                if (cellTrigger != null)
                {
                    if (!undoCellTriggers.ContainsKey(cell))
                    {
                        undoCellTriggers[cell] = map.CellTriggers[cell];
                    }

                    map.CellTriggers[cell] = null;
                    redoCellTriggers[cell] = null;

                    mapPanel.Invalidate();

                    plugin.Dirty = true;
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

        private void PickCellTrigger(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var cellTrigger = map.CellTriggers[cell];
                if (cellTrigger != null)
                {
                    triggerCombo.SelectedItem = cellTrigger.Trigger;
                }
            }
        }

        private void CommitChange()
        {
            var undoCellTriggers2 = new Dictionary<int, CellTrigger>(undoCellTriggers);
            void undoAction(UndoRedoEventArgs e)
            {
                foreach (var kv in undoCellTriggers2)
                {
                    e.Map.CellTriggers[kv.Key] = kv.Value;
                }
                e.MapPanel.Invalidate();
            }

            var redoCellTriggers2 = new Dictionary<int, CellTrigger>(redoCellTriggers);
            void redoAction(UndoRedoEventArgs e)
            {
                foreach (var kv in redoCellTriggers2)
                {
                    e.Map.CellTriggers[kv.Key] = kv.Value;
                }
                e.MapPanel.Invalidate();
            }

            undoCellTriggers.Clear();
            redoCellTriggers.Clear();

            url.Track(undoAction, redoAction);
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to set cell trigger, Right-Click to clear cell trigger";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click or Right-Click to pick cell trigger";
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
                    (mapPanel as Control).KeyDown -= WaypointsTool_KeyDown;
                    (mapPanel as Control).KeyUp -= WaypointsTool_KeyUp;

                    navigationWidget.MouseCellChanged -= MouseoverWidget_MouseCellChanged;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
