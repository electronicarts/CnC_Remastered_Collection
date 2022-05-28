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
    partial class ObjectToolDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ObjectToolDialog));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.objectTypeMapPanel = new MobiusEditor.Controls.MapPanel();
            this.objectTypeComboBox = new MobiusEditor.Controls.TypeComboBox();
            this.objectProperties = new MobiusEditor.Controls.ObjectProperties();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.objectTypeMapPanel, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.objectTypeComboBox, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.objectProperties, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(365, 582);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // objectTypeMapPanel
            // 
            this.objectTypeMapPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.objectTypeMapPanel.Location = new System.Drawing.Point(4, 35);
            this.objectTypeMapPanel.MapImage = null;
            this.objectTypeMapPanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.objectTypeMapPanel.MaxZoom = 8;
            this.objectTypeMapPanel.MinZoom = 1;
            this.objectTypeMapPanel.Name = "objectTypeMapPanel";
            this.objectTypeMapPanel.Quality = 1;
            this.objectTypeMapPanel.Size = new System.Drawing.Size(357, 272);
            this.objectTypeMapPanel.TabIndex = 3;
            this.objectTypeMapPanel.Zoom = 1;
            this.objectTypeMapPanel.ZoomStep = 1;
            // 
            // objectTypeComboBox
            // 
            this.objectTypeComboBox.DisplayMember = "Name";
            this.objectTypeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.objectTypeComboBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.objectTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.objectTypeComboBox.FormattingEnabled = true;
            this.objectTypeComboBox.Location = new System.Drawing.Point(4, 4);
            this.objectTypeComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.objectTypeComboBox.MissingThumbnail = ((System.Drawing.Image)(resources.GetObject("objectTypeComboBox.MissingThumbnail")));
            this.objectTypeComboBox.Name = "objectTypeComboBox";
            this.objectTypeComboBox.Size = new System.Drawing.Size(357, 23);
            this.objectTypeComboBox.TabIndex = 2;
            this.objectTypeComboBox.ValueMember = "Type";
            // 
            // objectProperties
            // 
            this.objectProperties.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.objectProperties.Location = new System.Drawing.Point(5, 316);
            this.objectProperties.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.objectProperties.Name = "objectProperties";
            this.objectProperties.Object = null;
            this.objectProperties.Size = new System.Drawing.Size(355, 261);
            this.objectProperties.TabIndex = 4;
            // 
            // ObjectToolDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(365, 582);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(381, 528);
            this.Name = "ObjectToolDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Map";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private Controls.TypeComboBox objectTypeComboBox;
        private Controls.MapPanel objectTypeMapPanel;
        private Controls.ObjectProperties objectProperties;
    }
}