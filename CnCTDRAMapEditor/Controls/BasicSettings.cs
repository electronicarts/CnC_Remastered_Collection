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
using System.Data;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Controls
{
    public partial class BasicSettings : UserControl
    {
        public BasicSettings(IGamePlugin plugin, dynamic basicSection)
        {
            InitializeComponent();

            playerComboBox.DataSource = plugin.Map.Houses.Select(h => h.Type.Name).ToArray();
            baseComboBox.DataSource = plugin.Map.Houses.Select(h => h.Type.Name).ToArray();
            introComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            briefComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            actionComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            winComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            win2ComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            win3ComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            win4ComboBox.DataSource = plugin.Map.MovieTypes.ToArray();
            loseComboBox.DataSource = plugin.Map.MovieTypes.ToArray();

            carryOverMoneyNud.DataBindings.Add("Value", basicSection, "CarryOverMoney");
            nameTxt.DataBindings.Add("Text", basicSection, "Name");
            percentNud.DataBindings.Add("Value", basicSection, "Percent");
            playerComboBox.DataBindings.Add("SelectedItem", basicSection, "Player");
            authorTxt.DataBindings.Add("Text", basicSection, "Author");
            isSinglePlayerCheckBox.DataBindings.Add("Checked", basicSection, "SoloMission");
            introComboBox.DataBindings.Add("SelectedItem", basicSection, "Intro");
            briefComboBox.DataBindings.Add("SelectedItem", basicSection, "Brief");
            actionComboBox.DataBindings.Add("SelectedItem", basicSection, "Action");
            winComboBox.DataBindings.Add("SelectedItem", basicSection, "Win");
            loseComboBox.DataBindings.Add("SelectedItem", basicSection, "Lose");

            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    buidLevelNud.DataBindings.Add("Value", basicSection, "BuildLevel");
                    baseLabel.Visible = baseComboBox.Visible = false;
                    win2Label.Visible = win2ComboBox.Visible = false;
                    win3Label.Visible = win3ComboBox.Visible = false;
                    win4Label.Visible = win4ComboBox.Visible = false;
                    break;
                case GameType.RedAlert:
                    buidLevelNud.Visible = buildLevelLabel.Visible = false;
                    baseComboBox.DataBindings.Add("SelectedItem", basicSection, "BasePlayer");
                    win2ComboBox.DataBindings.Add("SelectedItem", basicSection, "Win2");
                    win3ComboBox.DataBindings.Add("SelectedItem", basicSection, "Win3");
                    win4ComboBox.DataBindings.Add("SelectedItem", basicSection, "Win4");
                    break;
            }

            introComboBox.Enabled = briefComboBox.Enabled = actionComboBox.Enabled = loseComboBox.Enabled = isSinglePlayerCheckBox.Checked;
            winComboBox.Enabled = win2ComboBox.Enabled = win3ComboBox.Enabled = win4ComboBox.Enabled = isSinglePlayerCheckBox.Checked;
        }

        private void isSinglePlayerCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            introComboBox.Enabled = briefComboBox.Enabled = actionComboBox.Enabled = loseComboBox.Enabled = isSinglePlayerCheckBox.Checked;
            winComboBox.Enabled = win2ComboBox.Enabled = win3ComboBox.Enabled = win4ComboBox.Enabled = isSinglePlayerCheckBox.Checked;
        }
    }
}
