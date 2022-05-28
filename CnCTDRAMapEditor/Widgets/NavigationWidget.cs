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
using MobiusEditor.Interface;
using MobiusEditor.Model;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace MobiusEditor.Widgets
{
    public class MouseCellChangedEventArgs : EventArgs
    {
        public Point OldCell { get; private set; }

        public Point NewCell { get; private set; }

        public MouseCellChangedEventArgs(Point oldCell, Point newCell)
        {
            OldCell = oldCell;
            NewCell = newCell;
        }
    }

    public class NavigationWidget : IWidget
    {
        private static readonly Pen defaultMouseoverPen = new Pen(Color.Yellow, 4);

        private readonly MapPanel mapPanel;
        private readonly Size cellSize;

        private bool dragging = false;
        private Point lastMouseLocation;

        public CellMetrics Metrics { get; private set; }

        public Point MouseCell { get; private set; }
        public Point MouseSubPixel { get; private set; }

        private Size mouseoverSize = new Size(1, 1);
        public Size MouseoverSize
        {
            get => mouseoverSize;
            set => mouseoverSize = !value.IsEmpty ? new Size(value.Width | 1, value.Height | 1) : Size.Empty;
        }

        public event EventHandler<MouseCellChangedEventArgs> MouseCellChanged;

        public NavigationWidget(MapPanel mapPanel, CellMetrics metrics, Size cellSize)
        {
            this.mapPanel = mapPanel;
            this.mapPanel.MouseDown += MapPanel_MouseDown;
            this.mapPanel.MouseMove += MapPanel_MouseMove;
            Metrics = metrics;
            this.cellSize = cellSize;
        }

        public void Refresh()
        {
            OnMouseMove(mapPanel.PointToClient(Control.MousePosition));
        }

        private void MapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if ((e.Button & MouseButtons.Middle) != MouseButtons.None)
            {
                lastMouseLocation = e.Location;
                dragging = true;
            }
        }

        private void MapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            OnMouseMove(e.Location);
        }

        private void OnMouseMove(Point location)
        {
            if (dragging)
            {
                if ((Control.MouseButtons & MouseButtons.Middle) == MouseButtons.None)
                {
                    dragging = false;
                }
                else
                {
                    var delta = location - (Size)lastMouseLocation;
                    if (!delta.IsEmpty)
                    {
                        mapPanel.AutoScrollPosition = new Point(-mapPanel.AutoScrollPosition.X - delta.X, -mapPanel.AutoScrollPosition.Y - delta.Y);
                    }
                }
            }

            lastMouseLocation = location;

            var newMousePosition = mapPanel.ClientToMap(location);
            MouseSubPixel = new Point(
                (newMousePosition.X * Globals.PixelWidth / cellSize.Width) % Globals.PixelWidth,
                (newMousePosition.Y * Globals.PixelHeight / cellSize.Height) % Globals.PixelHeight
            );

            var newMouseCell = new Point(newMousePosition.X / cellSize.Width, newMousePosition.Y / cellSize.Height);
            if (MouseCell == newMouseCell)
            {
                return;
            }

            if (!Metrics.Contains(newMouseCell))
            {
                return;
            }

            var oldCell = MouseCell;
            MouseCell = newMouseCell;

            MouseCellChanged?.Invoke(this, new MouseCellChangedEventArgs(oldCell, MouseCell));

            mapPanel.Invalidate();
        }

        public void Render(Graphics graphics)
        {
            if (!MouseoverSize.IsEmpty)
            {
                var rect = new Rectangle(new Point(MouseCell.X * cellSize.Width, MouseCell.Y * cellSize.Height), cellSize);
                rect.Inflate(cellSize.Width * (MouseoverSize.Width / 2), cellSize.Height * (MouseoverSize.Height / 2));
                graphics.DrawRectangle(defaultMouseoverPen, rect);
            }
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    mapPanel.MouseDown -= MapPanel_MouseDown;
                    mapPanel.MouseMove -= MapPanel_MouseMove;
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}
