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

namespace MobiusEditor.RedAlert
{
    public class TerrainTypes
    {
        public static readonly TerrainType Tree1Class = new TerrainType(0, "t01", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree2Class = new TerrainType(1, "t02", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree3Class = new TerrainType(2, "t03", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree5Class = new TerrainType(3, "t05", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree6Class = new TerrainType(4, "t06", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree7Class = new TerrainType(5, "t07", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree8Class = new TerrainType(6, "t08", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Tree10Class = new TerrainType(7, "t10", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree11Class = new TerrainType(8, "t11", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree12Class = new TerrainType(9, "t12", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree13Class = new TerrainType(10, "t13", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree14Class = new TerrainType(11, "t14", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree15Class = new TerrainType(12, "t15", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, true } });
        public static readonly TerrainType Tree16Class = new TerrainType(13, "t16", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Tree17Class = new TerrainType(14, "t17", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 2] { { false, false }, { true, false } });
        public static readonly TerrainType Clump1Class = new TerrainType(15, "tc01", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 3] { { false, false, false }, { true, true, false } });
        public static readonly TerrainType Clump2Class = new TerrainType(16, "tc02", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 3] { { false, true, false }, { true, true, false } });
        public static readonly TerrainType Clump3Class = new TerrainType(17, "tc03", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[2, 3] { { true, true, false }, { true, true, false } });
        public static readonly TerrainType Clump4Class = new TerrainType(18, "tc04", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[3, 4] { { false, false, false, false }, { true, true, true, false }, { true, false, false, false } });
        public static readonly TerrainType Clump5Class = new TerrainType(19, "tc05", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[3, 4] { { false, false, true, false }, { true, true, true, false }, { false, true, true, false } });
        public static readonly TerrainType Ice01 = new TerrainType(20, "ice01", new TheaterType[] { TheaterTypes.Snow }, new bool[2, 2] { { true, true }, { true, true } }, TemplateTypeFlag.Water);
        public static readonly TerrainType Ice02 = new TerrainType(21, "ice02", new TheaterType[] { TheaterTypes.Snow }, new bool[2, 2] { { true, false }, { true, false } }, TemplateTypeFlag.Water);
        public static readonly TerrainType Ice03 = new TerrainType(22, "ice03", new TheaterType[] { TheaterTypes.Snow }, new bool[1, 2] { { true, true } }, TemplateTypeFlag.Water);
        public static readonly TerrainType Ice04 = new TerrainType(23, "ice04", new TheaterType[] { TheaterTypes.Snow }, new bool[1, 2] { { true, false } }, TemplateTypeFlag.Water);
        public static readonly TerrainType Ice05 = new TerrainType(24, "ice05", new TheaterType[] { TheaterTypes.Snow }, new bool[1, 2] { { true, false } }, TemplateTypeFlag.Water);
        public static readonly TerrainType Boxes01 = new TerrainType(25, "boxes01", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes02 = new TerrainType(26, "boxes02", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes03 = new TerrainType(27, "boxes03", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes04 = new TerrainType(28, "boxes04", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes05 = new TerrainType(29, "boxes05", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes06 = new TerrainType(30, "boxes06", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes07 = new TerrainType(31, "boxes07", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes08 = new TerrainType(32, "boxes08", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Boxes09 = new TerrainType(33, "boxes09", new TheaterType[] { TheaterTypes.Interior }, new bool[1, 2] { { true, false } });
        public static readonly TerrainType Mine = new TerrainType(34, "mine", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow }, new bool[1, 2] { { true, false } }, TemplateTypeFlag.OreMine);


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
