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
    partial class GenericToolDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TemplateToolDialog));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.genericTypeMapPanel = new MobiusEditor.Controls.MapPanel();
            this.genericTypeComboBox = new MobiusEditor.Controls.TypeComboBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.genericTypeMapPanel, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.genericTypeComboBox, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(274, 254);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // templateTypeMapPanel
            // 
            this.genericTypeMapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.genericTypeMapPanel.Location = new System.Drawing.Point(3, 30);
            this.genericTypeMapPanel.MaxZoom = 8;
            this.genericTypeMapPanel.MinZoom = 1;
            this.genericTypeMapPanel.Name = "templateTypeMapPanel";
            this.genericTypeMapPanel.Quality = 1;
            this.genericTypeMapPanel.Size = new System.Drawing.Size(268, 221);
            this.genericTypeMapPanel.TabIndex = 3;
            this.genericTypeMapPanel.Zoom = 1;
            // 
            // templateTypeComboBox
            // 
            this.genericTypeComboBox.DisplayMember = "Name";
            this.genericTypeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.genericTypeComboBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.genericTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.genericTypeComboBox.FormattingEnabled = true;
            this.genericTypeComboBox.Location = new System.Drawing.Point(3, 3);
            this.genericTypeComboBox.MissingThumbnail = ((System.Drawing.Image)(resources.GetObject("templateTypeComboBox.MissingThumbnail")));
            this.genericTypeComboBox.Name = "templateTypeComboBox";
            this.genericTypeComboBox.Size = new System.Drawing.Size(268, 21);
            this.genericTypeComboBox.TabIndex = 2;
            this.genericTypeComboBox.ValueMember = "Type";
            // 
            // TemplateToolDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(274, 254);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(290, 293);
            this.Name = "TemplateToolDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Map";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private Controls.TypeComboBox genericTypeComboBox;
        private Controls.MapPanel genericTypeMapPanel;
    }
}