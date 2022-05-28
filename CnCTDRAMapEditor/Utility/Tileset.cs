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
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Xml;

namespace MobiusEditor.Utility
{
    public class Tile
    {
        public Image Image { get; private set; }

        public Rectangle OpaqueBounds { get; private set; }

        public Tile(Image image, Rectangle opaqueBounds)
        {
            Image = image;
            OpaqueBounds = opaqueBounds;
        }

        public Tile(Image image)
            : this(image, new Rectangle(0, 0, image.Width, image.Height))
        {
        }
    }

    public class Tileset
    {
        private class TileData
        {
            public int FPS { get; set; }
            public string[] Frames { get; set; }

            public Dictionary<string, Tile[]> TeamColorTiles { get; } = new Dictionary<string, Tile[]>();
        }

        private readonly Dictionary<string, Dictionary<int, TileData>> tiles = new Dictionary<string, Dictionary<int, TileData>>(StringComparer.OrdinalIgnoreCase);

        private readonly TextureManager textureManager;

        private static readonly Bitmap transparentTileImage;

        static Tileset()
        {
            transparentTileImage = new Bitmap(Globals.TileWidth, Globals.TileHeight);
            transparentTileImage.MakeTransparent();
        }

        public Tileset(TextureManager textureManager)
        {
            this.textureManager = textureManager;
        }

        public void Reset()
        {
            foreach (var item in tiles)
            {
                foreach (var tileItem in item.Value)
                {
                    tileItem.Value.TeamColorTiles.Clear();
                }
            }
        }

        public void Load(string xml, string texturesPath)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(xml);

            var rootPath = Path.Combine(texturesPath, xmlDoc.SelectSingleNode("TilesetTypeClass/RootTexturePath").InnerText);
            foreach (XmlNode tileNode in xmlDoc.SelectNodes("TilesetTypeClass/Tiles/Tile"))
            {
                TileData tileData = new TileData();

                var name = tileNode.SelectSingleNode("Key/Name").InnerText;
                var shape = int.Parse(tileNode.SelectSingleNode("Key/Shape").InnerText);
                var fpsNode = tileNode.SelectSingleNode("Value/AnimationData/FPS");
                tileData.FPS = (fpsNode != null) ? int.Parse(fpsNode.InnerText) : 0;

                var frameNodes = tileNode.SelectNodes("Value/Frames/Frame");
#if false
                tileData.Frames = new string[frameNodes.Count];
#else
                tileData.Frames = new string[Math.Min(1, frameNodes.Count)];
#endif

                for (var i = 0; i < tileData.Frames.Length; ++i)
                {
                    string filename = null;
                    if (!string.IsNullOrEmpty(frameNodes[i].InnerText))
                    {
                        filename = Path.Combine(rootPath, frameNodes[i].InnerText);
                    }

                    tileData.Frames[i] = filename;
                }

                if (!tiles.TryGetValue(name, out Dictionary<int, TileData> shapes))
                {
                    shapes = new Dictionary<int, TileData>();
                    tiles[name] = shapes;
                }

                shapes[shape] = tileData;
            }
        }

        public bool GetTileData(string name, int shape, TeamColor teamColor, out int fps, out Tile[] tiles)
        {
            fps = 0;
            tiles = null;

            if (!this.tiles.TryGetValue(name, out Dictionary<int, TileData> shapes))
            {
                return false;
            }

            if (shape < 0)
            {
                shape = Math.Max(0, shapes.Max(kv => kv.Key) + shape + 1);
            }
            if (!shapes.TryGetValue(shape, out TileData tileData))
            {
                return false;
            }

            var key = teamColor?.Name ?? string.Empty;
            if (!tileData.TeamColorTiles.TryGetValue(key, out Tile[] tileDataTiles))
            {
                tileDataTiles = new Tile[tileData.Frames.Length];
                tileData.TeamColorTiles[key] = tileDataTiles;

                for (int i = 0; i < tileDataTiles.Length; ++i)
                {
                    var filename = tileData.Frames[i];
                    if (!string.IsNullOrEmpty(filename))
                    {
                        (Bitmap bitmap, Rectangle opaqueBounds) = textureManager.GetTexture(filename, teamColor);
                        tileDataTiles[i] = new Tile(bitmap, opaqueBounds);
                    }
                    else
                    {
                        tileDataTiles[i] = new Tile(transparentTileImage);
                    }
                }
            }

            fps = tileData.FPS;
            tiles = tileDataTiles;

            return true;
        }
    }
}
