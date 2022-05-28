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
using MobiusEditor.Model;
using MobiusEditor.Utility;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Controls
{
    public partial class TerrainProperties : UserControl
    {
        private bool isMockObject;

        public IGamePlugin Plugin { get; private set; }

        private Terrain terrain;
        public Terrain Terrain
        {
            get => terrain;
            set
            {
                if (terrain != value)
                {
                    terrain = value;
                    Rebind();
                }
            }
        }

        public TerrainProperties()
        {
            InitializeComponent();
        }

        public void Initialize(IGamePlugin plugin, bool isMockObject)
        {
            this.isMockObject = isMockObject;

            Plugin = plugin;
            plugin.Map.Triggers.CollectionChanged += Triggers_CollectionChanged;

            UpdateDataSource();

            Disposed += (sender, e) =>
            {
                Terrain = null;
                plugin.Map.Triggers.CollectionChanged -= Triggers_CollectionChanged;
            };
        }

        private void Triggers_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            UpdateDataSource();
        }

        private void UpdateDataSource()
        {
            triggerComboBox.DataSource = Trigger.None.Yield().Concat(Plugin.Map.Triggers.Select(t => t.Name).Distinct()).ToArray();
        }

        private void Rebind()
        {
            triggerComboBox.DataBindings.Clear();

            if (terrain == null)
            {
                return;
            }

            triggerComboBox.DataBindings.Add("SelectedItem", terrain, "Trigger");
        }

        private void Obj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "Type":
                    {
                        Rebind();
                    }
                    break;
            }

            if (!isMockObject)
            {
                Plugin.Dirty = true;
            }
        }

        private void comboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            foreach (Binding binding in (sender as ComboBox).DataBindings)
            {
                binding.WriteValue();
            }
        }

        private void nud_ValueChanged(object sender, EventArgs e)
        {
            foreach (Binding binding in (sender as NumericUpDown).DataBindings)
            {
                binding.WriteValue();
            }
        }
    }

    public class TerrainPropertiesPopup : ToolStripDropDown
    {
        private readonly ToolStripControlHost host;

        public TerrainProperties TerrainProperties { get; private set; }

        public TerrainPropertiesPopup(IGamePlugin plugin, Terrain terrain)
        {
            TerrainProperties = new TerrainProperties();
            TerrainProperties.Initialize(plugin, false);
            TerrainProperties.Terrain = terrain;

            host = new ToolStripControlHost(TerrainProperties);
            Padding = Margin = host.Padding = host.Margin = Padding.Empty;
            MinimumSize = TerrainProperties.MinimumSize;
            TerrainProperties.MinimumSize = TerrainProperties.Size;
            MaximumSize = TerrainProperties.MaximumSize;
            TerrainProperties.MaximumSize = TerrainProperties.Size;
            Size = TerrainProperties.Size;
            Items.Add(host);
            TerrainProperties.Disposed += (sender, e) =>
            {
                TerrainProperties = null;
                Dispose(true);
            };
        }

        protected override void OnClosed(ToolStripDropDownClosedEventArgs e)
        {
            base.OnClosed(e);

            TerrainProperties.Terrain = null;
        }
    }
}
