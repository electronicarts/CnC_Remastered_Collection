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
namespace MobiusEditor.Dialogs
{
    partial class TeamTypesDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.settingsPanel = new System.Windows.Forms.Panel();
            this.teamTypeTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.houseComboBox = new System.Windows.Forms.ComboBox();
            this.roundaboutCheckBox = new System.Windows.Forms.CheckBox();
            this.learningCheckBox = new System.Windows.Forms.CheckBox();
            this.suicideCheckBox = new System.Windows.Forms.CheckBox();
            this.autocreateCheckBox = new System.Windows.Forms.CheckBox();
            this.mercernaryCheckBox = new System.Windows.Forms.CheckBox();
            this.reinforcableCheckBox = new System.Windows.Forms.CheckBox();
            this.prebuiltCheckBox = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.initNumNud = new System.Windows.Forms.NumericUpDown();
            this.maxAllowedNud = new System.Windows.Forms.NumericUpDown();
            this.fearNud = new System.Windows.Forms.NumericUpDown();
            this.waypointLabel = new System.Windows.Forms.Label();
            this.triggerLabel = new System.Windows.Forms.Label();
            this.waypointComboBox = new System.Windows.Forms.ComboBox();
            this.triggerComboBox = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.recruitPriorityNud = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.teamsDataGridView = new System.Windows.Forms.DataGridView();
            this.teamsTypeColumn = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.teamsCountColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.missionsDataGridView = new System.Windows.Forms.DataGridView();
            this.missionsMissionColumn = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.missionsArgumentColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.teamTypesListView = new System.Windows.Forms.ListView();
            this.nameColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.teamTypesContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addTeamTypeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeTeamTypeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tableLayoutPanel1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.settingsPanel.SuspendLayout();
            this.teamTypeTableLayoutPanel.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.initNumNud)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.maxAllowedNud)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fearNud)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.recruitPriorityNud)).BeginInit();
            this.tableLayoutPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.teamsDataGridView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.missionsDataGridView)).BeginInit();
            this.teamTypesContextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 80F));
            this.tableLayoutPanel1.Controls.Add(this.flowLayoutPanel1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.settingsPanel, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.teamTypesListView, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1313, 640);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoSize = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.SetColumnSpan(this.flowLayoutPanel1, 2);
            this.flowLayoutPanel1.Controls.Add(this.btnCancel);
            this.flowLayoutPanel1.Controls.Add(this.btnOK);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(4, 583);
            this.flowLayoutPanel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(1305, 52);
            this.flowLayoutPanel1.TabIndex = 1;
            // 
            // btnCancel
            // 
            this.btnCancel.AutoSize = true;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(1200, 3);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(102, 46);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            this.btnOK.AutoSize = true;
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(1119, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 46);
            this.btnOK.TabIndex = 3;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // settingsPanel
            // 
            this.settingsPanel.Controls.Add(this.teamTypeTableLayoutPanel);
            this.settingsPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.settingsPanel.Location = new System.Drawing.Point(277, 15);
            this.settingsPanel.Margin = new System.Windows.Forms.Padding(15);
            this.settingsPanel.Name = "settingsPanel";
            this.settingsPanel.Size = new System.Drawing.Size(1021, 548);
            this.settingsPanel.TabIndex = 3;
            // 
            // teamTypeTableLayoutPanel
            // 
            this.teamTypeTableLayoutPanel.ColumnCount = 3;
            this.teamTypeTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.teamTypeTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.teamTypeTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.teamTypeTableLayoutPanel.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.teamTypeTableLayoutPanel.Controls.Add(this.tableLayoutPanel3, 2, 0);
            this.teamTypeTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.teamTypeTableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.teamTypeTableLayoutPanel.Name = "teamTypeTableLayoutPanel";
            this.teamTypeTableLayoutPanel.RowCount = 1;
            this.teamTypeTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.teamTypeTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 548F));
            this.teamTypeTableLayoutPanel.Size = new System.Drawing.Size(1021, 548);
            this.teamTypeTableLayoutPanel.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.AutoSize = true;
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel2.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.houseComboBox, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.roundaboutCheckBox, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.learningCheckBox, 1, 2);
            this.tableLayoutPanel2.Controls.Add(this.suicideCheckBox, 1, 3);
            this.tableLayoutPanel2.Controls.Add(this.autocreateCheckBox, 1, 4);
            this.tableLayoutPanel2.Controls.Add(this.mercernaryCheckBox, 1, 5);
            this.tableLayoutPanel2.Controls.Add(this.reinforcableCheckBox, 1, 6);
            this.tableLayoutPanel2.Controls.Add(this.prebuiltCheckBox, 1, 7);
            this.tableLayoutPanel2.Controls.Add(this.label2, 0, 9);
            this.tableLayoutPanel2.Controls.Add(this.label3, 0, 10);
            this.tableLayoutPanel2.Controls.Add(this.label4, 0, 11);
            this.tableLayoutPanel2.Controls.Add(this.initNumNud, 1, 9);
            this.tableLayoutPanel2.Controls.Add(this.maxAllowedNud, 1, 10);
            this.tableLayoutPanel2.Controls.Add(this.fearNud, 1, 11);
            this.tableLayoutPanel2.Controls.Add(this.waypointLabel, 0, 12);
            this.tableLayoutPanel2.Controls.Add(this.triggerLabel, 0, 13);
            this.tableLayoutPanel2.Controls.Add(this.waypointComboBox, 1, 12);
            this.tableLayoutPanel2.Controls.Add(this.triggerComboBox, 1, 13);
            this.tableLayoutPanel2.Controls.Add(this.label9, 0, 8);
            this.tableLayoutPanel2.Controls.Add(this.recruitPriorityNud, 1, 8);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 15;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(307, 542);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 34);
            this.label1.TabIndex = 0;
            this.label1.Text = "House";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // houseComboBox
            // 
            this.houseComboBox.DisplayMember = "Name";
            this.houseComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.houseComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.houseComboBox.FormattingEnabled = true;
            this.houseComboBox.Location = new System.Drawing.Point(109, 3);
            this.houseComboBox.Name = "houseComboBox";
            this.houseComboBox.Size = new System.Drawing.Size(195, 28);
            this.houseComboBox.TabIndex = 1;
            this.houseComboBox.ValueMember = "Type";
            // 
            // roundaboutCheckBox
            // 
            this.roundaboutCheckBox.AutoSize = true;
            this.roundaboutCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.roundaboutCheckBox.Location = new System.Drawing.Point(109, 37);
            this.roundaboutCheckBox.Name = "roundaboutCheckBox";
            this.roundaboutCheckBox.Size = new System.Drawing.Size(195, 24);
            this.roundaboutCheckBox.TabIndex = 2;
            this.roundaboutCheckBox.Text = "Roundabout";
            this.roundaboutCheckBox.UseVisualStyleBackColor = true;
            // 
            // learningCheckBox
            // 
            this.learningCheckBox.AutoSize = true;
            this.learningCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.learningCheckBox.Location = new System.Drawing.Point(109, 67);
            this.learningCheckBox.Name = "learningCheckBox";
            this.learningCheckBox.Size = new System.Drawing.Size(195, 24);
            this.learningCheckBox.TabIndex = 3;
            this.learningCheckBox.Text = "Learning";
            this.learningCheckBox.UseVisualStyleBackColor = true;
            // 
            // suicideCheckBox
            // 
            this.suicideCheckBox.AutoSize = true;
            this.suicideCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.suicideCheckBox.Location = new System.Drawing.Point(109, 97);
            this.suicideCheckBox.Name = "suicideCheckBox";
            this.suicideCheckBox.Size = new System.Drawing.Size(195, 24);
            this.suicideCheckBox.TabIndex = 4;
            this.suicideCheckBox.Text = "Suicide";
            this.suicideCheckBox.UseVisualStyleBackColor = true;
            // 
            // autocreateCheckBox
            // 
            this.autocreateCheckBox.AutoSize = true;
            this.autocreateCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.autocreateCheckBox.Location = new System.Drawing.Point(109, 127);
            this.autocreateCheckBox.Name = "autocreateCheckBox";
            this.autocreateCheckBox.Size = new System.Drawing.Size(195, 24);
            this.autocreateCheckBox.TabIndex = 5;
            this.autocreateCheckBox.Text = "Auto-create";
            this.autocreateCheckBox.UseVisualStyleBackColor = true;
            // 
            // mercernaryCheckBox
            // 
            this.mercernaryCheckBox.AutoSize = true;
            this.mercernaryCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mercernaryCheckBox.Location = new System.Drawing.Point(109, 157);
            this.mercernaryCheckBox.Name = "mercernaryCheckBox";
            this.mercernaryCheckBox.Size = new System.Drawing.Size(195, 24);
            this.mercernaryCheckBox.TabIndex = 6;
            this.mercernaryCheckBox.Text = "Mercernary";
            this.mercernaryCheckBox.UseVisualStyleBackColor = true;
            // 
            // reinforcableCheckBox
            // 
            this.reinforcableCheckBox.AutoSize = true;
            this.reinforcableCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.reinforcableCheckBox.Location = new System.Drawing.Point(109, 187);
            this.reinforcableCheckBox.Name = "reinforcableCheckBox";
            this.reinforcableCheckBox.Size = new System.Drawing.Size(195, 24);
            this.reinforcableCheckBox.TabIndex = 7;
            this.reinforcableCheckBox.Text = "Reinforcable";
            this.reinforcableCheckBox.UseVisualStyleBackColor = true;
            // 
            // prebuiltCheckBox
            // 
            this.prebuiltCheckBox.AutoSize = true;
            this.prebuiltCheckBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.prebuiltCheckBox.Location = new System.Drawing.Point(109, 217);
            this.prebuiltCheckBox.Name = "prebuiltCheckBox";
            this.prebuiltCheckBox.Size = new System.Drawing.Size(195, 24);
            this.prebuiltCheckBox.TabIndex = 8;
            this.prebuiltCheckBox.Text = "Prebuilt";
            this.prebuiltCheckBox.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(3, 276);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 32);
            this.label2.TabIndex = 9;
            this.label2.Text = "Init Num";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Location = new System.Drawing.Point(3, 308);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(100, 32);
            this.label3.TabIndex = 10;
            this.label3.Text = "Max Allowed";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Location = new System.Drawing.Point(3, 340);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(100, 32);
            this.label4.TabIndex = 11;
            this.label4.Text = "Fear";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // initNumNud
            // 
            this.initNumNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.initNumNud.Location = new System.Drawing.Point(109, 279);
            this.initNumNud.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.initNumNud.Name = "initNumNud";
            this.initNumNud.Size = new System.Drawing.Size(195, 26);
            this.initNumNud.TabIndex = 12;
            // 
            // maxAllowedNud
            // 
            this.maxAllowedNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.maxAllowedNud.Location = new System.Drawing.Point(109, 311);
            this.maxAllowedNud.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.maxAllowedNud.Name = "maxAllowedNud";
            this.maxAllowedNud.Size = new System.Drawing.Size(195, 26);
            this.maxAllowedNud.TabIndex = 13;
            // 
            // fearNud
            // 
            this.fearNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fearNud.Location = new System.Drawing.Point(109, 343);
            this.fearNud.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.fearNud.Name = "fearNud";
            this.fearNud.Size = new System.Drawing.Size(195, 26);
            this.fearNud.TabIndex = 14;
            // 
            // waypointLabel
            // 
            this.waypointLabel.AutoSize = true;
            this.waypointLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.waypointLabel.Location = new System.Drawing.Point(3, 372);
            this.waypointLabel.Name = "waypointLabel";
            this.waypointLabel.Size = new System.Drawing.Size(100, 34);
            this.waypointLabel.TabIndex = 15;
            this.waypointLabel.Text = "Waypoint";
            this.waypointLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // triggerLabel
            // 
            this.triggerLabel.AutoSize = true;
            this.triggerLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.triggerLabel.Location = new System.Drawing.Point(3, 406);
            this.triggerLabel.Name = "triggerLabel";
            this.triggerLabel.Size = new System.Drawing.Size(100, 34);
            this.triggerLabel.TabIndex = 16;
            this.triggerLabel.Text = "Trigger";
            this.triggerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // waypointComboBox
            // 
            this.waypointComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.waypointComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.waypointComboBox.FormattingEnabled = true;
            this.waypointComboBox.Location = new System.Drawing.Point(109, 375);
            this.waypointComboBox.Name = "waypointComboBox";
            this.waypointComboBox.Size = new System.Drawing.Size(195, 28);
            this.waypointComboBox.TabIndex = 17;
            // 
            // triggerComboBox
            // 
            this.triggerComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.triggerComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.triggerComboBox.FormattingEnabled = true;
            this.triggerComboBox.Location = new System.Drawing.Point(109, 409);
            this.triggerComboBox.Name = "triggerComboBox";
            this.triggerComboBox.Size = new System.Drawing.Size(195, 28);
            this.triggerComboBox.TabIndex = 18;
            // 
            // label9
            // 
            this.label9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label9.Location = new System.Drawing.Point(3, 244);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(100, 32);
            this.label9.TabIndex = 19;
            this.label9.Text = "Priority";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // recruitPriorityNud
            // 
            this.recruitPriorityNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.recruitPriorityNud.Location = new System.Drawing.Point(109, 247);
            this.recruitPriorityNud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.recruitPriorityNud.Minimum = new decimal(new int[] {
            -2147483648,
            0,
            0,
            -2147483648});
            this.recruitPriorityNud.Name = "recruitPriorityNud";
            this.recruitPriorityNud.Size = new System.Drawing.Size(195, 26);
            this.recruitPriorityNud.TabIndex = 20;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.label7, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.label8, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.teamsDataGridView, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.missionsDataGridView, 1, 1);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(316, 3);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 2;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(702, 542);
            this.tableLayoutPanel3.TabIndex = 1;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(57, 20);
            this.label7.TabIndex = 0;
            this.label7.Text = "Teams";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(354, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(70, 20);
            this.label8.TabIndex = 1;
            this.label8.Text = "Missions";
            // 
            // teamsDataGridView
            // 
            this.teamsDataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.teamsDataGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.teamsDataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.teamsDataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.teamsTypeColumn,
            this.teamsCountColumn});
            this.teamsDataGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.teamsDataGridView.Location = new System.Drawing.Point(3, 23);
            this.teamsDataGridView.Name = "teamsDataGridView";
            this.teamsDataGridView.RowTemplate.Height = 28;
            this.teamsDataGridView.Size = new System.Drawing.Size(345, 516);
            this.teamsDataGridView.TabIndex = 2;
            this.teamsDataGridView.VirtualMode = true;
            this.teamsDataGridView.CancelRowEdit += new System.Windows.Forms.QuestionEventHandler(this.teamsDataGridView_CancelRowEdit);
            this.teamsDataGridView.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.teamsDataGridView_CellValueNeeded);
            this.teamsDataGridView.CellValuePushed += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.teamsDataGridView_CellValuePushed);
            this.teamsDataGridView.NewRowNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.teamsDataGridView_NewRowNeeded);
            this.teamsDataGridView.RowDirtyStateNeeded += new System.Windows.Forms.QuestionEventHandler(this.teamsDataGridView_RowDirtyStateNeeded);
            this.teamsDataGridView.RowValidated += new System.Windows.Forms.DataGridViewCellEventHandler(this.teamsDataGridView_RowValidated);
            this.teamsDataGridView.UserAddedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.teamsDataGridView_UserAddedRow);
            this.teamsDataGridView.UserDeletedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.teamsDataGridView_UserDeletedRow);
            this.teamsDataGridView.UserDeletingRow += new System.Windows.Forms.DataGridViewRowCancelEventHandler(this.teamsDataGridView_UserDeletingRow);
            // 
            // teamsTypeColumn
            // 
            this.teamsTypeColumn.HeaderText = "Type";
            this.teamsTypeColumn.Name = "teamsTypeColumn";
            this.teamsTypeColumn.Width = 54;
            // 
            // teamsCountColumn
            // 
            this.teamsCountColumn.HeaderText = "Count";
            this.teamsCountColumn.Name = "teamsCountColumn";
            this.teamsCountColumn.Width = 88;
            // 
            // missionsDataGridView
            // 
            this.missionsDataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.missionsDataGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.missionsDataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.missionsDataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.missionsMissionColumn,
            this.missionsArgumentColumn});
            this.missionsDataGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.missionsDataGridView.Location = new System.Drawing.Point(354, 23);
            this.missionsDataGridView.Name = "missionsDataGridView";
            this.missionsDataGridView.RowTemplate.Height = 28;
            this.missionsDataGridView.Size = new System.Drawing.Size(345, 516);
            this.missionsDataGridView.TabIndex = 3;
            this.missionsDataGridView.VirtualMode = true;
            this.missionsDataGridView.CancelRowEdit += new System.Windows.Forms.QuestionEventHandler(this.missionsDataGridView_CancelRowEdit);
            this.missionsDataGridView.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.missionsDataGridView_CellValueNeeded);
            this.missionsDataGridView.CellValuePushed += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.missionsDataGridView_CellValuePushed);
            this.missionsDataGridView.NewRowNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.missionsDataGridView_NewRowNeeded);
            this.missionsDataGridView.RowDirtyStateNeeded += new System.Windows.Forms.QuestionEventHandler(this.missionsDataGridView_RowDirtyStateNeeded);
            this.missionsDataGridView.RowValidated += new System.Windows.Forms.DataGridViewCellEventHandler(this.missionsDataGridView_RowValidated);
            this.missionsDataGridView.UserAddedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.missionsDataGridView_UserAddedRow);
            this.missionsDataGridView.UserDeletedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.missionsDataGridView_UserDeletedRow);
            this.missionsDataGridView.UserDeletingRow += new System.Windows.Forms.DataGridViewRowCancelEventHandler(this.missionsDataGridView_UserDeletingRow);
            // 
            // missionsMissionColumn
            // 
            this.missionsMissionColumn.HeaderText = "Mission";
            this.missionsMissionColumn.Name = "missionsMissionColumn";
            this.missionsMissionColumn.Width = 68;
            // 
            // missionsArgumentColumn
            // 
            this.missionsArgumentColumn.HeaderText = "Argument";
            this.missionsArgumentColumn.Name = "missionsArgumentColumn";
            this.missionsArgumentColumn.Width = 115;
            // 
            // teamTypesListView
            // 
            this.teamTypesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.nameColumnHeader});
            this.teamTypesListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.teamTypesListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.teamTypesListView.HideSelection = false;
            this.teamTypesListView.LabelEdit = true;
            this.teamTypesListView.Location = new System.Drawing.Point(3, 3);
            this.teamTypesListView.MultiSelect = false;
            this.teamTypesListView.Name = "teamTypesListView";
            this.teamTypesListView.ShowItemToolTips = true;
            this.teamTypesListView.Size = new System.Drawing.Size(256, 572);
            this.teamTypesListView.TabIndex = 4;
            this.teamTypesListView.UseCompatibleStateImageBehavior = false;
            this.teamTypesListView.View = System.Windows.Forms.View.Details;
            this.teamTypesListView.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.teamTypesListView_AfterLabelEdit);
            this.teamTypesListView.SelectedIndexChanged += new System.EventHandler(this.teamTypesListView_SelectedIndexChanged);
            this.teamTypesListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.teamTypesListView_KeyDown);
            this.teamTypesListView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.teamTypesListView_MouseDown);
            // 
            // teamTypesContextMenuStrip
            // 
            this.teamTypesContextMenuStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.teamTypesContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addTeamTypeToolStripMenuItem,
            this.removeTeamTypeToolStripMenuItem});
            this.teamTypesContextMenuStrip.Name = "teamTypesContextMenuStrip";
            this.teamTypesContextMenuStrip.Size = new System.Drawing.Size(237, 64);
            // 
            // addTeamTypeToolStripMenuItem
            // 
            this.addTeamTypeToolStripMenuItem.Name = "addTeamTypeToolStripMenuItem";
            this.addTeamTypeToolStripMenuItem.Size = new System.Drawing.Size(236, 30);
            this.addTeamTypeToolStripMenuItem.Text = "&Add Team Type";
            this.addTeamTypeToolStripMenuItem.Click += new System.EventHandler(this.addTeamTypeToolStripMenuItem_Click);
            // 
            // removeTeamTypeToolStripMenuItem
            // 
            this.removeTeamTypeToolStripMenuItem.Name = "removeTeamTypeToolStripMenuItem";
            this.removeTeamTypeToolStripMenuItem.Size = new System.Drawing.Size(236, 30);
            this.removeTeamTypeToolStripMenuItem.Text = "&Remove Team Type";
            this.removeTeamTypeToolStripMenuItem.Click += new System.EventHandler(this.removeTeamTypeToolStripMenuItem_Click);
            // 
            // TeamTypesDialog
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(1313, 640);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "TeamTypesDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Team Types";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.settingsPanel.ResumeLayout(false);
            this.teamTypeTableLayoutPanel.ResumeLayout(false);
            this.teamTypeTableLayoutPanel.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.initNumNud)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.maxAllowedNud)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fearNud)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.recruitPriorityNud)).EndInit();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.teamsDataGridView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.missionsDataGridView)).EndInit();
            this.teamTypesContextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Panel settingsPanel;
        private System.Windows.Forms.ListView teamTypesListView;
        private System.Windows.Forms.ColumnHeader nameColumnHeader;
        private System.Windows.Forms.ContextMenuStrip teamTypesContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addTeamTypeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeTeamTypeToolStripMenuItem;
        private System.Windows.Forms.TableLayoutPanel teamTypeTableLayoutPanel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox houseComboBox;
        private System.Windows.Forms.CheckBox roundaboutCheckBox;
        private System.Windows.Forms.CheckBox learningCheckBox;
        private System.Windows.Forms.CheckBox suicideCheckBox;
        private System.Windows.Forms.CheckBox autocreateCheckBox;
        private System.Windows.Forms.CheckBox mercernaryCheckBox;
        private System.Windows.Forms.CheckBox reinforcableCheckBox;
        private System.Windows.Forms.CheckBox prebuiltCheckBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown initNumNud;
        private System.Windows.Forms.NumericUpDown maxAllowedNud;
        private System.Windows.Forms.NumericUpDown fearNud;
        private System.Windows.Forms.Label waypointLabel;
        private System.Windows.Forms.Label triggerLabel;
        private System.Windows.Forms.ComboBox waypointComboBox;
        private System.Windows.Forms.ComboBox triggerComboBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.DataGridView teamsDataGridView;
        private System.Windows.Forms.DataGridView missionsDataGridView;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown recruitPriorityNud;
        private System.Windows.Forms.DataGridViewComboBoxColumn teamsTypeColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn teamsCountColumn;
        private System.Windows.Forms.DataGridViewComboBoxColumn missionsMissionColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn missionsArgumentColumn;
    }
}