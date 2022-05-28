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
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;

namespace MobiusEditor.Utility
{
    public class TilesetManager
    {
        private readonly Dictionary<string, Tileset> tilesets = new Dictionary<string, Tileset>();

        private readonly MegafileManager megafileManager;

        public TilesetManager(MegafileManager megafileManager, TextureManager textureManager, string xmlPath, string texturesPath)
        {
            this.megafileManager = megafileManager;

            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(megafileManager.Open(xmlPath));

            foreach (XmlNode fileNode in xmlDoc.SelectNodes("TilesetFiles/File"))
            {
                var xmlFile = Path.Combine(Path.GetDirectoryName(xmlPath), fileNode.InnerText);
                XmlDocument fileXmlDoc = new XmlDocument();
                fileXmlDoc.Load(megafileManager.Open(xmlFile));

                foreach (XmlNode tilesetNode in fileXmlDoc.SelectNodes("Tilesets/TilesetTypeClass"))
                {
                    var tileset = new Tileset(textureManager);
                    tileset.Load(tilesetNode.OuterXml, texturesPath);

                    tilesets[tilesetNode.Attributes["name"].Value] = tileset;
                }
            }
        }

        public void Reset()
        {
            foreach (var item in tilesets)
            {
                item.Value.Reset();
            }
        }

        public bool GetTeamColorTileData(IEnumerable<string> searchTilesets, string name, int shape, TeamColor teamColor, out int fps, out Tile[] tiles)
        {
            fps = 0;
            tiles = null;

            foreach (var tileset in tilesets.Join(searchTilesets, x => x.Key, y => y, (x, y) => x.Value))
            {
                if (tileset.GetTileData(name, shape, teamColor, out fps, out tiles))
                {
                    return true;
                }
            }

            return false;
        }

        public bool GetTeamColorTileData(IEnumerable<string> searchTilesets, string name, int shape, TeamColor teamColor, out int fps, out Tile tile)
        {
            tile = null;
            if (!GetTeamColorTileData(searchTilesets, name, shape, teamColor, out fps, out Tile[] tiles))
            {
                return false;
            }
            tile = tiles[0];
            return true;
        }

        public bool GetTeamColorTileData(IEnumerable<string> searchTilesets, string name, int shape, TeamColor teamColor, out Tile[] tiles)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, teamColor, out int fps, out tiles);
        }

        public bool GetTeamColorTileData(IEnumerable<string> searchTilesets, string name, int shape, TeamColor teamColor, out Tile tile)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, teamColor, out int fps, out tile);
        }

        public bool GetTileData(IEnumerable<string> searchTilesets, string name, int shape, out int fps, out Tile[] tiles)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, null, out fps, out tiles);
        }

        public bool GetTileData(IEnumerable<string> searchTilesets, string name, int shape, out int fps, out Tile tile)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, null, out fps, out tile);
        }

        public bool GetTileData(IEnumerable<string> searchTilesets, string name, int shape, out Tile[] tiles)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, null, out tiles);
        }

        public bool GetTileData(IEnumerable<string> searchTilesets, string name, int shape, out Tile tile)
        {
            return GetTeamColorTileData(searchTilesets, name, shape, null, out tile);
        }
    }
}
