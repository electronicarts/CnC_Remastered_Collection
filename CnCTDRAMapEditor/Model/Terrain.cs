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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using MobiusEditor.Interface;

namespace MobiusEditor.Model
{
    public class Terrain : ICellOverlapper, ICellOccupier, INotifyPropertyChanged, ICloneable
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private TerrainType type;
        public TerrainType Type { get => type; set => SetField(ref type, value); }

        private int icon;
        public int Icon { get => icon; set => SetField(ref icon, value); }

        public Rectangle OverlapBounds => Type.OverlapBounds;

        public bool[,] OccupyMask => Type.OccupyMask;

        private string trigger = Model.Trigger.None;
        public string Trigger { get => trigger; set => SetField(ref trigger, value); }

        public Color Tint { get; set; } = Color.White;

        public Terrain Clone()
        {
            return new Terrain()
            {
                Type = Type,
                Icon = Icon,
                Trigger = Trigger
            };
        }

        protected bool SetField<T>(ref T field, T value, [CallerMemberName] string propertyName = null)
        {
            if (EqualityComparer<T>.Default.Equals(field, value))
            {
                return false;
            }
            field = value;
            OnPropertyChanged(propertyName);
            return true;
        }

        protected void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        object ICloneable.Clone()
        {
            return Clone();
        }
    }
}
