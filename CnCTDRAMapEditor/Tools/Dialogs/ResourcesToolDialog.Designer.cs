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
namespace MobiusEditor.Tools.Dialogs
{
    partial class ResourcesToolDialog
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
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.label10 = new System.Windows.Forms.Label();
            this.totalResourcesLbl = new System.Windows.Forms.Label();
            this.gemsCheckBox = new System.Windows.Forms.CheckBox();
            this.resourceBrushSizeNud = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.tableLayoutPanel6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.resourceBrushSizeNud)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 2;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel6.Controls.Add(this.label10, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.totalResourcesLbl, 1, 0);
            this.tableLayoutPanel6.Controls.Add(this.gemsCheckBox, 0, 3);
            this.tableLayoutPanel6.Controls.Add(this.resourceBrushSizeNud, 1, 2);
            this.tableLayoutPanel6.Controls.Add(this.label1, 0, 2);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 4;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(196, 67);
            this.tableLayoutPanel6.TabIndex = 1;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(3, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(88, 13);
            this.label10.TabIndex = 0;
            this.label10.Text = "Total Resources:";
            // 
            // totalResourcesLbl
            // 
            this.totalResourcesLbl.AutoSize = true;
            this.totalResourcesLbl.Location = new System.Drawing.Point(97, 0);
            this.totalResourcesLbl.Name = "totalResourcesLbl";
            this.totalResourcesLbl.Size = new System.Drawing.Size(13, 13);
            this.totalResourcesLbl.TabIndex = 1;
            this.totalResourcesLbl.Text = "0";
            // 
            // gemsCheckBox
            // 
            this.gemsCheckBox.AutoSize = true;
            this.tableLayoutPanel6.SetColumnSpan(this.gemsCheckBox, 2);
            this.gemsCheckBox.Location = new System.Drawing.Point(3, 42);
            this.gemsCheckBox.Name = "gemsCheckBox";
            this.gemsCheckBox.Size = new System.Drawing.Size(53, 17);
            this.gemsCheckBox.TabIndex = 2;
            this.gemsCheckBox.Text = "Gems";
            this.gemsCheckBox.UseVisualStyleBackColor = true;
            // 
            // resourceBrushSizeNud
            // 
            this.resourceBrushSizeNud.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.resourceBrushSizeNud.Increment = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.resourceBrushSizeNud.Location = new System.Drawing.Point(97, 16);
            this.resourceBrushSizeNud.Maximum = new decimal(new int[] {
            9,
            0,
            0,
            0});
            this.resourceBrushSizeNud.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.resourceBrushSizeNud.Name = "resourceBrushSizeNud";
            this.resourceBrushSizeNud.Size = new System.Drawing.Size(99, 20);
            this.resourceBrushSizeNud.TabIndex = 3;
            this.resourceBrushSizeNud.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 26);
            this.label1.TabIndex = 4;
            this.label1.Text = "Brush Size";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ResourcesToolDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(196, 67);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel6);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(212, 106);
            this.Name = "ResourcesToolDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Resources";
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.resourceBrushSizeNud)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label totalResourcesLbl;
        private System.Windows.Forms.CheckBox gemsCheckBox;
        private System.Windows.Forms.NumericUpDown resourceBrushSizeNud;
        private System.Windows.Forms.Label label1;
    }
}