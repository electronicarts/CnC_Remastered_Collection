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

namespace MobiusEditor.RedAlert
{
    public static class BuildingTypes
    {
        public static readonly BuildingType AdvancedTech = new BuildingType(0, "atek", "TEXT_STRUCTURE_RA_ATEK", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType IronCurtain = new BuildingType(1, "iron", "TEXT_STRUCTURE_RA_IRON", new bool[2, 2] { { false, false }, { true, true } }, false, "USSR");
        public static readonly BuildingType Weapon = new BuildingType(2, "weap", "TEXT_STRUCTURE_RA_WEAP", new bool[2, 3] { { true, true, true }, { true, true, true } }, true, "Greece", false, false, "weap2");
        public static readonly BuildingType Chronosphere = new BuildingType(3, "pdox", "TEXT_STRUCTURE_RA_PDOX", new bool[2, 2] { { true, true }, { true, true } }, false, "Greece");
        public static readonly BuildingType Pillbox = new BuildingType(4, "pbox", "TEXT_STRUCTURE_RA_PBOX", new bool[1, 1] { { true } }, false, "Greece");
        public static readonly BuildingType CamoPillbox = new BuildingType(5, "hbox", "TEXT_STRUCTURE_RA_HBOX", new bool[1, 1] { { true } }, false, "Greece");
        public static readonly BuildingType Command = new BuildingType(6, "dome", "TEXT_STRUCTURE_RA_DOME", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType GapGenerator = new BuildingType(7, "gap", "TEXT_STRUCTURE_RA_GAP", new bool[2, 1] { { false }, { true } }, false, "Greece");
        public static readonly BuildingType Turret = new BuildingType(8, "gun", "TEXT_STRUCTURE_RA_GUN", new bool[1, 1] { { true } }, false, "Greece", false, true);
        public static readonly BuildingType AAGun = new BuildingType(9, "agun", "TEXT_STRUCTURE_RA_AGUN", new bool[2, 1] { { false }, { true } }, false, "Greece", false, true);
        public static readonly BuildingType FlameTurret = new BuildingType(10, "ftur", "TEXT_STRUCTURE_RA_FTUR", new bool[1, 1] { { true } }, false, "USSR");
        public static readonly BuildingType Const = new BuildingType(11, "fact", "TEXT_STRUCTURE_RA_FACT", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, true, "Greece");
        public static readonly BuildingType Refinery = new BuildingType(12, "proc", "TEXT_STRUCTURE_RA_PROC", new bool[3, 3] { { false, true, false }, { true, true, true }, { true, false, false } }, true, "Greece");
        public static readonly BuildingType Storage = new BuildingType(13, "silo", "TEXT_STRUCTURE_RA_SILO", new bool[1, 1] { { true } }, false, "Greece");
        public static readonly BuildingType Helipad = new BuildingType(14, "hpad", "TEXT_STRUCTURE_RA_HPAD", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType SAM = new BuildingType(15, "sam", "TEXT_STRUCTURE_RA_SAM", new bool[1, 2] { { true, true } }, false, "USSR");
        public static readonly BuildingType AirStrip = new BuildingType(16, "afld", "TEXT_STRUCTURE_RA_AFLD", new bool[2, 3] { { true, true, true }, { true, true, true } }, false, "USSR");
        public static readonly BuildingType Power = new BuildingType(17, "powr", "TEXT_STRUCTURE_RA_POWR", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType AdvancedPower = new BuildingType(18, "apwr", "TEXT_STRUCTURE_RA_APWR", new bool[3, 3] { { false, false, false }, { true, true, true }, { true, true, true } }, true, "Greece");
        public static readonly BuildingType SovietTech = new BuildingType(19, "stek", "TEXT_STRUCTURE_RA_STEK", new bool[3, 3] { { false, false, false }, { true, true, true }, { true, true, true } }, true, "USSR");
        public static readonly BuildingType Hospital = new BuildingType(20, "hosp", "TEXT_STRUCTURE_RA_HOSP", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType Barracks = new BuildingType(21, "barr", "TEXT_STRUCTURE_RA_BARR", new bool[2, 2] { { true, true }, { true, true } }, true, "USSR");
        public static readonly BuildingType Tent = new BuildingType(22, "tent", "TEXT_STRUCTURE_RA_TENT", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType Kennel = new BuildingType(23, "kenn", "TEXT_STRUCTURE_RA_KENN", new bool[1, 1] { { true } }, false, "USSR");
        public static readonly BuildingType Repair = new BuildingType(24, "fix", "TEXT_STRUCTURE_RA_FIX", new bool[3, 3] { { false, true, false }, { true, true, true }, { false, true, false } }, false, "Greece");
        public static readonly BuildingType BioLab = new BuildingType(25, "bio", "TEXT_STRUCTURE_RA_BIO", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece");
        public static readonly BuildingType Mission = new BuildingType(26, "miss", "TEXT_STRUCTURE_RA_MISS", new bool[2, 3] { { true, true, true }, { true, true, true } }, true, "Greece");
        public static readonly BuildingType ShipYard = new BuildingType(27, "syrd", "TEXT_STRUCTURE_RA_SYRD", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, false, "Greece");
        public static readonly BuildingType SubPen = new BuildingType(28, "spen", "TEXT_STRUCTURE_RA_SPEN", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, false, "USSR");
        public static readonly BuildingType MissileSilo = new BuildingType(29, "mslo", "TEXT_STRUCTURE_RA_MSLO", new bool[1, 2] { { true, true } }, false, "Greece", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType ForwardCom = new BuildingType(30, "fcom", "TEXT_STRUCTURE_RA_FCOM", new bool[2, 2] { { false, false }, { true, true } }, true, "USSR");
        public static readonly BuildingType Tesla = new BuildingType(31, "tsla", "TEXT_STRUCTURE_RA_TSLA", new bool[2, 1] { { false }, { true } }, false, "USSR");
        public static readonly BuildingType FakeWeapon = new BuildingType(32, "weap", "TEXT_STRUCTURE_RA_WEAF", new bool[2, 3] { { true, true, true }, { true, true, true } }, true, "Greece", true, false, "weap2");
        public static readonly BuildingType FakeConst = new BuildingType(33, "fact", "TEXT_STRUCTURE_RA_FACF", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, true, "Greece", true);
        public static readonly BuildingType FakeShipYard = new BuildingType(34, "syrd", "TEXT_STRUCTURE_RA_SYRF", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, false, "Greece", true);
        public static readonly BuildingType FakeSubPen = new BuildingType(35, "spen", "TEXT_STRUCTURE_RA_SPEF", new bool[3, 3] { { true, true, true }, { true, true, true }, { true, true, true } }, false, "USSR", true);
        public static readonly BuildingType FakeCommand = new BuildingType(36, "dome", "TEXT_STRUCTURE_RA_DOMF", new bool[2, 2] { { true, true }, { true, true } }, true, "Greece", true);
        public static readonly BuildingType AVMine = new BuildingType(43, "minv", "TEXT_STRUCTURE_RA_MINV", new bool[1, 1] { { true } }, false, "Greece");
        public static readonly BuildingType APMine = new BuildingType(44, "minp", "TEXT_STRUCTURE_RA_MINP", new bool[1, 1] { { true } }, false, "Greece");
        public static readonly BuildingType V01 = new BuildingType(45, "v01", "TEXT_STRUCTURE_TITLE_CIV1", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V02 = new BuildingType(46, "v02", "TEXT_STRUCTURE_TITLE_CIV2", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V03 = new BuildingType(47, "v03", "TEXT_STRUCTURE_TITLE_CIV3", new bool[2, 2] { { false, true }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V04 = new BuildingType(48, "v04", "TEXT_STRUCTURE_TITLE_CIV4", new bool[2, 2] { { false, false }, { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V05 = new BuildingType(49, "v05", "TEXT_STRUCTURE_TITLE_CIV5", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V06 = new BuildingType(50, "v06", "TEXT_STRUCTURE_TITLE_CIV6", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V07 = new BuildingType(51, "v07", "TEXT_STRUCTURE_TITLE_CIV7", new bool[1, 2] { { true, true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V08 = new BuildingType(52, "v08", "TEXT_STRUCTURE_TITLE_CIV8", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V09 = new BuildingType(53, "v09", "TEXT_STRUCTURE_TITLE_CIV9", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V10 = new BuildingType(54, "v10", "TEXT_STRUCTURE_TITLE_CIV10", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V11 = new BuildingType(55, "v11", "TEXT_STRUCTURE_TITLE_CIV11", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V12 = new BuildingType(56, "v12", "TEXT_STRUCTURE_RA_CIVILIAN", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V13 = new BuildingType(57, "v13", "TEXT_STRUCTURE_RA_CIVILIAN", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V14 = new BuildingType(58, "v14", "TEXT_STRUCTURE_TITLE_CIV13", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V15 = new BuildingType(59, "v15", "TEXT_STRUCTURE_TITLE_CIV14", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V16 = new BuildingType(60, "v16", "TEXT_STRUCTURE_TITLE_CIV15", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V17 = new BuildingType(61, "v17", "TEXT_STRUCTURE_TITLE_CIV16", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V18 = new BuildingType(62, "v18", "TEXT_STRUCTURE_TITLE_CIV17", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType V19 = new BuildingType(63, "v19", "TEXT_STRUCTURE_RA_CIVILIAN", new bool[1, 1] { { true } }, false, "Neutral", new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Snow });
        public static readonly BuildingType Barrel = new BuildingType(82, "barl", "TEXT_STRUCTURE_RA_BARL", new bool[1, 1] { { true } }, false, "Neutral");
        public static readonly BuildingType Barrel3 = new BuildingType(83, "brl3", "TEXT_STRUCTURE_RA_BRL3", new bool[1, 1] { { true } }, false, "Neutral");
        public static readonly BuildingType Queen = new BuildingType(84, "quee", "TEXT_STRUCTURE_RA_QUEE", new bool[1, 2] { { true, true } }, false, "Special");
        public static readonly BuildingType Larva1 = new BuildingType(85, "lar1", "TEXT_STRUCTURE_RA_LAR1", new bool[1, 1] { { true } }, false, "Special");
        public static readonly BuildingType Larva2 = new BuildingType(86, "lar2", "TEXT_STRUCTURE_RA_LAR2", new bool[1, 1] { { true } }, false, "Special");

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
