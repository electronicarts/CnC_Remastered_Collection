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
namespace MobiusEditor.Controls
{
    partial class ObjectProperties
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.directionLabel = new System.Windows.Forms.Label();
            this.missionLabel = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.houseComboBox = new MobiusEditor.Controls.PropertiesComboBox();
            this.strengthNud = new System.Windows.Forms.NumericUpDown();
            this.directionComboBox = new MobiusEditor.Controls.PropertiesComboBox();
            this.missionComboBox = new MobiusEditor.Controls.PropertiesComboBox();
            this.triggerComboBox = new MobiusEditor.Controls.PropertiesComboBox();
            this.basePriorityNud = new System.Windows.Forms.NumericUpDown();
            this.prebuiltCheckBox = new System.Windows.Forms.CheckBox();
            this.basePriorityLabel = new System.Windows.Forms.Label();
            this.sellableCheckBox = new System.Windows.Forms.CheckBox();
            this.rebuildCheckBox = new System.Windows.Forms.CheckBox();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.strengthNud)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.basePriorityNud)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.25F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.75F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.directionLabel, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.missionLabel, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.houseComboBox, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.strengthNud, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.directionComboBox, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.missionComboBox, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.triggerComboBox, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.basePriorityNud, 1, 5);
            this.tableLayoutPanel1.Controls.Add(this.prebuiltCheckBox, 1, 7);
            this.tableLayoutPanel1.Controls.Add(this.basePriorityLabel, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.sellableCheckBox, 1, 8);
            this.tableLayoutPanel1.Controls.Add(this.rebuildCheckBox, 1, 9);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 11;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(299, 262);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(4, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 32);
            this.label1.TabIndex = 0;
            this.label1.Text = "House";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(4, 32);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 30);
            this.label2.TabIndex = 1;
            this.label2.Text = "Strength";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // directionLabel
            // 
            this.directionLabel.AutoSize = true;
            this.directionLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.directionLabel.Location = new System.Drawing.Point(4, 62);
            this.directionLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.directionLabel.Name = "directionLabel";
            this.directionLabel.Size = new System.Drawing.Size(85, 32);
            this.directionLabel.TabIndex = 2;
            this.directionLabel.Text = "Direction";
            this.directionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // missionLabel
            // 
            this.missionLabel.AutoSize = true;
            this.missionLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.missionLabel.Location = new System.Drawing.Point(4, 94);
            this.missionLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.missionLabel.Name = "missionLabel";
            this.missionLabel.Size = new System.Drawing.Size(85, 32);
            this.missionLabel.TabIndex = 3;
            this.missionLabel.Text = "Mission";
            this.missionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label5.Location = new System.Drawing.Point(4, 126);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(85, 32);
            this.label5.TabIndex = 4;
            this.label5.Text = "Trigger";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // houseComboBox
            // 
            this.houseComboBox.DisplayMember = "Name";
            this.houseComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.houseComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.houseComboBox.FormattingEnabled = true;
            this.houseComboBox.Location = new System.Drawing.Point(97, 4);
            this.houseComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.houseComboBox.Name = "houseComboBox";
            this.houseComboBox.Size = new System.Drawing.Size(198, 24);
            this.houseComboBox.TabIndex = 5;
            this.houseComboBox.ValueMember = "Type";
            this.houseComboBox.SelectedIndexChanged += new System.EventHandler(this.comboBox_SelectedValueChanged);
            // 
            // strengthNud
            // 
            this.strengthNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.strengthNud.Location = new System.Drawing.Point(97, 36);
            this.strengthNud.Margin = new System.Windows.Forms.Padding(4);
            this.strengthNud.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.strengthNud.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.strengthNud.Name = "strengthNud";
            this.strengthNud.Size = new System.Drawing.Size(198, 22);
            this.strengthNud.TabIndex = 6;
            this.strengthNud.Value = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.strengthNud.ValueChanged += new System.EventHandler(this.nud_ValueChanged);
            // 
            // directionComboBox
            // 
            this.directionComboBox.DisplayMember = "Name";
            this.directionComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.directionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.directionComboBox.FormattingEnabled = true;
            this.directionComboBox.Location = new System.Drawing.Point(97, 66);
            this.directionComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.directionComboBox.Name = "directionComboBox";
            this.directionComboBox.Size = new System.Drawing.Size(198, 24);
            this.directionComboBox.TabIndex = 7;
            this.directionComboBox.ValueMember = "Type";
            this.directionComboBox.SelectedIndexChanged += new System.EventHandler(this.comboBox_SelectedValueChanged);
            // 
            // missionComboBox
            // 
            this.missionComboBox.DisplayMember = "Name";
            this.missionComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.missionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.missionComboBox.FormattingEnabled = true;
            this.missionComboBox.Location = new System.Drawing.Point(97, 98);
            this.missionComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.missionComboBox.Name = "missionComboBox";
            this.missionComboBox.Size = new System.Drawing.Size(198, 24);
            this.missionComboBox.TabIndex = 8;
            this.missionComboBox.ValueMember = "Type";
            this.missionComboBox.SelectedIndexChanged += new System.EventHandler(this.comboBox_SelectedValueChanged);
            // 
            // triggerComboBox
            // 
            this.triggerComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.triggerComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.triggerComboBox.FormattingEnabled = true;
            this.triggerComboBox.Location = new System.Drawing.Point(97, 130);
            this.triggerComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.triggerComboBox.Name = "triggerComboBox";
            this.triggerComboBox.Size = new System.Drawing.Size(198, 24);
            this.triggerComboBox.TabIndex = 9;
            this.triggerComboBox.SelectedIndexChanged += new System.EventHandler(this.comboBox_SelectedValueChanged);
            // 
            // basePriorityNud
            // 
            this.basePriorityNud.Dock = System.Windows.Forms.DockStyle.Fill;
            this.basePriorityNud.Location = new System.Drawing.Point(96, 160);
            this.basePriorityNud.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.basePriorityNud.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.basePriorityNud.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.basePriorityNud.Name = "basePriorityNud";
            this.basePriorityNud.Size = new System.Drawing.Size(200, 22);
            this.basePriorityNud.TabIndex = 11;
            this.basePriorityNud.ValueChanged += new System.EventHandler(this.nud_ValueChanged);
            // 
            // prebuiltCheckBox
            // 
            this.prebuiltCheckBox.AutoSize = true;
            this.prebuiltCheckBox.Location = new System.Drawing.Point(96, 186);
            this.prebuiltCheckBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.prebuiltCheckBox.Name = "prebuiltCheckBox";
            this.prebuiltCheckBox.Size = new System.Drawing.Size(78, 21);
            this.prebuiltCheckBox.TabIndex = 12;
            this.prebuiltCheckBox.Text = "Prebuilt";
            this.prebuiltCheckBox.UseVisualStyleBackColor = true;
            this.prebuiltCheckBox.CheckedChanged += new System.EventHandler(this.checkBox_CheckedChanged);
            // 
            // basePriorityLabel
            // 
            this.basePriorityLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.basePriorityLabel.Location = new System.Drawing.Point(3, 158);
            this.basePriorityLabel.Name = "basePriorityLabel";
            this.basePriorityLabel.Size = new System.Drawing.Size(87, 26);
            this.basePriorityLabel.TabIndex = 13;
            this.basePriorityLabel.Text = "Base Priority";
            this.basePriorityLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // sellableCheckBox
            // 
            this.sellableCheckBox.AutoSize = true;
            this.sellableCheckBox.Location = new System.Drawing.Point(96, 211);
            this.sellableCheckBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.sellableCheckBox.Name = "sellableCheckBox";
            this.sellableCheckBox.Size = new System.Drawing.Size(80, 21);
            this.sellableCheckBox.TabIndex = 14;
            this.sellableCheckBox.Text = "Sellable";
            this.sellableCheckBox.UseVisualStyleBackColor = true;
            this.sellableCheckBox.CheckedChanged += new System.EventHandler(this.checkBox_CheckedChanged);
            // 
            // rebuildCheckBox
            // 
            this.rebuildCheckBox.AutoSize = true;
            this.rebuildCheckBox.Location = new System.Drawing.Point(96, 236);
            this.rebuildCheckBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.rebuildCheckBox.Name = "rebuildCheckBox";
            this.rebuildCheckBox.Size = new System.Drawing.Size(78, 21);
            this.rebuildCheckBox.TabIndex = 15;
            this.rebuildCheckBox.Text = "Rebuild";
            this.rebuildCheckBox.UseVisualStyleBackColor = true;
            this.rebuildCheckBox.CheckedChanged += new System.EventHandler(this.checkBox_CheckedChanged);
            // 
            // ObjectProperties
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tableLayoutPanel1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "ObjectProperties";
            this.Size = new System.Drawing.Size(299, 262);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.strengthNud)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.basePriorityNud)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label directionLabel;
        private System.Windows.Forms.Label missionLabel;
        private System.Windows.Forms.Label label5;
        private MobiusEditor.Controls.PropertiesComboBox houseComboBox;
        private System.Windows.Forms.NumericUpDown strengthNud;
        private MobiusEditor.Controls.PropertiesComboBox directionComboBox;
        private MobiusEditor.Controls.PropertiesComboBox missionComboBox;
        private MobiusEditor.Controls.PropertiesComboBox triggerComboBox;
        private System.Windows.Forms.NumericUpDown basePriorityNud;
        private System.Windows.Forms.CheckBox prebuiltCheckBox;
        private System.Windows.Forms.Label basePriorityLabel;
        private System.Windows.Forms.CheckBox sellableCheckBox;
        private System.Windows.Forms.CheckBox rebuildCheckBox;
    }
}
