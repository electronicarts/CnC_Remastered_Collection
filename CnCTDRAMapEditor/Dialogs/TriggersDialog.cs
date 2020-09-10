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
using System.Data;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Dialogs
{
    public partial class TriggersDialog : Form
    {
        private readonly IGamePlugin plugin;
        private readonly int maxTriggers;

        private readonly List<Trigger> triggers;
        public IEnumerable<Trigger> Triggers => triggers;

        private ListViewItem SelectedItem => (triggersListView.SelectedItems.Count > 0) ? triggersListView.SelectedItems[0] : null;

        private Trigger SelectedTrigger => SelectedItem?.Tag as Trigger;

        public TriggersDialog(IGamePlugin plugin, int maxTriggers)
        {
            this.plugin = plugin;
            this.maxTriggers = maxTriggers;

            InitializeComponent();

            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    existenceLabel.Text = "Loop";
                    event1Label.Text = "Event";
                    action1Label.Text = "Action";
                    typeLabel.Visible = typeComboBox.Visible = false;
                    event2Label.Visible = event2ComboBox.Visible = event2Flp.Visible = false;
                    action2Label.Visible = action2ComboBox.Visible = action2Flp.Visible = false;
                    break;
                case GameType.RedAlert:
                    teamLabel.Visible = teamComboBox.Visible = false;
                    break;
            }

            triggers = new List<Trigger>(plugin.Map.Triggers.Select(t => t.Clone()));

            triggersListView.BeginUpdate();
            {
                foreach (var trigger in triggers)
                {
                    var item = new ListViewItem(trigger.Name)
                    {
                        Tag = trigger
                    };
                    triggersListView.Items.Add(item).ToolTipText = trigger.Name;
                }
            }
            triggersListView.EndUpdate();

            string[] existenceNames = Enum.GetNames(typeof(TriggerPersistantType));
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    existenceNames = new string[] { "No", "And", "Or" };
                    break;
                case GameType.RedAlert:
                    existenceNames = new string[] { "Temporary", "Semi-Constant", "Constant" };
                    break;
            }

            string[] typeNames = new string[]
            {
                "E => A1 [+ A2]",
                "E1 && E2 => A1 [+ A2]",
                "E1 || E2 => A1 [+ A2]",
                "E1 => A1; E2 => A2",
            };

            houseComboBox.DataSource = "None".Yield().Concat(plugin.Map.Houses.Select(t => t.Type.Name)).ToArray();
            existenceComboBox.DataSource = Enum.GetValues(typeof(TriggerPersistantType)).Cast<int>()
                .Select(v => new { Name = existenceNames[v], Value = (TriggerPersistantType)v })
                .ToArray();
            typeComboBox.DataSource = Enum.GetValues(typeof(TriggerMultiStyleType)).Cast<int>()
                .Select(v => new { Name = typeNames[v], Value = (TriggerMultiStyleType)v })
                .ToArray();
            event1ComboBox.DataSource = plugin.Map.EventTypes.Where(t => !string.IsNullOrEmpty(t)).ToArray();
            event2ComboBox.DataSource = plugin.Map.EventTypes.Where(t => !string.IsNullOrEmpty(t)).ToArray();
            action1ComboBox.DataSource = plugin.Map.ActionTypes.Where(t => !string.IsNullOrEmpty(t)).ToArray();
            action2ComboBox.DataSource = plugin.Map.ActionTypes.Where(t => !string.IsNullOrEmpty(t)).ToArray();
            teamComboBox.DataSource = "None".Yield().Concat(plugin.Map.TeamTypes.Select(t => t.Name)).ToArray();

            triggersTableLayoutPanel.Visible = false;
        }

        private void triggersListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            houseComboBox.DataBindings.Clear();
            existenceComboBox.DataBindings.Clear();
            typeComboBox.DataBindings.Clear();
            event1ComboBox.DataBindings.Clear();
            event2ComboBox.DataBindings.Clear();
            action1ComboBox.DataBindings.Clear();
            action2ComboBox.DataBindings.Clear();
            teamComboBox.DataBindings.Clear();

            if (SelectedTrigger != null)
            {
                houseComboBox.DataBindings.Add("SelectedItem", SelectedTrigger, "House");
                existenceComboBox.DataBindings.Add("SelectedValue", SelectedTrigger, "PersistantType");
                event1ComboBox.DataBindings.Add("SelectedItem", SelectedTrigger.Event1, "EventType");
                action1ComboBox.DataBindings.Add("SelectedItem", SelectedTrigger.Action1, "ActionType");

                UpdateTriggerControls(SelectedTrigger,
                    SelectedTrigger?.Event1, SelectedTrigger?.Action1,
                    event1ComboBox, event1Nud, event1ValueComboBox,
                    action1ComboBox, action1Nud, action1ValueComboBox);

                switch (plugin.GameType)
                {
                    case GameType.TiberianDawn:
                        teamComboBox.DataBindings.Add("SelectedItem", SelectedTrigger.Action1, "Team");
                        break;
                    case GameType.RedAlert:
                        typeComboBox.DataBindings.Add("SelectedValue", SelectedTrigger, "EventControl");
                        event2ComboBox.DataBindings.Add("SelectedItem", SelectedTrigger.Event2, "EventType");
                        action2ComboBox.DataBindings.Add("SelectedItem", SelectedTrigger.Action2, "ActionType");
                        UpdateTriggerControls(SelectedTrigger,
                            SelectedTrigger?.Event2, SelectedTrigger?.Action2,
                            event2ComboBox, event2Nud, event2ValueComboBox,
                            action2ComboBox, action2Nud, action2ValueComboBox);
                        break;
                }

                triggersTableLayoutPanel.Visible = true;
            }
            else
            {
                triggersTableLayoutPanel.Visible = false;
            }
        }

        private void teamTypesListView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                var hitTest = triggersListView.HitTest(e.Location);

                bool canAdd = (hitTest.Item == null) && (triggersListView.Items.Count < maxTriggers);
                bool canRemove = hitTest.Item != null;
                addTriggerToolStripMenuItem.Visible = canAdd;
                removeTriggerToolStripMenuItem.Visible = canRemove;

                if (canAdd || canRemove)
                {
                    triggersContextMenuStrip.Show(Cursor.Position);
                }
            }
        }

        private void teamTypesListView_KeyDown(object sender, KeyEventArgs e)
        {
            if ((e.KeyData == Keys.F2) && (triggersListView.SelectedItems.Count > 0))
            {
                triggersListView.SelectedItems[0].BeginEdit();
            }
        }

        private void addTriggerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var nameChars = Enumerable.Range(97, 26).Concat(Enumerable.Range(48, 10));

            string name = string.Empty;
            foreach (var nameChar in nameChars)
            {
                name = new string((char)nameChar, 4);
                if (!triggers.Where(t => t.Equals(name)).Any())
                {
                    break;
                }
            }

            var trigger = new Trigger { Name = name, House = plugin.Map.HouseTypes.First().Name };
            var item = new ListViewItem(trigger.Name)
            {
                Tag = trigger
            };
            triggers.Add(trigger);
            triggersListView.Items.Add(item).ToolTipText = trigger.Name;

            item.Selected = true;
            item.BeginEdit();
        }

        private void removeTriggerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SelectedItem != null)
            {
                triggers.Remove(SelectedTrigger);
                triggersListView.Items.Remove(SelectedItem);
            }
        }

        private void teamTypesListView_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            int maxLength = int.MaxValue;
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    maxLength = 4;
                    break;
                case GameType.RedAlert:
                    maxLength = 23;
                    break;
            }

            if (string.IsNullOrEmpty(e.Label))
            {
                e.CancelEdit = true;
            }
            else if (e.Label.Length > maxLength)
            {
                e.CancelEdit = true;
                MessageBox.Show(string.Format("Trigger name is longer than {0} characters.", maxLength), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (triggers.Where(t => (t != SelectedTrigger) && t.Equals(e.Label)).Any())
            {
                e.CancelEdit = true;
                MessageBox.Show(string.Format("Trigger with name '{0]' already exists", e.Label), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                SelectedTrigger.Name = e.Label;
                triggersListView.Items[e.Item].ToolTipText = SelectedTrigger.Name;
            }
        }

        private void typeComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            if (plugin.GameType == GameType.RedAlert)
            {
                var eventType = (TriggerMultiStyleType)typeComboBox.SelectedValue;
                event2Label.Visible = event2ComboBox.Visible = event2Flp.Visible = eventType != TriggerMultiStyleType.Only;
            }
        }

        private void trigger1ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateTriggerControls(SelectedTrigger,
                SelectedTrigger?.Event1, SelectedTrigger?.Action1,
                event1ComboBox, event1Nud, event1ValueComboBox,
                action1ComboBox, action1Nud, action1ValueComboBox);
        }

        private void trigger2ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateTriggerControls(SelectedTrigger,
                SelectedTrigger?.Event2, SelectedTrigger?.Action2,
                event2ComboBox, event2Nud, event2ValueComboBox,
                action2ComboBox, action2Nud, action2ValueComboBox);
        }

        private void UpdateTriggerControls(Trigger trigger, TriggerEvent triggerEvent, TriggerAction triggerAction, ComboBox eventComboBox, NumericUpDown eventNud, ComboBox eventValueComboBox, ComboBox actionComboBox, NumericUpDown actionNud, ComboBox actionValueComboBox)
        {
            eventNud.Visible = false;
            eventNud.DataBindings.Clear();
            eventValueComboBox.Visible = false;
            eventValueComboBox.DataBindings.Clear();
            eventValueComboBox.DataSource = null;
            eventValueComboBox.DisplayMember = null;
            eventValueComboBox.ValueMember = null;

            if (triggerEvent != null)
            {
                switch (plugin.GameType)
                {
                    case GameType.TiberianDawn:
                        switch (eventComboBox.SelectedItem)
                        {
                            case TiberianDawn.EventTypes.EVENT_TIME:
                            case TiberianDawn.EventTypes.EVENT_CREDITS:
                            case TiberianDawn.EventTypes.EVENT_NUNITS_DESTROYED:
                            case TiberianDawn.EventTypes.EVENT_NBUILDINGS_DESTROYED:
                                eventNud.Visible = true;
                                eventNud.DataBindings.Add("Value", triggerEvent, "Data");
                                break;
                            case TiberianDawn.EventTypes.EVENT_BUILD:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = plugin.Map.BuildingTypes.Select(t => new { Name = t.DisplayName, Value = (long)t.ID }).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            default:
                                break;
                        }
                        break;
                    case GameType.RedAlert:
                        switch (eventComboBox.SelectedItem)
                        {
                            case RedAlert.EventTypes.TEVENT_LEAVES_MAP:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DataSource = plugin.Map.TeamTypes.Select(t => t.Name).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedItem", triggerEvent, "Team");
                                break;
                            case RedAlert.EventTypes.TEVENT_PLAYER_ENTERED:
                            case RedAlert.EventTypes.TEVENT_CROSS_HORIZONTAL:
                            case RedAlert.EventTypes.TEVENT_CROSS_VERTICAL:
                            case RedAlert.EventTypes.TEVENT_ENTERS_ZONE:
                            case RedAlert.EventTypes.TEVENT_LOW_POWER:
                            case RedAlert.EventTypes.TEVENT_THIEVED:
                            case RedAlert.EventTypes.TEVENT_HOUSE_DISCOVERED:
                            case RedAlert.EventTypes.TEVENT_BUILDINGS_DESTROYED:
                            case RedAlert.EventTypes.TEVENT_UNITS_DESTROYED:
                            case RedAlert.EventTypes.TEVENT_ALL_DESTROYED:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = new { Name = "None", Value = (long)-1 }.Yield().Concat(plugin.Map.Houses.Select(t => new { t.Type.Name, Value = (long)t.Type.ID })).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            case RedAlert.EventTypes.TEVENT_BUILDING_EXISTS:
                            case RedAlert.EventTypes.TEVENT_BUILD:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = plugin.Map.BuildingTypes.Select(t => new { Name = t.DisplayName, Value = (long)t.ID }).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            case RedAlert.EventTypes.TEVENT_BUILD_UNIT:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = plugin.Map.UnitTypes.Where(t => t.IsUnit).Select(t => new { Name = t.DisplayName, Value = (long)t.ID }).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            case RedAlert.EventTypes.TEVENT_BUILD_INFANTRY:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = plugin.Map.InfantryTypes.Select(t => new { Name = t.DisplayName, Value = (long)t.ID }).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            case RedAlert.EventTypes.TEVENT_BUILD_AIRCRAFT:
                                eventValueComboBox.Visible = true;
                                eventValueComboBox.DisplayMember = "Name";
                                eventValueComboBox.ValueMember = "Value";
                                eventValueComboBox.DataSource = plugin.Map.UnitTypes.Where(t => t.IsAircraft).Select(t => new { Name = t.DisplayName, Value = (long)t.ID }).ToArray();
                                eventValueComboBox.DataBindings.Add("SelectedValue", triggerEvent, "Data");
                                break;
                            case RedAlert.EventTypes.TEVENT_NUNITS_DESTROYED:
                            case RedAlert.EventTypes.TEVENT_NBUILDINGS_DESTROYED:
                            case RedAlert.EventTypes.TEVENT_CREDITS:
                            case RedAlert.EventTypes.TEVENT_TIME:
                            case RedAlert.EventTypes.TEVENT_GLOBAL_SET:
                            case RedAlert.EventTypes.TEVENT_GLOBAL_CLEAR:
                                eventNud.Visible = true;
                                eventNud.DataBindings.Add("Value", triggerEvent, "Data");
                                break;
                            default:
                                break;
                        }
                        break;
                }
            }

            actionNud.Visible = false;
            actionNud.DataBindings.Clear();
            actionNud.Minimum = long.MinValue;
            actionNud.Maximum = long.MaxValue;
            actionValueComboBox.Visible = false;
            actionValueComboBox.DataBindings.Clear();
            actionValueComboBox.DataSource = null;
            actionValueComboBox.DisplayMember = null;
            actionValueComboBox.ValueMember = null;

            if (triggerAction != null)
            {
                switch (plugin.GameType)
                {
                    case GameType.RedAlert:
                        switch (actionComboBox.SelectedItem)
                        {
                            case RedAlert.ActionTypes.TACTION_CREATE_TEAM:
                            case RedAlert.ActionTypes.TACTION_DESTROY_TEAM:
                            case RedAlert.ActionTypes.TACTION_REINFORCEMENTS:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DataSource = plugin.Map.TeamTypes.Select(t => t.Name).ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedItem", triggerAction, "Team");
                                break;
                            case RedAlert.ActionTypes.TACTION_WIN:
                            case RedAlert.ActionTypes.TACTION_LOSE:
                            case RedAlert.ActionTypes.TACTION_BEGIN_PRODUCTION:
                            case RedAlert.ActionTypes.TACTION_FIRE_SALE:
                            case RedAlert.ActionTypes.TACTION_AUTOCREATE:
                            case RedAlert.ActionTypes.TACTION_ALL_HUNT:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = new { Name = "None", Value = (long)-1 }.Yield().Concat(plugin.Map.Houses.Select(t => new { t.Type.Name, Value = (long)t.Type.ID })).ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_FORCE_TRIGGER:
                            case RedAlert.ActionTypes.TACTION_DESTROY_TRIGGER:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DataSource = plugin.Map.Triggers.Select(t => t.Name).ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedItem", triggerAction, "Trigger");
                                break;
                            case RedAlert.ActionTypes.TACTION_DZ:
                            case RedAlert.ActionTypes.TACTION_REVEAL_SOME:
                            case RedAlert.ActionTypes.TACTION_REVEAL_ZONE:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = new { Name = "None", Value = (long)-1 }.Yield().Concat(plugin.Map.Waypoints.Select((t, i) => new { t.Name, Value = (long)i })).ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_1_SPECIAL:
                            case RedAlert.ActionTypes.TACTION_FULL_SPECIAL:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.SpecialWeaponType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.SpecialWeaponType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_PLAY_MUSIC:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.ThemeType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.ThemeType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_PLAY_MOVIE:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.VQType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.VQType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_PLAY_SOUND:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.VocType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.VocType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_PLAY_SPEECH:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.VoxType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.VoxType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_PREFERRED_TARGET:
                                actionValueComboBox.Visible = true;
                                actionValueComboBox.DisplayMember = "Name";
                                actionValueComboBox.ValueMember = "Value";
                                actionValueComboBox.DataSource = Enum.GetValues(typeof(RedAlert.ActionDataTypes.QuarryType)).Cast<int>()
                                    .Select(v => new { Name = Enum.GetName(typeof(RedAlert.ActionDataTypes.QuarryType), v), Value = (long)v })
                                    .ToArray();
                                actionValueComboBox.DataBindings.Add("SelectedValue", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_TEXT_TRIGGER:
                                actionNud.Visible = true;
                                actionNud.Minimum = 1;
                                actionNud.Maximum = 209;
                                actionNud.DataBindings.Add("Value", triggerAction, "Data");
                                break;
                            case RedAlert.ActionTypes.TACTION_ADD_TIMER:
                            case RedAlert.ActionTypes.TACTION_SUB_TIMER:
                            case RedAlert.ActionTypes.TACTION_SET_TIMER:
                            case RedAlert.ActionTypes.TACTION_SET_GLOBAL:
                            case RedAlert.ActionTypes.TACTION_CLEAR_GLOBAL:
                            case RedAlert.ActionTypes.TACTION_BASE_BUILDING:
                                actionNud.Visible = true;
                                actionNud.DataBindings.Add("Value", triggerAction, "Data");
                                break;
                            default:
                                break;
                        }
                        break;
                }
            }
        }
    }
}
