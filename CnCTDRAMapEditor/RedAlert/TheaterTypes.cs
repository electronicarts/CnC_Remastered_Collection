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
using MobiusEditor.Utility;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.RedAlert
{
    public static class TheaterTypes
    {
        private static readonly IEnumerable<string> commonTilesets = new string[] { "RA_Units", "RA_Structures", "RA_VFX", "Common_VFX" };

        public static readonly TheaterType Temperate = new TheaterType(0, "temperate", "RA_Terrain_Temperate".Yield().Concat(commonTilesets));
        public static readonly TheaterType Snow = new TheaterType(1, "snow", "RA_Terrain_Snow".Yield().Concat(commonTilesets));
        public static readonly TheaterType Interior = new TheaterType(2, "interior", "RA_Terrain_Interior".Yield().Concat(commonTilesets));

        private static TheaterType[] Types;

        static TheaterTypes()
        {
            Types =
                (from field in typeof(TheaterTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(TheaterType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as TheaterType).ToArray();
        }

        public static IEnumerable<TheaterType> GetTypes()
        {
            return Types;
        }
    }
}
