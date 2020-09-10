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
    public enum TriggerPersistantType
    {
        Volatile = 0,
        SemiPersistant = 1,
        Persistant = 2
    }

    public enum TriggerMultiStyleType
    {
        Only = 0,
        And = 1,
        Or = 2,
        Linked = 3
    }

    public class TriggerEvent : ICloneable
    {
        public static readonly string None = "None";

        public string EventType { get; set; }

        public string Team { get; set; }

        public long Data { get; set; }

        public TriggerEvent Clone()
        {
            return new TriggerEvent()
            {
                EventType = EventType,
                Team = Team,
                Data = Data
            };
        }

        object ICloneable.Clone()
        {
            return Clone();
        }
    }

    public class TriggerAction : ICloneable
    {
        public static readonly string None = "None";

        public string ActionType { get; set; }

        public string Trigger { get; set; }

        public string Team { get; set; }

        public long Data { get; set; }

        public TriggerAction Clone()
        {
            return new TriggerAction()
            {
                ActionType = ActionType,
                Trigger = Trigger,
                Team = Team,
                Data = Data
            };
        }

        object ICloneable.Clone()
        {
            return Clone();
        }
    }

    public class Trigger : INamedType, ICloneable
    {
        public static readonly string None = "None";

        public string Name { get; set; }

        public TriggerPersistantType PersistantType { get; set; } = TriggerPersistantType.Volatile;

        public string House { get; set; }

        public TriggerMultiStyleType EventControl { get; set; } = TriggerMultiStyleType.Only;

        public TriggerEvent Event1 { get; private set; } = new TriggerEvent { EventType = TriggerEvent.None };

        public TriggerEvent Event2 { get; private set; } = new TriggerEvent { EventType = TriggerEvent.None };

        public TriggerAction Action1 { get; private set; } = new TriggerAction { ActionType = TriggerEvent.None };

        public TriggerAction Action2 { get; private set; } = new TriggerAction { ActionType = TriggerEvent.None };

        public Trigger Clone()
        {
            return new Trigger()
            {
                Name = Name,
                PersistantType = PersistantType,
                House = House,
                EventControl = EventControl,
                Event1 = Event1.Clone(),
                Event2 = Event2.Clone(),
                Action1 = Action1.Clone(),
                Action2 = Action2.Clone()
            };
        }

        public override bool Equals(object obj)
        {
            if (obj is Trigger)
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

        object ICloneable.Clone()
        {
            return Clone();
        }
    }
}
