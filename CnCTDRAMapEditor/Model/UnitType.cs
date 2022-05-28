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
using MobiusEditor.Utility;
using System;
using System.Drawing;

namespace MobiusEditor.Model
{
    public static class UnitTypeIDMask
    {
        public const sbyte Aircraft   = 1 << 5;
        public const sbyte Vessel     = 1 << 6;
    }

    public class UnitType : ICellOverlapper, ICellOccupier, ITechnoType, IBrowsableType
    {
        public sbyte ID { get; private set; }

        public string Name { get; private set; }

        public string DisplayName { get; private set; }

        public Rectangle OverlapBounds => new Rectangle(-1, -1, 3, 3);

        public bool[,] OccupyMask => new bool[1, 1] { { true } };

        public string OwnerHouse { get; private set; }

        public bool HasTurret { get; private set; }

        public bool IsFixedWing { get; private set; }

        public bool IsUnit => !IsAircraft && !IsVessel;

        public bool IsAircraft => (ID & UnitTypeIDMask.Aircraft) != 0;

        public bool IsVessel => (ID & UnitTypeIDMask.Vessel) != 0;

        public Size RenderSize { get; set; }

        public Image Thumbnail { get; set; }

        public UnitType(sbyte id, string name, string textId, string ownerHouse, bool hasTurret, bool isFixedWing)
        {
            ID = id;
            Name = name;
            DisplayName = Globals.TheGameTextManager[textId];
            OwnerHouse = ownerHouse;
            HasTurret = hasTurret;
            IsFixedWing = isFixedWing;
        }

        public UnitType(sbyte id, string name, string textId, string ownerHouse, bool hasTurret)
            : this(id, name, textId, ownerHouse, hasTurret, false)
        {
        }

        public UnitType(sbyte id, string name, string textId)
            : this(id, name, textId, null, false)
        {
        }

        public UnitType(sbyte id, string name, string textId, string ownerHouse)
            : this(id, name, textId, ownerHouse, false)
        {
        }

        public UnitType(sbyte id, string name, string textId, bool hasTurret)
            : this(id, name, textId, null, hasTurret)
        {
        }

        public override bool Equals(object obj)
        {
            if (obj is UnitType)
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
            if (Globals.TheTilesetManager.GetTileData(theater.Tilesets, Name, 0, out Tile tile))
            {
                RenderSize = new Size(tile.Image.Width / Globals.TileScale, tile.Image.Height / Globals.TileScale);
            }

            var mockUnit = new Unit()
            {
                Type = this,
                House = house,
                Strength = 256,
                Direction = direction
            };
            var unitThumbnail = new Bitmap(Globals.TileWidth * 3, Globals.TileHeight * 3);
            using (var g = Graphics.FromImage(unitThumbnail))
            {
                MapRenderer.Render(gameType, theater, new Point(1, 1), Globals.TileSize, mockUnit).Item2(g);
            }
            Thumbnail = unitThumbnail;
        }
    }
}
