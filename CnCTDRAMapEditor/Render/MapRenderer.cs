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
using MobiusEditor.Interface;
using MobiusEditor.Model;
using MobiusEditor.Utility;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;

namespace MobiusEditor.Render
{
    public static class MapRenderer
    {
        private static readonly int[] Facing16 = new int[256]
        {
            0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,
            2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,
            4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,
            8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,
            10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
            12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,
            14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0
        };

        private static readonly int[] Facing32 = new int[256]
        {
            0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,
            3,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,
            8,8,8,9,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
            13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,
            16,16,16,16,16,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,
            19,20,20,20,20,20,20,21,21,21,21,21,21,21,22,22,22,22,22,22,22,23,23,23,23,23,23,23,24,24,24,24,
            24,24,24,25,25,25,25,25,25,25,26,26,26,26,26,26,26,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,
            29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,0,0,0,0,0,0
        };

        private static readonly int[] HumanShape = new int[32]
        {
            0,0,7,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,3,3,3,3,2,2,2,2,1,1,1,1,1,0
        };

        private static readonly int[] BodyShape = new int[32]
        {
            0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
        };

        private static readonly Point[] TurretAdjust = new Point[]
        {
            new Point(1, 2),	// N
            new Point(-1, 1),
            new Point(-2, 0),
            new Point(-3, 0),
            new Point(-3, 1),	// NW
            new Point(-4, -1),
            new Point(-4, -1),
            new Point(-5, -2),
            new Point(-5, -3),	// W
            new Point(-5, -3),
            new Point(-3, -3),
            new Point(-3, -4),
            new Point(-3, -4),	// SW
            new Point(-3, -5),
            new Point(-2, -5),
            new Point(-1, -5),
            new Point(0, -5),	// S
            new Point(1, -6),
            new Point(2, -5),
            new Point(3, -5),
            new Point(4, -5),	// SE
            new Point(6, -4),
            new Point(6, -3),
            new Point(6, -3),
            new Point(6, -3),	// E
            new Point(5, -1),
            new Point(5, -1),
            new Point(4, 0),
            new Point(3, 0),	// NE
            new Point(2, 0),
            new Point(2, 1),
            new Point(1, 2)
        };

        private static readonly int[] tiberiumCounts = new int[] { 0, 1, 3, 4, 6, 7, 8, 10, 11 };
        private static readonly int randomSeed;

        static MapRenderer()
        {
            randomSeed = Guid.NewGuid().GetHashCode();
        }

        public static void Render(GameType gameType, Map map, Graphics graphics, ISet<Point> locations, MapLayerFlag layers, int tileScale)
        {
            var tileSize = new Size(Globals.OriginalTileWidth / tileScale, Globals.OriginalTileHeight / tileScale);
            var tiberiumOrGoldTypes = map.OverlayTypes.Where(t => t.IsTiberiumOrGold).Select(t => t).ToArray();
            var gemTypes = map.OverlayTypes.Where(t => t.IsGem).ToArray();

            var overlappingRenderList = new List<(Rectangle, Action<Graphics>)>();

            Func<IEnumerable<Point>> renderLocations = null;
            if (locations != null)
            {
                renderLocations = () => locations;
            }
            else
            {
                IEnumerable<Point> allCells()
                {
                    for (var y = 0; y < map.Metrics.Height; ++y)
                    {
                        for (var x = 0; x < map.Metrics.Width; ++x)
                        {
                            yield return new Point(x, y);
                        }
                    }
                }

                renderLocations = allCells;
            }

            if ((layers & MapLayerFlag.Template) != MapLayerFlag.None)
            {
                foreach (var topLeft in renderLocations())
                {
                    map.Metrics.GetCell(topLeft, out int cell);

                    var template = map.Templates[topLeft];
                    var name = template?.Type.Name ?? map.TemplateTypes.Where(t => t.Equals("clear1")).FirstOrDefault().Name;
                    var icon = template?.Icon ?? ((cell & 0x03) | ((cell >> 4) & 0x0C));

                    if (Globals.TheTilesetManager.GetTileData(map.Theater.Tilesets, name, icon, out Tile tile))
                    {
                        var renderBounds = new Rectangle(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height, tileSize.Width, tileSize.Height);
                        graphics.DrawImage(tile.Image, renderBounds);
                    }
                    else
                    {
                        Debug.Print(string.Format("Template {0} ({1}) not found", name, icon));
                    }
                }
            }

            if ((layers & MapLayerFlag.Smudge) != MapLayerFlag.None)
            {
                foreach (var topLeft in renderLocations())
                {
                    var smudge = map.Smudge[topLeft];
                    if (smudge != null)
                    {
                        Render(map.Theater, topLeft, tileSize, smudge).Item2(graphics);
                    }
                }
            }

            if ((layers & MapLayerFlag.OverlayAll) != MapLayerFlag.None)
            {
                foreach (var topLeft in renderLocations())
                {
                    var overlay = map.Overlay[topLeft];
                    if (overlay == null)
                    {
                        continue;
                    }

                    if ((overlay.Type.IsResource && ((layers & MapLayerFlag.Resources) != MapLayerFlag.None)) ||
                        (overlay.Type.IsWall && ((layers & MapLayerFlag.Walls) != MapLayerFlag.None)) ||
                        ((layers & MapLayerFlag.Overlay) != MapLayerFlag.None))
                    {
                        Render(map.Theater, tiberiumOrGoldTypes, gemTypes, topLeft, tileSize, tileScale, overlay).Item2(graphics);
                    }
                }
            }

            if ((layers & MapLayerFlag.Terrain) != MapLayerFlag.None)
            {
                foreach (var (topLeft, terrain) in map.Technos.OfType<Terrain>())
                {
                    if ((locations != null) && !locations.Contains(topLeft))
                    {
                        continue;
                    }

                    string tileName = terrain.Type.Name;
                    if ((terrain.Type.TemplateType & TemplateTypeFlag.OreMine) != TemplateTypeFlag.None)
                    {
                        tileName = "OREMINE";
                    }

                    if (Globals.TheTilesetManager.GetTileData(map.Theater.Tilesets, tileName, terrain.Icon, out Tile tile))
                    {
                        var tint = terrain.Tint;
                        var imageAttributes = new ImageAttributes();
                        if (tint != Color.White)
                        {
                            var colorMatrix = new ColorMatrix(new float[][]
                            {
                                new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                                new float[] {0, tint.G / 255.0f, 0, 0, 0},
                                new float[] {0, 0, tint.B / 255.0f, 0, 0},
                                new float[] {0, 0, 0, tint.A / 255.0f, 0},
                                new float[] {0, 0, 0, 0, 1},
                            }
                            );
                            imageAttributes.SetColorMatrix(colorMatrix);
                        }

                        var location = new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height);
                        var size = new Size(tile.Image.Width / tileScale, tile.Image.Height / tileScale);
                        var terrainBounds = new Rectangle(location, size);
                        overlappingRenderList.Add((terrainBounds, g => g.DrawImage(tile.Image, terrainBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes)));
                    }
                    else
                    {
                        Debug.Print(string.Format("Terrain {0} ({1}) not found", tileName, terrain.Icon));
                    }
                }
            }

            if ((layers & MapLayerFlag.Buildings) != MapLayerFlag.None)
            {
                foreach (var (topLeft, building) in map.Buildings.OfType<Building>())
                {
                    if ((locations != null) && !locations.Contains(topLeft))
                    {
                        continue;
                    }

                    overlappingRenderList.Add(Render(gameType, map.Theater, topLeft, tileSize, tileScale, building));
                }
            }

            if ((layers & MapLayerFlag.Infantry) != MapLayerFlag.None)
            {
                foreach (var (topLeft, infantryGroup) in map.Technos.OfType<InfantryGroup>())
                {
                    if ((locations != null) && !locations.Contains(topLeft))
                    {
                        continue;
                    }

                    for (int i = 0; i < infantryGroup.Infantry.Length; ++i)
                    {
                        var infantry = infantryGroup.Infantry[i];
                        if (infantry == null)
                        {
                            continue;
                        }

                        overlappingRenderList.Add(Render(map.Theater, topLeft, tileSize, infantry, (InfantryStoppingType)i));
                    }
                }
            }

            if ((layers & MapLayerFlag.Units) != MapLayerFlag.None)
            {
                foreach (var (topLeft, unit) in map.Technos.OfType<Unit>())
                {
                    if ((locations != null) && !locations.Contains(topLeft))
                    {
                        continue;
                    }

                    overlappingRenderList.Add(Render(gameType, map.Theater, topLeft, tileSize, unit));
                }
            }

            foreach (var (location, renderer) in overlappingRenderList.Where(x => !x.Item1.IsEmpty).OrderBy(x => x.Item1.Bottom))
            {
                renderer(graphics);
            }
        }

        public static void Render(GameType gameType, Map map, Graphics graphics, ISet<Point> locations, MapLayerFlag layers)
        {
            Render(gameType, map, graphics, locations, layers, Globals.TileScale);
        }

        public static (Rectangle, Action<Graphics>) Render(TheaterType theater, Point topLeft, Size tileSize, Smudge smudge)
        {
            var tint = smudge.Tint;
            var imageAttributes = new ImageAttributes();
            if (tint != Color.White)
            {
                var colorMatrix = new ColorMatrix(new float[][]
                {
                    new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                    new float[] {0, tint.G / 255.0f, 0, 0, 0},
                    new float[] {0, 0, tint.B / 255.0f, 0, 0},
                    new float[] {0, 0, 0, tint.A / 255.0f, 0},
                    new float[] {0, 0, 0, 0, 1},
                }
                );
                imageAttributes.SetColorMatrix(colorMatrix);
            }

            if (Globals.TheTilesetManager.GetTileData(theater.Tilesets, smudge.Type.Name, smudge.Icon, out Tile tile))
            {
                var location = new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height);
                var smudgeBounds = new Rectangle(location, smudge.Type.RenderSize);

                void render(Graphics g)
                {
                    g.DrawImage(tile.Image, smudgeBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                }

                return (smudgeBounds, render);
            }
            else
            {
                Debug.Print(string.Format("Smudge {0} ({1}) not found", smudge.Type.Name, smudge.Icon));
                return (Rectangle.Empty, (g) => { });
            }
        }

        public static (Rectangle, Action<Graphics>) Render(TheaterType theater, OverlayType[] tiberiumOrGoldTypes, OverlayType[] gemTypes, Point topLeft, Size tileSize, int tileScale, Overlay overlay)
        {
            var name = overlay.Type.Name;
            if (overlay.Type.IsGem)
            {
                name = gemTypes[new Random(randomSeed ^ topLeft.GetHashCode()).Next(tiberiumOrGoldTypes.Length)].Name;
            }
            else if (overlay.Type.IsTiberiumOrGold)
            {
                name = tiberiumOrGoldTypes[new Random(randomSeed ^ topLeft.GetHashCode()).Next(tiberiumOrGoldTypes.Length)].Name;
            }

            if (Globals.TheTilesetManager.GetTileData(theater.Tilesets, name, overlay.Icon, out Tile tile))
            {
                var size = (overlay.Type.IsCrate || overlay.Type.IsFlag) ? new Size(tile.Image.Width / tileScale, tile.Image.Height / tileScale) : tileSize;
                var location = new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height)
                    + new Size(tileSize.Width / 2, tileSize.Height / 2)
                    - new Size(size.Width / 2, size.Height / 2);
                var overlayBounds = new Rectangle(location, size);

                var tint = overlay.Tint;
                void render(Graphics g)
                {
                    var imageAttributes = new ImageAttributes();
                    if (tint != Color.White)
                    {
                        var colorMatrix = new ColorMatrix(new float[][]
                        {
                            new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                            new float[] {0, tint.G / 255.0f, 0, 0, 0},
                            new float[] {0, 0, tint.B / 255.0f, 0, 0},
                            new float[] {0, 0, 0, tint.A / 255.0f, 0},
                            new float[] {0, 0, 0, 0, 1},
                        }
                        );
                        imageAttributes.SetColorMatrix(colorMatrix);
                    }

                    g.DrawImage(tile.Image, overlayBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                }

                return (overlayBounds, render);
            }
            else
            {
                Debug.Print(string.Format("Overlay {0} ({1}) not found", overlay.Type.Name, overlay.Icon));
                return (Rectangle.Empty, (g) => { });
            }
        }

        public static (Rectangle, Action<Graphics>) Render(GameType gameType, TheaterType theater, Point topLeft, Size tileSize, int tileScale, Building building)
        {
            var tint = building.Tint;

            var stringFormat = new StringFormat
            {
                Alignment = StringAlignment.Center,
                LineAlignment = StringAlignment.Center
            };
            var fakeBackgroundBrush = new SolidBrush(Color.FromArgb(building.Tint.A / 2, Color.Black));
            var fakeTextBrush = new SolidBrush(Color.FromArgb(building.Tint.A, Color.White));
            var baseBackgroundBrush = new SolidBrush(Color.FromArgb(building.Tint.A / 2, Color.Black));
            var baseTextBrush = new SolidBrush(Color.FromArgb(building.Tint.A, Color.Red));

            var icon = 0;
            if (building.Type.HasTurret)
            {
                icon = BodyShape[Facing32[building.Direction.ID]];
                if (building.Strength < 128)
                {
                    switch (gameType)
                    {
                        case GameType.TiberianDawn:
                            icon += 64;
                            break;
                        case GameType.RedAlert:
                            icon += building.Type.Equals("sam") ? 35 : 64;
                            break;
                    }
                }
            }
            else
            {
                if (building.Strength <= 1)
                {
                    icon = -1;
                }
                else if (building.Strength < 128)
                {
                    icon = -2;
                    if (building.Type.Equals("weap") || building.Type.Equals("weaf"))
                    {
                        icon = 1;
                    }
                    else if ((gameType == GameType.TiberianDawn) && building.Type.Equals("proc"))
                    {
                        icon = 30;
                    }
                    else if (building.Type.Equals("eye"))
                    {
                        icon = 16;
                    }
                    else if (building.Type.Equals("silo"))
                    {
                        icon = 5;
                    }
                    else if (building.Type.Equals("fix"))
                    {
                        icon = 7;
                    }
                    else if (building.Type.Equals("v19"))
                    {
                        icon = 14;
                    }
                }
            }

            if (Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, building.Type.Tilename, icon, Globals.TheTeamColorManager[building.House.BuildingTeamColor], out Tile tile))
            {
                var location = new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height);
                var size = new Size(tile.Image.Width / tileScale, tile.Image.Height / tileScale);
                var maxSize = new Size(building.Type.Size.Width * tileSize.Width, building.Type.Size.Height * tileSize.Height);
                if ((size.Width >= size.Height) && (size.Width > maxSize.Width))
                {
                    size.Height = size.Height * maxSize.Width / size.Width;
                    size.Width = maxSize.Width;
                }
                else if ((size.Height >= size.Width) && (size.Height > maxSize.Height))
                {
                    size.Width = size.Width * maxSize.Height / size.Height;
                    size.Height = maxSize.Height;
                }
                var buildingBounds = new Rectangle(location, size);

                Tile factoryOverlayTile = null;
                if (building.Type.FactoryOverlay != null)
                {
                    int overlayIcon = 0;
                    if (building.Strength < 128)
                    {
                        switch (gameType)
                        {
                            case GameType.TiberianDawn:
                                overlayIcon = 10;
                                break;
                            case GameType.RedAlert:
                                overlayIcon = 4;
                                break;
                        }
                    }

                    Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, building.Type.FactoryOverlay, overlayIcon, Globals.TheTeamColorManager[building.House.BuildingTeamColor], out factoryOverlayTile);
                }

                void render(Graphics g)
                {
                    var imageAttributes = new ImageAttributes();
                    if (tint != Color.White)
                    {
                        var colorMatrix = new ColorMatrix(new float[][]
                        {
                            new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                            new float[] {0, tint.G / 255.0f, 0, 0, 0},
                            new float[] {0, 0, tint.B / 255.0f, 0, 0},
                            new float[] {0, 0, 0, tint.A / 255.0f, 0},
                            new float[] {0, 0, 0, 0, 1},
                        }
                        );
                        imageAttributes.SetColorMatrix(colorMatrix);
                    }

                    if (factoryOverlayTile != null)
                    {
                        g.DrawImage(tile.Image, buildingBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                        g.DrawImage(factoryOverlayTile.Image, buildingBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                    }
                    else
                    {
                        g.DrawImage(tile.Image, buildingBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                    }

                    if (building.Type.IsFake)
                    {
                        var text = Globals.TheGameTextManager["TEXT_UI_FAKE"];
                        var textSize = g.MeasureString(text, SystemFonts.CaptionFont) + new SizeF(6.0f, 6.0f);
                        var textBounds = new RectangleF(buildingBounds.Location, textSize);
                        g.FillRectangle(fakeBackgroundBrush, textBounds);
                        g.DrawString(text, SystemFonts.CaptionFont, fakeTextBrush, textBounds, stringFormat);
                    }

                    if (building.BasePriority >= 0)
                    {
                        var text = building.BasePriority.ToString();
                        var textSize = g.MeasureString(text, SystemFonts.CaptionFont) + new SizeF(6.0f, 6.0f);
                        var textBounds = new RectangleF(buildingBounds.Location +
                            new Size((int)((buildingBounds.Width - textSize.Width) / 2.0f), (int)(buildingBounds.Height - textSize.Height)),
                            textSize
                        );
                        g.FillRectangle(baseBackgroundBrush, textBounds);
                        g.DrawString(text, SystemFonts.CaptionFont, baseTextBrush, textBounds, stringFormat);
                    }
                }

                return (buildingBounds, render);
            }
            else
            {
                Debug.Print(string.Format("Building {0} (0) not found", building.Type.Name));
                return (Rectangle.Empty, (g) => { });
            }
        }

        public static (Rectangle, Action<Graphics>) Render(TheaterType theater, Point topLeft, Size tileSize, Infantry infantry, InfantryStoppingType infantryStoppingType)
        {
            var icon = HumanShape[Facing32[infantry.Direction.ID]];

            string teamColor = infantry.House?.UnitTeamColor;
            if (Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, infantry.Type.Name, icon, Globals.TheTeamColorManager[teamColor], out Tile tile))
            {
                var baseLocation = new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height)
                    + new Size(tileSize.Width / 2, tileSize.Height / 2);

                var offset = Point.Empty;
                switch (infantryStoppingType)
                {
                    case InfantryStoppingType.UpperLeft:
                        offset.X = -tileSize.Width / 4;
                        offset.Y = -tileSize.Height / 4;
                        break;
                    case InfantryStoppingType.UpperRight:
                        offset.X = tileSize.Width / 4;
                        offset.Y = -tileSize.Height / 4;
                        break;
                    case InfantryStoppingType.LowerLeft:
                        offset.X = -tileSize.Width / 4;
                        offset.Y = tileSize.Height / 4;
                        break;
                    case InfantryStoppingType.LowerRight:
                        offset.X = tileSize.Width / 4;
                        offset.Y = tileSize.Height / 4;
                        break;
                }
                baseLocation.Offset(offset);

                var virtualBounds = new Rectangle(
                    new Point(baseLocation.X - (tile.OpaqueBounds.Width / 2), baseLocation.Y - tile.OpaqueBounds.Height),
                    tile.OpaqueBounds.Size
                );
                var renderBounds = new Rectangle(
                    baseLocation - new Size(infantry.Type.RenderSize.Width / 2, infantry.Type.RenderSize.Height / 2),
                    infantry.Type.RenderSize
                );

                var tint = infantry.Tint;
                void render(Graphics g)
                {
                    var imageAttributes = new ImageAttributes();
                    if (tint != Color.White)
                    {
                        var colorMatrix = new ColorMatrix(new float[][]
                        {
                            new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                            new float[] {0, tint.G / 255.0f, 0, 0, 0},
                            new float[] {0, 0, tint.B / 255.0f, 0, 0},
                            new float[] {0, 0, 0, tint.A / 255.0f, 0},
                            new float[] {0, 0, 0, 0, 1},
                        }
                        );
                        imageAttributes.SetColorMatrix(colorMatrix);
                    }
                    g.DrawImage(tile.Image, renderBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                }

                return (virtualBounds, render);
            }
            else
            {
                Debug.Print(string.Format("Infantry {0} ({1}) not found", infantry.Type.Name, icon));
                return (Rectangle.Empty, (g) => { });
            }
        }

        public static (Rectangle, Action<Graphics>) Render(GameType gameType, TheaterType theater, Point topLeft, Size tileSize, Unit unit)
        {
            int icon = 0;
            if (gameType == GameType.TiberianDawn)
            {
                if (unit.Type == TiberianDawn.UnitTypes.GunBoat)
                {
                    switch (unit.Direction.Facing)
                    {
                        case FacingType.NorthEast:
                        case FacingType.East:
                        case FacingType.SouthEast:
                            icon = 96;
                            break;
                        default:
                            icon = 0;
                            break;
                    }
                }
                else if ((unit.Type == TiberianDawn.UnitTypes.Tric) ||
                         (unit.Type == TiberianDawn.UnitTypes.Trex) ||
                         (unit.Type == TiberianDawn.UnitTypes.Rapt) ||
                         (unit.Type == TiberianDawn.UnitTypes.Steg))
                {
                    var facing = ((unit.Direction.ID + 0x10) & 0xFF) >> 5;
                    icon = BodyShape[facing + ((facing > 0) ? 24 : 0)];
                }
                else if ((unit.Type == TiberianDawn.UnitTypes.Hover) ||
                         (unit.Type == TiberianDawn.UnitTypes.Visceroid))
                {
                    icon = 0;
                }
                else
                {
                    icon = BodyShape[Facing32[unit.Direction.ID]];
                }
            }
            else if (gameType == GameType.RedAlert)
            {
                if (unit.Type.IsAircraft)
                {
                    if ((unit.Type == RedAlert.UnitTypes.Tran) ||
                        (unit.Type == RedAlert.UnitTypes.Heli) ||
                        (unit.Type == RedAlert.UnitTypes.Hind))
                    {
                        icon = BodyShape[Facing32[unit.Direction.ID]];
                    }
                    else
                    {
                        icon = BodyShape[Facing16[unit.Direction.ID] * 2] / 2;
                    }
                }
                else if (unit.Type.IsVessel)
                {
                    if ((unit.Type == RedAlert.UnitTypes.Transport) ||
                        (unit.Type == RedAlert.UnitTypes.Carrier))
                    {
                        icon = 0;
                    }
                    else
                    {
                        icon = BodyShape[Facing16[unit.Direction.ID] * 2] >> 1;
                    }
                }
                else
                {
                    if ((unit.Type == RedAlert.UnitTypes.Ant1) ||
                        (unit.Type == RedAlert.UnitTypes.Ant2) ||
                        (unit.Type == RedAlert.UnitTypes.Ant3))
                    {
                        icon = ((BodyShape[Facing32[unit.Direction.ID]] + 2) / 4) & 0x07;
                    }
                    else
                    {
                        icon = BodyShape[Facing32[unit.Direction.ID]];
                    }
                }
            }

            string teamColor = null;
            if (unit.House != null)
            {
                if (!unit.House.OverrideTeamColors.TryGetValue(unit.Type.Name, out teamColor))
                {
                    teamColor = unit.House.UnitTeamColor;
                }
            }

            if (Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, unit.Type.Name, icon, Globals.TheTeamColorManager[teamColor], out Tile tile))
            {
                var location =
                    new Point(topLeft.X * tileSize.Width, topLeft.Y * tileSize.Height) +
                    new Size(tileSize.Width / 2, tileSize.Height / 2);
                var renderBounds = new Rectangle(
                    location - new Size(unit.Type.RenderSize.Width / 2, unit.Type.RenderSize.Height / 2),
                    unit.Type.RenderSize
                );

                Tile radarTile = null;
                if ((unit.Type == RedAlert.UnitTypes.MGG) ||
                    (unit.Type == RedAlert.UnitTypes.MRJammer) ||
                    (unit.Type == RedAlert.UnitTypes.Tesla))
                {
                    Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, unit.Type.Name, 32, Globals.TheTeamColorManager[teamColor], out radarTile);
                }

                Tile turretTile = null;
                if (unit.Type.HasTurret)
                {
                    var turretName = unit.Type.Name;
                    var turretIcon = icon + 32;
                    if (unit.Type == RedAlert.UnitTypes.Phase)
                    {
                        turretIcon += 6;
                    }
#if TODO
                    else if (unit.Type == RedAlert.UnitTypes.Cruiser)
                    {
                        turretName = "TURR";
                        turretIcon = BodyShape[Facing32[unit.Direction.ID]];
                    }
                    else if (unit.Type == RedAlert.UnitTypes.Destroyer)
                    {
                        turretName = "SSAM";
                        turretIcon = BodyShape[Facing32[unit.Direction.ID]];
                    }
                    else if (unit.Type == RedAlert.UnitTypes.PTBoat)
                    {
                        turretName = "MGUN";
                        turretIcon = BodyShape[Facing32[unit.Direction.ID]];
                    }
#endif

                    Globals.TheTilesetManager.GetTeamColorTileData(theater.Tilesets, turretName, turretIcon, Globals.TheTeamColorManager[teamColor], out turretTile);
                }

                var tint = unit.Tint;
                void render(Graphics g)
                {
                    var imageAttributes = new ImageAttributes();
                    if (tint != Color.White)
                    {
                        var colorMatrix = new ColorMatrix(new float[][]
                        {
                            new float[] {tint.R / 255.0f, 0, 0, 0, 0},
                            new float[] {0, tint.G / 255.0f, 0, 0, 0},
                            new float[] {0, 0, tint.B / 255.0f, 0, 0},
                            new float[] {0, 0, 0, tint.A / 255.0f, 0},
                            new float[] {0, 0, 0, 0, 1},
                        }
                        );
                        imageAttributes.SetColorMatrix(colorMatrix);
                    }

                    g.DrawImage(tile.Image, renderBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);

                    if (radarTile != null)
                    {
                        Point turretAdjust = Point.Empty;
                        if (unit.Type == RedAlert.UnitTypes.MGG)
                        {
                            turretAdjust = TurretAdjust[Facing32[unit.Direction.ID]];
                        }
                        else if (unit.Type != RedAlert.UnitTypes.Tesla)
                        {
                            turretAdjust.Y = -5;
                        }

                        var radarBounds = renderBounds;
                        radarBounds.Offset(
                            turretAdjust.X * tileSize.Width / Globals.PixelWidth,
                            turretAdjust.Y * tileSize.Height / Globals.PixelHeight
                        );

                        g.DrawImage(radarTile.Image, radarBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                    }
                    if (turretTile != null)
                    {
                        Point turretAdjust = Point.Empty;
                        if (gameType == GameType.RedAlert)
                        {
                            if (unit.Type.IsVessel)
                            {

                            }
                            else if (unit.Type == RedAlert.UnitTypes.Jeep)
                            {
                                turretAdjust.Y = -4;
                            }
                        }
                        else if (gameType == GameType.TiberianDawn)
                        {
                            if ((unit.Type == TiberianDawn.UnitTypes.Jeep) ||
                                (unit.Type == TiberianDawn.UnitTypes.Buggy))
                            {
                                turretAdjust.Y = -4;
                            }
                            else if ((unit.Type == TiberianDawn.UnitTypes.SAM) ||
                                     (unit.Type == TiberianDawn.UnitTypes.MLRS))
                            {
                                turretAdjust = TurretAdjust[Facing32[unit.Direction.ID]];
                            }
                        }

                        var turretBounds = renderBounds;
                        turretBounds.Offset(
                            turretAdjust.X * tileSize.Width / Globals.PixelWidth,
                            turretAdjust.Y * tileSize.Height / Globals.PixelHeight
                        );

                        g.DrawImage(turretTile.Image, turretBounds, 0, 0, tile.Image.Width, tile.Image.Height, GraphicsUnit.Pixel, imageAttributes);
                    }
                }

                return (renderBounds, render);
            }
            else
            {
                Debug.Print(string.Format("Unit {0} ({1}) not found", unit.Type.Name, icon));
                return (Rectangle.Empty, (g) => { });
            }
        }
    }
}
