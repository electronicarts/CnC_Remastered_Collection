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
using MobiusEditor.Utility;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;

namespace MobiusEditor.Model
{
    public struct AlliesMask : IEnumerable<int>
    {
        public int Value { get; private set; }

        public AlliesMask(int value)
        {
            Value = value;
        }

        public void Set(int id)
        {
            if ((id < 0) || (id > 31))
            {
                throw new ArgumentOutOfRangeException();
            }

            Value |= (1 << id);
        }

        public void Clear(int id)
        {
            if ((id < 0) || (id > 31))
            {
                throw new ArgumentOutOfRangeException();
            }

            Value &= ~(1 << id);
        }

        public override bool Equals(object obj)
        {
            return Value.Equals(obj);
        }

        public override int GetHashCode()
        {
            return Value.GetHashCode();
        }

        public override string ToString()
        {
            return Value.ToString();
        }

        public IEnumerator<int> GetEnumerator()
        {
            for (int i = 0, mask = 1; i < 32; ++i, mask <<= 1)
            {
                if ((Value & mask) != 0)
                {
                    yield return i;
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }

    public class AlliesMaskTypeConverter : TypeConverter
    {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            return (context is MapContext) && (sourceType == typeof(string));
        }

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            return (context is MapContext) && (destinationType == typeof(string));
        }

        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
        {
            if (!(value is AlliesMask) || !CanConvertTo(context, destinationType))
            {
                return null;
            }

            var map = (context as MapContext).Map;
            var alliesMask = (AlliesMask)value;

            var allies = new List<string>(map.Houses.Length);
            foreach (var id in alliesMask)
            {
                if (map.Houses.Where(h => h.Type.Equals((sbyte)id)).FirstOrDefault() is House house)
                {
                    allies.Add(house.Type.Name);
                }
            }

            return string.Join(",", allies);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (!CanConvertFrom(context, value?.GetType()))
            {
                return null;
            }

            var map = (context as MapContext).Instance as Map;
            var alliesMask = new AlliesMask(0);

            var allies = (value as string).Split(',');
            foreach (var ally in allies)
            {
                if (map.Houses.Where(h => h.Type.Equals(ally)).FirstOrDefault() is House house)
                {
                    alliesMask.Set(house.Type.ID);
                }
            }

            return alliesMask;
        }
    }

    public class House
    {
        public readonly HouseType Type;

        [NonSerializedINIKey]
        [DefaultValue(true)]
        public bool Enabled { get; set; }

        [TypeConverter(typeof(AlliesMaskTypeConverter))]
        public AlliesMask Allies { get; set; }

        [DefaultValue(150)]
        public int MaxBuilding { get; set; }

        [DefaultValue(150)]
        public int MaxUnit { get; set; }

        [DefaultValue("North")]
        public string Edge { get; set; }

        [DefaultValue(0)]
        public int Credits { get; set; } = 0;

        public House(HouseType type)
        {
            Type = type;
            Allies = new AlliesMask(1 << Type.ID);
        }
    }
}
