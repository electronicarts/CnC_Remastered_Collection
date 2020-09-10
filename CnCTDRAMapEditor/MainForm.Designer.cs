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
namespace MobiusEditor
{
    partial class MainForm
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
                activeTool?.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.mainMenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileNewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileOpenMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileSaveMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileSaveAsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.fileExportMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filePublishMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.fileRecentFilesMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.fileExitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editUndoMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editRedoMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsMapSettingsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsTeamTypesMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsTriggersMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.layersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersBoundariesMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersOverlayMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersTerrainMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersWaypointsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersObjectTriggersMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewLayersCellTriggersMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.developerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.developerGenerateMapPreviewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.developerGenerateMapPreviewDirectoryMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.developerGoToINIMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.developerDebugShowOverlapCellsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.cellStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.mouseToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.mainToolStrip = new System.Windows.Forms.ToolStrip();
            this.mapToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.smudgeToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.overlayToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.terrainToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.infantryToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.unitToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.buildingToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.resourcesToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.wallsToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.waypointsToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.cellTriggersToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.mapPanel = new MobiusEditor.Controls.MapPanel();
            this.copyrightStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.mainMenuStrip.SuspendLayout();
            this.mainStatusStrip.SuspendLayout();
            this.mainToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainMenuStrip
            // 
            this.mainMenuStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.mainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.settingsToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.developerToolStripMenuItem});
            this.mainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.mainMenuStrip.Name = "mainMenuStrip";
            this.mainMenuStrip.Padding = new System.Windows.Forms.Padding(8, 2, 0, 2);
            this.mainMenuStrip.Size = new System.Drawing.Size(1369, 28);
            this.mainMenuStrip.TabIndex = 1;
            this.mainMenuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileNewMenuItem,
            this.fileOpenMenuItem,
            this.fileSaveMenuItem,
            this.fileSaveAsMenuItem,
            this.toolStripMenuItem4,
            this.fileExportMenuItem,
            this.filePublishMenuItem,
            this.toolStripMenuItem3,
            this.fileRecentFilesMenuItem,
            this.toolStripMenuItem1,
            this.fileExitMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // fileNewMenuItem
            // 
            this.fileNewMenuItem.Name = "fileNewMenuItem";
            this.fileNewMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileNewMenuItem.Text = "&New...";
            this.fileNewMenuItem.Click += new System.EventHandler(this.fileNewMenuItem_Click);
            // 
            // fileOpenMenuItem
            // 
            this.fileOpenMenuItem.Name = "fileOpenMenuItem";
            this.fileOpenMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileOpenMenuItem.Text = "&Open...";
            this.fileOpenMenuItem.Click += new System.EventHandler(this.fileOpenMenuItem_Click);
            // 
            // fileSaveMenuItem
            // 
            this.fileSaveMenuItem.Name = "fileSaveMenuItem";
            this.fileSaveMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileSaveMenuItem.Text = "&Save";
            this.fileSaveMenuItem.Click += new System.EventHandler(this.fileSaveMenuItem_Click);
            // 
            // fileSaveAsMenuItem
            // 
            this.fileSaveAsMenuItem.Name = "fileSaveAsMenuItem";
            this.fileSaveAsMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileSaveAsMenuItem.Text = "Save &As...";
            this.fileSaveAsMenuItem.Click += new System.EventHandler(this.fileSaveAsMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(159, 6);
            // 
            // fileExportMenuItem
            // 
            this.fileExportMenuItem.Name = "fileExportMenuItem";
            this.fileExportMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileExportMenuItem.Text = "&Export...";
            this.fileExportMenuItem.Click += new System.EventHandler(this.fileExportMenuItem_Click);
            // 
            // filePublishMenuItem
            // 
            this.filePublishMenuItem.Name = "filePublishMenuItem";
            this.filePublishMenuItem.Size = new System.Drawing.Size(162, 26);
            this.filePublishMenuItem.Text = "&Publish...";
            this.filePublishMenuItem.Click += new System.EventHandler(this.filePublishMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(159, 6);
            // 
            // fileRecentFilesMenuItem
            // 
            this.fileRecentFilesMenuItem.Name = "fileRecentFilesMenuItem";
            this.fileRecentFilesMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileRecentFilesMenuItem.Text = "&Recent Files";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(159, 6);
            // 
            // fileExitMenuItem
            // 
            this.fileExitMenuItem.Name = "fileExitMenuItem";
            this.fileExitMenuItem.Size = new System.Drawing.Size(162, 26);
            this.fileExitMenuItem.Text = "&Exit";
            this.fileExitMenuItem.Click += new System.EventHandler(this.fileExitMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editUndoMenuItem,
            this.editRedoMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(47, 24);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // editUndoMenuItem
            // 
            this.editUndoMenuItem.Name = "editUndoMenuItem";
            this.editUndoMenuItem.ShortcutKeyDisplayString = "Ctrl + Z";
            this.editUndoMenuItem.Size = new System.Drawing.Size(179, 26);
            this.editUndoMenuItem.Text = "&Undo";
            this.editUndoMenuItem.Click += new System.EventHandler(this.editUndoMenuItem_Click);
            // 
            // editRedoMenuItem
            // 
            this.editRedoMenuItem.Name = "editRedoMenuItem";
            this.editRedoMenuItem.ShortcutKeyDisplayString = "Ctrl + Y";
            this.editRedoMenuItem.Size = new System.Drawing.Size(179, 26);
            this.editRedoMenuItem.Text = "&Redo";
            this.editRedoMenuItem.Click += new System.EventHandler(this.editRedoMenuItem_Click);
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingsMapSettingsMenuItem,
            this.settingsTeamTypesMenuItem,
            this.settingsTriggersMenuItem});
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(74, 24);
            this.settingsToolStripMenuItem.Text = "&Settings";
            // 
            // settingsMapSettingsMenuItem
            // 
            this.settingsMapSettingsMenuItem.Name = "settingsMapSettingsMenuItem";
            this.settingsMapSettingsMenuItem.Size = new System.Drawing.Size(180, 26);
            this.settingsMapSettingsMenuItem.Text = "&Map Settings...";
            this.settingsMapSettingsMenuItem.Click += new System.EventHandler(this.settingsMapSettingsMenuItem_Click);
            // 
            // settingsTeamTypesMenuItem
            // 
            this.settingsTeamTypesMenuItem.Name = "settingsTeamTypesMenuItem";
            this.settingsTeamTypesMenuItem.Size = new System.Drawing.Size(180, 26);
            this.settingsTeamTypesMenuItem.Text = "&Team Types...";
            this.settingsTeamTypesMenuItem.Click += new System.EventHandler(this.settingsTeamTypesMenuItem_Click);
            // 
            // settingsTriggersMenuItem
            // 
            this.settingsTriggersMenuItem.Name = "settingsTriggersMenuItem";
            this.settingsTriggersMenuItem.Size = new System.Drawing.Size(180, 26);
            this.settingsTriggersMenuItem.Text = "T&riggers...";
            this.settingsTriggersMenuItem.Click += new System.EventHandler(this.settingsTriggersMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.layersToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(53, 24);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // layersToolStripMenuItem
            // 
            this.layersToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewLayersBoundariesMenuItem,
            this.viewLayersOverlayMenuItem,
            this.viewLayersTerrainMenuItem,
            this.viewLayersWaypointsMenuItem,
            this.viewLayersObjectTriggersMenuItem,
            this.viewLayersCellTriggersMenuItem});
            this.layersToolStripMenuItem.Name = "layersToolStripMenuItem";
            this.layersToolStripMenuItem.Size = new System.Drawing.Size(125, 26);
            this.layersToolStripMenuItem.Text = "&Layers";
            // 
            // viewLayersBoundariesMenuItem
            // 
            this.viewLayersBoundariesMenuItem.Checked = true;
            this.viewLayersBoundariesMenuItem.CheckOnClick = true;
            this.viewLayersBoundariesMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersBoundariesMenuItem.Name = "viewLayersBoundariesMenuItem";
            this.viewLayersBoundariesMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersBoundariesMenuItem.Text = "&Boundaries";
            this.viewLayersBoundariesMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // viewLayersOverlayMenuItem
            // 
            this.viewLayersOverlayMenuItem.Checked = true;
            this.viewLayersOverlayMenuItem.CheckOnClick = true;
            this.viewLayersOverlayMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersOverlayMenuItem.Name = "viewLayersOverlayMenuItem";
            this.viewLayersOverlayMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersOverlayMenuItem.Text = "&Overlay";
            this.viewLayersOverlayMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // viewLayersTerrainMenuItem
            // 
            this.viewLayersTerrainMenuItem.Checked = true;
            this.viewLayersTerrainMenuItem.CheckOnClick = true;
            this.viewLayersTerrainMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersTerrainMenuItem.Name = "viewLayersTerrainMenuItem";
            this.viewLayersTerrainMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersTerrainMenuItem.Text = "&Terrain";
            this.viewLayersTerrainMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // viewLayersWaypointsMenuItem
            // 
            this.viewLayersWaypointsMenuItem.Checked = true;
            this.viewLayersWaypointsMenuItem.CheckOnClick = true;
            this.viewLayersWaypointsMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersWaypointsMenuItem.Name = "viewLayersWaypointsMenuItem";
            this.viewLayersWaypointsMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersWaypointsMenuItem.Text = "&Waypoints";
            this.viewLayersWaypointsMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // viewLayersObjectTriggersMenuItem
            // 
            this.viewLayersObjectTriggersMenuItem.Checked = true;
            this.viewLayersObjectTriggersMenuItem.CheckOnClick = true;
            this.viewLayersObjectTriggersMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersObjectTriggersMenuItem.Name = "viewLayersObjectTriggersMenuItem";
            this.viewLayersObjectTriggersMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersObjectTriggersMenuItem.Text = "O&bject Triggers";
            this.viewLayersObjectTriggersMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // viewLayersCellTriggersMenuItem
            // 
            this.viewLayersCellTriggersMenuItem.Checked = true;
            this.viewLayersCellTriggersMenuItem.CheckOnClick = true;
            this.viewLayersCellTriggersMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.viewLayersCellTriggersMenuItem.Name = "viewLayersCellTriggersMenuItem";
            this.viewLayersCellTriggersMenuItem.Size = new System.Drawing.Size(185, 26);
            this.viewLayersCellTriggersMenuItem.Text = "&Cell Triggers";
            this.viewLayersCellTriggersMenuItem.CheckedChanged += new System.EventHandler(this.viewLayersMenuItem_CheckedChanged);
            // 
            // developerToolStripMenuItem
            // 
            this.developerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.developerGenerateMapPreviewMenuItem,
            this.developerGoToINIMenuItem,
            this.toolStripMenuItem2,
            this.debugToolStripMenuItem});
            this.developerToolStripMenuItem.Name = "developerToolStripMenuItem";
            this.developerToolStripMenuItem.Size = new System.Drawing.Size(90, 24);
            this.developerToolStripMenuItem.Text = "&Developer";
            // 
            // developerGenerateMapPreviewMenuItem
            // 
            this.developerGenerateMapPreviewMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.developerGenerateMapPreviewDirectoryMenuItem});
            this.developerGenerateMapPreviewMenuItem.Name = "developerGenerateMapPreviewMenuItem";
            this.developerGenerateMapPreviewMenuItem.Size = new System.Drawing.Size(234, 26);
            this.developerGenerateMapPreviewMenuItem.Text = "&Generate map preview";
            this.developerGenerateMapPreviewMenuItem.Click += new System.EventHandler(this.developerGenerateMapPreviewMenuItem_Click);
            // 
            // developerGenerateMapPreviewDirectoryMenuItem
            // 
            this.developerGenerateMapPreviewDirectoryMenuItem.Name = "developerGenerateMapPreviewDirectoryMenuItem";
            this.developerGenerateMapPreviewDirectoryMenuItem.Size = new System.Drawing.Size(154, 26);
            this.developerGenerateMapPreviewDirectoryMenuItem.Text = "&Directory...";
            this.developerGenerateMapPreviewDirectoryMenuItem.Click += new System.EventHandler(this.developerGenerateMapPreviewDirectoryMenuItem_Click);
            // 
            // developerGoToINIMenuItem
            // 
            this.developerGoToINIMenuItem.Name = "developerGoToINIMenuItem";
            this.developerGoToINIMenuItem.Size = new System.Drawing.Size(234, 26);
            this.developerGoToINIMenuItem.Text = "Go to &INI";
            this.developerGoToINIMenuItem.Click += new System.EventHandler(this.developerGoToINIMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(231, 6);
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.developerDebugShowOverlapCellsMenuItem});
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(234, 26);
            this.debugToolStripMenuItem.Text = "&Debug";
            // 
            // developerDebugShowOverlapCellsMenuItem
            // 
            this.developerDebugShowOverlapCellsMenuItem.CheckOnClick = true;
            this.developerDebugShowOverlapCellsMenuItem.Name = "developerDebugShowOverlapCellsMenuItem";
            this.developerDebugShowOverlapCellsMenuItem.Size = new System.Drawing.Size(209, 26);
            this.developerDebugShowOverlapCellsMenuItem.Text = "Show &Overlap cells";
            this.developerDebugShowOverlapCellsMenuItem.CheckedChanged += new System.EventHandler(this.developerDebugShowOverlapCellsMenuItem_CheckedChanged);
            // 
            // mainStatusStrip
            // 
            this.mainStatusStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.mainStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStatusLabel,
            this.cellStatusLabel,
            this.copyrightStatusLabel});
            this.mainStatusStrip.Location = new System.Drawing.Point(0, 651);
            this.mainStatusStrip.Name = "mainStatusStrip";
            this.mainStatusStrip.Padding = new System.Windows.Forms.Padding(2, 0, 19, 0);
            this.mainStatusStrip.Size = new System.Drawing.Size(1369, 25);
            this.mainStatusStrip.TabIndex = 2;
            this.mainStatusStrip.Text = "statusStrip1";
            // 
            // toolStatusLabel
            // 
            this.toolStatusLabel.Name = "toolStatusLabel";
            this.toolStatusLabel.Size = new System.Drawing.Size(0, 20);
            // 
            // cellStatusLabel
            // 
            this.cellStatusLabel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.cellStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.cellStatusLabel.Name = "cellStatusLabel";
            this.cellStatusLabel.Size = new System.Drawing.Size(4, 20);
            // 
            // mainToolStrip
            // 
            this.mainToolStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.mainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mapToolStripButton,
            this.smudgeToolStripButton,
            this.overlayToolStripButton,
            this.terrainToolStripButton,
            this.infantryToolStripButton,
            this.unitToolStripButton,
            this.buildingToolStripButton,
            this.resourcesToolStripButton,
            this.wallsToolStripButton,
            this.waypointsToolStripButton,
            this.cellTriggersToolStripButton});
            this.mainToolStrip.Location = new System.Drawing.Point(0, 28);
            this.mainToolStrip.Name = "mainToolStrip";
            this.mainToolStrip.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.mainToolStrip.Size = new System.Drawing.Size(1369, 31);
            this.mainToolStrip.TabIndex = 3;
            this.mainToolStrip.Text = "toolStrip1";
            this.mainToolStrip.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mainToolStrip_MouseMove);
            // 
            // mapToolStripButton
            // 
            this.mapToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("mapToolStripButton.Image")));
            this.mapToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mapToolStripButton.Name = "mapToolStripButton";
            this.mapToolStripButton.Size = new System.Drawing.Size(67, 28);
            this.mapToolStripButton.Text = "Map";
            this.mapToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // smudgeToolStripButton
            // 
            this.smudgeToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("smudgeToolStripButton.Image")));
            this.smudgeToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.smudgeToolStripButton.Name = "smudgeToolStripButton";
            this.smudgeToolStripButton.Size = new System.Drawing.Size(92, 28);
            this.smudgeToolStripButton.Text = "Smudge";
            this.smudgeToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // overlayToolStripButton
            // 
            this.overlayToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("overlayToolStripButton.Image")));
            this.overlayToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.overlayToolStripButton.Name = "overlayToolStripButton";
            this.overlayToolStripButton.Size = new System.Drawing.Size(87, 28);
            this.overlayToolStripButton.Text = "Overlay";
            this.overlayToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // terrainToolStripButton
            // 
            this.terrainToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("terrainToolStripButton.Image")));
            this.terrainToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.terrainToolStripButton.Name = "terrainToolStripButton";
            this.terrainToolStripButton.Size = new System.Drawing.Size(82, 28);
            this.terrainToolStripButton.Text = "Terrain";
            this.terrainToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // infantryToolStripButton
            // 
            this.infantryToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("infantryToolStripButton.Image")));
            this.infantryToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.infantryToolStripButton.Name = "infantryToolStripButton";
            this.infantryToolStripButton.Size = new System.Drawing.Size(87, 28);
            this.infantryToolStripButton.Text = "Infantry";
            this.infantryToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // unitToolStripButton
            // 
            this.unitToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("unitToolStripButton.Image")));
            this.unitToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.unitToolStripButton.Name = "unitToolStripButton";
            this.unitToolStripButton.Size = new System.Drawing.Size(70, 28);
            this.unitToolStripButton.Text = "Units";
            this.unitToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // buildingToolStripButton
            // 
            this.buildingToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("buildingToolStripButton.Image")));
            this.buildingToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buildingToolStripButton.Name = "buildingToolStripButton";
            this.buildingToolStripButton.Size = new System.Drawing.Size(102, 28);
            this.buildingToolStripButton.Text = "Structures";
            this.buildingToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // resourcesToolStripButton
            // 
            this.resourcesToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("resourcesToolStripButton.Image")));
            this.resourcesToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.resourcesToolStripButton.Name = "resourcesToolStripButton";
            this.resourcesToolStripButton.Size = new System.Drawing.Size(103, 28);
            this.resourcesToolStripButton.Text = "Resources";
            this.resourcesToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // wallsToolStripButton
            // 
            this.wallsToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("wallsToolStripButton.Image")));
            this.wallsToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.wallsToolStripButton.Name = "wallsToolStripButton";
            this.wallsToolStripButton.Size = new System.Drawing.Size(72, 28);
            this.wallsToolStripButton.Text = "Walls";
            this.wallsToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // waypointsToolStripButton
            // 
            this.waypointsToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("waypointsToolStripButton.Image")));
            this.waypointsToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.waypointsToolStripButton.Name = "waypointsToolStripButton";
            this.waypointsToolStripButton.Size = new System.Drawing.Size(106, 28);
            this.waypointsToolStripButton.Text = "Waypoints";
            this.waypointsToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // cellTriggersToolStripButton
            // 
            this.cellTriggersToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("cellTriggersToolStripButton.Image")));
            this.cellTriggersToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cellTriggersToolStripButton.Name = "cellTriggersToolStripButton";
            this.cellTriggersToolStripButton.Size = new System.Drawing.Size(119, 28);
            this.cellTriggersToolStripButton.Text = "Cell Triggers";
            this.cellTriggersToolStripButton.Click += new System.EventHandler(this.mainToolStripButton_Click);
            // 
            // mapPanel
            // 
            this.mapPanel.BackColor = System.Drawing.Color.Black;
            this.mapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mapPanel.FocusOnMouseEnter = true;
            this.mapPanel.Location = new System.Drawing.Point(0, 59);
            this.mapPanel.MapImage = null;
            this.mapPanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.mapPanel.MaxZoom = 8;
            this.mapPanel.MinZoom = 1;
            this.mapPanel.Name = "mapPanel";
            this.mapPanel.Quality = 2;
            this.mapPanel.Size = new System.Drawing.Size(1369, 592);
            this.mapPanel.TabIndex = 4;
            this.mapPanel.Zoom = 1;
            this.mapPanel.ZoomStep = 1;
            this.mapPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseMove);
            // 
            // copyrightStatusLabel
            // 
            this.copyrightStatusLabel.Name = "copyrightStatusLabel";
            this.copyrightStatusLabel.Size = new System.Drawing.Size(1305, 20);
            this.copyrightStatusLabel.Spring = true;
            this.copyrightStatusLabel.Text = "©2020 Electronic Arts Inc.";
            this.copyrightStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1369, 676);
            this.Controls.Add(this.mapPanel);
            this.Controls.Add(this.mainToolStrip);
            this.Controls.Add(this.mainStatusStrip);
            this.Controls.Add(this.mainMenuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.mainMenuStrip;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "MainForm";
            this.Text = "CnC TDRA Map Editor";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.mainMenuStrip.ResumeLayout(false);
            this.mainMenuStrip.PerformLayout();
            this.mainStatusStrip.ResumeLayout(false);
            this.mainStatusStrip.PerformLayout();
            this.mainToolStrip.ResumeLayout(false);
            this.mainToolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip mainMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileOpenMenuItem;
        private System.Windows.Forms.StatusStrip mainStatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel cellStatusLabel;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileSaveAsMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem fileExitMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileNewMenuItem;
        private System.Windows.Forms.ToolStripMenuItem developerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem developerGenerateMapPreviewMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileExportMenuItem;
        private System.Windows.Forms.ToolStripMenuItem developerGenerateMapPreviewDirectoryMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsMapSettingsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editUndoMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editRedoMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem developerDebugShowOverlapCellsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filePublishMenuItem;
        private System.Windows.Forms.ToolTip mouseToolTip;
        private System.Windows.Forms.ToolStrip mainToolStrip;
        private System.Windows.Forms.ToolStripButton mapToolStripButton;
        private System.Windows.Forms.ToolStripButton smudgeToolStripButton;
        private System.Windows.Forms.ToolStripButton overlayToolStripButton;
        private System.Windows.Forms.ToolStripButton terrainToolStripButton;
        private System.Windows.Forms.ToolStripButton infantryToolStripButton;
        private System.Windows.Forms.ToolStripButton unitToolStripButton;
        private System.Windows.Forms.ToolStripButton buildingToolStripButton;
        private System.Windows.Forms.ToolStripButton resourcesToolStripButton;
        private System.Windows.Forms.ToolStripButton wallsToolStripButton;
        private System.Windows.Forms.ToolStripButton waypointsToolStripButton;
        private Controls.MapPanel mapPanel;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem fileRecentFilesMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileSaveMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem developerGoToINIMenuItem;
        private System.Windows.Forms.ToolStripButton cellTriggersToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem settingsTeamTypesMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsTriggersMenuItem;
        private System.Windows.Forms.ToolStripMenuItem layersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersBoundariesMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersObjectTriggersMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersCellTriggersMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersWaypointsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersTerrainMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewLayersOverlayMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStatusLabel;
        private System.Windows.Forms.ToolStripStatusLabel copyrightStatusLabel;
    }
}

