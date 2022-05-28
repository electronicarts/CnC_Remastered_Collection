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
    partial class TerrainToolDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TerrainToolDialog));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.terrainTypeMapPanel = new MobiusEditor.Controls.MapPanel();
            this.terrainTypeComboBox = new MobiusEditor.Controls.TypeComboBox();
            this.terrainProperties = new MobiusEditor.Controls.TerrainProperties();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Controls.Add(this.terrainTypeMapPanel, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.terrainTypeComboBox, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.terrainProperties, 0, 3);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(411, 466);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // terrainTypeMapPanel
            // 
            this.terrainTypeMapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.terrainTypeMapPanel.Location = new System.Drawing.Point(4, 42);
            this.terrainTypeMapPanel.MapImage = null;
            this.terrainTypeMapPanel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.terrainTypeMapPanel.MaxZoom = 8;
            this.terrainTypeMapPanel.MinZoom = 1;
            this.terrainTypeMapPanel.Name = "terrainTypeMapPanel";
            this.terrainTypeMapPanel.Quality = 1;
            this.terrainTypeMapPanel.Size = new System.Drawing.Size(403, 370);
            this.terrainTypeMapPanel.TabIndex = 3;
            this.terrainTypeMapPanel.Zoom = 1;
            this.terrainTypeMapPanel.ZoomStep = 1;
            // 
            // terrainTypeComboBox
            // 
            this.terrainTypeComboBox.DisplayMember = "Name";
            this.terrainTypeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.terrainTypeComboBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.terrainTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.terrainTypeComboBox.FormattingEnabled = true;
            this.terrainTypeComboBox.Location = new System.Drawing.Point(4, 5);
            this.terrainTypeComboBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.terrainTypeComboBox.MissingThumbnail = ((System.Drawing.Image)(resources.GetObject("terrainTypeComboBox.MissingThumbnail")));
            this.terrainTypeComboBox.Name = "terrainTypeComboBox";
            this.terrainTypeComboBox.Size = new System.Drawing.Size(403, 27);
            this.terrainTypeComboBox.TabIndex = 2;
            this.terrainTypeComboBox.ValueMember = "Type";
            // 
            // terrainProperties
            // 
            this.terrainProperties.Dock = System.Windows.Forms.DockStyle.Top;
            this.terrainProperties.Location = new System.Drawing.Point(4, 422);
            this.terrainProperties.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.terrainProperties.Name = "terrainProperties";
            this.terrainProperties.Size = new System.Drawing.Size(403, 38);
            this.terrainProperties.TabIndex = 4;
            this.terrainProperties.Terrain = null;
            // 
            // TerrainToolDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(411, 466);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(424, 421);
            this.Name = "TerrainToolDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Terrain";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private Controls.TypeComboBox terrainTypeComboBox;
        private Controls.MapPanel terrainTypeMapPanel;
        private Controls.TerrainProperties terrainProperties;
    }
}