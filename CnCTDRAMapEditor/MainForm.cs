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
using MobiusEditor.Dialogs;
using MobiusEditor.Event;
using MobiusEditor.Interface;
using MobiusEditor.Model;
using MobiusEditor.Tools;
using MobiusEditor.Tools.Dialogs;
using MobiusEditor.Utility;
using Steamworks;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MobiusEditor
{
    public partial class MainForm : Form
    {
        [Flags]
        private enum ToolType
        {
            None        = 0,
            Map         = 1 << 0,
            Smudge      = 1 << 1,
            Overlay     = 1 << 2,
            Terrain     = 1 << 3,
            Infantry    = 1 << 4,
            Unit        = 1 << 5,
            Building    = 1 << 6,
            Resources   = 1 << 7,
            Wall        = 1 << 8,
            Waypoint    = 1 << 9,
            CellTrigger = 1 << 10
        }

        private static readonly ToolType[] toolTypes;

        private ToolType availableToolTypes = ToolType.None;

        private ToolType activeToolType = ToolType.None;
        private ToolType ActiveToolType
        {
            get => activeToolType;
            set
            {
                var firstAvailableTool = value;
                if ((availableToolTypes & firstAvailableTool) == ToolType.None)
                {
                    var otherAvailableToolTypes = toolTypes.Where(t => (availableToolTypes & t) != ToolType.None);
                    firstAvailableTool = otherAvailableToolTypes.Any() ? otherAvailableToolTypes.First() : ToolType.None;
                }

                if (activeToolType != firstAvailableTool)
                {
                    activeToolType = firstAvailableTool;
                    RefreshActiveTool();
                }
            }
        }

        private MapLayerFlag activeLayers;
        public MapLayerFlag ActiveLayers
        {
            get => activeLayers;
            set
            {
                if (activeLayers != value)
                {
                    activeLayers = value;
                    if (activeTool != null)
                    {
                        activeTool.Layers = ActiveLayers;
                    }
                }
            }
        }

        private ITool activeTool;
        private Form activeToolForm;

        private IGamePlugin plugin;
        private string filename;

        private readonly MRU mru;

        private readonly UndoRedoList<UndoRedoEventArgs> url = new UndoRedoList<UndoRedoEventArgs>();

        private readonly Timer steamUpdateTimer = new Timer();

        static MainForm()
        {
            toolTypes = ((IEnumerable<ToolType>)Enum.GetValues(typeof(ToolType))).Where(t => t != ToolType.None).ToArray();
        }

        public MainForm()
        {
            InitializeComponent();

            mru = new MRU("Software\\Petroglyph\\CnCRemasteredEditor", 10, fileRecentFilesMenuItem);
            mru.FileSelected += Mru_FileSelected;

            foreach (ToolStripButton toolStripButton in mainToolStrip.Items)
            {
                toolStripButton.MouseMove += mainToolStrip_MouseMove;
            }

#if !DEVELOPER
            fileExportMenuItem.Visible = false;
            developerToolStripMenuItem.Visible = false;
#endif

            url.Tracked += UndoRedo_Updated;
            url.Undone += UndoRedo_Updated;
            url.Redone += UndoRedo_Updated;
            UpdateUndoRedo();

            steamUpdateTimer.Interval = 500;
            steamUpdateTimer.Tick += SteamUpdateTimer_Tick;
        }

        private void SteamUpdateTimer_Tick(object sender, EventArgs e)
        {
            if (SteamworksUGC.IsInit)
            {
                SteamworksUGC.Service();
            }
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            RefreshAvailableTools();
            UpdateVisibleLayers();

            filePublishMenuItem.Visible = SteamworksUGC.IsInit;

            steamUpdateTimer.Start();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            steamUpdateTimer.Stop();
            steamUpdateTimer.Dispose();
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Q)
            {
                mapToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.W)
            {
                smudgeToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.E)
            {
                overlayToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.R)
            {
                terrainToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.T)
            {
                infantryToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.Y)
            {
                unitToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.A)
            {
                buildingToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.S)
            {
                resourcesToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.D)
            {
                wallsToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.F)
            {
                waypointsToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == Keys.G)
            {
                cellTriggersToolStripButton.PerformClick();
                return true;
            }
            else if (keyData == (Keys.Control | Keys.Z))
            {
                if (editUndoMenuItem.Enabled)
                {
                    editUndoMenuItem_Click(this, new EventArgs());
                }
                return true;
            }
            else if (keyData == (Keys.Control | Keys.Y))
            {
                if (editRedoMenuItem.Enabled)
                {
                    editRedoMenuItem_Click(this, new EventArgs());
                }
                return true;
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void UpdateUndoRedo()
        {
            editUndoMenuItem.Enabled = url.CanUndo;
            editRedoMenuItem.Enabled = url.CanRedo;
        }

        private void UndoRedo_Updated(object sender, EventArgs e)
        {
            UpdateUndoRedo();
        }

        private void fileNewMenuItem_Click(object sender, EventArgs e)
        {
            if (!PromptSaveMap())
            {
                return;
            }

            NewMapDialog nmd = new NewMapDialog();
            if (nmd.ShowDialog() == DialogResult.OK)
            {
                if (plugin != null)
                {
                    plugin.Map.Triggers.CollectionChanged -= Triggers_CollectionChanged;
                    plugin.Dispose();
                }
                plugin = null;

                Globals.TheTilesetManager.Reset();
                Globals.TheTextureManager.Reset();

                if (nmd.GameType == GameType.TiberianDawn)
                {
                    Globals.TheTeamColorManager.Reset();
                    Globals.TheTeamColorManager.Load(@"DATA\XML\CNCTDTEAMCOLORS.XML");

                    plugin = new TiberianDawn.GamePlugin();
                    plugin.New(nmd.TheaterName);
                }
                else if (nmd.GameType == GameType.RedAlert)
                {
                    Globals.TheTeamColorManager.Reset();
                    Globals.TheTeamColorManager.Load(@"DATA\XML\CNCRATEAMCOLORS.XML");

                    plugin = new RedAlert.GamePlugin();
                    plugin.New(nmd.TheaterName);
                }

                if (SteamworksUGC.IsInit)
                {
                    plugin.Map.BasicSection.Author = SteamFriends.GetPersonaName();
                }

                plugin.Map.Triggers.CollectionChanged += Triggers_CollectionChanged;
                mapPanel.MapImage = plugin.MapImage;

                filename = null;
                Text = "CnC TDRA Map Editor";
                url.Clear();

                ClearActiveTool();
                RefreshAvailableTools();
                RefreshActiveTool();
            }
        }

        private void fileOpenMenuItem_Click(object sender, EventArgs e)
        {
            if (!PromptSaveMap())
            {
                return;
            }

            var pgmFilter =
#if DEVELOPER
                "|PGM files (*.pgm)|*.pgm"
#else
                string.Empty
#endif
            ;

            OpenFileDialog ofd = new OpenFileDialog
            {
                AutoUpgradeEnabled = false,
                RestoreDirectory = true
            };
            ofd.Filter = "Tiberian Dawn files (*.ini;*.bin)|*.ini;*.bin|Red Alert files (*.mpr)|*.mpr" + pgmFilter  + "|All files (*.*)|*.*";
            if (plugin != null)
            {
                switch (plugin.GameType)
                {
                    case GameType.TiberianDawn:
                        ofd.InitialDirectory = TiberianDawn.Constants.SaveDirectory;
                        ofd.FilterIndex = 1;
                        break;
                    case GameType.RedAlert:
                        ofd.InitialDirectory = RedAlert.Constants.SaveDirectory;
                        ofd.FilterIndex = 2;
                        break;
                }
            }
            else
            {
                ofd.InitialDirectory = Globals.RootSaveDirectory;
            }
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                var fileInfo = new FileInfo(ofd.FileName);
                if (LoadFile(fileInfo.FullName))
                {
                    mru.Add(fileInfo);
                }
                else
                {
                    mru.Remove(fileInfo);
                    MessageBox.Show(string.Format("Error loading {0}.", ofd.FileName), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void fileSaveMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            if (string.IsNullOrEmpty(filename))
            {
                fileSaveAsMenuItem.PerformClick();
            }
            else
            {
                var fileInfo = new FileInfo(filename);
                if (SaveFile(fileInfo.FullName))
                {
                    mru.Add(fileInfo);
                }
                else
                {
                    mru.Remove(fileInfo);
                }
            }
        }

        private void fileSaveAsMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            SaveFileDialog sfd = new SaveFileDialog
            {
                AutoUpgradeEnabled = false,
                RestoreDirectory = true
            };
            var filters = new List<string>();
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    filters.Add("Tiberian Dawn files (*.ini;*.bin)|*.ini;*.bin");
                    sfd.InitialDirectory = TiberianDawn.Constants.SaveDirectory;
                    break;
                case GameType.RedAlert:
                    filters.Add("Red Alert files (*.mpr)|*.mpr");
                    sfd.InitialDirectory = RedAlert.Constants.SaveDirectory;
                    break;
            }
            filters.Add("All files (*.*)|*.*");

            sfd.Filter = string.Join("|", filters);
            if (!string.IsNullOrEmpty(filename))
            {
                sfd.InitialDirectory = Path.GetDirectoryName(filename);
                sfd.FileName = Path.GetFileName(filename);
            }
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                var fileInfo = new FileInfo(sfd.FileName);
                if (SaveFile(fileInfo.FullName))
                {
                    mru.Add(fileInfo);
                }
                else
                {
                    mru.Remove(fileInfo);
                }
            }
        }

        private void fileExportMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            SaveFileDialog sfd = new SaveFileDialog
            {
                AutoUpgradeEnabled = false,
                RestoreDirectory = true
            };
            sfd.Filter = "MEG files (*.meg)|*.meg";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                plugin.Save(sfd.FileName, FileType.MEG);
            }
        }

        private void fileExitMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void editUndoMenuItem_Click(object sender, EventArgs e)
        {
            if (url.CanUndo)
            {
                url.Undo(new UndoRedoEventArgs(mapPanel, plugin.Map));
            }
        }

        private void editRedoMenuItem_Click(object sender, EventArgs e)
        {
            if (url.CanRedo)
            {
                url.Redo(new UndoRedoEventArgs(mapPanel, plugin.Map));
            }
        }

        private void settingsMapSettingsMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            var basicSettings = new PropertyTracker<BasicSection>(plugin.Map.BasicSection);
            var briefingSettings = new PropertyTracker<BriefingSection>(plugin.Map.BriefingSection);
            var houseSettingsTrackers = plugin.Map.Houses.ToDictionary(h => h, h => new PropertyTracker<House>(h));

            MapSettingsDialog msd = new MapSettingsDialog(plugin, basicSettings, briefingSettings, houseSettingsTrackers);
            if (msd.ShowDialog() == DialogResult.OK)
            {
                basicSettings.Commit();
                briefingSettings.Commit();
                foreach (var houseSettingsTracker in houseSettingsTrackers.Values)
                {
                    houseSettingsTracker.Commit();
                }
                plugin.Dirty = true;
            }
        }

        private void settingsTeamTypesMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            int maxTeams = 0;
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    {
                        maxTeams = TiberianDawn.Constants.MaxTeams;
                    }
                    break;
                case GameType.RedAlert:
                    {
                        maxTeams = RedAlert.Constants.MaxTeams;
                    }
                    break;
            }

            TeamTypesDialog ttd = new TeamTypesDialog(plugin, maxTeams);
            if (ttd.ShowDialog() == DialogResult.OK)
            {
                plugin.Map.TeamTypes.Clear();
                plugin.Map.TeamTypes.AddRange(ttd.TeamTypes.Select(t => t.Clone()));
                plugin.Dirty = true;
            }
        }

        private void settingsTriggersMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            int maxTriggers = 0;
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    {
                        maxTriggers = TiberianDawn.Constants.MaxTriggers;
                    }
                    break;
                case GameType.RedAlert:
                    {
                        maxTriggers = RedAlert.Constants.MaxTriggers;
                    }
                    break;
            }

            TriggersDialog td = new TriggersDialog(plugin, maxTriggers);
            if (td.ShowDialog() == DialogResult.OK)
            {
                var oldTriggers =
                    from leftTrigger in plugin.Map.Triggers
                    join rightTrigger in td.Triggers
                    on leftTrigger.Name equals rightTrigger.Name into result
                    where result.Count() == 0
                    select leftTrigger;
                var newTriggers =
                    from leftTrigger in td.Triggers
                    join rightTrigger in plugin.Map.Triggers
                    on leftTrigger.Name equals rightTrigger.Name into result
                    where result.Count() == 0
                    select leftTrigger;
                var sameTriggers =
                    from leftTrigger in plugin.Map.Triggers
                    join rightTrigger in td.Triggers
                    on leftTrigger.Name equals rightTrigger.Name
                    select new
                    {
                        OldTrigger = leftTrigger,
                        NewTrigger = rightTrigger
                    };

                foreach (var oldTrigger in oldTriggers.ToArray())
                {
                    plugin.Map.Triggers.Remove(oldTrigger);
                }

                foreach (var newTrigger in newTriggers.ToArray())
                {
                    plugin.Map.Triggers.Add(newTrigger.Clone());
                }

                foreach (var item in sameTriggers.ToArray())
                {
                    plugin.Map.Triggers.Add(item.NewTrigger.Clone());
                    plugin.Map.Triggers.Remove(item.OldTrigger);
                }

                plugin.Dirty = true;
            }
        }

        private void Mru_FileSelected(object sender, FileInfo e)
        {
            if (!PromptSaveMap())
            {
                return;
            }

            if (LoadFile(e.FullName))
            {
                mru.Add(e);
            }
            else
            {
                mru.Remove(e);
                MessageBox.Show(string.Format("Error loading {0}.", e.FullName), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void mapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (plugin != null)
            {
                var mapPoint = mapPanel.ClientToMap(e.Location);
                var location = new Point((int)Math.Floor((double)mapPoint.X / Globals.TileWidth), (int)Math.Floor((double)mapPoint.Y / Globals.TileHeight));
                if (plugin.Map.Metrics.GetCell(location, out int cell))
                {
                    var sb = new StringBuilder();
                    sb.AppendFormat("X = {0}, Y = {1}, Cell = {2}", location.X, location.Y, cell);

                    var template = plugin.Map.Templates[cell];
                    var templateType = template?.Type;  
                    if (templateType != null)
                    {
                        sb.AppendFormat(", Template = {0} ({1})", templateType.DisplayName, template.Icon);
                    }

                    var smudge = plugin.Map.Smudge[cell];
                    var smudgeType = smudge?.Type;
                    if (smudgeType != null)
                    {
                        sb.AppendFormat(", Smudge = {0}", smudgeType.DisplayName);
                    }

                    var overlay = plugin.Map.Overlay[cell];
                    var overlayType = overlay?.Type;
                    if (overlayType != null)
                    {
                        sb.AppendFormat(", Overlay = {0}", overlayType.DisplayName);
                    }

                    var terrain = plugin.Map.Technos[location] as Terrain;
                    var terrainType = terrain?.Type;
                    if (terrainType != null)
                    {
                        sb.AppendFormat(", Terrain = {0}", terrainType.DisplayName);
                    }

                    if (plugin.Map.Technos[location] is InfantryGroup infantryGroup)
                    {
                        var subPixel = new Point(
                            (mapPoint.X * Globals.PixelWidth / Globals.TileWidth) % Globals.PixelWidth,
                            (mapPoint.Y * Globals.PixelHeight / Globals.TileHeight) % Globals.PixelHeight
                        );

                        var i = InfantryGroup.ClosestStoppingTypes(subPixel).Cast<int>().First();
                        if (infantryGroup.Infantry[i] != null)
                        {
                            sb.AppendFormat(", Infantry = {0}", infantryGroup.Infantry[i].Type.DisplayName);
                        }
                    }

                    var unit = plugin.Map.Technos[location] as Unit;
                    var unitType = unit?.Type;
                    if (unitType != null)
                    {
                        sb.AppendFormat(", Unit = {0}", unitType.DisplayName);
                    }

                    var building = plugin.Map.Technos[location] as Building;
                    var buildingType = building?.Type;
                    if (buildingType != null)
                    {
                        sb.AppendFormat(", Building = {0}", buildingType.DisplayName);
                    }

                    cellStatusLabel.Text = sb.ToString();
                }
                else
                {
                    cellStatusLabel.Text = string.Empty;
                }
            }
        }

        private bool LoadFile(string loadFilename)
        {
            FileType fileType = FileType.None;
            switch (Path.GetExtension(loadFilename).ToLower())
            {
                case ".ini":
                case ".mpr":
                    fileType = FileType.INI;
                    break;
                case ".bin":
                    fileType = FileType.BIN;
                    break;
#if DEVELOPER
                case ".pgm":
                    fileType = FileType.PGM;
                    break;
#endif
            }

            if (fileType == FileType.None)
            {
                return false;
            }

            GameType gameType = GameType.None;
            switch (fileType)
            {
                case FileType.INI:
                    {
                        var ini = new INI();
                        try
                        {
                            using (var reader = new StreamReader(loadFilename))
                            {
                                ini.Parse(reader);
                            }
                        }
                        catch (FileNotFoundException)
                        {
                            return false;
                        }
                        gameType = File.Exists(Path.ChangeExtension(loadFilename, ".bin")) ? GameType.TiberianDawn : GameType.RedAlert;
                    }
                    break;
                case FileType.BIN:
                    gameType = GameType.TiberianDawn;
                    break;
#if DEVELOPER
                case FileType.PGM:
                    {
                        try
                        {
                            using (var megafile = new Megafile(loadFilename))
                            {
                                if (megafile.Any(f => Path.GetExtension(f).ToLower() == ".mpr"))
                                {
                                    gameType = GameType.RedAlert;
                                }
                                else
                                {
                                    gameType = GameType.TiberianDawn;
                                }
                            }
                        }
                        catch (FileNotFoundException)
                        {
                            return false;
                        }
                    }
                    break;
#endif
            }

            if (gameType == GameType.None)
            {
                return false;
            }

            if (plugin != null)
            {
                plugin.Map.Triggers.CollectionChanged -= Triggers_CollectionChanged;
                plugin.Dispose();
            }
            plugin = null;

            Globals.TheTilesetManager.Reset();
            Globals.TheTextureManager.Reset();

            switch (gameType)
            {
                case GameType.TiberianDawn:
                    {
                        Globals.TheTeamColorManager.Reset();
                        Globals.TheTeamColorManager.Load(@"DATA\XML\CNCTDTEAMCOLORS.XML");
                        plugin = new TiberianDawn.GamePlugin();
                    }
                    break;
                case GameType.RedAlert:
                    {
                        Globals.TheTeamColorManager.Reset();
                        Globals.TheTeamColorManager.Load(@"DATA\XML\CNCRATEAMCOLORS.XML");
                        plugin = new RedAlert.GamePlugin();
                    }
                    break;
            }

            try
            {
                var errors = plugin.Load(loadFilename, fileType).ToArray();
                if (errors.Length > 0)
                {
                    ErrorMessageBox errorMessageBox = new ErrorMessageBox { Errors = errors };
                    errorMessageBox.ShowDialog();
                }
            }
            catch (Exception)
            {
#if DEVELOPER
                throw;
#else
                return false;
#endif
            }

            plugin.Map.Triggers.CollectionChanged += Triggers_CollectionChanged;
            mapPanel.MapImage = plugin.MapImage;

            plugin.Dirty = false;
            filename = loadFilename;
            Text = string.Format("CnC TDRA Map Editor - {0}", filename);

            url.Clear();

            ClearActiveTool();
            RefreshAvailableTools();
            RefreshActiveTool();

            return true;
        }

        private bool SaveFile(string saveFilename)
        {
            FileType fileType = FileType.None;
            switch (Path.GetExtension(saveFilename).ToLower())
            {
                case ".ini":
                case ".mpr":
                    fileType = FileType.INI;
                    break;
                case ".bin":
                    fileType = FileType.BIN;
                    break;
            }

            if (fileType == FileType.None)
            {
                return false;
            }

            if (string.IsNullOrEmpty(plugin.Map.SteamSection.Title))
            {
                plugin.Map.SteamSection.Title = plugin.Map.BasicSection.Name;
            }

            if (!plugin.Save(saveFilename, fileType))
            {
                return false;
            }

            if (new FileInfo(saveFilename).Length > Globals.MaxMapSize)
            {
                MessageBox.Show(string.Format("Map file exceeds the maximum size of {0} bytes.", Globals.MaxMapSize), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            plugin.Dirty = false;
            filename = saveFilename;
            Text = string.Format("CnC TDRA Map Editor - {0}", filename);

            return true;
        }

        private void RefreshAvailableTools()
        {
            availableToolTypes = ToolType.None;
            if (plugin != null)
            {
                availableToolTypes |= ToolType.Waypoint;

                if (plugin.Map.TemplateTypes.Any()) availableToolTypes |= ToolType.Map;
                if (plugin.Map.SmudgeTypes.Any()) availableToolTypes |= ToolType.Smudge;
                if (plugin.Map.OverlayTypes.Any(t => t.IsPlaceable && ((t.Theaters == null) || t.Theaters.Contains(plugin.Map.Theater)))) availableToolTypes |= ToolType.Overlay;
                if (plugin.Map.TerrainTypes.Any(t => t.Theaters.Contains(plugin.Map.Theater))) availableToolTypes |= ToolType.Terrain;
                if (plugin.Map.InfantryTypes.Any()) availableToolTypes |= ToolType.Infantry;
                if (plugin.Map.UnitTypes.Any()) availableToolTypes |= ToolType.Unit;
                if (plugin.Map.BuildingTypes.Any()) availableToolTypes |= ToolType.Building;
                if (plugin.Map.OverlayTypes.Any(t => t.IsResource)) availableToolTypes |= ToolType.Resources;
                if (plugin.Map.OverlayTypes.Any(t => t.IsWall)) availableToolTypes |= ToolType.Wall;
                if (plugin.Map.Triggers.Any()) availableToolTypes |= ToolType.CellTrigger;
            }

            mapToolStripButton.Enabled = (availableToolTypes & ToolType.Map) != ToolType.None;
            smudgeToolStripButton.Enabled = (availableToolTypes & ToolType.Smudge) != ToolType.None;
            overlayToolStripButton.Enabled = (availableToolTypes & ToolType.Overlay) != ToolType.None;
            terrainToolStripButton.Enabled = (availableToolTypes & ToolType.Terrain) != ToolType.None;
            infantryToolStripButton.Enabled = (availableToolTypes & ToolType.Infantry) != ToolType.None;
            unitToolStripButton.Enabled = (availableToolTypes & ToolType.Unit) != ToolType.None;
            buildingToolStripButton.Enabled = (availableToolTypes & ToolType.Building) != ToolType.None;
            resourcesToolStripButton.Enabled = (availableToolTypes & ToolType.Resources) != ToolType.None;
            wallsToolStripButton.Enabled = (availableToolTypes & ToolType.Wall) != ToolType.None;
            waypointsToolStripButton.Enabled = (availableToolTypes & ToolType.Waypoint) != ToolType.None;
            cellTriggersToolStripButton.Enabled = (availableToolTypes & ToolType.CellTrigger) != ToolType.None;

            ActiveToolType = activeToolType;
        }

        private void ClearActiveTool()
        {
            activeTool?.Dispose();
            activeTool = null;

            if (activeToolForm != null)
            {
                activeToolForm.ResizeEnd -= ActiveToolForm_ResizeEnd;
                activeToolForm.Close();
                activeToolForm = null;
            }

            toolStatusLabel.Text = string.Empty;
        }

        private void RefreshActiveTool()
        {
            if (plugin == null)
            {
                return;
            }

            if (activeTool == null)
            {
                activeLayers = MapLayerFlag.None;
            }

            ClearActiveTool();

            switch (ActiveToolType)
            {
                case ToolType.Map:
                    {
                        TemplateToolDialog toolDialog = new TemplateToolDialog();

                        activeTool = new TemplateTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.TemplateTypeListView, toolDialog.TemplateTypeMapPanel, mouseToolTip, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    } break;
                case ToolType.Smudge:
                    {
                        GenericToolDialog toolDialog = new GenericToolDialog
                        {
                            Text = "Smudge"
                        };

                        toolDialog.GenericTypeComboBox.Types = plugin.Map.SmudgeTypes.Where(t => (t.Flag & SmudgeTypeFlag.Bib) == SmudgeTypeFlag.None).OrderBy(t => t.Name);

                        activeTool = new SmudgeTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.GenericTypeComboBox, toolDialog.GenericTypeMapPanel, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Overlay:
                    {
                        GenericToolDialog toolDialog = new GenericToolDialog
                        {
                            Text = "Overlay"
                        };

                        toolDialog.GenericTypeComboBox.Types = plugin.Map.OverlayTypes.Where(t => t.IsPlaceable && ((t.Theaters == null) || t.Theaters.Contains(plugin.Map.Theater))).OrderBy(t => t.Name);

                        activeTool = new OverlaysTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.GenericTypeComboBox, toolDialog.GenericTypeMapPanel, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Resources:
                    {
                        ResourcesToolDialog toolDialog = new ResourcesToolDialog();

                        activeTool = new ResourcesTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.TotalResourcesLbl, toolDialog.ResourceBrushSizeNud, toolDialog.GemsCheckBox, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Terrain:
                    {
                        TerrainToolDialog toolDialog = new TerrainToolDialog(plugin);

                        toolDialog.TerrainTypeComboBox.Types = plugin.Map.TerrainTypes.Where(t => t.Theaters.Contains(plugin.Map.Theater)).OrderBy(t => t.Name);

                        activeTool = new TerrainTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.TerrainTypeComboBox, toolDialog.TerrainTypeMapPanel, toolDialog.TerrainProperties, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Infantry:
                    {
                        ObjectToolDialog toolDialog = new ObjectToolDialog(plugin)
                        {
                            Text = "Infantry"
                        };

                        toolDialog.ObjectTypeComboBox.Types = plugin.Map.InfantryTypes.OrderBy(t => t.Name);

                        activeTool = new InfantryTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.ObjectTypeComboBox, toolDialog.ObjectTypeMapPanel, toolDialog.ObjectProperties, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Unit:
                    {
                        ObjectToolDialog toolDialog = new ObjectToolDialog(plugin)
                        {
                            Text = "Units"
                        };

                        toolDialog.ObjectTypeComboBox.Types = plugin.Map.UnitTypes
                            .Where(t => !t.IsFixedWing)
                            .OrderBy(t => t.Name);

                        activeTool = new UnitTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.ObjectTypeComboBox, toolDialog.ObjectTypeMapPanel, toolDialog.ObjectProperties, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Building:
                    {
                        ObjectToolDialog toolDialog = new ObjectToolDialog(plugin)
                        {
                            Text = "Structures"
                        };

                        toolDialog.ObjectTypeComboBox.Types = plugin.Map.BuildingTypes
                            .Where(t => (t.Theaters == null) || t.Theaters.Contains(plugin.Map.Theater))
                            .OrderBy(t => t.IsFake)
                            .ThenBy(t => t.Name);

                        activeTool = new BuildingTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.ObjectTypeComboBox, toolDialog.ObjectTypeMapPanel, toolDialog.ObjectProperties, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Wall:
                    {
                        GenericToolDialog toolDialog = new GenericToolDialog
                        {
                            Text = "Walls"
                        };

                        toolDialog.GenericTypeComboBox.Types = plugin.Map.OverlayTypes.Where(t => t.IsWall).OrderBy(t => t.Name);

                        activeTool = new WallsTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.GenericTypeComboBox, toolDialog.GenericTypeMapPanel, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.Waypoint:
                    {
                        WaypointsToolDialog toolDialog = new WaypointsToolDialog();

                        toolDialog.WaypointCombo.DataSource = plugin.Map.Waypoints.Select(w => w.Name).ToArray();

                        activeTool = new WaypointsTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.WaypointCombo, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
                case ToolType.CellTrigger:
                    {
                        CellTriggersToolDialog toolDialog = new CellTriggersToolDialog();

                        toolDialog.TriggerCombo.DataSource = plugin.Map.Triggers.Select(t => t.Name).ToArray();

                        activeTool = new CellTriggersTool(mapPanel, ActiveLayers, toolStatusLabel, toolDialog.TriggerCombo, plugin, url);
                        activeToolForm = toolDialog;
                        activeToolForm.Show(this);
                    }
                    break;
            }

            if (activeToolForm != null)
            {
                activeToolForm.ResizeEnd += ActiveToolForm_ResizeEnd;
                clampActiveToolForm();
            }

            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    mapPanel.MaxZoom = 8;
                    mapPanel.ZoomStep = 1;
                    break;
                case GameType.RedAlert:
                    mapPanel.MaxZoom = 16;
                    mapPanel.ZoomStep = 2;
                    break;
            }

            mapToolStripButton.Checked = ActiveToolType == ToolType.Map;
            smudgeToolStripButton.Checked = ActiveToolType == ToolType.Smudge;
            overlayToolStripButton.Checked = ActiveToolType == ToolType.Overlay;
            terrainToolStripButton.Checked = ActiveToolType == ToolType.Terrain;
            infantryToolStripButton.Checked = ActiveToolType == ToolType.Infantry;
            unitToolStripButton.Checked = ActiveToolType == ToolType.Unit;
            buildingToolStripButton.Checked = ActiveToolType == ToolType.Building;
            resourcesToolStripButton.Checked = ActiveToolType == ToolType.Resources;
            wallsToolStripButton.Checked = ActiveToolType == ToolType.Wall;
            waypointsToolStripButton.Checked = ActiveToolType == ToolType.Waypoint;
            cellTriggersToolStripButton.Checked = ActiveToolType == ToolType.CellTrigger;

            Focus();

            UpdateVisibleLayers();
            mapPanel.Invalidate();
        }

        private void clampActiveToolForm()
        {
            if (activeToolForm == null)
            {
                return;
            }

            Rectangle bounds = activeToolForm.DesktopBounds;
            Rectangle workingArea = Screen.FromControl(this).WorkingArea;
            if (bounds.Right > workingArea.Right)
            {
                bounds.X = workingArea.Right - bounds.Width;
            }
            if (bounds.X < workingArea.Left)
            {
                bounds.X = workingArea.Left;
            }
            if (bounds.Bottom > workingArea.Bottom)
            {
                bounds.Y = workingArea.Bottom - bounds.Height;
            }
            if (bounds.Y < workingArea.Top)
            {
                bounds.Y = workingArea.Top;
            }
            activeToolForm.DesktopBounds = bounds;
        }

        private void ActiveToolForm_ResizeEnd(object sender, EventArgs e)
        {
            clampActiveToolForm();
        }

        private void Triggers_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            RefreshAvailableTools();
        }

        private void mainToolStripButton_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            if (sender == mapToolStripButton)
            {
                ActiveToolType = ToolType.Map;
            }
            else if (sender == smudgeToolStripButton)
            {
                ActiveToolType = ToolType.Smudge;
            }
            else if (sender == overlayToolStripButton)
            {
                ActiveToolType = ToolType.Overlay;
            }
            else if (sender == terrainToolStripButton)
            {
                ActiveToolType = ToolType.Terrain;
            }
            else if (sender == infantryToolStripButton)
            {
                ActiveToolType = ToolType.Infantry;
            }
            else if (sender == unitToolStripButton)
            {
                ActiveToolType = ToolType.Unit;
            }
            else if (sender == buildingToolStripButton)
            {
                ActiveToolType = ToolType.Building;
            }
            else if (sender == resourcesToolStripButton)
            {
                ActiveToolType = ToolType.Resources;
            }
            else if (sender == wallsToolStripButton)
            {
                ActiveToolType = ToolType.Wall;
            }
            else if (sender == waypointsToolStripButton)
            {
                ActiveToolType = ToolType.Waypoint;
            }
            else if (sender == cellTriggersToolStripButton)
            {
                ActiveToolType = ToolType.CellTrigger;
            }
        }

        private void UpdateVisibleLayers()
        {
            MapLayerFlag layers = MapLayerFlag.All;
            if (!viewLayersBoundariesMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.Boundaries;
            }
            if (!viewLayersOverlayMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.OverlayAll;
            }
            if (!viewLayersTerrainMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.Terrain;
            }
            if (!viewLayersWaypointsMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.Waypoints;
            }
            if (!viewLayersCellTriggersMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.CellTriggers;
            }
            if (!viewLayersObjectTriggersMenuItem.Checked)
            {
                layers &= ~MapLayerFlag.TechnoTriggers;
            }
            ActiveLayers = layers;
        }

        private void viewLayersMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            UpdateVisibleLayers();
        }

        private void toolTabControl_Selected(object sender, TabControlEventArgs e)
        {
            if (plugin == null)
            {
                return;
            }
        }

        private void developerGenerateMapPreviewMenuItem_Click(object sender, EventArgs e)
        {
#if DEVELOPER
            if ((plugin == null) || string.IsNullOrEmpty(filename))
            {
                return;
            }

            plugin.Map.GenerateMapPreview().Save(Path.ChangeExtension(filename, ".tga"));
#endif
        }

        private void developerGoToINIMenuItem_Click(object sender, EventArgs e)
        {
#if DEVELOPER
            if ((plugin == null) || string.IsNullOrEmpty(filename))
            {
                return;
            }

            var path = Path.ChangeExtension(filename, ".mpr");
            if (!File.Exists(path))
            {
                path = Path.ChangeExtension(filename, ".ini");
            }

            try
            {
                Process.Start(path);
            }
            catch (Win32Exception)
            {
                Process.Start("notepad.exe", path);
            }
            catch (Exception) { }
#endif
        }

        private void developerGenerateMapPreviewDirectoryMenuItem_Click(object sender, EventArgs e)
        {
#if DEVELOPER
            FolderBrowserDialog fbd = new FolderBrowserDialog
            {
                ShowNewFolderButton = false
            };
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                var extensions = new string[] { ".ini", ".mpr" };
                foreach (var file in Directory.EnumerateFiles(fbd.SelectedPath).Where(file => extensions.Contains(Path.GetExtension(file).ToLower())))
                {
                    GameType gameType = GameType.None;

                    var ini = new INI();
                    using (var reader = new StreamReader(file))
                    {
                        ini.Parse(reader);
                    }
                    gameType = ini.Sections.Contains("MapPack") ? GameType.RedAlert : GameType.TiberianDawn;

                    if (gameType == GameType.None)
                    {
                        continue;
                    }

                    IGamePlugin plugin = null;
                    switch (gameType)
                    {
                        case GameType.TiberianDawn:
                            {
                                plugin = new TiberianDawn.GamePlugin(false);
                            }
                            break;
                        case GameType.RedAlert:
                            {
                                plugin = new RedAlert.GamePlugin(false);
                            }
                            break;
                    }

                    plugin.Load(file, FileType.INI);
                    plugin.Map.GenerateMapPreview().Save(Path.ChangeExtension(file, ".tga"));
                    plugin.Dispose();
                }
            }
#endif
        }

        private void developerDebugShowOverlapCellsMenuItem_CheckedChanged(object sender, EventArgs e)
        {
#if DEVELOPER
            Globals.Developer.ShowOverlapCells = developerDebugShowOverlapCellsMenuItem.Checked;
#endif
        }

        private void filePublishMenuItem_Click(object sender, EventArgs e)
        {
            if (plugin == null)
            {
                return;
            }

            if (!PromptSaveMap())
            {
                return;
            }

            if (plugin.Dirty)
            {
                MessageBox.Show("Map must be saved before publishing.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (new FileInfo(filename).Length > Globals.MaxMapSize)
            {
                return;
            }

            using (var sd = new SteamDialog(plugin))
            {
                sd.ShowDialog();
            }

            fileSaveMenuItem.PerformClick();
        }

        private void mainToolStrip_MouseMove(object sender, MouseEventArgs e)
        {
            mainToolStrip.Focus();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = !PromptSaveMap();
        }

        private bool PromptSaveMap()
        {
            bool cancel = false;
            if (plugin?.Dirty ?? false)
            {
                var message = string.IsNullOrEmpty(filename) ? "Save new map?" : string.Format("Save map '{0}'?", filename);
                var result = MessageBox.Show(message, "Save", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                switch (result)
                {
                    case DialogResult.Yes:
                        {
                            if (string.IsNullOrEmpty(filename))
                            {
                                fileSaveAsMenuItem.PerformClick();
                            }
                            else
                            {
                                fileSaveMenuItem.PerformClick();
                            }
                        }
                        break;
                    case DialogResult.No:
                        break;
                    case DialogResult.Cancel:
                        cancel = true;
                        break;
                }
            }
            return !cancel;
        }
    }
}
