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
    public static class OverlayTypes
    {
        public static readonly OverlayType Sandbag = new OverlayType(0, "sbag", "TEXT_STRUCTURE_RA_SBAG", OverlayTypeFlag.Wall);
        public static readonly OverlayType Cyclone = new OverlayType(1, "cycl", "TEXT_STRUCTURE_RA_CYCL", OverlayTypeFlag.Wall);
        public static readonly OverlayType Brick = new OverlayType(2, "brik", "TEXT_STRUCTURE_RA_BRIK", OverlayTypeFlag.Wall);
        public static readonly OverlayType Barbwire = new OverlayType(3, "barb", "TEXT_STRUCTURE_RA_BARB", OverlayTypeFlag.Wall);
        public static readonly OverlayType Wood = new OverlayType(4, "wood", "TEXT_STRUCTURE_TD_WOOD", OverlayTypeFlag.Wall);
        public static readonly OverlayType Gold1 = new OverlayType(5, "gold01", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Gold2 = new OverlayType(6, "gold02", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Gold3 = new OverlayType(7, "gold03", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Gold4 = new OverlayType(8, "gold04", OverlayTypeFlag.TiberiumOrGold);
        public static readonly OverlayType Gems1 = new OverlayType(9, "gem01", OverlayTypeFlag.Gems);
        public static readonly OverlayType Gems2 = new OverlayType(10, "gem02", OverlayTypeFlag.Gems);
        public static readonly OverlayType Gems3 = new OverlayType(11, "gem03", OverlayTypeFlag.Gems);
        public static readonly OverlayType Gems4 = new OverlayType(12, "gem04", OverlayTypeFlag.Gems);
        public static readonly OverlayType V12 = new OverlayType(13, "v12", "TEXT_STRUCTURE_RA_CIVILIAN", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V13 = new OverlayType(14, "v13", "TEXT_STRUCTURE_RA_CIVILIAN", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V14 = new OverlayType(15, "v14", "TEXT_STRUCTURE_TITLE_CIV13", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V15 = new OverlayType(16, "v15", "TEXT_STRUCTURE_TITLE_CIV14", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V16 = new OverlayType(17, "v16", "TEXT_STRUCTURE_TITLE_CIV15", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V17 = new OverlayType(18, "v17", "TEXT_STRUCTURE_TITLE_CIV16", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType V18 = new OverlayType(19, "v18", "TEXT_STRUCTURE_TITLE_CIV17", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly OverlayType FlagSpot = new OverlayType(20, "fpls", OverlayTypeFlag.Flag);
        public static readonly OverlayType WoodCrate = new OverlayType(21, "wcrate", OverlayTypeFlag.Crate);
        public static readonly OverlayType SteelCrate = new OverlayType(22, "scrate", OverlayTypeFlag.Crate);
        public static readonly OverlayType Fence = new OverlayType(23, "fenc", "TEXT_STRUCTURE_RA_FENC", OverlayTypeFlag.Wall);
        public static readonly OverlayType WaterCrate = new OverlayType(24, "wwcrate", OverlayTypeFlag.Crate);

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
