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
    public static class SmudgeTypes
    {
        public static readonly SmudgeType Crater1 = new SmudgeType(0, "cr1");
        public static readonly SmudgeType Crater2 = new SmudgeType(1, "cr2");
        public static readonly SmudgeType Crater3 = new SmudgeType(2, "cr3");
        public static readonly SmudgeType Crater4 = new SmudgeType(3, "cr4");
        public static readonly SmudgeType Crater5 = new SmudgeType(4, "cr5");
        public static readonly SmudgeType Crater6 = new SmudgeType(5, "cr6");
        public static readonly SmudgeType Scorch1 = new SmudgeType(6, "sc1");
        public static readonly SmudgeType Scorch2 = new SmudgeType(7, "sc2");
        public static readonly SmudgeType Scorch3 = new SmudgeType(8, "sc3");
        public static readonly SmudgeType Scorch4 = new SmudgeType(9, "sc4");
        public static readonly SmudgeType Scorch5 = new SmudgeType(10, "sc5");
        public static readonly SmudgeType Scorch6 = new SmudgeType(11, "sc6");
        public static readonly SmudgeType Bib1 = new SmudgeType(12, "bib1", new Size(4, 2), SmudgeTypeFlag.Bib1);
        public static readonly SmudgeType Bib2 = new SmudgeType(13, "bib2", new Size(3, 2), SmudgeTypeFlag.Bib2);
        public static readonly SmudgeType Bib3 = new SmudgeType(14, "bib3", new Size(2, 2), SmudgeTypeFlag.Bib3);

        private static SmudgeType[] Types;

        static SmudgeTypes()
        {
            Types =
                (from field in typeof(SmudgeTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(SmudgeType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as SmudgeType).ToArray();
        }

        public static IEnumerable<SmudgeType> GetTypes()
        {
            return Types;
        }
    }
}
