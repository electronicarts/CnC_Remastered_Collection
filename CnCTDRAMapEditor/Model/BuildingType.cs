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
using MobiusEditor.Render;
using System;
using System.Drawing;

namespace MobiusEditor.Model
{
    public class BuildingType : ICellOverlapper, ICellOccupier, ITechnoType, IBrowsableType
    {
        public sbyte ID { get; private set; }

        public string Name { get; private set; }

        public string DisplayName { get; private set; }

        public string Tilename { get; private set; }

        public Rectangle OverlapBounds => new Rectangle(Point.Empty, new Size(OccupyMask.GetLength(1), OccupyMask.GetLength(0)));

        public bool[,] OccupyMask { get; private set; }

        public bool[,] BaseOccupyMask { get; private set; }

        public Size Size { get; private set; }

        public bool HasBib { get; private set; }

        public string OwnerHouse { get; private set; }

        public TheaterType[] Theaters { get; private set; }

        public bool IsFake { get; private set; }

        public bool HasTurret { get; private set; }

        public string FactoryOverlay { get; private set; }

        public Image Thumbnail { get; set; }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse, TheaterType[] theaters, bool isFake, bool hasTurret, string factoryOverlay)
        {
            ID = id;
            Name = isFake ? (name.Substring(0, name.Length - 1) + "f") : name;
            DisplayName = Globals.TheGameTextManager[textId];
            Tilename = name;
            BaseOccupyMask = occupyMask;
            Size = new Size(BaseOccupyMask.GetLength(1), BaseOccupyMask.GetLength(0));
            HasBib = hasBib;
            OwnerHouse = ownerHouse;
            Theaters = theaters;
            IsFake = isFake;
            HasTurret = hasTurret;
            FactoryOverlay = factoryOverlay;

            if (HasBib)
            {
                OccupyMask = new bool[BaseOccupyMask.GetLength(0) + 1, BaseOccupyMask.GetLength(1)];
                for (var i = 0; i < BaseOccupyMask.GetLength(0) - 1; ++i)
                {
                    for (var j = 0; j < BaseOccupyMask.GetLength(1); ++j)
                    {
                        OccupyMask[i, j] = BaseOccupyMask[i, j];
                    }
                }
                for (var j = 0; j < OccupyMask.GetLength(1); ++j)
                {
                    OccupyMask[OccupyMask.GetLength(0) - 2, j] = true;
                    OccupyMask[OccupyMask.GetLength(0) - 1, j] = true;
                }
            }
            else
            {
                OccupyMask = BaseOccupyMask;
            }
        }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse, bool isFake, bool hasTurret, string factoryOverlay)
            : this(id, name, textId, occupyMask, hasBib, ownerHouse, null, isFake, hasTurret, factoryOverlay)
        {
        }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse)
            : this(id, name, textId, occupyMask, hasBib, ownerHouse, null, false, false, null)
        {
        }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse, TheaterType[] theaters)
            : this(id, name, textId, occupyMask, hasBib, ownerHouse, theaters, false, false, null)
        {
        }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse, bool isFake)
            : this(id, name, textId, occupyMask, hasBib, ownerHouse, null, isFake, false, null)
        {
        }

        public BuildingType(sbyte id, string name, string textId, bool[,] occupyMask, bool hasBib, string ownerHouse, bool isFake, bool hasTurret)
            : this(id, name, textId, occupyMask, hasBib, ownerHouse, null, isFake, hasTurret, null)
        {
        }

        public override bool Equals(object obj)
        {
            if (obj is BuildingType)
            {
                return this == obj;
            }
            else if (obj is sbyte)
            {
                return ID == (sbyte)obj;
            }
            else if (obj is string)
            {
                return string.Equals(Name, obj as string, StringComparison.OrdinalIgnoreCase);
            }

            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public override string ToString()
        {
            return Name;
        }

        public void Init(GameType gameType, TheaterType theater, HouseType house, DirectionType direction)
        {
            var mockBuilding = new Building()
            {
                Type = this,
                House = house,
                Strength = 256,
                Direction = direction
            };

            var render = MapRenderer.Render(gameType, theater, Point.Empty, Globals.TileSize, Globals.TileScale, mockBuilding);
            if (!render.Item1.IsEmpty)
            {
                var buildingPreview = new Bitmap(render.Item1.Width, render.Item1.Height);
                using (var g = Graphics.FromImage(buildingPreview))
                {
                    render.Item2(g);
                }
                Thumbnail = buildingPreview;
            }
        }
    }
}
