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
using MobiusEditor.Render;
using MobiusEditor.Utility;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using TGASharpLib;

namespace MobiusEditor.Model
{
    [Flags]
    public enum MapLayerFlag
    {
        None            = 0,
        Basic           = 1 << 0,
        Map             = 1 << 1,
        Template        = 1 << 2,
        Terrain         = 1 << 3,
        Resources       = 1 << 4,
        Walls           = 1 << 5,
        Overlay         = 1 << 6,
        Smudge          = 1 << 7,
        Waypoints       = 1 << 8,
        CellTriggers    = 1 << 9,
        Houses          = 1 << 10,
        Infantry        = 1 << 11,
        Units           = 1 << 12,
        Buildings       = 1 << 13,
        Boundaries      = 1 << 14,
        TechnoTriggers  = 1 << 15,

        OverlayAll = Resources | Walls | Overlay,
        Technos = Terrain | Walls | Infantry | Units | Buildings,

        All = int.MaxValue
    }

    public class MapContext : ITypeDescriptorContext
    {
        public IContainer Container { get; private set; }

        public object Instance { get; private set; }

        public PropertyDescriptor PropertyDescriptor { get; private set; }

        public Map Map => Instance as Map;

        public readonly bool FractionalPercentages;

        public MapContext(Map map, bool fractionalPercentages)
        {
            Instance = map;
            FractionalPercentages = fractionalPercentages;
        }

        public object GetService(Type serviceType) => null;

        public void OnComponentChanged() { }

        public bool OnComponentChanging() => true;
    }

    public class Map : ICloneable
    {
        private int updateCount = 0;
        private bool updating = false;
        private IDictionary<MapLayerFlag, ISet<Point>> invalidateLayers = new Dictionary<MapLayerFlag, ISet<Point>>();
        private bool invalidateOverlappers;

        public readonly BasicSection BasicSection;

        public readonly MapSection MapSection = new MapSection();

        public readonly BriefingSection BriefingSection = new BriefingSection();

        public readonly SteamSection SteamSection = new SteamSection();

        public TheaterType Theater { get => MapSection.Theater; set => MapSection.Theater = value; }

        public Point TopLeft
        {
            get => new Point(MapSection.X, MapSection.Y);
            set { MapSection.X = value.X; MapSection.Y = value.Y; }
        }

        public Size Size
        {
            get => new Size(MapSection.Width, MapSection.Height);
            set { MapSection.Width = value.Width; MapSection.Height = value.Height; }
        }

        public Rectangle Bounds
        {
            get => new Rectangle(TopLeft, Size);
            set { MapSection.X = value.Left; MapSection.Y = value.Top; MapSection.Width = value.Width; MapSection.Height = value.Height; }
        }

        public readonly Type HouseType;

        public readonly HouseType[] HouseTypes;

        public readonly List<TheaterType> TheaterTypes;

        public readonly List<TemplateType> TemplateTypes;

        public readonly List<TerrainType> TerrainTypes;

        public readonly List<OverlayType> OverlayTypes;

        public readonly List<SmudgeType> SmudgeTypes;

        public readonly string[] EventTypes;

        public readonly string[] ActionTypes;

        public readonly string[] MissionTypes;

        public readonly List<DirectionType> DirectionTypes;

        public readonly List<InfantryType> InfantryTypes;

        public readonly List<UnitType> UnitTypes;

        public readonly List<BuildingType> BuildingTypes;

        public readonly string[] TeamMissionTypes;

        public readonly CellMetrics Metrics;

        public readonly CellGrid<Template> Templates;

        public readonly CellGrid<Overlay> Overlay;

        public readonly CellGrid<Smudge> Smudge;

        public readonly OccupierSet<ICellOccupier> Technos;

        public readonly OccupierSet<ICellOccupier> Buildings;

        public readonly OverlapperSet<ICellOverlapper> Overlappers;

        public readonly Waypoint[] Waypoints;

        public readonly CellGrid<CellTrigger> CellTriggers;

        public readonly ObservableCollection<Trigger> Triggers;

        public readonly List<TeamType> TeamTypes;

        public House[] Houses;

        public readonly List<string> MovieTypes;

        public int TiberiumOrGoldValue { get; set; }

        public int GemValue { get; set; }

        public int TotalResources
        {
            get
            {
                int totalResources = 0;
                foreach (var (cell, value) in Overlay)
                {
                    if (value.Type.IsResource)
                    {
                        totalResources += (value.Icon + 1) * (value.Type.IsGem ? GemValue : TiberiumOrGoldValue);
                    }
                }
                return totalResources;
            }
        }

        public Map(BasicSection basicSection, TheaterType theater, Size cellSize, Type houseType,
            IEnumerable<HouseType> houseTypes, IEnumerable<TheaterType> theaterTypes, IEnumerable<TemplateType> templateTypes,
            IEnumerable<TerrainType> terrainTypes, IEnumerable<OverlayType> overlayTypes, IEnumerable<SmudgeType> smudgeTypes,
            IEnumerable<string> eventTypes, IEnumerable<string> actionTypes, IEnumerable<string> missionTypes,
            IEnumerable<DirectionType> directionTypes, IEnumerable<InfantryType> infantryTypes, IEnumerable<UnitType> unitTypes,
            IEnumerable<BuildingType> buildingTypes, IEnumerable<string> teamMissionTypes, IEnumerable<Waypoint> waypoints,
            IEnumerable<string> movieTypes)
        {
            BasicSection = basicSection;

            HouseType = houseType;
            HouseTypes = houseTypes.ToArray();
            TheaterTypes = new List<TheaterType>(theaterTypes);
            TemplateTypes = new List<TemplateType>(templateTypes);
            TerrainTypes = new List<TerrainType>(terrainTypes);
            OverlayTypes = new List<OverlayType>(overlayTypes);
            SmudgeTypes = new List<SmudgeType>(smudgeTypes);
            EventTypes = eventTypes.ToArray();
            ActionTypes = actionTypes.ToArray();
            MissionTypes = missionTypes.ToArray();
            DirectionTypes = new List<DirectionType>(directionTypes);
            InfantryTypes = new List<InfantryType>(infantryTypes);
            UnitTypes = new List<UnitType>(unitTypes);
            BuildingTypes = new List<BuildingType>(buildingTypes);
            TeamMissionTypes = teamMissionTypes.ToArray();
            MovieTypes = new List<string>(movieTypes);

            Metrics = new CellMetrics(cellSize);
            Templates = new CellGrid<Template>(Metrics);
            Overlay = new CellGrid<Overlay>(Metrics);
            Smudge = new CellGrid<Smudge>(Metrics);
            Technos = new OccupierSet<ICellOccupier>(Metrics);
            Buildings = new OccupierSet<ICellOccupier>(Metrics);
            Overlappers = new OverlapperSet<ICellOverlapper>(Metrics);
            Triggers = new ObservableCollection<Trigger>();
            TeamTypes = new List<TeamType>();
            Houses = HouseTypes.Select(t => { var h = (House)Activator.CreateInstance(HouseType, t); h.SetDefault(); return h; }).ToArray();
            Waypoints = waypoints.ToArray();
            CellTriggers = new CellGrid<CellTrigger>(Metrics);

            MapSection.SetDefault();
            BriefingSection.SetDefault();
            SteamSection.SetDefault();
            Templates.Clear();
            Overlay.Clear();
            Smudge.Clear();
            Technos.Clear();
            Overlappers.Clear();
            CellTriggers.Clear();

            TopLeft = new Point(1, 1);
            Size = Metrics.Size - new Size(2, 2);
            Theater = theater;

            Overlay.CellChanged += Overlay_CellChanged;
            Technos.OccupierAdded += Technos_OccupierAdded;
            Technos.OccupierRemoved += Technos_OccupierRemoved;
            Buildings.OccupierAdded += Buildings_OccupierAdded;
            Buildings.OccupierRemoved += Buildings_OccupierRemoved;
            Triggers.CollectionChanged += Triggers_CollectionChanged;
        }

        public void BeginUpdate()
        {
            updateCount++;
        }

        public void EndUpdate()
        {
            if (--updateCount == 0)
            {
                Update();
            }
        }

        public void InitTheater(GameType gameType)
        {
            foreach (var templateType in TemplateTypes)
            {
                templateType.Init(Theater);
            }

            foreach (var smudgeType in SmudgeTypes)
            {
                smudgeType.Init(Theater);
            }

            foreach (var overlayType in OverlayTypes)
            {
                overlayType.Init(Theater);
            }

            foreach (var terrainType in TerrainTypes)
            {
                terrainType.Init(Theater);
            }

            foreach (var infantryType in InfantryTypes)
            {
                infantryType.Init(gameType, Theater, HouseTypes.Where(h => h.Equals(infantryType.OwnerHouse)).FirstOrDefault(), DirectionTypes.Where(d => d.Facing == FacingType.South).First());
            }

            foreach (var unitType in UnitTypes)
            {
                unitType.Init(gameType, Theater, HouseTypes.Where(h => h.Equals(unitType.OwnerHouse)).FirstOrDefault(), DirectionTypes.Where(d => d.Facing == FacingType.North).First());
            }

            foreach (var buildingType in BuildingTypes)
            {
                buildingType.Init(gameType, Theater, HouseTypes.Where(h => h.Equals(buildingType.OwnerHouse)).FirstOrDefault(), DirectionTypes.Where(d => d.Facing == FacingType.North).First());
            }
        }

        private void Update()
        {
            updating = true;

            if (invalidateLayers.TryGetValue(MapLayerFlag.Resources, out ISet<Point> locations))
            {
                UpdateResourceOverlays(locations);
            }

            if (invalidateLayers.TryGetValue(MapLayerFlag.Walls, out locations))
            {
                UpdateWallOverlays(locations);
            }

            if (invalidateOverlappers)
            {
                Overlappers.Clear();
                foreach (var (location, techno) in Technos)
                {
                    if (techno is ICellOverlapper)
                    {
                        Overlappers.Add(location, techno as ICellOverlapper);
                    }
                }
            }

            invalidateLayers.Clear();
            invalidateOverlappers = false;
            updating = false;
        }

        private void UpdateResourceOverlays(ISet<Point> locations)
        {
            var tiberiumCounts = new int[] { 0, 1, 3, 4, 6, 7, 8, 10, 11 };
            var gemCounts = new int[] { 0, 0, 0, 1, 1, 1, 2, 2, 2 };

            foreach (var (cell, overlay) in Overlay.IntersectsWith(locations).Where(o => o.Value.Type.IsResource))
            {
                int count = 0;
                foreach (var facing in CellMetrics.AdjacentFacings)
                {
                    var adjacentTiberium = Overlay.Adjacent(cell, facing);
                    if (adjacentTiberium?.Type.IsResource ?? false)
                    {
                        count++;
                    }
                }

                overlay.Icon = overlay.Type.IsGem ? gemCounts[count] : tiberiumCounts[count];
            }
        }

        private void UpdateWallOverlays(ISet<Point> locations)
        {
            foreach (var (cell, overlay) in Overlay.IntersectsWith(locations).Where(o => o.Value.Type.IsWall))
            {
                var northWall = Overlay.Adjacent(cell, FacingType.North);
                var eastWall = Overlay.Adjacent(cell, FacingType.East);
                var southWall = Overlay.Adjacent(cell, FacingType.South);
                var westWall = Overlay.Adjacent(cell, FacingType.West);

                int icon = 0;
                if (northWall?.Type == overlay.Type)
                {
                    icon |= 1;
                }
                if (eastWall?.Type == overlay.Type)
                {
                    icon |= 2;
                }
                if (southWall?.Type == overlay.Type)
                {
                    icon |= 4;
                }
                if (westWall?.Type == overlay.Type)
                {
                    icon |= 8;
                }

                overlay.Icon = icon;
            }
        }

        private void RemoveBibs(Building building)
        {
            var bibCells = Smudge.IntersectsWith(building.BibCells).Where(x => (x.Value.Type.Flag & SmudgeTypeFlag.Bib) != SmudgeTypeFlag.None).Select(x => x.Cell).ToArray();
            foreach (var cell in bibCells)
            {
                Smudge[cell] = null;
            }
            building.BibCells.Clear();
        }

        private void AddBibs(Point location, Building building)
        {
            if (!building.Type.HasBib)
            {
                return;
            }

            var bib1Type = SmudgeTypes.Where(t => t.Flag == SmudgeTypeFlag.Bib1).FirstOrDefault();
            var bib2Type = SmudgeTypes.Where(t => t.Flag == SmudgeTypeFlag.Bib2).FirstOrDefault();
            var bib3Type = SmudgeTypes.Where(t => t.Flag == SmudgeTypeFlag.Bib3).FirstOrDefault();

            SmudgeType bibType = null;
            switch (building.Type.Size.Width)
            {
                case 2:
                    bibType = bib3Type;
                    break;
                case 3:
                    bibType = bib2Type;
                    break;
                case 4:
                    bibType = bib1Type;
                    break;
            }
            if (bibType != null)
            {
                int icon = 0;
                for (var y = 0; y < bibType.Size.Height; ++y)
                {
                    for (var x = 0; x < bibType.Size.Width; ++x, ++icon)
                    {
                        if (Metrics.GetCell(new Point(location.X + x, location.Y + building.Type.Size.Height + y - 1), out int subCell))
                        {
                            Smudge[subCell] = new Smudge
                            {
                                Type = bibType,
                                Icon = icon,
                                Data = 0,
                                Tint = building.Tint
                            };
                            building.BibCells.Add(subCell);
                        }
                    }
                }
            }
        }

        public Map Clone()
        {
            var map = new Map(BasicSection, Theater, Metrics.Size, HouseType,
                HouseTypes, TheaterTypes, TemplateTypes, TerrainTypes, OverlayTypes, SmudgeTypes,
                EventTypes, ActionTypes, MissionTypes, DirectionTypes, InfantryTypes, UnitTypes,
                BuildingTypes, TeamMissionTypes, Waypoints, MovieTypes)
            {
                TopLeft = TopLeft,
                Size = Size
            };

            map.BeginUpdate();

            MapSection.CopyTo(map.MapSection);
            BriefingSection.CopyTo(map.BriefingSection);
            SteamSection.CopyTo(map.SteamSection);
            Templates.CopyTo(map.Templates);
            Overlay.CopyTo(map.Overlay);
            Smudge.CopyTo(map.Smudge);
            CellTriggers.CopyTo(map.CellTriggers);
            Array.Copy(Houses, map.Houses, map.Houses.Length);

            foreach (var trigger in Triggers)
            {
                map.Triggers.Add(trigger);
            }

            foreach (var (location, occupier) in Technos)
            {
                if (occupier is InfantryGroup infantryGroup)
                {
                    var newInfantryGroup = new InfantryGroup();
                    Array.Copy(infantryGroup.Infantry, newInfantryGroup.Infantry, newInfantryGroup.Infantry.Length);
                    map.Technos.Add(location, newInfantryGroup);
                }
                else if (!(occupier is Building))
                {
                    map.Technos.Add(location, occupier);
                }
            }

            foreach (var (location, building) in Buildings)
            {
                map.Buildings.Add(location, building);
            }

            map.TeamTypes.AddRange(TeamTypes);

            map.EndUpdate();

            return map;
        }

        public TGA GeneratePreview(Size previewSize, bool sharpen)
        {
            var mapBounds = new Rectangle(
                Bounds.Left * Globals.OriginalTileWidth,
                Bounds.Top * Globals.OriginalTileHeight,
                Bounds.Width * Globals.OriginalTileWidth,
                Bounds.Height * Globals.OriginalTileHeight
            );
            var previewScale = Math.Min(previewSize.Width / (float)mapBounds.Width, previewSize.Height / (float)mapBounds.Height);
            var scaledSize = new Size((int)(previewSize.Width / previewScale), (int)(previewSize.Height / previewScale));

            using (var fullBitmap = new Bitmap(Metrics.Width * Globals.OriginalTileWidth, Metrics.Height * Globals.OriginalTileHeight))
            using (var croppedBitmap = new Bitmap(previewSize.Width, previewSize.Height))
            {
                var locations = Bounds.Points().ToHashSet();
                using (var g = Graphics.FromImage(fullBitmap))
                {
                    MapRenderer.Render(GameType.None, this, g, locations, MapLayerFlag.Template | MapLayerFlag.Resources, 1);
                }

                using (var g = Graphics.FromImage(croppedBitmap))
                {
                    Matrix transform = new Matrix();
                    transform.Scale(previewScale, previewScale);
                    transform.Translate((scaledSize.Width - mapBounds.Width) / 2, (scaledSize.Height - mapBounds.Height) / 2);

                    g.Transform = transform;
                    g.Clear(Color.Black);
                    g.DrawImage(fullBitmap, new Rectangle(0, 0, mapBounds.Width, mapBounds.Height), mapBounds, GraphicsUnit.Pixel);
                }

                fullBitmap.Dispose();

                if (sharpen)
                {
                    using (var sharpenedImage = croppedBitmap.Sharpen(1.0f))
                    {
                        croppedBitmap.Dispose();
                        return TGA.FromBitmap(sharpenedImage);
                    }
                }
                else
                {
                    return TGA.FromBitmap(croppedBitmap);
                }
            }
        }

        public TGA GenerateMapPreview()
        {
            return GeneratePreview(Globals.MapPreviewSize, false);
        }

        public TGA GenerateWorkshopPreview()
        {
            return GeneratePreview(Globals.WorkshopPreviewSize, true);
        }

        object ICloneable.Clone()
        {
            return Clone();
        }

        private void Overlay_CellChanged(object sender, CellChangedEventArgs<Overlay> e)
        {
            if (e.OldValue?.Type.IsWall ?? false)
            {
                Buildings.Remove(e.OldValue);
            }

            if (e.Value?.Type.IsWall ?? false)
            {
                Buildings.Add(e.Location, e.Value);
            }

            if (updating)
            {
                return;
            }

            foreach (var overlay in new Overlay[] { e.OldValue, e.Value })
            {
                if (overlay == null)
                {
                    continue;
                }

                MapLayerFlag layer = MapLayerFlag.None;
                if (overlay.Type.IsResource)
                {
                    layer = MapLayerFlag.Resources;
                }
                else if (overlay.Type.IsWall)
                {
                    layer = MapLayerFlag.Walls;
                }
                else
                {
                    continue;
                }

                if (!invalidateLayers.TryGetValue(layer, out ISet<Point> locations))
                {
                    locations = new HashSet<Point>();
                    invalidateLayers[layer] = locations;
                }

                locations.UnionWith(Rectangle.Inflate(new Rectangle(e.Location, new Size(1, 1)), 1, 1).Points());
            }

            if (updateCount == 0)
            {
                Update();
            }
        }

        private void Technos_OccupierAdded(object sender, OccupierAddedEventArgs<ICellOccupier> e)
        {
            if (e.Occupier is ICellOverlapper overlapper)
            {
                if (updateCount == 0)
                {
                    Overlappers.Add(e.Location, overlapper);
                }
                else
                {
                    invalidateOverlappers = true;
                }
            }
        }

        private void Technos_OccupierRemoved(object sender, OccupierRemovedEventArgs<ICellOccupier> e)
        {
            if (e.Occupier is ICellOverlapper overlapper)
            {
                if (updateCount == 0)
                {
                    Overlappers.Remove(overlapper);
                }
                else
                {
                    invalidateOverlappers = true;
                }
            }
        }

        private void Buildings_OccupierAdded(object sender, OccupierAddedEventArgs<ICellOccupier> e)
        {
            if (e.Occupier is Building building)
            {
                Technos.Add(e.Location, e.Occupier, building.Type.BaseOccupyMask);
                AddBibs(e.Location, building);
            }
            else
            {
                Technos.Add(e.Location, e.Occupier);
            }
        }

        private void Buildings_OccupierRemoved(object sender, OccupierRemovedEventArgs<ICellOccupier> e)
        {
            if (e.Occupier is Building building)
            {
                RemoveBibs(building);
            }

            Technos.Remove(e.Occupier);
        }

        private void Triggers_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            foreach (var (_, building) in Buildings.OfType<Building>())
            {
                if (!string.IsNullOrEmpty(building.Trigger))
                {
                    if (Triggers.Where(t => building.Trigger.Equals(t.Name)).FirstOrDefault() == null)
                    {
                        building.Trigger = Trigger.None;
                    }
                }
            }
        }
    }
}
