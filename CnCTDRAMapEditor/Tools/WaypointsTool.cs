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
    public class WaypointsTool : ViewTool
    {
        private readonly ComboBox waypointCombo;

        private (Waypoint waypoint, int? cell)? undoWaypoint;
        private (Waypoint waypoint, int? cell)? redoWaypoint;

        private bool placementMode;

        public WaypointsTool(MapPanel mapPanel, MapLayerFlag layers, ToolStripStatusLabel statusLbl, ComboBox waypointCombo, IGamePlugin plugin, UndoRedoList<UndoRedoEventArgs> url)
            : base(mapPanel, layers, statusLbl, plugin, url)
        {
            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            (this.mapPanel as Control).KeyDown += WaypointsTool_KeyDown;
            (this.mapPanel as Control).KeyUp += WaypointsTool_KeyUp;

            this.waypointCombo = waypointCombo;

            UpdateStatus();
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (placementMode)
            {
                if (e.Button == MouseButtons.Left)
                {
                    SetWaypoint(navigationWidget.MouseCell);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    RemoveWaypoint(navigationWidget.MouseCell);
                }
            }
            else if ((e.Button == MouseButtons.Left) || (e.Button == MouseButtons.Right))
            {
                PickWaypoint(navigationWidget.MouseCell);
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

        private void SetWaypoint(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var waypoint = map.Waypoints[waypointCombo.SelectedIndex];
                if (waypoint.Cell != cell)
                {
                    if (undoWaypoint == null)
                    {
                        undoWaypoint = (waypoint, waypoint.Cell);
                    }
                    else if (undoWaypoint.Value.cell == cell)
                    {
                        undoWaypoint = null;
                    }

                    waypoint.Cell = cell;
                    redoWaypoint = (waypoint, waypoint.Cell);

                    CommitChange();

                    mapPanel.Invalidate();

                    plugin.Dirty = true;
                }
            }
        }

        private void RemoveWaypoint(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                var waypoint = map.Waypoints.Where(w => w.Cell == cell).FirstOrDefault();
                if (waypoint != null)
                {
                    if (undoWaypoint == null)
                    {
                        undoWaypoint = (waypoint, waypoint.Cell);
                    }

                    waypoint.Cell = null;
                    redoWaypoint = (waypoint, null);

                    CommitChange();

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

        private void PickWaypoint(Point location)
        {
            if (map.Metrics.GetCell(location, out int cell))
            {
                for (var i = 0; i < map.Waypoints.Length; ++i)
                {
                    if (map.Waypoints[i].Cell == cell)
                    {
                        waypointCombo.SelectedIndex = i;
                        break;
                    }
                }
            }
        }

        private void CommitChange()
        {
            var undoWaypoint2 = undoWaypoint;
            void undoAction(UndoRedoEventArgs e)
            {
                undoWaypoint2.Value.waypoint.Cell = undoWaypoint2.Value.cell;
                mapPanel.Invalidate();
            }

            var redoWaypoint2 = redoWaypoint;
            void redoAction(UndoRedoEventArgs e)
            {
                redoWaypoint2.Value.waypoint.Cell = redoWaypoint2.Value.cell;
                mapPanel.Invalidate();
            }

            undoWaypoint = null;
            redoWaypoint = null;

            url.Track(undoAction, redoAction);
        }

        private void UpdateStatus()
        {
            if (placementMode)
            {
                statusLbl.Text = "Left-Click to set cell waypoint, Right-Click to clear cell waypoint";
            }
            else
            {
                statusLbl.Text = "Shift to enter placement mode, Left-Click or Right-Click to pick cell waypoint";
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
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                    (mapPanel as Control).KeyDown -= WaypointsTool_KeyDown;
                    (mapPanel as Control).KeyUp -= WaypointsTool_KeyUp;
                }
                disposedValue = true;
            }

            base.Dispose(disposing);
        }
        #endregion
    }
}
