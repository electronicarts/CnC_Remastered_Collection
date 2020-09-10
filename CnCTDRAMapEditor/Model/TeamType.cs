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
using System.Collections.Generic;
using System.Linq;

namespace MobiusEditor.Model
{
    public class TeamTypeClass : ICloneable
    {
        public ITechnoType Type { get; set; }

        public byte Count { get; set; }

        public TeamTypeClass Clone()
        {
            return new TeamTypeClass()
            {
                Type = Type,
                Count = Count
            };
        }

        object ICloneable.Clone()
        {
            return Clone();
        }
    }

    public class TeamTypeMission : ICloneable
    {
        public string Mission { get; set; }

        public int Argument { get; set; }

        public TeamTypeMission Clone()
        {
            return new TeamTypeMission()
            {
                Mission = Mission,
                Argument = Argument
            };
        }

        object ICloneable.Clone()
        {
            return Clone();
        }
    }

    public class TeamType : INamedType, ICloneable
    {
        public static readonly string None = "None";

        public string Name { get; set; }

        public HouseType House { get; set; }

        public bool IsRoundAbout { get; set; }

        public bool IsLearning { get; set; }

        public bool IsSuicide { get; set; }

        public bool IsAutocreate { get; set; }

        public bool IsMercenary { get; set; }

        public int RecruitPriority { get; set; }

        public byte MaxAllowed { get; set; }

        public byte InitNum { get; set; }

        public byte Fear { get; set; }

        public bool IsReinforcable { get; set; }

        public bool IsPrebuilt { get; set; }

        public int Origin { get; set; }

        public string Trigger { get; set; } = Model.Trigger.None;

        public List<TeamTypeClass> Classes { get; } = new List<TeamTypeClass>();

        public List<TeamTypeMission> Missions { get; } = new List<TeamTypeMission>();

        public TeamType Clone()
        {
            var teamType = new TeamType()
            {
                Name = Name,
                House = House,
                IsRoundAbout = IsRoundAbout,
                IsLearning = IsLearning,
                IsSuicide = IsSuicide,
                IsAutocreate = IsAutocreate,
                IsMercenary = IsMercenary,
                RecruitPriority = RecruitPriority,
                MaxAllowed = MaxAllowed,
                InitNum = InitNum,
                Fear = Fear,
                IsReinforcable = IsReinforcable,
                IsPrebuilt = IsPrebuilt,
                Origin = Origin,
                Trigger = Trigger
            };

            teamType.Classes.AddRange(Classes.Select(c => c.Clone()));
            teamType.Missions.AddRange(Missions.Select(m => m.Clone()));

            return teamType;
        }

        public override bool Equals(object obj)
        {
            if (obj is TeamType)
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
