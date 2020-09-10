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

namespace MobiusEditor.TiberianDawn
{
    public static class EventTypes
    {
        public const string EVENT_NONE = "None";
        public const string EVENT_PLAYER_ENTERED = "Player Enters";
        public const string EVENT_DISCOVERED = "Discovered";
        public const string EVENT_ATTACKED = "Attacked";
        public const string EVENT_DESTROYED = "Destroyed";
        public const string EVENT_ANY = "Any";
        public const string EVENT_HOUSE_DISCOVERED = "House Discov.";
        public const string EVENT_UNITS_DESTROYED = "Units Destr.";
        public const string EVENT_BUILDINGS_DESTROYED = "Bldgs Destr.";
        public const string EVENT_ALL_DESTROYED = "All Destr.";
        public const string EVENT_CREDITS = "Credits";
        public const string EVENT_TIME = "Time";
        public const string EVENT_NBUILDINGS_DESTROYED = "# Bldgs Dstr.";
        public const string EVENT_NUNITS_DESTROYED = "# Units Dstr.";
        public const string EVENT_NOFACTORIES = "No Factories";
        public const string EVENT_EVAC_CIVILIAN = "Civ. Evac.";
        public const string EVENT_BUILD = "Built It";

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
