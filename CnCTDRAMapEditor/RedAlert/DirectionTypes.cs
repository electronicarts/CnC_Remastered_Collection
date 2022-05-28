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
    public static class DirectionTypes
    {
        public static readonly DirectionType North = new DirectionType(0, "North", FacingType.North);
        public static readonly DirectionType NorthNorthEast = new DirectionType(16, "North-NorthEast");
        public static readonly DirectionType NorthEast = new DirectionType(32, "NorthEast", FacingType.NorthEast);
        public static readonly DirectionType EastNorthEast = new DirectionType(48, "East-NorthEast");
        public static readonly DirectionType East = new DirectionType(64, "East", FacingType.East);
        public static readonly DirectionType EastSouthEast = new DirectionType(80, "East-SouthEast");
        public static readonly DirectionType SouthEast = new DirectionType(96, "SouthEast", FacingType.SouthEast);
        public static readonly DirectionType SouthSouthEast = new DirectionType(112, "South-SouthEast");
        public static readonly DirectionType South = new DirectionType(128, "South", FacingType.South);
        public static readonly DirectionType SouthSouthWest = new DirectionType(144, "South-SouthWest");
        public static readonly DirectionType SouthWest = new DirectionType(160, "SouthWest", FacingType.SouthWest);
        public static readonly DirectionType WestSouthWest = new DirectionType(176, "West-SouthWest");
        public static readonly DirectionType West = new DirectionType(192, "West", FacingType.West);
        public static readonly DirectionType WestNorthWest = new DirectionType(208, "West-NorthWest");
        public static readonly DirectionType NorthWest = new DirectionType(224, "NorthWest", FacingType.NorthWest);
        public static readonly DirectionType NorthNorthWest = new DirectionType(240, "North-NorthWest");

        private static DirectionType[] Types;

        static DirectionTypes()
        {
            Types =
                (from field in typeof(DirectionTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(DirectionType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as DirectionType).ToArray();
        }

        public static IEnumerable<DirectionType> GetTypes()
        {
            return Types;
        }
    }
}
