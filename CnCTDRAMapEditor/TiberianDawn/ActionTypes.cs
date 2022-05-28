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
    public static class ActionTypes
    {
        public const string ACTION_NONE = "None";
        public const string ACTION_WIN = "Win";
        public const string ACTION_LOSE = "Lose";
        public const string ACTION_BEGIN_PRODUCTION = "Production";
        public const string ACTION_CREATE_TEAM = "Create Team";
        public const string ACTION_DESTROY_TEAM = "Dstry Teams";
        public const string ACTION_ALL_HUNT = "All to Hunt";
        public const string ACTION_REINFORCEMENTS = "Reinforce.";
        public const string ACTION_DZ = "DZ at 'Z'";
        public const string ACTION_AIRSTRIKE = "Airstrike";
        public const string ACTION_NUKE = "Nuclear Missile";
        public const string ACTION_ION = "Ion Cannon";
        public const string ACTION_DESTROY_XXXX = "Dstry Trig 'XXXX'";
        public const string ACTION_DESTROY_YYYY = "Dstry Trig 'YYYY'";
        public const string ACTION_DESTROY_ZZZZ = "Dstry Trig 'ZZZZ'";
        public const string ACTION_AUTOCREATE = "Autocreate";
        public const string ACTION_WINLOSE = "Cap=Win/Des=Lose";
        public const string ACTION_ALLOWWIN = "Allow Win";

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
