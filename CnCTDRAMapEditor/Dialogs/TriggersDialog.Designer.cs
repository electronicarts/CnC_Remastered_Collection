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
    partial class TriggersDialog
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
            this.triggersTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.houseComboBox = new System.Windows.Forms.ComboBox();
            this.typeLabel = new System.Windows.Forms.Label();
            this.event1Label = new System.Windows.Forms.Label();
            this.event2Label = new System.Windows.Forms.Label();
            this.action1Label = new System.Windows.Forms.Label();
            this.action2Label = new System.Windows.Forms.Label();
            this.action1ComboBox = new System.Windows.Forms.ComboBox();
            this.action2ComboBox = new System.Windows.Forms.ComboBox();
            this.existenceLabel = new System.Windows.Forms.Label();
            this.existenceComboBox = new System.Windows.Forms.ComboBox();
            this.typeComboBox = new System.Windows.Forms.ComboBox();
            this.event1ComboBox = new System.Windows.Forms.ComboBox();
            this.event2ComboBox = new System.Windows.Forms.ComboBox();
            this.teamLabel = new System.Windows.Forms.Label();
            this.teamComboBox = new System.Windows.Forms.ComboBox();
            this.event1Flp = new System.Windows.Forms.FlowLayoutPanel();
            this.event1Nud = new System.Windows.Forms.NumericUpDown();
            this.event1ValueComboBox = new System.Windows.Forms.ComboBox();
            this.event2Flp = new System.Windows.Forms.FlowLayoutPanel();
            this.event2Nud = new System.Windows.Forms.NumericUpDown();
            this.event2ValueComboBox = new System.Windows.Forms.ComboBox();
            this.action1Flp = new System.Windows.Forms.FlowLayoutPanel();
            this.action1Nud = new System.Windows.Forms.NumericUpDown();
            this.action1ValueComboBox = new System.Windows.Forms.ComboBox();
            this.action2Flp = new System.Windows.Forms.FlowLayoutPanel();
            this.action2Nud = new System.Windows.Forms.NumericUpDown();
            this.action2ValueComboBox = new System.Windows.Forms.ComboBox();
            this.triggersListView = new System.Windows.Forms.ListView();
            this.nameColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.triggersContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addTriggerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeTriggerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tableLayoutPanel1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.settingsPanel.SuspendLayout();
            this.triggersTableLayoutPanel.SuspendLayout();
            this.event1Flp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.event1Nud)).BeginInit();
            this.event2Flp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.event2Nud)).BeginInit();
            this.action1Flp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.action1Nud)).BeginInit();
            this.action2Flp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.action2Nud)).BeginInit();
            this.triggersContextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.flowLayoutPanel1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.settingsPanel, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.triggersListView, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(562, 499);
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
            this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 462);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(556, 34);
            this.flowLayoutPanel1.TabIndex = 1;
            // 
            // btnCancel
            // 
            this.btnCancel.AutoSize = true;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(486, 2);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(2);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(68, 30);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            this.btnOK.AutoSize = true;
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(432, 2);
            this.btnOK.Margin = new System.Windows.Forms.Padding(2);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(50, 30);
            this.btnOK.TabIndex = 3;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // settingsPanel
            // 
            this.settingsPanel.Controls.Add(this.triggersTableLayoutPanel);
            this.settingsPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.settingsPanel.Location = new System.Drawing.Point(149, 10);
            this.settingsPanel.Margin = new System.Windows.Forms.Padding(10);
            this.settingsPanel.Name = "settingsPanel";
            this.settingsPanel.Size = new System.Drawing.Size(403, 439);
            this.settingsPanel.TabIndex = 3;
            // 
            // triggersTableLayoutPanel
            // 
            this.triggersTableLayoutPanel.AutoSize = true;
            this.triggersTableLayoutPanel.ColumnCount = 4;
            this.triggersTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.triggersTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.triggersTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.triggersTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.triggersTableLayoutPanel.Controls.Add(this.label1, 0, 0);
            this.triggersTableLayoutPanel.Controls.Add(this.houseComboBox, 1, 0);
            this.triggersTableLayoutPanel.Controls.Add(this.typeLabel, 0, 9);
            this.triggersTableLayoutPanel.Controls.Add(this.event1Label, 0, 10);
            this.triggersTableLayoutPanel.Controls.Add(this.event2Label, 0, 11);
            this.triggersTableLayoutPanel.Controls.Add(this.action1Label, 0, 12);
            this.triggersTableLayoutPanel.Controls.Add(this.action2Label, 0, 13);
            this.triggersTableLayoutPanel.Controls.Add(this.action1ComboBox, 1, 12);
            this.triggersTableLayoutPanel.Controls.Add(this.action2ComboBox, 1, 13);
            this.triggersTableLayoutPanel.Controls.Add(this.existenceLabel, 0, 8);
            this.triggersTableLayoutPanel.Controls.Add(this.existenceComboBox, 1, 8);
            this.triggersTableLayoutPanel.Controls.Add(this.typeComboBox, 1, 9);
            this.triggersTableLayoutPanel.Controls.Add(this.event1ComboBox, 1, 10);
            this.triggersTableLayoutPanel.Controls.Add(this.event2ComboBox, 1, 11);
            this.triggersTableLayoutPanel.Controls.Add(this.teamLabel, 0, 14);
            this.triggersTableLayoutPanel.Controls.Add(this.teamComboBox, 1, 14);
            this.triggersTableLayoutPanel.Controls.Add(this.event1Flp, 2, 10);
            this.triggersTableLayoutPanel.Controls.Add(this.event2Flp, 2, 11);
            this.triggersTableLayoutPanel.Controls.Add(this.action1Flp, 2, 12);
            this.triggersTableLayoutPanel.Controls.Add(this.action2Flp, 2, 13);
            this.triggersTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.triggersTableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.triggersTableLayoutPanel.Margin = new System.Windows.Forms.Padding(2);
            this.triggersTableLayoutPanel.Name = "triggersTableLayoutPanel";
            this.triggersTableLayoutPanel.RowCount = 17;
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.triggersTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.triggersTableLayoutPanel.Size = new System.Drawing.Size(403, 439);
            this.triggersTableLayoutPanel.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(2, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "House";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // houseComboBox
            // 
            this.houseComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.houseComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.houseComboBox.FormattingEnabled = true;
            this.houseComboBox.Location = new System.Drawing.Point(73, 2);
            this.houseComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.houseComboBox.Name = "houseComboBox";
            this.houseComboBox.Size = new System.Drawing.Size(141, 21);
            this.houseComboBox.TabIndex = 1;
            // 
            // typeLabel
            // 
            this.typeLabel.AutoSize = true;
            this.typeLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.typeLabel.Location = new System.Drawing.Point(2, 50);
            this.typeLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.typeLabel.Name = "typeLabel";
            this.typeLabel.Size = new System.Drawing.Size(67, 25);
            this.typeLabel.TabIndex = 9;
            this.typeLabel.Text = "Type";
            this.typeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // event1Label
            // 
            this.event1Label.AutoSize = true;
            this.event1Label.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event1Label.Location = new System.Drawing.Point(2, 75);
            this.event1Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.event1Label.Name = "event1Label";
            this.event1Label.Size = new System.Drawing.Size(67, 57);
            this.event1Label.TabIndex = 10;
            this.event1Label.Text = "Event 1";
            this.event1Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // event2Label
            // 
            this.event2Label.AutoSize = true;
            this.event2Label.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event2Label.Location = new System.Drawing.Point(2, 132);
            this.event2Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.event2Label.Name = "event2Label";
            this.event2Label.Size = new System.Drawing.Size(67, 57);
            this.event2Label.TabIndex = 11;
            this.event2Label.Text = "Event 2";
            this.event2Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // action1Label
            // 
            this.action1Label.AutoSize = true;
            this.action1Label.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action1Label.Location = new System.Drawing.Point(2, 189);
            this.action1Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.action1Label.Name = "action1Label";
            this.action1Label.Size = new System.Drawing.Size(67, 57);
            this.action1Label.TabIndex = 15;
            this.action1Label.Text = "Action 1";
            this.action1Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // action2Label
            // 
            this.action2Label.AutoSize = true;
            this.action2Label.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action2Label.Location = new System.Drawing.Point(2, 246);
            this.action2Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.action2Label.Name = "action2Label";
            this.action2Label.Size = new System.Drawing.Size(67, 57);
            this.action2Label.TabIndex = 16;
            this.action2Label.Text = "Action 2";
            this.action2Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // action1ComboBox
            // 
            this.action1ComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action1ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.action1ComboBox.FormattingEnabled = true;
            this.action1ComboBox.Location = new System.Drawing.Point(73, 191);
            this.action1ComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.action1ComboBox.Name = "action1ComboBox";
            this.action1ComboBox.Size = new System.Drawing.Size(141, 21);
            this.action1ComboBox.TabIndex = 17;
            this.action1ComboBox.SelectedIndexChanged += new System.EventHandler(this.trigger1ComboBox_SelectedIndexChanged);
            // 
            // action2ComboBox
            // 
            this.action2ComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action2ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.action2ComboBox.FormattingEnabled = true;
            this.action2ComboBox.Location = new System.Drawing.Point(73, 248);
            this.action2ComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.action2ComboBox.Name = "action2ComboBox";
            this.action2ComboBox.Size = new System.Drawing.Size(141, 21);
            this.action2ComboBox.TabIndex = 18;
            this.action2ComboBox.SelectedIndexChanged += new System.EventHandler(this.trigger2ComboBox_SelectedIndexChanged);
            // 
            // existenceLabel
            // 
            this.existenceLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.existenceLabel.Location = new System.Drawing.Point(2, 25);
            this.existenceLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.existenceLabel.Name = "existenceLabel";
            this.existenceLabel.Size = new System.Drawing.Size(67, 25);
            this.existenceLabel.TabIndex = 19;
            this.existenceLabel.Text = "Existence";
            this.existenceLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // existenceComboBox
            // 
            this.existenceComboBox.DisplayMember = "Name";
            this.existenceComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.existenceComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.existenceComboBox.FormattingEnabled = true;
            this.existenceComboBox.Location = new System.Drawing.Point(73, 27);
            this.existenceComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.existenceComboBox.Name = "existenceComboBox";
            this.existenceComboBox.Size = new System.Drawing.Size(141, 21);
            this.existenceComboBox.TabIndex = 20;
            this.existenceComboBox.ValueMember = "Value";
            // 
            // typeComboBox
            // 
            this.typeComboBox.DisplayMember = "Name";
            this.typeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeComboBox.FormattingEnabled = true;
            this.typeComboBox.Location = new System.Drawing.Point(73, 52);
            this.typeComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.typeComboBox.Name = "typeComboBox";
            this.typeComboBox.Size = new System.Drawing.Size(141, 21);
            this.typeComboBox.TabIndex = 21;
            this.typeComboBox.ValueMember = "Value";
            this.typeComboBox.SelectedValueChanged += new System.EventHandler(this.typeComboBox_SelectedValueChanged);
            // 
            // event1ComboBox
            // 
            this.event1ComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event1ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.event1ComboBox.FormattingEnabled = true;
            this.event1ComboBox.Location = new System.Drawing.Point(73, 77);
            this.event1ComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.event1ComboBox.Name = "event1ComboBox";
            this.event1ComboBox.Size = new System.Drawing.Size(141, 21);
            this.event1ComboBox.TabIndex = 22;
            this.event1ComboBox.SelectedIndexChanged += new System.EventHandler(this.trigger1ComboBox_SelectedIndexChanged);
            // 
            // event2ComboBox
            // 
            this.event2ComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event2ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.event2ComboBox.FormattingEnabled = true;
            this.event2ComboBox.Location = new System.Drawing.Point(73, 134);
            this.event2ComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.event2ComboBox.Name = "event2ComboBox";
            this.event2ComboBox.Size = new System.Drawing.Size(141, 21);
            this.event2ComboBox.TabIndex = 23;
            this.event2ComboBox.SelectedIndexChanged += new System.EventHandler(this.trigger2ComboBox_SelectedIndexChanged);
            // 
            // teamLabel
            // 
            this.teamLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.teamLabel.Location = new System.Drawing.Point(2, 303);
            this.teamLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.teamLabel.Name = "teamLabel";
            this.teamLabel.Size = new System.Drawing.Size(67, 25);
            this.teamLabel.TabIndex = 32;
            this.teamLabel.Text = "Team";
            this.teamLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // teamComboBox
            // 
            this.teamComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.teamComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.teamComboBox.FormattingEnabled = true;
            this.teamComboBox.Location = new System.Drawing.Point(73, 305);
            this.teamComboBox.Margin = new System.Windows.Forms.Padding(2);
            this.teamComboBox.Name = "teamComboBox";
            this.teamComboBox.Size = new System.Drawing.Size(141, 21);
            this.teamComboBox.TabIndex = 33;
            // 
            // event1Flp
            // 
            this.event1Flp.AutoSize = true;
            this.event1Flp.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.event1Flp.Controls.Add(this.event1Nud);
            this.event1Flp.Controls.Add(this.event1ValueComboBox);
            this.event1Flp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event1Flp.Location = new System.Drawing.Point(219, 78);
            this.event1Flp.Name = "event1Flp";
            this.event1Flp.Size = new System.Drawing.Size(172, 51);
            this.event1Flp.TabIndex = 36;
            // 
            // event1Nud
            // 
            this.event1Nud.Location = new System.Drawing.Point(2, 2);
            this.event1Nud.Margin = new System.Windows.Forms.Padding(2);
            this.event1Nud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.event1Nud.Minimum = new decimal(new int[] {
            -2147483648,
            0,
            0,
            -2147483648});
            this.event1Nud.Name = "event1Nud";
            this.event1Nud.Size = new System.Drawing.Size(70, 20);
            this.event1Nud.TabIndex = 31;
            // 
            // event1ValueComboBox
            // 
            this.event1ValueComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.event1ValueComboBox.FormattingEnabled = true;
            this.event1ValueComboBox.Location = new System.Drawing.Point(3, 27);
            this.event1ValueComboBox.Name = "event1ValueComboBox";
            this.event1ValueComboBox.Size = new System.Drawing.Size(166, 21);
            this.event1ValueComboBox.TabIndex = 32;
            // 
            // event2Flp
            // 
            this.event2Flp.AutoSize = true;
            this.event2Flp.Controls.Add(this.event2Nud);
            this.event2Flp.Controls.Add(this.event2ValueComboBox);
            this.event2Flp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.event2Flp.Location = new System.Drawing.Point(219, 135);
            this.event2Flp.Name = "event2Flp";
            this.event2Flp.Size = new System.Drawing.Size(172, 51);
            this.event2Flp.TabIndex = 37;
            // 
            // event2Nud
            // 
            this.event2Nud.Location = new System.Drawing.Point(2, 2);
            this.event2Nud.Margin = new System.Windows.Forms.Padding(2);
            this.event2Nud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.event2Nud.Minimum = new decimal(new int[] {
            -2147483648,
            0,
            0,
            -2147483648});
            this.event2Nud.Name = "event2Nud";
            this.event2Nud.Size = new System.Drawing.Size(70, 20);
            this.event2Nud.TabIndex = 30;
            // 
            // event2ValueComboBox
            // 
            this.event2ValueComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.event2ValueComboBox.FormattingEnabled = true;
            this.event2ValueComboBox.Location = new System.Drawing.Point(3, 27);
            this.event2ValueComboBox.Name = "event2ValueComboBox";
            this.event2ValueComboBox.Size = new System.Drawing.Size(166, 21);
            this.event2ValueComboBox.TabIndex = 33;
            // 
            // action1Flp
            // 
            this.action1Flp.AutoSize = true;
            this.action1Flp.Controls.Add(this.action1Nud);
            this.action1Flp.Controls.Add(this.action1ValueComboBox);
            this.action1Flp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action1Flp.Location = new System.Drawing.Point(219, 192);
            this.action1Flp.Name = "action1Flp";
            this.action1Flp.Size = new System.Drawing.Size(172, 51);
            this.action1Flp.TabIndex = 38;
            // 
            // action1Nud
            // 
            this.action1Nud.Location = new System.Drawing.Point(2, 2);
            this.action1Nud.Margin = new System.Windows.Forms.Padding(2);
            this.action1Nud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.action1Nud.Minimum = new decimal(new int[] {
            -2147483648,
            0,
            0,
            -2147483648});
            this.action1Nud.Name = "action1Nud";
            this.action1Nud.Size = new System.Drawing.Size(70, 20);
            this.action1Nud.TabIndex = 31;
            // 
            // action1ValueComboBox
            // 
            this.action1ValueComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.action1ValueComboBox.FormattingEnabled = true;
            this.action1ValueComboBox.Location = new System.Drawing.Point(3, 27);
            this.action1ValueComboBox.Name = "action1ValueComboBox";
            this.action1ValueComboBox.Size = new System.Drawing.Size(166, 21);
            this.action1ValueComboBox.TabIndex = 33;
            // 
            // action2Flp
            // 
            this.action2Flp.AutoSize = true;
            this.action2Flp.Controls.Add(this.action2Nud);
            this.action2Flp.Controls.Add(this.action2ValueComboBox);
            this.action2Flp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.action2Flp.Location = new System.Drawing.Point(219, 249);
            this.action2Flp.Name = "action2Flp";
            this.action2Flp.Size = new System.Drawing.Size(172, 51);
            this.action2Flp.TabIndex = 39;
            // 
            // action2Nud
            // 
            this.action2Nud.Location = new System.Drawing.Point(2, 2);
            this.action2Nud.Margin = new System.Windows.Forms.Padding(2);
            this.action2Nud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.action2Nud.Minimum = new decimal(new int[] {
            -2147483648,
            0,
            0,
            -2147483648});
            this.action2Nud.Name = "action2Nud";
            this.action2Nud.Size = new System.Drawing.Size(70, 20);
            this.action2Nud.TabIndex = 32;
            // 
            // action2ValueComboBox
            // 
            this.action2ValueComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.action2ValueComboBox.FormattingEnabled = true;
            this.action2ValueComboBox.Location = new System.Drawing.Point(3, 27);
            this.action2ValueComboBox.Name = "action2ValueComboBox";
            this.action2ValueComboBox.Size = new System.Drawing.Size(166, 21);
            this.action2ValueComboBox.TabIndex = 33;
            // 
            // triggersListView
            // 
            this.triggersListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.nameColumnHeader});
            this.triggersListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.triggersListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.triggersListView.HideSelection = false;
            this.triggersListView.LabelEdit = true;
            this.triggersListView.Location = new System.Drawing.Point(2, 2);
            this.triggersListView.Margin = new System.Windows.Forms.Padding(2);
            this.triggersListView.MultiSelect = false;
            this.triggersListView.Name = "triggersListView";
            this.triggersListView.ShowItemToolTips = true;
            this.triggersListView.Size = new System.Drawing.Size(135, 455);
            this.triggersListView.TabIndex = 4;
            this.triggersListView.UseCompatibleStateImageBehavior = false;
            this.triggersListView.View = System.Windows.Forms.View.Details;
            this.triggersListView.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.teamTypesListView_AfterLabelEdit);
            this.triggersListView.SelectedIndexChanged += new System.EventHandler(this.triggersListView_SelectedIndexChanged);
            this.triggersListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.teamTypesListView_KeyDown);
            this.triggersListView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.teamTypesListView_MouseDown);
            // 
            // triggersContextMenuStrip
            // 
            this.triggersContextMenuStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.triggersContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addTriggerToolStripMenuItem,
            this.removeTriggerToolStripMenuItem});
            this.triggersContextMenuStrip.Name = "teamTypesContextMenuStrip";
            this.triggersContextMenuStrip.Size = new System.Drawing.Size(157, 48);
            // 
            // addTriggerToolStripMenuItem
            // 
            this.addTriggerToolStripMenuItem.Name = "addTriggerToolStripMenuItem";
            this.addTriggerToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.addTriggerToolStripMenuItem.Text = "&Add Trigger";
            this.addTriggerToolStripMenuItem.Click += new System.EventHandler(this.addTriggerToolStripMenuItem_Click);
            // 
            // removeTriggerToolStripMenuItem
            // 
            this.removeTriggerToolStripMenuItem.Name = "removeTriggerToolStripMenuItem";
            this.removeTriggerToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.removeTriggerToolStripMenuItem.Text = "&Remove Trigger";
            this.removeTriggerToolStripMenuItem.Click += new System.EventHandler(this.removeTriggerToolStripMenuItem_Click);
            // 
            // TriggersDialog
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(562, 499);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "TriggersDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Triggers";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.settingsPanel.ResumeLayout(false);
            this.settingsPanel.PerformLayout();
            this.triggersTableLayoutPanel.ResumeLayout(false);
            this.triggersTableLayoutPanel.PerformLayout();
            this.event1Flp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.event1Nud)).EndInit();
            this.event2Flp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.event2Nud)).EndInit();
            this.action1Flp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.action1Nud)).EndInit();
            this.action2Flp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.action2Nud)).EndInit();
            this.triggersContextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Panel settingsPanel;
        private System.Windows.Forms.ListView triggersListView;
        private System.Windows.Forms.ColumnHeader nameColumnHeader;
        private System.Windows.Forms.ContextMenuStrip triggersContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addTriggerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeTriggerToolStripMenuItem;
        private System.Windows.Forms.TableLayoutPanel triggersTableLayoutPanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox houseComboBox;
        private System.Windows.Forms.Label typeLabel;
        private System.Windows.Forms.Label event1Label;
        private System.Windows.Forms.Label event2Label;
        private System.Windows.Forms.Label action2Label;
        private System.Windows.Forms.ComboBox action2ComboBox;
        private System.Windows.Forms.Label existenceLabel;
        private System.Windows.Forms.ComboBox existenceComboBox;
        private System.Windows.Forms.ComboBox typeComboBox;
        private System.Windows.Forms.ComboBox event1ComboBox;
        private System.Windows.Forms.ComboBox event2ComboBox;
        private System.Windows.Forms.ComboBox action1ComboBox;
        private System.Windows.Forms.Label action1Label;
        private System.Windows.Forms.Label teamLabel;
        private System.Windows.Forms.ComboBox teamComboBox;
        private System.Windows.Forms.FlowLayoutPanel event1Flp;
        private System.Windows.Forms.NumericUpDown event1Nud;
        private System.Windows.Forms.FlowLayoutPanel event2Flp;
        private System.Windows.Forms.NumericUpDown event2Nud;
        private System.Windows.Forms.FlowLayoutPanel action1Flp;
        private System.Windows.Forms.NumericUpDown action1Nud;
        private System.Windows.Forms.FlowLayoutPanel action2Flp;
        private System.Windows.Forms.NumericUpDown action2Nud;
        private System.Windows.Forms.ComboBox event1ValueComboBox;
        private System.Windows.Forms.ComboBox event2ValueComboBox;
        private System.Windows.Forms.ComboBox action1ValueComboBox;
        private System.Windows.Forms.ComboBox action2ValueComboBox;
    }
}