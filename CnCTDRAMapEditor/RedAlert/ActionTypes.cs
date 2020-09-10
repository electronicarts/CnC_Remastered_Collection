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
    public static class ActionTypes
    {
        public const string TACTION_NONE = "None";
        public const string TACTION_WIN = "Winner is...";
        public const string TACTION_LOSE = "Loser is...";
        public const string TACTION_BEGIN_PRODUCTION = "Production Begins";
        public const string TACTION_CREATE_TEAM = "Create Team...";
        public const string TACTION_DESTROY_TEAM = "Destroy All Teams";
        public const string TACTION_ALL_HUNT = "All to Hunt...";
        public const string TACTION_REINFORCEMENTS = "Reinforcement (team)...";
        public const string TACTION_DZ = "Drop Zone Flare (waypoint)...";
        public const string TACTION_FIRE_SALE = "Fire Sale...";
        public const string TACTION_PLAY_MOVIE = "Play Movie...";
        public const string TACTION_TEXT_TRIGGER = "Text Trigger (ID num)...";
        public const string TACTION_DESTROY_TRIGGER = "Destroy Trigger...";
        public const string TACTION_AUTOCREATE = "Autocreate Begins...";
        public const string TACTION_WINLOSE = "";
        public const string TACTION_ALLOWWIN = "Allow Win";
        public const string TACTION_REVEAL_ALL = "Reveal all map";
        public const string TACTION_REVEAL_SOME = "Reveal around waypoint...";
        public const string TACTION_REVEAL_ZONE = "Reveal zone of waypoint...";
        public const string TACTION_PLAY_SOUND = "Play sound effect...";
        public const string TACTION_PLAY_MUSIC = "Play music theme...";
        public const string TACTION_PLAY_SPEECH = "Play speech...";
        public const string TACTION_FORCE_TRIGGER = "Force Trigger...";
        public const string TACTION_START_TIMER = "Timer Start";
        public const string TACTION_STOP_TIMER = "Timer Stop";
        public const string TACTION_ADD_TIMER = "Timer Extend (1/10th min)...";
        public const string TACTION_SUB_TIMER = "Timer Shorten (1/10th min)...";
        public const string TACTION_SET_TIMER = "Timer Set (1/10th min)...";
        public const string TACTION_SET_GLOBAL = "Global Set...";
        public const string TACTION_CLEAR_GLOBAL = "Global Clear...";
        public const string TACTION_BASE_BUILDING = "Auto Base Building...";
        public const string TACTION_CREEP_SHADOW = "Grow shroud one 'step'";
        public const string TACTION_DESTROY_OBJECT = "Destroy attached building";
        public const string TACTION_1_SPECIAL = "Add 1-time special weapon...";
        public const string TACTION_FULL_SPECIAL = "Add repeating special weapon...";
        public const string TACTION_PREFERRED_TARGET = "Preferred target...";
        public const string TACTION_LAUNCH_NUKES = "Launch Nukes";

        private static readonly string[] Types;

        static ActionTypes()
        {
            Types =
                (from field in typeof(ActionTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsLiteral && !field.IsInitOnly && typeof(string).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as string).ToArray();
        }

        public static IEnumerable<string> GetTypes()
        {
            return Types;
        }
    }
}
