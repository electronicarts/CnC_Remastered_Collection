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
    public static class InfantryTypes
    {
        public static readonly InfantryType E1 = new InfantryType(0, "e1", "TEXT_UNIT_TITLE_GDI_MINIGUNNER", "Goodguy");
        public static readonly InfantryType E2 = new InfantryType(1, "e2", "TEXT_UNIT_TITLE_GDI_GRENADIER", "Goodguy");
        public static readonly InfantryType E3 = new InfantryType(2, "e3", "TEXT_UNIT_TITLE_GDI_ROCKET_SOLDIER", "Badguy");
        public static readonly InfantryType E4 = new InfantryType(3, "e4", "TEXT_UNIT_TITLE_NOD_FLAMETHROWER", "Badguy");
        public static readonly InfantryType E5 = new InfantryType(4, "e5", "TEXT_UNIT_TITLE_NOD_CHEM_WARRIOR", "Badguy");
        public static readonly InfantryType E7 = new InfantryType(5, "e6", "TEXT_UNIT_TITLE_GDI_ENGINEER", "Goodguy");
        public static readonly InfantryType Commando = new InfantryType(6, "rmbo", "TEXT_UNIT_TITLE_GDI_COMMANDO", "Goodguy");
        public static readonly InfantryType C1 = new InfantryType(7, "c1", "TEXT_UNIT_TITLE_CIV1", "Neutral");
        public static readonly InfantryType C2 = new InfantryType(8, "c2", "TEXT_UNIT_TITLE_CIV2", "Neutral");
        public static readonly InfantryType C3 = new InfantryType(9, "c3", "TEXT_UNIT_TITLE_CIV3", "Neutral");
        public static readonly InfantryType C4 = new InfantryType(10, "c4", "TEXT_UNIT_TITLE_CIV4", "Neutral");
        public static readonly InfantryType C5 = new InfantryType(11, "c5", "TEXT_UNIT_TITLE_CIV5", "Neutral");
        public static readonly InfantryType C6 = new InfantryType(12, "c6", "TEXT_UNIT_TITLE_CIV6", "Neutral");
        public static readonly InfantryType C7 = new InfantryType(13, "c7", "TEXT_UNIT_TITLE_CIV7", "Neutral");
        public static readonly InfantryType C8 = new InfantryType(14, "c8", "TEXT_UNIT_TITLE_CIV8", "Neutral");
        public static readonly InfantryType C9 = new InfantryType(15, "c9", "TEXT_UNIT_TITLE_CIV9", "Neutral");
        public static readonly InfantryType C10 = new InfantryType(16, "c10", "TEXT_UNIT_TITLE_C10", "Neutral");
        public static readonly InfantryType Moebius = new InfantryType(17, "moebius", "TEXT_UNIT_TITLE_MOEBIUS", "Neutral");
        public static readonly InfantryType Delphi = new InfantryType(18, "delphi", "TEXT_UNIT_TITLE_DELPHI", "Neutral");
        public static readonly InfantryType DrChan = new InfantryType(19, "chan", "TEXT_UNIT_TITLE_CHAN", "Neutral");

        private static readonly InfantryType[] Types;

        static InfantryTypes()
        {
            Types =
                (from field in typeof(InfantryTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(InfantryType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as InfantryType).ToArray();
        }

        public static IEnumerable<InfantryType> GetTypes()
        {
            return Types;
        }
    }
}
