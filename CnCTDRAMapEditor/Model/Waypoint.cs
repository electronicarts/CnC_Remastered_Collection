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
using MobiusEditor.Interface;
using System;

namespace MobiusEditor.Model
{
    public enum WaypointFlag
    {
        None,
        PlayerStart
    }

    public class Waypoint : INamedType
    {
        public string Name { get; private set; }

        public WaypointFlag Flag { get; private set; }

        public int? Cell { get; set; }

        public Waypoint(string name, WaypointFlag flag)
        {
            Name = name;
            Flag = flag;
        }

        public Waypoint(string name)
            : this(name, WaypointFlag.None)
        {
        }

        public override bool Equals(object obj)
        {
            if (obj is Waypoint)
            {
                return this == obj;
            }
            else if (obj is string)
            {
                return string.Equals(Name, obj as string, StringComparison.OrdinalIgnoreCase);
            }

            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            return Name.GetHashCode();
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
