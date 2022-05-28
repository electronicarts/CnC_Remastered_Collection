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
using MobiusEditor.Utility;
using System;
using System.Drawing;
using System.IO;

namespace MobiusEditor
{
    public static class Globals
    {
        static Globals()
        {
            TileScale = Properties.Settings.Default.Quality;
        }

        public const string TilesetsXMLPath = @"DATA\XML\TILESETS.XML";
        public const string TexturesPath = @"DATA\ART\TEXTURES\SRGB";
        public const string MegafilePath = @"DATA";
        public const string GameTextFilenameFormat = @"DATA\TEXT\MASTERTEXTFILE_{0}.LOC";

        public const int OriginalTileWidth = 128;
        public const int OriginalTileHeight = 128;
        public static readonly Size OriginalTileSize = new Size(OriginalTileWidth, OriginalTileHeight);

        public static int TileScale { get; set; }
        public static int TileWidth => OriginalTileWidth / TileScale;
        public static int TileHeight => OriginalTileHeight / TileScale;
        public static Size TileSize => new Size(TileWidth, TileHeight);

        public const int PixelWidth = 24;
        public const int PixelHeight = 24;

        public static readonly Size MapPreviewSize = new Size(512, 512);
        public static readonly Size WorkshopPreviewSize = new Size(512, 512);

        public static readonly string[] Edges = new string[] { "North", "South", "West", "East" };
        public const int NumInfantryStops = 5;

        public const int MaxTeamClasses = 5;
        public const int MaxTeamMissions = 20;

        public const long MaxMapSize = 131072;

        public static MegafileManager TheMegafileManager;
        public static TextureManager TheTextureManager;
        public static TilesetManager TheTilesetManager;
        public static TeamColorManager TheTeamColorManager;
        public static GameTextManager TheGameTextManager;

        public static readonly string RootSaveDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Personal), @"CnCRemastered\Local_Custom_Maps");

#if DEVELOPER
        public static class Developer
        {
            public static bool ShowOverlapCells = false;
        }
#endif
    }
}
