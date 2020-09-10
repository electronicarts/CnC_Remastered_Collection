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
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;

namespace MobiusEditor.Model
{
    public class Building : ICellOverlapper, ICellOccupier, INotifyPropertyChanged, ICloneable
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private BuildingType type;
        public BuildingType Type { get => type; set => SetField(ref type, value); }

        public Rectangle OverlapBounds => Type.OverlapBounds;

        public bool[,] OccupyMask => Type.OccupyMask;

        private HouseType house;
        public HouseType House { get => house; set => SetField(ref house, value); }

        private int strength;
        public int Strength { get => strength; set => SetField(ref strength, value); }

        private DirectionType direction;
        public DirectionType Direction { get => direction; set => SetField(ref direction, value); }

        private string trigger = Model.Trigger.None;
        public string Trigger { get => trigger; set => SetField(ref trigger, value); }

        private int basePriority = -1;
        public int BasePriority { get => basePriority; set => SetField(ref basePriority, value); }

        private bool isPrebuilt = true;
        public bool IsPrebuilt { get => isPrebuilt; set => SetField(ref isPrebuilt, value); }

        private bool sellable;
        public bool Sellable { get => sellable; set => SetField(ref sellable, value); }

        private bool rebuild;
        public bool Rebuild { get => rebuild; set => SetField(ref rebuild, value); }

        public ISet<int> BibCells { get; private set; } = new HashSet<int>();

        private Color tint = Color.White;
        public Color Tint
        {
            get => IsPrebuilt ? tint : Color.FromArgb((int)(tint.A * 0.75f), tint.R, tint.G, tint.B);
            set => tint = value;
        }

        public Building Clone()
        {
            return new Building()
            {
                Type = Type,
                House = House,
                Strength = Strength,
                Direction = Direction,
                Trigger = Trigger,
                BasePriority = BasePriority,
                IsPrebuilt = IsPrebuilt,
                Sellable = Sellable,
                Rebuild = Rebuild,
                Tint = Tint
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
