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

namespace MobiusEditor.RedAlert
{
    public static class TeamMissionTypes
    {
        private static readonly string[] Types = new string[]
        {
            "Attack...",
            "Attack Waypoint...",
            "Change Formation to...",
            "Move to waypoint...",
            "Move to Cell...",
            "Guard area (1/10th min)...",
            "Jump to line #...",
            "Attack Tarcom",
            "Unload",
            "Deploy",
            "Follow friendlies",
            "Do this...",
            "Set global...",
            "Invulnerable",
            "Load onto Transport",
            "Spy on bldg @ waypt...",
            "Patrol to waypoint..."
        };

        public static IEnumerable<string> GetTypes()
        {
            return Types;
        }
    }
}
