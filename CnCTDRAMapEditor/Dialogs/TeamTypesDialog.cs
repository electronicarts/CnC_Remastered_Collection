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
    public partial class TeamTypesDialog : Form
    {
        private readonly IGamePlugin plugin;
        private readonly int maxTeams;
        private readonly IEnumerable<ITechnoType> technoTypes;

        private readonly List<TeamType> teamTypes;
        public IEnumerable<TeamType> TeamTypes => teamTypes;

        private ListViewItem SelectedItem => (teamTypesListView.SelectedItems.Count > 0) ? teamTypesListView.SelectedItems[0] : null;

        private TeamType SelectedTeamType => SelectedItem?.Tag as TeamType;

        private TeamTypeClass mockClass;
        private TeamTypeMission mockMission;
        private int classEditRow = -1;
        private int missionEditRow = -1;

        public TeamTypesDialog(IGamePlugin plugin, int maxTeams)
        {
            this.plugin = plugin;
            this.maxTeams = maxTeams;
            technoTypes = plugin.Map.InfantryTypes.Cast<ITechnoType>().Concat(plugin.Map.UnitTypes.Cast<ITechnoType>());

            InitializeComponent();

            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    triggerLabel.Visible = triggerComboBox.Visible = false;
                    waypointLabel.Visible = waypointComboBox.Visible = false;
                    break;
                case GameType.RedAlert:
                    learningCheckBox.Visible = false;
                    mercernaryCheckBox.Visible = false;
                    break;
            }

            teamTypes = new List<TeamType>(plugin.Map.TeamTypes.Select(t => t.Clone()));

            teamTypesListView.BeginUpdate();
            {
                foreach (var teamType in this.teamTypes)
                {
                    var item = new ListViewItem(teamType.Name)
                    {
                        Tag = teamType
                    };
                    teamTypesListView.Items.Add(item).ToolTipText = teamType.Name;
                }
            }
            teamTypesListView.EndUpdate();

            houseComboBox.DataSource = plugin.Map.Houses.Select(t => new TypeItem<HouseType>(t.Type.Name, t.Type)).ToArray();
            waypointComboBox.DataSource = "(none)".Yield().Concat(plugin.Map.Waypoints.Select(w => w.Name)).ToArray();
            triggerComboBox.DataSource = Trigger.None.Yield().Concat(plugin.Map.Triggers.Select(t => t.Name)).ToArray();

            teamsTypeColumn.DisplayMember = "Name";
            teamsTypeColumn.ValueMember = "Type";
            teamsTypeColumn.DataSource = technoTypes.Select(t => new TypeItem<ITechnoType>(t.Name, t)).ToArray();

            missionsMissionColumn.DataSource = plugin.Map.TeamMissionTypes;

            teamTypeTableLayoutPanel.Visible = false;
        }

        private void teamTypesListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            houseComboBox.DataBindings.Clear();
            roundaboutCheckBox.DataBindings.Clear();
            learningCheckBox.DataBindings.Clear();
            suicideCheckBox.DataBindings.Clear();
            autocreateCheckBox.DataBindings.Clear();
            mercernaryCheckBox.DataBindings.Clear();
            reinforcableCheckBox.DataBindings.Clear();
            prebuiltCheckBox.DataBindings.Clear();
            recruitPriorityNud.DataBindings.Clear();
            initNumNud.DataBindings.Clear();
            maxAllowedNud.DataBindings.Clear();
            fearNud.DataBindings.Clear();
            waypointComboBox.DataBindings.Clear();
            triggerComboBox.DataBindings.Clear();

            if (SelectedTeamType != null)
            {
                houseComboBox.DataBindings.Add("SelectedValue", SelectedTeamType, "House");
                roundaboutCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsRoundAbout");
                learningCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsLearning");
                suicideCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsSuicide");
                autocreateCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsAutocreate");
                mercernaryCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsMercenary");
                reinforcableCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsReinforcable");
                prebuiltCheckBox.DataBindings.Add("Checked", SelectedTeamType, "IsPrebuilt");
                recruitPriorityNud.DataBindings.Add("Value", SelectedTeamType, "RecruitPriority");
                initNumNud.DataBindings.Add("Value", SelectedTeamType, "InitNum");
                maxAllowedNud.DataBindings.Add("Value", SelectedTeamType, "MaxAllowed");
                fearNud.DataBindings.Add("Value", SelectedTeamType, "Fear");
                waypointComboBox.DataBindings.Add("SelectedIndex", SelectedTeamType, "Origin");
                triggerComboBox.DataBindings.Add("SelectedItem", SelectedTeamType, "Trigger");

                mockClass = null;
                mockMission = null;
                classEditRow = -1;
                missionEditRow = -1;

                teamsDataGridView.Rows.Clear();
                missionsDataGridView.Rows.Clear();

                teamsDataGridView.RowCount = SelectedTeamType.Classes.Count + 1;
                missionsDataGridView.RowCount = SelectedTeamType.Missions.Count + 1;

                updateDataGridViewAddRows(teamsDataGridView, Globals.MaxTeamClasses);
                updateDataGridViewAddRows(missionsDataGridView, Globals.MaxTeamMissions);

                teamTypeTableLayoutPanel.Visible = true;
            }
            else
            {
                teamTypeTableLayoutPanel.Visible = false;
            }
        }

        private void teamTypesListView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                var hitTest = teamTypesListView.HitTest(e.Location);

                bool canAdd = (hitTest.Item == null) && (teamTypesListView.Items.Count < maxTeams);
                bool canRemove = hitTest.Item != null;
                addTeamTypeToolStripMenuItem.Visible = canAdd;
                removeTeamTypeToolStripMenuItem.Visible = canRemove;

                if (canAdd || canRemove)
                {
                    teamTypesContextMenuStrip.Show(Cursor.Position);
                }
            }
        }

        private void teamTypesListView_KeyDown(object sender, KeyEventArgs e)
        {
            if ((e.KeyData == Keys.F2) && (teamTypesListView.SelectedItems.Count > 0))
            {
                teamTypesListView.SelectedItems[0].BeginEdit();
            }
        }

        private void addTeamTypeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var nameChars = Enumerable.Range(97, 26).Concat(Enumerable.Range(48, 10));

            string name = string.Empty;
            foreach (var nameChar in nameChars)
            {
                name = new string((char)nameChar, 4);
                if (!teamTypes.Where(t => t.Equals(name)).Any())
                {
                    break;
                }
            }

            var teamType = new TeamType { Name = name, House = plugin.Map.HouseTypes.First() };
            var item = new ListViewItem(teamType.Name)
            {
                Tag = teamType
            };
            teamTypes.Add(teamType);
            teamTypesListView.Items.Add(item).ToolTipText = teamType.Name;

            item.Selected = true;
            item.BeginEdit();
        }

        private void removeTeamTypeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SelectedItem != null)
            {
                teamTypes.Remove(SelectedTeamType);
                teamTypesListView.Items.Remove(SelectedItem);
            }
        }

        private void teamTypesListView_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            int maxLength = int.MaxValue;
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    maxLength = 8;
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
                MessageBox.Show(string.Format("Team name is longer than {0} characters.", maxLength), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (teamTypes.Where(t => (t != SelectedTeamType) && t.Equals(e.Label)).Any())
            {
                e.CancelEdit = true;
                MessageBox.Show(string.Format("Team with name '{0]' already exists", e.Label), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                SelectedTeamType.Name = e.Label;
                teamTypesListView.Items[e.Item].ToolTipText = SelectedTeamType.Name;
            }
        }

        private void teamsDataGridView_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            if (SelectedTeamType == null)
            {
                return;
            }

            TeamTypeClass teamTypeClass = null;
            if (e.RowIndex == classEditRow)
            {
                teamTypeClass = mockClass;
            }
            else if (e.RowIndex < SelectedTeamType.Classes.Count)
            {
                teamTypeClass = SelectedTeamType.Classes[e.RowIndex];
            }

            if (teamTypeClass == null)
            {
                return;
            }

            switch (e.ColumnIndex)
            {
                case 0:
                    e.Value = teamTypeClass.Type;
                    break;
                case 1:
                    e.Value = teamTypeClass.Count;
                    break;
            }
        }

        private void teamsDataGridView_CellValuePushed(object sender, DataGridViewCellValueEventArgs e)
        {
            if (SelectedTeamType == null)
            {
                return;
            }

            if (mockClass == null)
            {
                mockClass = (e.RowIndex < SelectedTeamType.Classes.Count) ?
                    new TeamTypeClass { Type = SelectedTeamType.Classes[e.RowIndex].Type, Count = SelectedTeamType.Classes[e.RowIndex].Count } :
                    new TeamTypeClass { Type = technoTypes.First(), Count = 0 };
            }
            classEditRow = e.RowIndex;

            switch (e.ColumnIndex)
            {
                case 0:
                    mockClass.Type = e.Value as ITechnoType;
                    break;
                case 1:
                    mockClass.Count = int.TryParse(e.Value as string, out int value) ? (byte)Math.Max(0, Math.Min(255, value)) : (byte)0;
                    break;
            }
        }

        private void teamsDataGridView_NewRowNeeded(object sender, DataGridViewRowEventArgs e)
        {
            mockClass = new TeamTypeClass { Type = technoTypes.First(), Count = 0 };
            classEditRow = teamsDataGridView.RowCount - 1;
        }

        private void teamsDataGridView_RowValidated(object sender, DataGridViewCellEventArgs e)
        {
            if ((mockClass != null) && (e.RowIndex >= SelectedTeamType.Classes.Count) && ((teamsDataGridView.Rows.Count > 1) || (e.RowIndex < (teamsDataGridView.Rows.Count - 1))))
            {
                SelectedTeamType.Classes.Add(mockClass);
                mockClass = null;
                classEditRow = -1;
            }
            else if ((mockClass != null) && (e.RowIndex < SelectedTeamType.Classes.Count))
            {
                SelectedTeamType.Classes[e.RowIndex] = mockClass;
                mockClass = null;
                classEditRow = -1;
            }
            else if (teamsDataGridView.ContainsFocus)
            {
                mockClass = null;
                classEditRow = -1;
            }
        }

        private void teamsDataGridView_RowDirtyStateNeeded(object sender, QuestionEventArgs e)
        {
            e.Response = teamsDataGridView.IsCurrentCellDirty;
        }

        private void teamsDataGridView_CancelRowEdit(object sender, QuestionEventArgs e)
        {
            if ((classEditRow == (teamsDataGridView.Rows.Count - 2)) && (classEditRow == SelectedTeamType.Classes.Count))
            {
                mockClass = new TeamTypeClass { Type = technoTypes.First(), Count = 0 };
            }
            else
            {
                mockClass = null;
                classEditRow = -1;
            }
        }

        private void teamsDataGridView_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            if (e.Row.Index < SelectedTeamType.Classes.Count)
            {
                SelectedTeamType.Classes.RemoveAt(e.Row.Index);
            }

            if (e.Row.Index == classEditRow)
            {
                mockClass = null;
                classEditRow = -1;
            }
        }

        private void teamsDataGridView_UserAddedRow(object sender, DataGridViewRowEventArgs e)
        {
            updateDataGridViewAddRows(teamsDataGridView, Globals.MaxTeamClasses);
        }

        private void teamsDataGridView_UserDeletedRow(object sender, DataGridViewRowEventArgs e)
        {
            updateDataGridViewAddRows(teamsDataGridView, Globals.MaxTeamClasses);
        }

        private void missionsDataGridView_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            if (SelectedTeamType == null)
            {
                return;
            }

            TeamTypeMission teamMissionType = null;
            if (e.RowIndex == missionEditRow)
            {
                teamMissionType = mockMission;
            }
            else if (e.RowIndex < SelectedTeamType.Missions.Count)
            {
                teamMissionType = SelectedTeamType.Missions[e.RowIndex];
            }

            if (teamMissionType == null)
            {
                return;
            }

            switch (e.ColumnIndex)
            {
                case 0:
                    e.Value = teamMissionType.Mission;
                    break;
                case 1:
                    e.Value = teamMissionType.Argument;
                    break;
            }
        }

        private void missionsDataGridView_CellValuePushed(object sender, DataGridViewCellValueEventArgs e)
        {
            if (SelectedTeamType == null)
            {
                return;
            }

            if (mockMission == null)
            {
                mockMission = (e.RowIndex < SelectedTeamType.Missions.Count) ?
                    new TeamTypeMission { Mission = SelectedTeamType.Missions[e.RowIndex].Mission, Argument = SelectedTeamType.Missions[e.RowIndex].Argument } :
                    new TeamTypeMission { Mission = plugin.Map.TeamMissionTypes.First(), Argument = 0 };
            }
            missionEditRow = e.RowIndex;

            switch (e.ColumnIndex)
            {
                case 0:
                    mockMission.Mission = e.Value as string;
                    break;
                case 1:
                    mockMission.Argument = int.TryParse(e.Value as string, out int value) ? value : 0;
                    break;
            }
        }

        private void missionsDataGridView_NewRowNeeded(object sender, DataGridViewRowEventArgs e)
        {
            mockMission = new TeamTypeMission { Mission = plugin.Map.TeamMissionTypes.First(), Argument = 0 };
            missionEditRow = missionsDataGridView.RowCount - 1;
        }

        private void missionsDataGridView_RowValidated(object sender, DataGridViewCellEventArgs e)
        {
            if ((mockMission != null) && (e.RowIndex >= SelectedTeamType.Missions.Count) && ((missionsDataGridView.Rows.Count > 1) || (e.RowIndex < (missionsDataGridView.Rows.Count - 1))))
            {
                SelectedTeamType.Missions.Add(mockMission);
                mockMission = null;
                missionEditRow = -1;
            }
            else if ((mockMission != null) && (e.RowIndex < SelectedTeamType.Missions.Count))
            {
                SelectedTeamType.Missions[e.RowIndex] = mockMission;
                mockMission = null;
                missionEditRow = -1;
            }
            else if (missionsDataGridView.ContainsFocus)
            {
                mockMission = null;
                missionEditRow = -1;
            }
        }

        private void missionsDataGridView_RowDirtyStateNeeded(object sender, QuestionEventArgs e)
        {
            e.Response = missionsDataGridView.IsCurrentCellDirty;
        }

        private void missionsDataGridView_CancelRowEdit(object sender, QuestionEventArgs e)
        {
            if ((missionEditRow == (missionsDataGridView.Rows.Count - 2)) && (missionEditRow == SelectedTeamType.Missions.Count))
            {
                mockMission = new TeamTypeMission { Mission = plugin.Map.TeamMissionTypes.First(), Argument = 0 };
            }
            else
            {
                mockMission = null;
                missionEditRow = -1;
            }
        }

        private void missionsDataGridView_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            if (e.Row.Index < SelectedTeamType.Missions.Count)
            {
                SelectedTeamType.Missions.RemoveAt(e.Row.Index);
            }

            if (e.Row.Index == missionEditRow)
            {
                mockMission = null;
                missionEditRow = -1;
            }
        }

        private void missionsDataGridView_UserAddedRow(object sender, DataGridViewRowEventArgs e)
        {
            updateDataGridViewAddRows(missionsDataGridView, Globals.MaxTeamMissions);
        }

        private void missionsDataGridView_UserDeletedRow(object sender, DataGridViewRowEventArgs e)
        {
            updateDataGridViewAddRows(missionsDataGridView, Globals.MaxTeamMissions);
        }

        private void updateDataGridViewAddRows(DataGridView dataGridView, int maxItems)
        {
            dataGridView.AllowUserToAddRows = dataGridView.Rows.Count <= maxItems;
        }
    }
}
