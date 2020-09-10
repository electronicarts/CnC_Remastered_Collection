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
using MobiusEditor.Interface;
using MobiusEditor.Utility;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;

namespace MobiusEditor.Model
{
    public class OverlapperSet<T> : IEnumerable<(Point Location, T Overlapper)>, IEnumerable where T : class, ICellOverlapper
    {
        private readonly CellMetrics metrics;
        private readonly Dictionary<T, Rectangle> overlappers = new Dictionary<T, Rectangle>();

        public Rectangle? this[T overlapper] => Contains(overlapper) ? overlappers[overlapper] : default;

        public IEnumerable<T> Overlappers => overlappers.Keys;

        public OverlapperSet(CellMetrics metrics)
        {
            this.metrics = metrics;
        }

        public bool Add(Point location, T overlapper)
        {
            if ((overlapper == null) || Contains(overlapper))
            {
                return false;
            }

            var rectangle = overlapper.OverlapBounds;
            rectangle.Offset(location);
            overlappers[overlapper] = rectangle;
            return true;
        }

        public bool Add(int x, int y, T occupier) => Add(new Point(x, y), occupier);

        public bool Add(int cell, T overlapper) => metrics.GetLocation(cell, out Point location) ? Add(location, overlapper) : false;

        public void Clear() => overlappers.Clear();

        public bool Contains(T occupier) => overlappers.ContainsKey(occupier);

        public void CopyTo(OverlapperSet<T> other)
        {
            foreach (var (Location, Occupier) in this)
            {
                other.Add(Location, Occupier);
            }
        }

        public IEnumerator<(Point Location, T Overlapper)> GetEnumerator() => overlappers.Select(kv => (kv.Value.Location, kv.Key)).GetEnumerator();

        public bool Remove(T overlapper)
        {
            if ((overlapper == null) || !overlappers.TryGetValue(overlapper, out Rectangle overlapRect))
            {
                return false;
            }

            overlappers.Remove(overlapper);
            return true;
        }

        public ISet<Point> Overlaps(IEnumerable<Rectangle> rectangles)
        {
            var rectangleSet = new HashSet<Rectangle>(rectangles);
            while (true)
            {
                var count = rectangleSet.Count;
                var overlap = overlappers.Values.Where(x => rectangleSet.Any(y => x.IntersectsWith(y))).ToArray();
                rectangleSet.UnionWith(overlap);
                if (rectangleSet.Count == count)
                {
                    break;
                }
            }

            return rectangleSet.SelectMany(x => x.Points()).ToHashSet();
        }

        public ISet<Point> Overlaps(Rectangle rectangle) => Overlaps(rectangle.Yield());

        public ISet<Point> Overlaps(IEnumerable<Point> points) => Overlaps(points.Select(p => new Rectangle(p, new Size(1, 1))));

        public ISet<Point> Overlaps(Point point) => Overlaps(point.Yield());

        public IEnumerable<(Point Location, U Overlapper)> OfType<U>() where U : T => this.Where(i => i.Overlapper is U).Select(i => (i.Location, (U)i.Overlapper));

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}
