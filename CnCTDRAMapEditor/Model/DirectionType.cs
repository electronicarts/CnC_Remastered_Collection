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
using System;

namespace MobiusEditor.Model
{
    public class DirectionType
    {
        public byte ID { get; private set; }

        public string Name { get; private set; }

        public FacingType Facing { get; private set; }

        public DirectionType(byte id, string name, FacingType facing)
        {
            ID = id;
            Name = name;
            Facing = facing;
        }

        public DirectionType(byte id, string name)
            : this(id, name, FacingType.None)
        {
        }

        public override bool Equals(object obj)
        {
            if (obj is DirectionType)
            {
                return this == obj;
            }
            else if (obj is byte)
            {
                return ID == (byte)obj;
            }
            else if (obj is string)
            {
                return string.Equals(Name, obj as string, StringComparison.OrdinalIgnoreCase);
            }
            else if (obj is FacingType)
            {
                return Facing == (FacingType)obj;
            }

            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
