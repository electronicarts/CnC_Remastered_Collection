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
    public static class UnitTypes
    {
        public static readonly UnitType HTank = new UnitType(0, "4tnk", "TEXT_UNIT_RA_4TNK", "USSR", true);
        public static readonly UnitType MTank = new UnitType(1, "3tnk", "TEXT_UNIT_RA_3TNK", "USSR", true);
        public static readonly UnitType MTank2 = new UnitType(2, "2tnk", "TEXT_UNIT_RA_2TNK", "Greece", true);
        public static readonly UnitType LTank = new UnitType(3, "1tnk", "TEXT_UNIT_RA_1TNK", "Greece", true);
        public static readonly UnitType APC = new UnitType(4, "apc", "TEXT_UNIT_RA_APC", "Greece");
        public static readonly UnitType MineLayer = new UnitType(5, "mnly", "TEXT_UNIT_RA_MNLY", "Greece");
        public static readonly UnitType Jeep = new UnitType(6, "jeep", "TEXT_UNIT_RA_JEEP", "Greece", true);
        public static readonly UnitType Harvester = new UnitType(7, "harv", "TEXT_UNIT_RA_HARV", "Greece");
        public static readonly UnitType Arty = new UnitType(8, "arty", "TEXT_UNIT_RA_ARTY", "Greece");
        public static readonly UnitType MRJammer = new UnitType(9, "mrj", "TEXT_UNIT_RA_MRJ", "Greece");
        public static readonly UnitType MGG = new UnitType(10, "mgg", "TEXT_UNIT_RA_MGG", "Greece");
        public static readonly UnitType MCV = new UnitType(11, "mcv", "TEXT_UNIT_RA_MCV", "Greece");
        public static readonly UnitType V2Launcher = new UnitType(12, "v2rl", "TEXT_UNIT_RA_V2RL", "USSR");
        public static readonly UnitType ConvoyTruck = new UnitType(13, "truk", "TEXT_UNIT_RA_TRUK", "Greece");
        public static readonly UnitType Ant1 = new UnitType(14, "ant1", "TEXT_UNIT_RA_ANT1", "Special");
        public static readonly UnitType Ant2 = new UnitType(15, "ant2", "TEXT_UNIT_RA_ANT2", "Special");
        public static readonly UnitType Ant3 = new UnitType(16, "ant3", "TEXT_UNIT_RA_ANT3", "Special");
        public static readonly UnitType Chrono = new UnitType(17, "ctnk", "TEXT_UNIT_RA_CTNK", "Greece");
        public static readonly UnitType Tesla = new UnitType(18, "ttnk", "TEXT_UNIT_RA_TTNK", "USSR");
        public static readonly UnitType MAD = new UnitType(19, "qtnk", "TEXT_UNIT_RA_QTNK", "Greece");
        public static readonly UnitType DemoTruck = new UnitType(20, "dtrk", "TEXT_UNIT_RA_DTRK", "USSR");
        public static readonly UnitType Phase = new UnitType(21, "stnk", "TEXT_UNIT_RA_STNK", "Greece", true);

        public static readonly UnitType Tran = new UnitType(0 | UnitTypeIDMask.Aircraft, "tran", "TEXT_UNIT_RA_TRAN", "USSR");
        public static readonly UnitType Badr = new UnitType(1 | UnitTypeIDMask.Aircraft, "badr", "TEXT_UNIT_RA_BADR", "USSR", false, true);
        public static readonly UnitType U2 = new UnitType(2 | UnitTypeIDMask.Aircraft, "u2", "TEXT_UNIT_RA_U2", "USSR", false, true);
        public static readonly UnitType Mig = new UnitType(3 | UnitTypeIDMask.Aircraft, "mig", "TEXT_UNIT_RA_MIG", "USSR", false, true);
        public static readonly UnitType Yak = new UnitType(4 | UnitTypeIDMask.Aircraft, "yak", "TEXT_UNIT_RA_YAK", "USSR", false, true);
        public static readonly UnitType Heli = new UnitType(5 | UnitTypeIDMask.Aircraft, "heli", "TEXT_UNIT_RA_HELI", "Greece");
        public static readonly UnitType Hind = new UnitType(6 | UnitTypeIDMask.Aircraft, "hind", "TEXT_UNIT_RA_HIND", "USSR");

        public static readonly UnitType Submarine = new UnitType(0 | UnitTypeIDMask.Vessel, "ss", "TEXT_UNIT_RA_SS", "USSR");
        public static readonly UnitType Destroyer = new UnitType(1 | UnitTypeIDMask.Vessel, "dd", "TEXT_UNIT_RA_DD", "Greece", true);
        public static readonly UnitType Cruiser = new UnitType(2 | UnitTypeIDMask.Vessel, "ca", "TEXT_UNIT_RA_CA", "Greece", true);
        public static readonly UnitType Transport = new UnitType(3 | UnitTypeIDMask.Vessel, "lst", "TEXT_UNIT_RA_LST", "Greece");
        public static readonly UnitType PTBoat = new UnitType(4 | UnitTypeIDMask.Vessel, "pt", "TEXT_UNIT_RA_PT", "Greece", true);
        public static readonly UnitType MissileSubmarine = new UnitType(5 | UnitTypeIDMask.Vessel, "msub", "TEXT_UNIT_RA_MSUB", "USSR");
        public static readonly UnitType Carrier = new UnitType(6 | UnitTypeIDMask.Vessel, "carr", "TEXT_UNIT_RA_CARR", "Greece");

        private static readonly UnitType[] Types;

        static UnitTypes()
        {
            Types =
                (from field in typeof(UnitTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(UnitType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as UnitType).ToArray();
        }

        public static IEnumerable<UnitType> GetTypes()
        {
            return Types;
        }
    }
}
