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
    public static class OverlayTypes
    {
        public static readonly OverlayType Sandbag = new OverlayType(1, "sbag", "TEXT_STRUCTURE_TITLE_GDI_SANDBAGS", OverlayTypeFlag.Wall);
        public static readonly OverlayType Cyclone = new OverlayType(2, "cycl", "TEXT_STRUCTURE_TITLE_GDI_CHAIN_LINK", OverlayTypeFlag.Wall);
        public static readonly OverlayType Brick = new OverlayType(3, "brik", "TEXT_STRUCTURE_TITLE_GDI_CONCRETE", OverlayTypeFlag.Wall);
        public static readonly OverlayType Barbwire = new OverlayType(4, "barb", "TEXT_STRUCTURE_RA_BARB", OverlayTypeFlag.Wall);
        public static readonly OverlayType Wood = new OverlayType(5, "wood", "TEXT_STRUCTURE_TD_WOOD", OverlayTypeFlag.Wall);
        public static readonly OverlayType Tiberium1 = new OverlayType(6, "ti1", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium2 = new OverlayType(7, "ti2", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium3 = new OverlayType(8, "ti3", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium4 = new OverlayType(9, "ti4", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium5 = new OverlayType(10, "ti5", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium6 = new OverlayType(11, "ti6", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium7 = new OverlayType(12, "ti7", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium8 = new OverlayType(13, "ti8", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium9 = new OverlayType(14, "ti9", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium10 = new OverlayType(15, "ti10", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium11 = new OverlayType(16, "ti11", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Tiberium12 = new OverlayType(17, "ti12", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType V12 = new OverlayType(20, "v12", "TEXT_STRUCTURE_TITLE_CIV12", new TheaterType[] { TheaterTypes.Temperate });
        public static readonly OverlayType V13 = new OverlayType(21, "v13", "TEXT_STRUCTURE_TITLE_CIV12", new TheaterType[] { TheaterTypes.Temperate });
        public static readonly OverlayType V14 = new OverlayType(22, "v14", "TEXT_STRUCTURE_TITLE_CIV13", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly OverlayType V15 = new OverlayType(23, "v15", "TEXT_STRUCTURE_TITLE_CIV14", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly OverlayType V16 = new OverlayType(24, "v16", "TEXT_STRUCTURE_TITLE_CIV15", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly OverlayType V17 = new OverlayType(25, "v17", "TEXT_STRUCTURE_TITLE_CIV16", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly OverlayType V18 = new OverlayType(26, "v18", "TEXT_STRUCTURE_TITLE_CIV17", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly OverlayType FlagSpot = new OverlayType(27, "fpls", OverlayTypeFlag.Flag);
        public static readonly OverlayType WoodCrate = new OverlayType(28, "wcrate", OverlayTypeFlag.Crate);
        public static readonly OverlayType SteelCrate = new OverlayType(29, "scrate", OverlayTypeFlag.Crate);

        private static OverlayType[] Types;

        static OverlayTypes()
        {
            Types =
                (from field in typeof(OverlayTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(OverlayType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as OverlayType).ToArray();
        }

        public static IEnumerable<OverlayType> GetTypes()
        {
            return Types;
        }
    }
}
