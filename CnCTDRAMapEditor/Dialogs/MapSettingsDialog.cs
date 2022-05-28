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
using MobiusEditor.Controls;
using MobiusEditor.Interface;
using MobiusEditor.Model;
using MobiusEditor.Utility;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace MobiusEditor.Dialogs
{
    public partial class MapSettingsDialog : Form
    {
        private const int TVIF_STATE = 0x8;
        private const int TVIS_STATEIMAGEMASK = 0xF000;
        private const int TV_FIRST = 0x1100;
        private const int TVM_SETITEM = TV_FIRST + 63;

        [DllImport("user32.dll")]
        static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        private struct TVITEM
        {
            public int mask;
            public IntPtr hItem;
            public int state;
            public int stateMask;
            [MarshalAs(UnmanagedType.LPTStr)]
            public String lpszText;
            public int cchTextMax;
            public int iImage;
            public int iSelectedImage;
            public int cChildren;
            public IntPtr lParam;
        }

        private readonly IGamePlugin plugin;
        private readonly PropertyTracker<BasicSection> basicSettingsTracker;
        private readonly PropertyTracker<BriefingSection> briefingSettingsTracker;
        private readonly IDictionary<House, PropertyTracker<House>> houseSettingsTrackers;
        private readonly TreeNode playersNode;

        public MapSettingsDialog(IGamePlugin plugin, PropertyTracker<BasicSection> basicSettingsTracker, PropertyTracker<BriefingSection> briefingSettingsTracker,
            IDictionary<House, PropertyTracker<House>> houseSettingsTrackers)
        {
            InitializeComponent();

            this.plugin = plugin;
            this.basicSettingsTracker = basicSettingsTracker;
            this.briefingSettingsTracker = briefingSettingsTracker;
            this.houseSettingsTrackers = houseSettingsTrackers;

            settingsTreeView.BeginUpdate();
            settingsTreeView.Nodes.Clear();
            settingsTreeView.Nodes.Add("BASIC", "Basic");
            settingsTreeView.Nodes.Add("BRIEFING", "Briefing");

            playersNode = settingsTreeView.Nodes.Add("Players");
            foreach (var player in plugin.Map.Houses)
            {
                var playerNode = playersNode.Nodes.Add(player.Type.Name, player.Type.Name);
                playerNode.Checked = player.Enabled;
            }
            playersNode.Expand();

            settingsTreeView.EndUpdate();
            settingsTreeView.SelectedNode = settingsTreeView.Nodes[0];
        }

        private void settingsTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            settingsPanel.Controls.Clear();

            switch (settingsTreeView.SelectedNode.Name)
            {
                case "BASIC":
                    {
                        settingsPanel.Controls.Add(new BasicSettings(plugin, basicSettingsTracker));
                    }
                    break;
                case "BRIEFING":
                    {
                        settingsPanel.Controls.Add(new BriefingSettings(plugin, briefingSettingsTracker));
                    }
                    break;
                default:
                    {
                        var player = plugin.Map.Houses.Where(h => h.Type.Name == settingsTreeView.SelectedNode.Name).FirstOrDefault();
                        if (player != null)
                        {
                            settingsPanel.Controls.Add(new PlayerSettings(plugin, houseSettingsTrackers[player]));
                        }
                    }
                    break;
            }
        }

        private void settingsTreeView_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            if (!playersNode.Nodes.Contains(e.Node))
            {
                HideCheckBox(e.Node);
                e.DrawDefault = true;
            }
            else
            {
                e.Graphics.DrawString(e.Node.Text, e.Node.TreeView.Font, new SolidBrush(settingsTreeView.ForeColor), e.Node.Bounds.X, e.Node.Bounds.Y);
            }
        }

        private void HideCheckBox(TreeNode node)
        {
            TVITEM tvi = new TVITEM
            {
                hItem = node.Handle,
                mask = TVIF_STATE,
                stateMask = TVIS_STATEIMAGEMASK,
                state = 0,
                lpszText = null,
                cchTextMax = 0,
                iImage = 0,
                iSelectedImage = 0,
                cChildren = 0,
                lParam = IntPtr.Zero
            };
            IntPtr lparam = Marshal.AllocHGlobal(Marshal.SizeOf(tvi));
            Marshal.StructureToPtr(tvi, lparam, false);
            SendMessage(node.TreeView.Handle, TVM_SETITEM, IntPtr.Zero, lparam);
        }

        private void settingsTreeView_AfterCheck(object sender, TreeViewEventArgs e)
        {
            var player = plugin.Map.Houses.Where(h => h.Type.Name == e.Node.Name).FirstOrDefault();
            if (player != null)
            {
                ((dynamic)houseSettingsTrackers[player]).Enabled = e.Node.Checked;
            }
        }
    }
}
