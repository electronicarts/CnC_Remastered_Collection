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
    partial class SteamDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SteamDialog));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.descriptionTxt = new System.Windows.Forms.TextBox();
            this.titleTxt = new System.Windows.Forms.TextBox();
            this.visibilityComboBox = new System.Windows.Forms.ComboBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.previewTxt = new System.Windows.Forms.TextBox();
            this.previewBtn = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnGoToSteam = new System.Windows.Forms.Button();
            this.publicMapContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.publishAsNewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusLbl = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.btnPublishMap = new MobiusEditor.Controls.MenuButton();
            this.imageTooltip = new MobiusEditor.Controls.ImageTooltip();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.publicMapContextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(500, 385);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.label4, 0, 3);
            this.tableLayoutPanel2.Controls.Add(this.descriptionTxt, 0, 4);
            this.tableLayoutPanel2.Controls.Add(this.titleTxt, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.visibilityComboBox, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.panel1, 1, 2);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(2, 2);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 5;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 13F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(496, 233);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(2, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 24);
            this.label1.TabIndex = 0;
            this.label1.Text = "Title";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(2, 24);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 25);
            this.label2.TabIndex = 1;
            this.label2.Text = "Visibility";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Location = new System.Drawing.Point(2, 49);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 27);
            this.label3.TabIndex = 2;
            this.label3.Text = "Preview";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label4
            // 
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Location = new System.Drawing.Point(2, 76);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(69, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Description";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // descriptionTxt
            // 
            this.tableLayoutPanel2.SetColumnSpan(this.descriptionTxt, 2);
            this.descriptionTxt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.descriptionTxt.Location = new System.Drawing.Point(2, 91);
            this.descriptionTxt.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.descriptionTxt.Multiline = true;
            this.descriptionTxt.Name = "descriptionTxt";
            this.descriptionTxt.Size = new System.Drawing.Size(492, 202);
            this.descriptionTxt.TabIndex = 4;
            this.descriptionTxt.TextChanged += new System.EventHandler(this.descriptionTxt_TextChanged);
            // 
            // titleTxt
            // 
            this.titleTxt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.titleTxt.Location = new System.Drawing.Point(75, 2);
            this.titleTxt.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.titleTxt.Name = "titleTxt";
            this.titleTxt.Size = new System.Drawing.Size(419, 20);
            this.titleTxt.TabIndex = 5;
            // 
            // visibilityComboBox
            // 
            this.visibilityComboBox.DisplayMember = "Name";
            this.visibilityComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.visibilityComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.visibilityComboBox.FormattingEnabled = true;
            this.visibilityComboBox.Location = new System.Drawing.Point(75, 26);
            this.visibilityComboBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.visibilityComboBox.Name = "visibilityComboBox";
            this.visibilityComboBox.Size = new System.Drawing.Size(419, 21);
            this.visibilityComboBox.TabIndex = 6;
            this.visibilityComboBox.ValueMember = "Value";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.previewTxt);
            this.panel1.Controls.Add(this.previewBtn);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(75, 51);
            this.panel1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(419, 23);
            this.panel1.TabIndex = 7;
            // 
            // previewTxt
            // 
            this.previewTxt.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.previewTxt.Location = new System.Drawing.Point(2, 2);
            this.previewTxt.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.previewTxt.Name = "previewTxt";
            this.previewTxt.Size = new System.Drawing.Size(392, 20);
            this.previewTxt.TabIndex = 2;
            this.previewTxt.TextChanged += new System.EventHandler(this.previewTxt_TextChanged);
            // 
            // previewBtn
            // 
            this.previewBtn.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.previewBtn.AutoSize = true;
            this.previewBtn.Location = new System.Drawing.Point(392, 2);
            this.previewBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.previewBtn.Name = "previewBtn";
            this.previewBtn.Size = new System.Drawing.Size(26, 23);
            this.previewBtn.TabIndex = 3;
            this.previewBtn.Text = "...";
            this.previewBtn.UseVisualStyleBackColor = true;
            this.previewBtn.Click += new System.EventHandler(this.previewBtn_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.btnClose);
            this.panel2.Controls.Add(this.btnGoToSteam);
            this.panel2.Controls.Add(this.btnPublishMap);
            this.panel2.Controls.Add(this.statusLbl);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(2, 239);
            this.panel2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(496, 34);
            this.panel2.TabIndex = 3;
            // 
            // btnClose
            // 
            this.btnClose.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClose.AutoSize = true;
            this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnClose.Location = new System.Drawing.Point(426, 2);
            this.btnClose.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(68, 30);
            this.btnClose.TabIndex = 6;
            this.btnClose.Text = "&Close";
            this.btnClose.UseVisualStyleBackColor = true;
            // 
            // btnGoToSteam
            // 
            this.btnGoToSteam.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnGoToSteam.AutoSize = true;
            this.btnGoToSteam.Location = new System.Drawing.Point(346, 2);
            this.btnGoToSteam.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnGoToSteam.Name = "btnGoToSteam";
            this.btnGoToSteam.Size = new System.Drawing.Size(76, 30);
            this.btnGoToSteam.TabIndex = 7;
            this.btnGoToSteam.Text = "Go to &Steam";
            this.btnGoToSteam.UseVisualStyleBackColor = true;
            this.btnGoToSteam.Click += new System.EventHandler(this.btnGoToSteam_Click);
            // 
            // publicMapContextMenuStrip
            // 
            this.publicMapContextMenuStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.publicMapContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.publishAsNewToolStripMenuItem});
            this.publicMapContextMenuStrip.Name = "publicMapContextMenuStrip";
            this.publicMapContextMenuStrip.Size = new System.Drawing.Size(157, 26);
            // 
            // publishAsNewToolStripMenuItem
            // 
            this.publishAsNewToolStripMenuItem.Name = "publishAsNewToolStripMenuItem";
            this.publishAsNewToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.publishAsNewToolStripMenuItem.Text = "Publish As New";
            this.publishAsNewToolStripMenuItem.Click += new System.EventHandler(this.publishAsNewToolStripMenuItem_Click);
            // 
            // statusLbl
            // 
            this.statusLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusLbl.Location = new System.Drawing.Point(7, 1);
            this.statusLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.statusLbl.Name = "statusLbl";
            this.statusLbl.Size = new System.Drawing.Size(264, 31);
            this.statusLbl.TabIndex = 9;
            this.statusLbl.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label5
            // 
            this.label5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label5.Location = new System.Drawing.Point(3, 275);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(494, 110);
            this.label5.TabIndex = 4;
            this.label5.Text = resources.GetString("label5.Text");
            // 
            // btnPublishMap
            // 
            this.btnPublishMap.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPublishMap.AutoSize = true;
            this.btnPublishMap.Location = new System.Drawing.Point(234, 2);
            this.btnPublishMap.Margin = new System.Windows.Forms.Padding(2);
            this.btnPublishMap.Menu = this.publicMapContextMenuStrip;
            this.btnPublishMap.Name = "btnPublishMap";
            this.btnPublishMap.Size = new System.Drawing.Size(111, 30);
            this.btnPublishMap.TabIndex = 8;
            this.btnPublishMap.Text = "&Publish Map";
            this.btnPublishMap.UseVisualStyleBackColor = true;
            this.btnPublishMap.Click += new System.EventHandler(this.btnPublishMap_Click);
            // 
            // imageTooltip
            // 
            this.imageTooltip.MaxSize = new System.Drawing.Size(0, 0);
            this.imageTooltip.OwnerDraw = true;
            this.imageTooltip.ShowAlways = true;
            // 
            // SteamDialog
            // 
            this.AcceptButton = this.btnPublishMap;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.CancelButton = this.btnClose;
            this.ClientSize = new System.Drawing.Size(500, 385);
            this.ControlBox = false;
            this.Controls.Add(this.tableLayoutPanel1);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(516, 357);
            this.Name = "SteamDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Steam Workshop: Publish Custom Map";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.publicMapContextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox descriptionTxt;
        private System.Windows.Forms.TextBox titleTxt;
        private System.Windows.Forms.ComboBox visibilityComboBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox previewTxt;
        private System.Windows.Forms.Button previewBtn;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnGoToSteam;
        private MobiusEditor.Controls.MenuButton btnPublishMap;
        private System.Windows.Forms.Label statusLbl;
        private System.Windows.Forms.ContextMenuStrip publicMapContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem publishAsNewToolStripMenuItem;
        private Controls.ImageTooltip imageTooltip;
        private System.Windows.Forms.Label label5;
    }
}