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
using System.Drawing;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.TiberianDawn
{
    public static class BuildingTypes
    {
        public static readonly BuildingType Weapon = new BuildingType(0, "weap", "TEXT_STRUCTURE_TITLE_GDI_WEAPONS_FACTORY", new bool[3, 3] { { false, false, false }, { true, true, true }, { true, true, true } }, true, "Goodguy", false, false, "weap2");
        public static readonly BuildingType GTower = new BuildingType(1, "gtwr", "TEXT_STRUCTURE_TITLE_GDI_GUARD_TOWER", new bool[1, 1] { { true } }, false, "Goodguy");
        public static readonly BuildingType ATower = new BuildingType(2, "atwr", "TEXT_STRUCTURE_TITLE_GDI_ADV_GUARD_TOWER", new bool[2, 1] { { false }, { true } }, false, "Goodguy");
        public static readonly BuildingType Obelisk = new BuildingType(3, "obli", "TEXT_STRUCTURE_TITLE_NOD_OBELISK", new bool[2, 1] { { false }, { true } }, false, "Badguy");
        public static readonly BuildingType Command = new BuildingType(4, "hq", "TEXT_STRUCTURE_TITLE_GDI_COMM_CENTER", new bool[2, 2] { { true, false }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType Turret = new BuildingType(5, "gun", "TEXT_STRUCTURE_TITLE_NOD_TURRET", new bool[1, 1] { { true } }, false, "Badguy", false, true);
        public static readonly BuildingType Const = new BuildingType(6, "fact", "TEXT_STRUCTURE_RA_FACT", new bool[2, 3] { { true, true, true }, { true, true, true } }, true, "Goodguy");
        public static readonly BuildingType Refinery = new BuildingType(7, "proc", "TEXT_STRUCTURE_TITLE_GDI_REFINERY", new bool[3, 3] { { false, true, false }, { true, true, true }, { false, false, false } }, true, "Goodguy");
        public static readonly BuildingType Storage = new BuildingType(8, "silo", "TEXT_STRUCTURE_TITLE_GDI_SILO", new bool[1, 2] { { true, true } }, true, "Goodguy");
        public static readonly BuildingType Helipad = new BuildingType(9, "hpad", "TEXT_STRUCTURE_TITLE_GDI_HELIPAD", new bool[2, 2] { { true, true }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType SAM = new BuildingType(10, "sam", "TEXT_STRUCTURE_TITLE_NOD_SAM_SITE", new bool[1, 2] { { true, true } }, false, "Badguy");
        public static readonly BuildingType AirStrip = new BuildingType(11, "afld", "TEXT_STRUCTURE_TITLE_NOD_AIRFIELD", new bool[2, 4] { { true, true, true, true }, { true, true, true, true } }, true, "Badguy");
        public static readonly BuildingType Power = new BuildingType(12, "nuke", "TEXT_STRUCTURE_TITLE_GDI_POWER_PLANT", new bool[2, 2] { { true, false }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType AdvancedPower = new BuildingType(13, "nuk2", "TEXT_STRUCTURE_TITLE_GDI_ADV_POWER_PLANT", new bool[2, 2] { { true, false }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType Hospital = new BuildingType(14, "hosp", "TEXT_UNIT_TITLE_HOSP", new bool[2, 2] { { true, true }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType Barracks = new BuildingType(15, "pyle", "TEXT_STRUCTURE_TITLE_GDI_BARRACKS", new bool[2, 2] { { true, true }, { false, false } }, true, "Goodguy");
        public static readonly BuildingType Tanker = new BuildingType(16, "arco", "TEXT_UNIT_TITLE_ARCO", new bool[1, 2] { { true, true } }, false, "Goodguy");
        public static readonly BuildingType Repair = new BuildingType(17, "fix", "TEXT_STRUCTURE_TITLE_GDI_REPAIR_FACILITY", new bool[3, 3] { { false, true, false }, { true, true, true }, { false, true, false} }, true, "Goodguy");
        public static readonly BuildingType BioLab = new BuildingType(18, "bio", "TEXT_UNIT_TITLE_BIO", new bool[2, 2] { { true, true }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType Hand = new BuildingType(19, "hand", "TEXT_STRUCTURE_TITLE_NOD_HAND_OF_NOD", new bool[3, 2] { { false, false }, { true, true }, { false, true } }, true, "Badguy");
        public static readonly BuildingType Temple = new BuildingType(20, "tmpl", "TEXT_STRUCTURE_TITLE_NOD_TEMPLE_OF_NOD", new bool[3, 3] { { false, false, false }, { true, true, true }, { true, true, true } }, true, "Badguy");
        public static readonly BuildingType Eye = new BuildingType(21, "eye", "TEXT_STRUCTURE_TITLE_GDI_ADV_COMM_CENTER", new bool[2, 2] { { true, false }, { true, true } }, true, "Goodguy");
        public static readonly BuildingType Mission = new BuildingType(22, "miss", "TEXT_STRUCTURE_TITLE_CIV35", new bool[2, 3] { { true, true, true }, { true, true, true } }, true, "Goodguy");
        public static readonly BuildingType V01 = new BuildingType(23, "v01", "TEXT_STRUCTURE_TITLE_CIV1", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V02 = new BuildingType(24, "v02", "TEXT_STRUCTURE_TITLE_CIV2", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V03 = new BuildingType(25, "v03", "TEXT_STRUCTURE_TITLE_CIV3", new bool[2, 2] { { false, true }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V04 = new BuildingType(26, "v04", "TEXT_STRUCTURE_TITLE_CIV4", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V05 = new BuildingType(27, "v05", "TEXT_STRUCTURE_TITLE_CIV5", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V06 = new BuildingType(28, "v06", "TEXT_STRUCTURE_TITLE_CIV6", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V07 = new BuildingType(29, "v07", "TEXT_STRUCTURE_TITLE_CIV7", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V08 = new BuildingType(30, "v08", "TEXT_STRUCTURE_TITLE_CIV8", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V09 = new BuildingType(31, "v09", "TEXT_STRUCTURE_TITLE_CIV9", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V10 = new BuildingType(32, "v10", "TEXT_STRUCTURE_TITLE_CIV10", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V11 = new BuildingType(33, "v11", "TEXT_STRUCTURE_TITLE_CIV11", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V12 = new BuildingType(34, "v12", "TEXT_STRUCTURE_TITLE_CIV12", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate });
        public static readonly BuildingType V13 = new BuildingType(35, "v13", "TEXT_STRUCTURE_TITLE_CIV12", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate });
        public static readonly BuildingType V14 = new BuildingType(36, "v14", "TEXT_STRUCTURE_TITLE_CIV13", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V15 = new BuildingType(37, "v15", "TEXT_STRUCTURE_TITLE_CIV14", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V16 = new BuildingType(38, "v16", "TEXT_STRUCTURE_TITLE_CIV15", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V17 = new BuildingType(39, "v17", "TEXT_STRUCTURE_TITLE_CIV16", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V18 = new BuildingType(40, "v18", "TEXT_STRUCTURE_TITLE_CIV17", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly BuildingType V19 = new BuildingType(41, "v19", "TEXT_STRUCTURE_CIVILIAN_TITLE", new bool[1, 1] { { true } }, false, "Neutral");
        public static readonly BuildingType V20 = new BuildingType(42, "v20", "TEXT_STRUCTURE_TITLE_CIV18", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V21 = new BuildingType(43, "v21", "TEXT_STRUCTURE_TITLE_CIV19", new bool[2, 2] { { true, true }, { false, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V22 = new BuildingType(44, "v22", "TEXT_STRUCTURE_TITLE_CIV20", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V23 = new BuildingType(45, "v23", "TEXT_STRUCTURE_TITLE_CIV21", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V24 = new BuildingType(46, "v24", "TEXT_STRUCTURE_TITLE_CIV22", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V25 = new BuildingType(47, "v25", "TEXT_STRUCTURE_TITLE_CIV1", new bool[2, 2] { { false, true }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V26 = new BuildingType(48, "v26", "TEXT_STRUCTURE_TITLE_CIV23", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V27 = new BuildingType(49, "v27", "TEXT_STRUCTURE_TITLE_CIV24", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V28 = new BuildingType(50, "v28", "TEXT_STRUCTURE_TITLE_CIV25", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V29 = new BuildingType(51, "v29", "TEXT_STRUCTURE_TITLE_CIV26", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V30 = new BuildingType(52, "v30", "TEXT_STRUCTURE_TITLE_CIV27", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V31 = new BuildingType(53, "v31", "TEXT_STRUCTURE_TITLE_CIV28", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V32 = new BuildingType(54, "v32", "TEXT_STRUCTURE_TITLE_CIV29", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V33 = new BuildingType(55, "v33", "TEXT_STRUCTURE_TITLE_CIV30", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V34 = new BuildingType(56, "v34", "TEXT_STRUCTURE_TITLE_CIV31", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V35 = new BuildingType(57, "v35", "TEXT_STRUCTURE_TITLE_CIV32", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V36 = new BuildingType(58, "v36", "TEXT_STRUCTURE_TITLE_CIV33", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });
        public static readonly BuildingType V37 = new BuildingType(59, "v37", "TEXT_STRUCTURE_TITLE_CIV34", new bool[2, 4] { { false, true, true, true }, { false, true, true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Desert });

        private static readonly BuildingType[] Types;

        static BuildingTypes()
        {
            Types =
                (from field in typeof(BuildingTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(BuildingType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as BuildingType).ToArray();
        }

        public static IEnumerable<BuildingType> GetTypes()
        {
            return Types;
        }
    }
}
