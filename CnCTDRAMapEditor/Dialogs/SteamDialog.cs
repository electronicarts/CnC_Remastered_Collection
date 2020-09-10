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
using MobiusEditor.Controls;
using MobiusEditor.Interface;
using MobiusEditor.Utility;
using Steamworks;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Forms;

namespace MobiusEditor.Dialogs
{
    public partial class SteamDialog : Form
    {
        private static readonly string PreviewDirectory = Path.Combine(Path.GetTempPath(), "CnCRCMapEditor");

        private readonly IGamePlugin plugin;
        private readonly Timer statusUpdateTimer = new Timer();

        public SteamDialog(IGamePlugin plugin)
        {
            this.plugin = plugin;

            InitializeComponent();

            visibilityComboBox.DataSource = new []
            {
                new { Name = "Public", Value = ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPublic },
                new { Name = "Friends Only", Value = ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityFriendsOnly },
                new { Name = "Private", Value = ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPrivate }
            };

            statusUpdateTimer.Interval = 500;
            statusUpdateTimer.Tick += StatusUpdateTimer_Tick;

            Disposed += (o, e) => { (previewTxt.Tag as Image)?.Dispose(); };
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            titleTxt.Text = plugin.Map.SteamSection.Title;
            descriptionTxt.Text = plugin.Map.SteamSection.Description;
            previewTxt.Text = plugin.Map.SteamSection.PreviewFile;
            visibilityComboBox.SelectedValue = plugin.Map.SteamSection.Visibility;

            btnPublishMap.SplitWidth = (plugin.Map.SteamSection.PublishedFileId != PublishedFileId_t.Invalid.m_PublishedFileId) ? MenuButton.DefaultSplitWidth : 0;

            Directory.CreateDirectory(PreviewDirectory);
            var previewPath = Path.Combine(PreviewDirectory, "Minimap.png");
            plugin.Map.GenerateWorkshopPreview().ToBitmap().Save(previewPath, ImageFormat.Png);

            if (plugin.Map.BasicSection.SoloMission)
            {
                var soloBannerPath = Path.Combine(PreviewDirectory, "SoloBanner.png");
                Properties.Resources.UI_CustomMissionPreviewDefault.Save(soloBannerPath, ImageFormat.Png);
                previewTxt.Text = soloBannerPath;
            }
            else
            {
                previewTxt.Text = previewPath;
            }

            imageTooltip.SetToolTip(previewTxt, "Preview.png");

            statusUpdateTimer.Start();

            UpdateControls();
        }

        private void StatusUpdateTimer_Tick(object sender, EventArgs e)
        {
            var status = SteamworksUGC.CurrentOperation?.Status;
            if (!string.IsNullOrEmpty(status))
            {
                statusLbl.Text = status;
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            statusUpdateTimer.Stop();
            statusUpdateTimer.Dispose();
        }

        protected virtual void OnPublishSuccess()
        {
            statusLbl.Text = "Done.";
            EnableControls(true);
        }

        protected virtual void OnOperationFailed(string status)
        {
            statusLbl.Text = status;
            EnableControls(true);
        }

        private void EnableControls(bool enable)
        {
            titleTxt.Enabled = enable;
            visibilityComboBox.Enabled = enable;
            previewTxt.Enabled = enable;
            previewBtn.Enabled = enable;
            descriptionTxt.Enabled = enable;
            btnPublishMap.Enabled = enable;
            btnClose.Enabled = enable;
        }

        private void btnGoToSteam_Click(object sender, EventArgs e)
        {
            var workshopUrl = SteamworksUGC.WorkshopURL;
            if (!string.IsNullOrEmpty(workshopUrl))
            {
                Process.Start(workshopUrl);
            }
        }

        private void btnPublishMap_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(plugin.Map.BasicSection.Name))
            {
                plugin.Map.BasicSection.Name = titleTxt.Text;
            }

            if (string.IsNullOrEmpty(plugin.Map.BasicSection.Author))
            {
                plugin.Map.BasicSection.Author = SteamFriends.GetPersonaName();
            }

            plugin.Map.SteamSection.PreviewFile = previewTxt.Text;
            plugin.Map.SteamSection.Title = titleTxt.Text;
            plugin.Map.SteamSection.Description = descriptionTxt.Text;
            plugin.Map.SteamSection.Visibility = (ERemoteStoragePublishedFileVisibility)visibilityComboBox.SelectedValue;

            var tempPath = Path.Combine(Path.GetTempPath(), "CnCRCMapEditorPublishUGC");
            Directory.CreateDirectory(tempPath);
            foreach (var file in new DirectoryInfo(tempPath).EnumerateFiles()) file.Delete();

            var pgmPath = Path.Combine(tempPath, "MAPDATA.PGM");
            plugin.Save(pgmPath, FileType.PGM);

            var tags = new List<string>();
            switch (plugin.GameType)
            {
                case GameType.TiberianDawn:
                    tags.Add("TD");
                    break;
                case GameType.RedAlert:
                    tags.Add("RA");
                    break;
            }

            if (plugin.Map.BasicSection.SoloMission)
            {
                tags.Add("SinglePlayer");
            }
            else
            {
                tags.Add("MultiPlayer");
            }

            if (SteamworksUGC.PublishUGC(tempPath, plugin.Map.SteamSection, tags, OnPublishSuccess, OnOperationFailed))
            {
                statusLbl.Text = SteamworksUGC.CurrentOperation.Status;
                EnableControls(false);
            }
        }

        private void previewBtn_Click(object sender, EventArgs e)
        {
            var ofd = new OpenFileDialog
            {
                AutoUpgradeEnabled = false,
                RestoreDirectory = true,
                Filter = "Preview Files (*.png)|*.png",
                CheckFileExists = true,
                InitialDirectory = Path.GetDirectoryName(previewTxt.Text),
                FileName = Path.GetFileName(previewTxt.Text)
            };
            if (!string.IsNullOrEmpty(previewTxt.Text))
            {
                ofd.FileName = previewTxt.Text;
            }
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                previewTxt.Text = ofd.FileName;
            }
        }

        private void publishAsNewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            plugin.Map.SteamSection.PublishedFileId = PublishedFileId_t.Invalid.m_PublishedFileId;
            btnPublishMap.PerformClick();
        }

        private void previewTxt_TextChanged(object sender, EventArgs e)
        {
            try
            {
                (previewTxt.Tag as Image)?.Dispose();

                Bitmap preview = null;
                using (Bitmap b = new Bitmap(previewTxt.Text))
                {
                    preview = new Bitmap(b.Width, b.Height, b.PixelFormat);
                    using (Graphics g = Graphics.FromImage(preview))
                    {
                        g.DrawImage(b, Point.Empty);
                        g.Flush();
                    }
                }

                previewTxt.Tag = preview;
            }
            catch (Exception)
            {
                previewTxt.Tag = null;
            }

            UpdateControls();
        }

        private void descriptionTxt_TextChanged(object sender, EventArgs e)
        {
            UpdateControls();
        }

        private void UpdateControls()
        {
            btnPublishMap.Enabled = (previewTxt.Tag != null) && !string.IsNullOrEmpty(descriptionTxt.Text);
        }
    }
}
