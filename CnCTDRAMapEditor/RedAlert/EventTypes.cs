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
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.RedAlert
{
    public static class EventTypes
    {
        public const string TEVENT_NONE = "None";
        public const string TEVENT_PLAYER_ENTERED = "Entered by...";
        public const string TEVENT_SPIED = "Spied by...";
        public const string TEVENT_THIEVED = "Thieved by...";
        public const string TEVENT_DISCOVERED = "Discovered by player";
        public const string TEVENT_HOUSE_DISCOVERED = "House Discovered...";
        public const string TEVENT_ATTACKED = "Attacked by anybody";
        public const string TEVENT_DESTROYED = "Destroyed by anybody";
        public const string TEVENT_ANY = "Any Event";
        public const string TEVENT_UNITS_DESTROYED = "Destroyed, Units, All...";
        public const string TEVENT_BUILDINGS_DESTROYED = "Destroyed, Buildings, All...";
        public const string TEVENT_ALL_DESTROYED = "Destroyed, All...";
        public const string TEVENT_CREDITS = "Credits exceed (x100)...";
        public const string TEVENT_TIME = "Elapsed Time (1/10th min)...";
        public const string TEVENT_MISSION_TIMER_EXPIRED = "Mission Timer Expired";
        public const string TEVENT_NBUILDINGS_DESTROYED = "Destroyed, Buildings, #...";
        public const string TEVENT_NUNITS_DESTROYED = "# Destroyed, Units, #...";
        public const string TEVENT_NOFACTORIES = "No Factories left";
        public const string TEVENT_EVAC_CIVILIAN = "Civilians Evacuated";
        public const string TEVENT_BUILD = "Build Building Type...";
        public const string TEVENT_BUILD_UNIT = "Build Unit Type...";
        public const string TEVENT_BUILD_INFANTRY = "Build Infantry Type...";
        public const string TEVENT_BUILD_AIRCRAFT = "Build Aircraft Type...";
        public const string TEVENT_LEAVES_MAP = "Leaves map (team)...";
        public const string TEVENT_ENTERS_ZONE = "Zone Entry by...";
        public const string TEVENT_CROSS_HORIZONTAL = "Crosses Horizontal Line...";
        public const string TEVENT_CROSS_VERTICAL = "Crosses Vertical Line...";
        public const string TEVENT_GLOBAL_SET = "Global is set...";
        public const string TEVENT_GLOBAL_CLEAR = "Global is clear...";
        public const string TEVENT_FAKES_DESTROYED = "Destroyed, Fakes, All...";
        public const string TEVENT_LOW_POWER = "Low Power...";
        public const string TEVENT_ALL_BRIDGES_DESTROYED = "All bridges destroyed";
        public const string TEVENT_BUILDING_EXISTS = "Building exists...";

        private static readonly string[] Types;

        static EventTypes()
        {
            Types =
                (from field in typeof(EventTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsLiteral && !field.IsInitOnly && typeof(string).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as string).ToArray();
        }

        public static IEnumerable<string> GetTypes()
        {
            return Types;
        }
    }
}
