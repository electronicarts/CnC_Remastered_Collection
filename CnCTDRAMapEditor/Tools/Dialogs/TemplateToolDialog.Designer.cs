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
    partial class TemplateToolDialog
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.templateTypeListView = new System.Windows.Forms.ListView();
            this.templateTypeMapPanel = new MobiusEditor.Controls.MapPanel();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(2, 2);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.templateTypeListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.templateTypeMapPanel);
            this.splitContainer1.Size = new System.Drawing.Size(270, 545);
            this.splitContainer1.SplitterDistance = 275;
            this.splitContainer1.TabIndex = 7;
            // 
            // templateTypeListView
            // 
            this.templateTypeListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateTypeListView.HideSelection = false;
            this.templateTypeListView.Location = new System.Drawing.Point(0, 0);
            this.templateTypeListView.Margin = new System.Windows.Forms.Padding(0);
            this.templateTypeListView.MultiSelect = false;
            this.templateTypeListView.Name = "templateTypeListView";
            this.templateTypeListView.Size = new System.Drawing.Size(270, 275);
            this.templateTypeListView.TabIndex = 7;
            this.templateTypeListView.UseCompatibleStateImageBehavior = false;
            // 
            // templateTypeMapPanel
            // 
            this.templateTypeMapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateTypeMapPanel.Location = new System.Drawing.Point(0, 0);
            this.templateTypeMapPanel.MapImage = null;
            this.templateTypeMapPanel.Margin = new System.Windows.Forms.Padding(0);
            this.templateTypeMapPanel.MaxZoom = 8;
            this.templateTypeMapPanel.MinZoom = 1;
            this.templateTypeMapPanel.Name = "templateTypeMapPanel";
            this.templateTypeMapPanel.Quality = 1;
            this.templateTypeMapPanel.Size = new System.Drawing.Size(270, 266);
            this.templateTypeMapPanel.TabIndex = 6;
            this.templateTypeMapPanel.Zoom = 1;
            this.templateTypeMapPanel.ZoomStep = 1;
            // 
            // TemplateToolDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(274, 549);
            this.ControlBox = false;
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(288, 287);
            this.Name = "TemplateToolDialog";
            this.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Map";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView templateTypeListView;
        private Controls.MapPanel templateTypeMapPanel;
    }
}