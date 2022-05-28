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
using MobiusEditor.Model;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.TiberianDawn
{
    public static class TerrainTypes
    {
        public static readonly TerrainType Tree1 = new TerrainType(0, "t01", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree2 = new TerrainType(1, "t02", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree3 = new TerrainType(2, "t03", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree4 = new TerrainType(3, "t04", new TheaterType[] { TheaterTypes.Desert }, new bool[1, 1] { { true } });
        public static readonly TerrainType Tree5 = new TerrainType(4, "t05", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree6 = new TerrainType(5, "t06", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree7 = new TerrainType(6, "t07", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree8 = new TerrainType(7, "t08", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Tree9 = new TerrainType(8, "t09", new TheaterType[] { TheaterTypes.Desert }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Tree10 = new TerrainType(9, "t10", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree11 = new TerrainType(10, "t11", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree12 = new TerrainType(11, "t12", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree13 = new TerrainType(12, "t13", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree14 = new TerrainType(13, "t14", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree15 = new TerrainType(14, "t15", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree16 = new TerrainType(15, "t16", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree17 = new TerrainType(16, "t17", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree18 = new TerrainType(17, "t18", new TheaterType[] { TheaterTypes.Desert }, new bool[2, 3] { { false, false, false }, { false, true, false } });
        public static readonly TerrainType Split1 = new TerrainType(18, "split2", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter }, new bool[2, 2] { { false, false }, { true, false } }, true);
        public static readonly TerrainType Split2 = new TerrainType(19, "split3", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter, TheaterTypes.Desert }, new bool[2, 2] { { false, false }, { true, false } }, true);
        public static readonly TerrainType Clump1 = new TerrainType(20, "tc01", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 3] { { false, false, false }, { true, true, false } });
        public static readonly TerrainType Clump2 = new TerrainType(21, "tc02", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 3] { { false, true, false }, { true, true, false } });
        public static readonly TerrainType Clump3 = new TerrainType(22, "tc03", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[2, 3] { { true, true, false }, { true, true, false } });
        public static readonly TerrainType Clump4 = new TerrainType(23, "tc04", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[3, 4] { { false, false, false, false }, { true, true, true, false }, { true, false, false, false } });
        public static readonly TerrainType Clump5 = new TerrainType(24, "tc05", new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate }, new bool[3, 4] { { false, false, true, false }, { true, true, true, false }, { false, true, true, false } });
        public static readonly TerrainType Rock1 = new TerrainType(25, "rock1", new TheaterType[] { TheaterTypes.Desert }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Rock2 = new TerrainType(26, "rock2", new TheaterType[] { TheaterTypes.Desert }, new bool[2, 2] { { true, true }, { false, false } });
        public static readonly TerrainType Rock3 = new TerrainType(27, "rock3", new TheaterType[] { TheaterTypes.Desert }, new bool[2, 3] { { false, false, false }, { true, true, false } });
        public static readonly TerrainType Rock4 = new TerrainType(28, "rock4", new TheaterType[] { TheaterTypes.Desert }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Rock5 = new TerrainType(29, "rock5", new TheaterType[] { TheaterTypes.Desert }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Rock6 = new TerrainType(30, "rock6", new TheaterType[] { TheaterTypes.Desert }, new bool[2, 3] { { false, false, false }, { true, true, true } });
        public static readonly TerrainType Rock7 = new TerrainType(31, "rock7", new TheaterType[] { TheaterTypes.Desert }, new bool[1, 5] { { true, true, true, true, false } });

        private static TerrainType[] Types;

        static TerrainTypes()
        {
            Types =
                (from field in typeof(TerrainTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(TerrainType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as TerrainType).ToArray();
        }

        public static IEnumerable<TerrainType> GetTypes()
        {
            return Types;
        }
    }
}
