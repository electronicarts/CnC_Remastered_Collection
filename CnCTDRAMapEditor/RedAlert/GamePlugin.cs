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
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MobiusEditor.RedAlert
{
    class GamePlugin : IGamePlugin
    {
        private static readonly IEnumerable<string> movieTypes = new string[]
        {
            "x",
            "AAGUN",
            "AFTRMATH",
            "AIRFIELD",
            "ALLIEND",
            "ALLY1",
            "ALLY10",
            "ALLY10B",
            "ALLY11",
            "ALLY12",
            "ALLY14",
            "ALLY2",
            "ALLY4",
            "ALLY5",
            "ALLY6",
            "ALLY8",
            "ALLY9",
            "ALLYMORF",
            "ANTEND",
            "ANTINTRO",
            "APCESCPE",
            "ASSESS",
            "AVERTED",
            "BATTLE",
            "BEACHEAD",
            "BINOC",
            "BMAP",
            "BOMBRUN",
            "BRDGTILT",
            "COUNTDWN",
            "CRONFAIL",
            "CRONTEST",
            "DESTROYR",
            "DOUBLE",
            "DPTHCHRG",
            "DUD",
            "ELEVATOR",
            "ENGLISH",
            "EXECUTE",
            "FLARE",
            "FROZEN",
            "GRVESTNE",
            "LANDING",
            "MASASSLT",
            "MCV",
            "MCVBRDGE",
            "MCV_LAND",
            "MIG",
            "MONTPASS",
            "MOVINGIN",
            "MTNKFACT",
            "NUKESTOK",
            "OILDRUM",
            "ONTHPRWL",
            "OVERRUN",
            "PERISCOP",
            "PROLOG",
            "RADRRAID",
            "REDINTRO",
            "RETALIATION_ALLIED1",
            "RETALIATION_ALLIED10",
            "RETALIATION_ALLIED2",
            "RETALIATION_ALLIED3",
            "RETALIATION_ALLIED4",
            "RETALIATION_ALLIED5",
            "RETALIATION_ALLIED6",
            "RETALIATION_ALLIED7",
            "RETALIATION_ALLIED8",
            "RETALIATION_ALLIED9",
            "RETALIATION_ANTS",
            "RETALIATION_SOVIET1",
            "RETALIATION_SOVIET10",
            "RETALIATION_SOVIET2",
            "RETALIATION_SOVIET3",
            "RETALIATION_SOVIET4",
            "RETALIATION_SOVIET5",
            "RETALIATION_SOVIET6",
            "RETALIATION_SOVIET7",
            "RETALIATION_SOVIET8",
            "RETALIATION_SOVIET9",
            "RETALIATION_WINA",
            "RETALIATION_WINS",
            "SEARCH",
            "SFROZEN",
            "SHIPSINK",
            "SHIPYARD", // MISSING
            "SHORBOM1",
            "SHORBOM2",
            "SHORBOMB",
            "SITDUCK",
            "SIZZLE",   //MISSING
            "SIZZLE2",  //MISSING
            "SLNTSRVC",
            "SNOWBASE",
            "SNOWBOMB",
            "SNSTRAFE",
            "SOVBATL",
            "SOVCEMET",
            "SOVFINAL",
            "SOVIET1",
            "SOVIET10",
            "SOVIET11",
            "SOVIET12",
            "SOVIET13",
            "SOVIET14",
            "SOVIET2",
            "SOVIET3",
            "SOVIET4",
            "SOVIET5",
            "SOVIET6",
            "SOVIET7",
            "SOVIET8",
            "SOVIET9",
            "SOVMCV",
            "SOVTSTAR",
            "SPOTTER",
            "SPY",
            "STRAFE",
            "TAKE_OFF",
            "TANYA1",
            "TANYA2",
            "TESLA",
            "TOOFAR",
            "TRINITY",
            "V2ROCKET",
        };

        private static readonly IEnumerable<ITechnoType> technoTypes;

        public GameType GameType => GameType.RedAlert;

        public Map Map { get; }

        public Image MapImage { get; private set; }

        public bool Dirty { get; set; }

        private INISectionCollection extraSections;

        static GamePlugin()
        {
            technoTypes = InfantryTypes.GetTypes().Cast<ITechnoType>().Concat(UnitTypes.GetTypes().Cast<ITechnoType>());
        }

        public GamePlugin(bool mapImage)
        {
            var playerWaypoints = Enumerable.Range(0, 8).Select(i => new Waypoint(string.Format("P{0}", i), WaypointFlag.PlayerStart));
            var generalWaypoints = Enumerable.Range(8, 90).Select(i => new Waypoint(i.ToString()));
            var specialWaypoints = new Waypoint[] { new Waypoint("Home"), new Waypoint("Reinf."), new Waypoint("Special") };
            var waypoints = playerWaypoints.Concat(generalWaypoints).Concat(specialWaypoints);

            var basicSection = new BasicSection();
            basicSection.SetDefault();

            var houseTypes = HouseTypes.GetTypes();
            basicSection.Player = houseTypes.First().Name;

            Map = new Map(basicSection, null, Constants.MaxSize, typeof(House),
                houseTypes, TheaterTypes.GetTypes(), TemplateTypes.GetTypes(), TerrainTypes.GetTypes(),
                OverlayTypes.GetTypes(), SmudgeTypes.GetTypes(), EventTypes.GetTypes(), ActionTypes.GetTypes(),
                MissionTypes.GetTypes(), DirectionTypes.GetTypes(), InfantryTypes.GetTypes(), UnitTypes.GetTypes(),
                BuildingTypes.GetTypes(), TeamMissionTypes.GetTypes(), waypoints, movieTypes)
            {
                TiberiumOrGoldValue = 35,
                GemValue = 110
            };

            Map.BasicSection.PropertyChanged += BasicSection_PropertyChanged;
            Map.MapSection.PropertyChanged += MapSection_PropertyChanged;

            if (mapImage)
            {
                MapImage = new Bitmap(Map.Metrics.Width * Globals.TileWidth, Map.Metrics.Height * Globals.TileHeight);
            }
        }

        public GamePlugin()
            : this(true)
        {
        }

        public void New(string theater)
        {
            Map.Theater = Map.TheaterTypes.Where(t => t.Equals(theater)).FirstOrDefault() ?? TheaterTypes.Temperate;
            Map.TopLeft = new Point(1, 1);
            Map.Size = Map.Metrics.Size - new Size(2, 2);

            UpdateBasePlayerHouse();

            Dirty = true;
        }

        public IEnumerable<string> Load(string path, FileType fileType)
        {
            var errors = new List<string>();

            switch (fileType)
            {
                case FileType.INI:
                case FileType.BIN:
                    {
                        var ini = new INI();
                        using (var reader = new StreamReader(path))
                        {
                            ini.Parse(reader);
                        }
                        errors.AddRange(LoadINI(ini));
                    }
                    break;
                case FileType.MEG:
                case FileType.PGM:
                    {
                        using (var megafile = new Megafile(path))
                        {
                            var mprFile = megafile.Where(p => Path.GetExtension(p).ToLower() == ".mpr").FirstOrDefault();
                            if (mprFile != null)
                            {
                                var ini = new INI();
                                using (var reader = new StreamReader(megafile.Open(mprFile)))
                                {
                                    ini.Parse(reader);
                                }
                                errors.AddRange(LoadINI(ini));
                            }
                        }
                    }
                    break;
                default:
                    throw new NotSupportedException();
            }

            return errors;
        }

        private IEnumerable<string> LoadINI(INI ini)
        {
            var errors = new List<string>();

            Map.BeginUpdate();

            var basicSection = ini.Sections.Extract("Basic");
            if (basicSection != null)
            {
                INI.ParseSection(new MapContext(Map, true), basicSection, Map.BasicSection);
            }

            Map.BasicSection.Player = Map.HouseTypes.Where(t => t.Equals(Map.BasicSection.Player)).FirstOrDefault()?.Name ?? Map.HouseTypes.First().Name;
            Map.BasicSection.BasePlayer = HouseTypes.GetBasePlayer(Map.BasicSection.Player);

            var mapSection = ini.Sections.Extract("Map");
            if (mapSection != null)
            {
                INI.ParseSection(new MapContext(Map, true), mapSection, Map.MapSection);
            }

            var steamSection = ini.Sections.Extract("Steam");
            if (steamSection != null)
            {
                INI.ParseSection(new MapContext(Map, true), steamSection, Map.SteamSection);
            }

            string indexToType(IList<string> list, string index)
            {
                return (int.TryParse(index, out int result) && (result >= 0) && (result < list.Count)) ? list[result] : list.First();
            }

            var teamTypesSection = ini.Sections.Extract("TeamTypes");
            if (teamTypesSection != null)
            {
                foreach (var (Key, Value) in teamTypesSection)
                {
                    try
                    {
                        var teamType = new TeamType { Name = Key };

                        var tokens = Value.Split(',').ToList();
                        teamType.House = Map.HouseTypes.Where(t => t.Equals(sbyte.Parse(tokens[0]))).FirstOrDefault(); tokens.RemoveAt(0);

                        var flags = int.Parse(tokens[0]); tokens.RemoveAt(0);
                        teamType.IsRoundAbout = (flags & 0x01) != 0;
                        teamType.IsSuicide = (flags & 0x02) != 0;
                        teamType.IsAutocreate = (flags & 0x04) != 0;
                        teamType.IsPrebuilt = (flags & 0x08) != 0;
                        teamType.IsReinforcable = (flags & 0x10) != 0;

                        teamType.RecruitPriority = int.Parse(tokens[0]); tokens.RemoveAt(0);
                        teamType.InitNum = byte.Parse(tokens[0]); tokens.RemoveAt(0);
                        teamType.MaxAllowed = byte.Parse(tokens[0]); tokens.RemoveAt(0);
                        teamType.Origin = int.Parse(tokens[0]) + 1; tokens.RemoveAt(0);
                        teamType.Trigger = tokens[0]; tokens.RemoveAt(0);

                        var numClasses = int.Parse(tokens[0]); tokens.RemoveAt(0);
                        for (int i = 0; i < Math.Min(Globals.MaxTeamClasses, numClasses); ++i)
                        {
                            var classTokens = tokens[0].Split(':'); tokens.RemoveAt(0);
                            if (classTokens.Length == 2)
                            {
                                var type = technoTypes.Where(t => t.Equals(classTokens[0])).FirstOrDefault();
                                var count = byte.Parse(classTokens[1]);
                                if (type != null)
                                {
                                    teamType.Classes.Add(new TeamTypeClass { Type = type, Count = count });
                                }
                                else
                                {
                                    errors.Add(string.Format("Team '{0}' references unknown class '{1}'", Key, classTokens[0]));
                                }
                            }
                            else
                            {
                                errors.Add(string.Format("Team '{0}' has wrong number of tokens for class index {1} (expecting 2)", Key, i));
                            }
                        }

                        var numMissions = int.Parse(tokens[0]); tokens.RemoveAt(0);
                        for (int i = 0; i < Math.Min(Globals.MaxTeamMissions, numMissions); ++i)
                        {
                            var missionTokens = tokens[0].Split(':'); tokens.RemoveAt(0);
                            if (missionTokens.Length == 2)
                            {
                                teamType.Missions.Add(new TeamTypeMission { Mission = indexToType(Map.TeamMissionTypes, missionTokens[0]), Argument = int.Parse(missionTokens[1]) });
                            }
                            else
                            {
                                errors.Add(string.Format("Team '{0}' has wrong number of tokens for mission index {1} (expecting 2)", Key, i));
                            }
                        }

                        Map.TeamTypes.Add(teamType);
                    }
                    catch (ArgumentOutOfRangeException) { }
                }
            }

            var triggersSection = ini.Sections.Extract("Trigs");
            if (triggersSection != null)
            {
                foreach (var (Key, Value) in triggersSection)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length == 18)
                    {
                        var trigger = new Trigger { Name = Key };

                        trigger.PersistantType = (TriggerPersistantType)int.Parse(tokens[0]);
                        trigger.House = Map.HouseTypes.Where(t => t.Equals(sbyte.Parse(tokens[1]))).FirstOrDefault()?.Name ?? "None";
                        trigger.EventControl = (TriggerMultiStyleType)int.Parse(tokens[2]);

                        trigger.Event1.EventType = indexToType(Map.EventTypes, tokens[4]);
                        trigger.Event1.Team = tokens[5];
                        trigger.Event1.Data = long.Parse(tokens[6]);

                        trigger.Event2.EventType = indexToType(Map.EventTypes, tokens[7]);
                        trigger.Event2.Team = tokens[8];
                        trigger.Event2.Data = long.Parse(tokens[9]);

                        trigger.Action1.ActionType = indexToType(Map.ActionTypes, tokens[10]);
                        trigger.Action1.Team = tokens[11];
                        trigger.Action1.Trigger = tokens[12];
                        trigger.Action1.Data = long.Parse(tokens[13]);

                        trigger.Action2.ActionType = indexToType(Map.ActionTypes, tokens[14]);
                        trigger.Action2.Team = tokens[15];
                        trigger.Action2.Trigger = tokens[16];
                        trigger.Action2.Data = long.Parse(tokens[17]);

                        // Fix up data caused by union usage in the legacy game
                        Action<TriggerEvent> fixEvent = (TriggerEvent e) =>
                        {
                            switch (e.EventType)
                            {
                                case EventTypes.TEVENT_THIEVED:
                                case EventTypes.TEVENT_PLAYER_ENTERED:
                                case EventTypes.TEVENT_CROSS_HORIZONTAL:
                                case EventTypes.TEVENT_CROSS_VERTICAL:
                                case EventTypes.TEVENT_ENTERS_ZONE:
                                case EventTypes.TEVENT_HOUSE_DISCOVERED:
                                case EventTypes.TEVENT_BUILDINGS_DESTROYED:
                                case EventTypes.TEVENT_UNITS_DESTROYED:
                                case EventTypes.TEVENT_ALL_DESTROYED:
                                case EventTypes.TEVENT_LOW_POWER:
                                case EventTypes.TEVENT_BUILDING_EXISTS:
                                case EventTypes.TEVENT_BUILD:
                                case EventTypes.TEVENT_BUILD_UNIT:
                                case EventTypes.TEVENT_BUILD_INFANTRY:
                                case EventTypes.TEVENT_BUILD_AIRCRAFT:
                                    e.Data &= 0xFF;
                                    break;
                                default:
                                    break;
                            }
                        };

                        Action<TriggerAction> fixAction = (TriggerAction a) =>
                        {
                            switch (a.ActionType)
                            {
                                case ActionTypes.TACTION_1_SPECIAL:
                                case ActionTypes.TACTION_FULL_SPECIAL:
                                case ActionTypes.TACTION_FIRE_SALE:
                                case ActionTypes.TACTION_WIN:
                                case ActionTypes.TACTION_LOSE:
                                case ActionTypes.TACTION_ALL_HUNT:
                                case ActionTypes.TACTION_BEGIN_PRODUCTION:
                                case ActionTypes.TACTION_AUTOCREATE:
                                case ActionTypes.TACTION_BASE_BUILDING:
                                case ActionTypes.TACTION_CREATE_TEAM:
                                case ActionTypes.TACTION_DESTROY_TEAM:
                                case ActionTypes.TACTION_REINFORCEMENTS:
                                case ActionTypes.TACTION_FORCE_TRIGGER:
                                case ActionTypes.TACTION_DESTROY_TRIGGER:
                                case ActionTypes.TACTION_DZ:
                                case ActionTypes.TACTION_REVEAL_SOME:
                                case ActionTypes.TACTION_REVEAL_ZONE:
                                case ActionTypes.TACTION_PLAY_MUSIC:
                                case ActionTypes.TACTION_PLAY_MOVIE:
                                case ActionTypes.TACTION_PLAY_SOUND:
                                case ActionTypes.TACTION_PLAY_SPEECH:
                                case ActionTypes.TACTION_PREFERRED_TARGET:
                                    a.Data &= 0xFF;
                                    break;
                                case ActionTypes.TACTION_TEXT_TRIGGER:
                                    a.Data = Math.Max(1, Math.Min(209, a.Data));
                                    break;
                                default:
                                    break;
                            }
                        };

                        fixEvent(trigger.Event1);
                        fixEvent(trigger.Event2);

                        fixAction(trigger.Action1);
                        fixAction(trigger.Action2);

                        Map.Triggers.Add(trigger);
                    }
                    else
                    {
                        errors.Add(string.Format("Trigger '{0}' has too few tokens (expecting 18)", Key));
                    }
                }
            }

            var mapPackSection = ini.Sections.Extract("MapPack");
            if (mapPackSection != null)
            {
                Map.Templates.Clear();

                var data = DecompressLCWSection(mapPackSection, 3);
                using (var reader = new BinaryReader(new MemoryStream(data)))
                {
                    for (var y = 0; y < Map.Metrics.Height; ++y)
                    {
                        for (var x = 0; x < Map.Metrics.Width; ++x)
                        {
                            var typeValue = reader.ReadUInt16();
                            var templateType = Map.TemplateTypes.Where(t => t.Equals(typeValue)).FirstOrDefault();
                            if ((templateType != null) && !templateType.Theaters.Contains(Map.Theater))
                            {
                                templateType = null;
                            }
                            Map.Templates[x, y] = (templateType != null) ? new Template { Type = templateType } : null;
                        }
                    }

                    for (var y = 0; y < Map.Metrics.Height; ++y)
                    {
                        for (var x = 0; x < Map.Metrics.Width; ++x)
                        {
                            var iconValue = reader.ReadByte();
                            var template = Map.Templates[x, y];
                            if (template != null)
                            {
                                if ((template.Type != TemplateTypes.Clear) && (iconValue >= template.Type.NumIcons))
                                {
                                    Map.Templates[x, y] = null;
                                }
                                else
                                {
                                    template.Icon = iconValue;
                                }
                            }
                        }
                    }
                }
            }

            var terrainSection = ini.Sections.Extract("Terrain");
            if (terrainSection != null)
            {
                foreach (var (Key, Value) in terrainSection)
                {
                    var cell = int.Parse(Key);
                    var name = Value.Split(',')[0];
                    var terrainType = Map.TerrainTypes.Where(t => t.Equals(name)).FirstOrDefault();
                    if (terrainType != null)
                    {
                        if (!Map.Technos.Add(cell, new Terrain
                        {
                            Type = terrainType,
                            Icon = terrainType.IsTransformable ? 22 : 0,
                            Trigger = Trigger.None
                        }))
                        {
                            var techno = Map.Technos[cell];
                            if (techno is Building building)
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps structure '{1}' in cell {2}, skipping", name, building.Type.Name, cell));
                            }
                            else if (techno is Overlay overlay)
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps overlay '{1}' in cell {2}, skipping", name, overlay.Type.Name, cell));
                            }
                            else if (techno is Terrain terrain)
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps terrain '{1}' in cell {2}, skipping", name, terrain.Type.Name, cell));
                            }
                            else if (techno is InfantryGroup infantry)
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps infantry in cell {1}, skipping", name, cell));
                            }
                            else if (techno is Unit unit)
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps unit '{1}' in cell {2}, skipping", name, unit.Type.Name, cell));
                            }
                            else
                            {
                                errors.Add(string.Format("Terrain '{0}' overlaps unknown techno in cell {1}, skipping", name, cell));
                            }
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Terrain '{0}' references unknown terrain", name));
                    }
                }
            }

            var overlayPackSection = ini.Sections.Extract("OverlayPack");
            if (overlayPackSection != null)
            {
                Map.Overlay.Clear();

                var data = DecompressLCWSection(overlayPackSection, 1);
                using (var reader = new BinaryReader(new MemoryStream(data)))
                {
                    for (var i = 0; i < Map.Metrics.Length; ++i)
                    {
                        var overlayId = reader.ReadSByte();
                        if (overlayId >= 0)
                        {
                            var overlayType = Map.OverlayTypes.Where(t => t.Equals(overlayId)).FirstOrDefault();
                            if (overlayType != null)
                            {
                                Map.Overlay[i] = new Overlay { Type = overlayType, Icon = 0 };
                            }
                            else
                            {
                                errors.Add(string.Format("Overlay ID {0} references unknown overlay", overlayId));
                            }
                        }
                    }
                }
            }

            var smudgeSection = ini.Sections.Extract("Smudge");
            if (smudgeSection != null)
            {
                foreach (var (Key, Value) in smudgeSection)
                {
                    var cell = int.Parse(Key);
                    var tokens = Value.Split(',');
                    if (tokens.Length == 3)
                    {
                        var smudgeType = Map.SmudgeTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault();
                        if (smudgeType != null)
                        {
                            if ((smudgeType.Flag & SmudgeTypeFlag.Bib) == SmudgeTypeFlag.None)
                            {
                                Map.Smudge[cell] = new Smudge
                                {
                                    Type = smudgeType,
                                    Icon = 0,
                                    Data = int.Parse(tokens[2])
                                };
                            }
                            else
                            {
                                errors.Add(string.Format("Smudge '{0}' is a bib, skipped", tokens[0]));
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Smudge '{0}' references unknown smudge", tokens[0]));
                        }
                    }
                }
            }

            var unitsSection = ini.Sections.Extract("Units");
            if (unitsSection != null)
            {
                foreach (var (_, Value) in unitsSection)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length == 7)
                    {
                        var unitType = Map.UnitTypes.Where(t => t.IsUnit && t.Equals(tokens[1])).FirstOrDefault();
                        if (unitType != null)
                        {
                            var direction = (byte)((int.Parse(tokens[4]) + 0x08) & ~0x0F);

                            var cell = int.Parse(tokens[3]);
                            if (!Map.Technos.Add(cell, new Unit()
                            {
                                Type = unitType,
                                House = Map.HouseTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault(),
                                Strength = int.Parse(tokens[2]),
                                Direction = Map.DirectionTypes.Where(d => d.Equals(direction)).FirstOrDefault(),
                                Mission = tokens[5],
                                Trigger = tokens[6]
                            }))
                            {
                                var techno = Map.Technos[cell];
                                if (techno is Building building)
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps structure '{1}' in cell {2}, skipping", tokens[1], building.Type.Name, cell));
                                }
                                else if (techno is Overlay overlay)
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps overlay '{1}' in cell {2}, skipping", tokens[1], overlay.Type.Name, cell));
                                }
                                else if (techno is Terrain terrain)
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps terrain '{1}' in cell {2}, skipping", tokens[1], terrain.Type.Name, cell));
                                }
                                else if (techno is InfantryGroup infantry)
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps infantry in cell {1}, skipping", tokens[1], cell));
                                }
                                else if (techno is Unit unit)
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps unit '{1}' in cell {2}, skipping", tokens[1], unit.Type.Name, cell));
                                }
                                else
                                {
                                    errors.Add(string.Format("Unit '{0}' overlaps unknown techno in cell {1}, skipping", tokens[1], cell));
                                }
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Unit '{0}' references unknown unit", tokens[1]));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Unit '{0}' has wrong number of tokens (expecting 7)", tokens[1]));
                    }
                }
            }

            var aircraftSections = ini.Sections.Extract("Aircraft");
            if (aircraftSections != null)
            {
                foreach (var (_, Value) in aircraftSections)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length == 6)
                    {
                        var aircraftType = Map.UnitTypes.Where(t => t.IsAircraft && t.Equals(tokens[1])).FirstOrDefault();
                        if (aircraftType != null)
                        {
                            var direction = (byte)((int.Parse(tokens[4]) + 0x08) & ~0x0F);
                            var cell = int.Parse(tokens[3]);
                            if (!Map.Technos.Add(cell, new Unit()
                                {
                                    Type = aircraftType,
                                    House = Map.HouseTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault(),
                                    Strength = int.Parse(tokens[2]),
                                    Direction = Map.DirectionTypes.Where(d => d.Equals(direction)).FirstOrDefault(),
                                    Mission = tokens[5]
                                }))
                            {
                                var techno = Map.Technos[cell];
                                if (techno is Building building)
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps structure '{1}' in cell {2}, skipping", tokens[1], building.Type.Name, cell));
                                }
                                else if (techno is Overlay overlay)
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps overlay '{1}' in cell {2}, skipping", tokens[1], overlay.Type.Name, cell));
                                }
                                else if (techno is Terrain terrain)
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps terrain '{1}' in cell {2}, skipping", tokens[1], terrain.Type.Name, cell));
                                }
                                else if (techno is InfantryGroup infantry)
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps infantry in cell {1}, skipping", tokens[1], cell));
                                }
                                else if (techno is Unit unit)
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps unit '{1}' in cell {2}, skipping", tokens[1], unit.Type.Name, cell));
                                }
                                else
                                {
                                    errors.Add(string.Format("Aircraft '{0}' overlaps unknown techno in cell {1}, skipping", tokens[1], cell));
                                }
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Aircraft '{0}' references unknown aircraft", tokens[1]));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Aircraft '{0}' has wrong number of tokens (expecting 6)", tokens[1]));
                    }
                }
            }

            var shipsSection = ini.Sections.Extract("Ships");
            if (shipsSection != null)
            {
                foreach (var (_, Value) in shipsSection)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length == 7)
                    {
                        var vesselType = Map.UnitTypes.Where(t => t.IsVessel && t.Equals(tokens[1])).FirstOrDefault();
                        if (vesselType != null)
                        {
                            var direction = (byte)((int.Parse(tokens[4]) + 0x08) & ~0x0F);

                            var cell = int.Parse(tokens[3]);
                            if (!Map.Technos.Add(cell, new Unit()
                                {
                                    Type = vesselType,
                                    House = Map.HouseTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault(),
                                    Strength = int.Parse(tokens[2]),
                                    Direction = Map.DirectionTypes.Where(d => d.Equals(direction)).FirstOrDefault(),
                                    Mission = Map.MissionTypes.Where(t => t.Equals(tokens[5])).FirstOrDefault(),
                                    Trigger = tokens[6]
                                }))
                            {
                                var techno = Map.Technos[cell];
                                if (techno is Building building)
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps structure '{1}' in cell {2}, skipping", tokens[1], building.Type.Name, cell));
                                }
                                else if (techno is Overlay overlay)
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps overlay '{1}' in cell {2}, skipping", tokens[1], overlay.Type.Name, cell));
                                }
                                else if (techno is Terrain terrain)
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps terrain '{1}' in cell {2}, skipping", tokens[1], terrain.Type.Name, cell));
                                }
                                else if (techno is InfantryGroup infantry)
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps infantry in cell {1}, skipping", tokens[1], cell));
                                }
                                else if (techno is Unit unit)
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps unit '{1}' in cell {2}, skipping", tokens[1], unit.Type.Name, cell));
                                }
                                else
                                {
                                    errors.Add(string.Format("Ship '{0}' overlaps unknown techno in cell {1}, skipping", tokens[1], cell));
                                }
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Ship '{0}' references unknown ship", tokens[1]));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Ship '{0}' has wrong number of tokens (expecting 7)", tokens[1]));
                    }
                }
            }

            var infantrySections = ini.Sections.Extract("Infantry");
            if (infantrySections != null)
            {
                foreach (var (_, Value) in infantrySections)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length == 8)
                    {
                        var infantryType = Map.InfantryTypes.Where(t => t.Equals(tokens[1])).FirstOrDefault();
                        if (infantryType != null)
                        {
                            var cell = int.Parse(tokens[3]);
                            var infantryGroup = Map.Technos[cell] as InfantryGroup;
                            if ((infantryGroup == null) && (Map.Technos[cell] == null))
                            {
                                infantryGroup = new InfantryGroup();
                                Map.Technos.Add(cell, infantryGroup);
                            }

                            if (infantryGroup != null)
                            {
                                var stoppingPos = int.Parse(tokens[4]);
                                if (stoppingPos < Globals.NumInfantryStops)
                                {
                                    var direction = (byte)((int.Parse(tokens[6]) + 0x08) & ~0x0F);

                                    if (infantryGroup.Infantry[stoppingPos] == null)
                                    {
                                        infantryGroup.Infantry[stoppingPos] = new Infantry(infantryGroup)
                                        {
                                            Type = infantryType,
                                            House = Map.HouseTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault(),
                                            Strength = int.Parse(tokens[2]),
                                            Direction = Map.DirectionTypes.Where(d => d.Equals(direction)).FirstOrDefault(),
                                            Mission = Map.MissionTypes.Where(t => t.Equals(tokens[5])).FirstOrDefault(),
                                            Trigger = tokens[7]
                                        };
                                    }
                                    else
                                    {
                                        errors.Add(string.Format("Infantry '{0}' overlaps another infantry at position {1} in cell {2}, skipping", tokens[1], stoppingPos, cell));
                                    }
                                }
                                else
                                {
                                    errors.Add(string.Format("Infantry '{0}' has invalid position {1} in cell {2}, skipping", tokens[1], stoppingPos, cell));
                                }
                            }
                            else
                            {
                                var techno = Map.Technos[cell];
                                if (techno is Building building)
                                {
                                    errors.Add(string.Format("Infantry '{0}' overlaps structure '{1}' in cell {2}, skipping", tokens[1], building.Type.Name, cell));
                                }
                                else if (techno is Overlay overlay)
                                {
                                    errors.Add(string.Format("Infantry '{0}' overlaps overlay '{1}' in cell {2}, skipping", tokens[1], overlay.Type.Name, cell));
                                }
                                else if (techno is Terrain terrain)
                                {
                                    errors.Add(string.Format("Infantry '{0}' overlaps terrain '{1}' in cell {2}, skipping", tokens[1], terrain.Type.Name, cell));
                                }
                                else if (techno is Unit unit)
                                {
                                    errors.Add(string.Format("Infantry '{0}' overlaps unit '{1}' in cell {2}, skipping", tokens[1], unit.Type.Name, cell));
                                }
                                else
                                {
                                    errors.Add(string.Format("Infantry '{0}' overlaps unknown techno in cell {1}, skipping", tokens[1], cell));
                                }
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Infantry '{0}' references unknown infantry", tokens[1]));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Infantry '{0}' has wrong number of tokens (expecting 8)", tokens[1]));
                    }
                }
            }

            var structuresSection = ini.Sections.Extract("Structures");
            if (structuresSection != null)
            {
                foreach (var (_, Value) in structuresSection)
                {
                    var tokens = Value.Split(',');
                    if (tokens.Length >= 6)
                    {
                        var buildingType = Map.BuildingTypes.Where(t => t.Equals(tokens[1])).FirstOrDefault();
                        if (buildingType != null)
                        {
                            var direction = (byte)((int.Parse(tokens[4]) + 0x08) & ~0x0F);
                            bool sellable = (tokens.Length >= 7) ? (int.Parse(tokens[6]) != 0) : false;
                            bool rebuild = (tokens.Length >= 8) ? (int.Parse(tokens[7]) != 0) : false;

                            var cell = int.Parse(tokens[3]);
                            if (!Map.Buildings.Add(cell, new Building()
                                {
                                    Type = buildingType,
                                    House = Map.HouseTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault(),
                                    Strength = int.Parse(tokens[2]),
                                    Direction = Map.DirectionTypes.Where(d => d.Equals(direction)).FirstOrDefault(),
                                    Trigger = tokens[5],
                                    Sellable = sellable,
                                    Rebuild = rebuild
                                }))
                            {
                                var techno = Map.Technos[cell];
                                if (techno is Building building)
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps structure '{1}' in cell {2}, skipping", tokens[1], building.Type.Name, cell));
                                }
                                else if (techno is Overlay overlay)
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps overlay '{1}' in cell {2}, skipping", tokens[1], overlay.Type.Name, cell));
                                }
                                else if (techno is Terrain terrain)
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps terrain '{1}' in cell {2}, skipping", tokens[1], terrain.Type.Name, cell));
                                }
                                else if (techno is InfantryGroup infantry)
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps infantry in cell {1}, skipping", tokens[1], cell));
                                }
                                else if (techno is Unit unit)
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps unit '{1}' in cell {2}, skipping", tokens[1], unit.Type.Name, cell));
                                }
                                else
                                {
                                    errors.Add(string.Format("Structure '{0}' overlaps unknown techno in cell {1}, skipping", tokens[1], cell));
                                }
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Structure '{0}' references unknown structure", tokens[1]));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Structure '{0}' has wrong number of tokens (expecting 6)", tokens[1]));
                    }
                }
            }

            var baseSection = ini.Sections.Extract("Base");
            if (baseSection != null)
            {
                foreach (var (Key, Value) in baseSection)
                {
                    if (Key.Equals("Player", StringComparison.OrdinalIgnoreCase))
                    {
                        Map.BasicSection.BasePlayer = Map.HouseTypes.Where(t => t.Equals(Value)).FirstOrDefault()?.Name ?? Map.HouseTypes.First().Name;
                    }
                    else if (int.TryParse(Key, out int priority))
                    {
                        var tokens = Value.Split(',');
                        if (tokens.Length == 2)
                        {
                            var buildingType = Map.BuildingTypes.Where(t => t.Equals(tokens[0])).FirstOrDefault();
                            if (buildingType != null)
                            {
                                var cell = int.Parse(tokens[1]);
                                Map.Metrics.GetLocation(cell, out Point location);
                                if (Map.Buildings.OfType<Building>().Where(x => x.Location == location).FirstOrDefault().Occupier is Building building)
                                {
                                    building.BasePriority = priority;
                                }
                                else
                                {
                                    Map.Buildings.Add(cell, new Building()
                                    {
                                        Type = buildingType,
                                        Strength = 256,
                                        Direction = DirectionTypes.North,
                                        BasePriority = priority,
                                        IsPrebuilt = false
                                    });
                                }
                            }
                            else
                            {
                                errors.Add(string.Format("Base priority {0} references unknown structure '{1}'", priority, tokens[0]));
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Base priority {0} has wrong number of tokens (expecting 2)", priority));
                        }
                    }
                    else if (!Key.Equals("Count", StringComparison.CurrentCultureIgnoreCase))
                    {
                        errors.Add(string.Format("Invalid base priority '{0}' (expecting integer)", Key));
                    }
                }
            }

            var waypointsSection = ini.Sections.Extract("Waypoints");
            if (waypointsSection != null)
            {
                foreach (var (Key, Value) in waypointsSection)
                {
                    if (int.TryParse(Key, out int waypoint))
                    {
                        if (int.TryParse(Value, out int cell))
                        {
                            if ((waypoint >= 0) && (waypoint < Map.Waypoints.Length))
                            {
                                if (Map.Metrics.Contains(cell))
                                {
                                    Map.Waypoints[waypoint].Cell = cell;
                                }
                                else
                                {
                                    Map.Waypoints[waypoint].Cell = null;
                                    if (cell != -1)
                                    {
                                        errors.Add(string.Format("Waypoint {0} cell value {1} out of range (expecting between {2} and {3})", waypoint, cell, 0, Map.Metrics.Length - 1));
                                    }
                                }
                            }
                            else if (cell != -1)
                            {
                                errors.Add(string.Format("Waypoint {0} out of range (expecting between {1} and {2})", waypoint, 0, Map.Waypoints.Length - 1));
                            }
                        }
                        else
                        {
                            errors.Add(string.Format("Waypoint {0} has invalid cell '{1}' (expecting integer)", waypoint, Value));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Invalid waypoint '{0}' (expecting integer)", Key));
                    }
                }
            }

            var cellTriggersSection = ini.Sections.Extract("CellTriggers");
            if (cellTriggersSection != null)
            {
                foreach (var (Key, Value) in cellTriggersSection)
                {
                    if (int.TryParse(Key, out int cell))
                    {
                        if (Map.Metrics.Contains(cell))
                        {
                            Map.CellTriggers[cell] = new CellTrigger
                            {
                                Trigger = Value
                            };
                        }
                        else
                        {
                            errors.Add(string.Format("Cell trigger {0} outside map bounds", cell));
                        }
                    }
                    else
                    {
                        errors.Add(string.Format("Invalid cell trigger '{0}' (expecting integer)", Key));
                    }
                }
            }

            var briefingSection = ini.Sections.Extract("Briefing");
            if (briefingSection != null)
            {
                if (briefingSection.Keys.Contains("Text"))
                {
                    Map.BriefingSection.Briefing = briefingSection["Text"].Replace("@", Environment.NewLine);
                }
                else
                {
                    Map.BriefingSection.Briefing = string.Join(" ", briefingSection.Keys.Select(k => k.Value)).Replace("@", Environment.NewLine);
                }
            }

            foreach (var house in Map.Houses)
            {
                if (house.Type.ID < 0)
                {
                    continue;
                }

                var houseSection = ini.Sections.Extract(house.Type.Name);
                if (houseSection != null)
                {
                    INI.ParseSection(new MapContext(Map, true), houseSection, house);
                    house.Enabled = true;
                }
                else
                {
                    house.Enabled = false;
                }
            }

            string indexToName<T>(IList<T> list, string index, string defaultValue) where T : INamedType
            {
                return (int.TryParse(index, out int result) && (result >= 0) && (result < list.Count)) ? list[result].Name : defaultValue;
            }

            foreach (var teamType in Map.TeamTypes)
            {
                teamType.Trigger = indexToName(Map.Triggers, teamType.Trigger, Trigger.None);
            }

            foreach (var trigger in Map.Triggers)
            {
                trigger.Event1.Team = indexToName(Map.TeamTypes, trigger.Event1.Team, TeamType.None);
                trigger.Event2.Team = indexToName(Map.TeamTypes, trigger.Event2.Team, TeamType.None);
                trigger.Action1.Team = indexToName(Map.TeamTypes, trigger.Action1.Team, TeamType.None);
                trigger.Action1.Trigger = indexToName(Map.Triggers, trigger.Action1.Trigger, Trigger.None);
                trigger.Action2.Team = indexToName(Map.TeamTypes, trigger.Action2.Team, TeamType.None);
                trigger.Action2.Trigger = indexToName(Map.Triggers, trigger.Action2.Trigger, Trigger.None);
            }

            UpdateBasePlayerHouse();

            extraSections = ini.Sections;

            Map.EndUpdate();

            return errors;
        }

        public bool Save(string path, FileType fileType)
        {
            if (!Validate())
            {
                return false;
            }

            switch (fileType)
            {
                case FileType.INI:
                case FileType.BIN:
                    {
                        var mprPath = Path.ChangeExtension(path, ".mpr");
                        var tgaPath = Path.ChangeExtension(path, ".tga");
                        var jsonPath = Path.ChangeExtension(path, ".json");

                        var ini = new INI();
                        using (var mprWriter = new StreamWriter(mprPath))
                        using (var tgaStream = new FileStream(tgaPath, FileMode.Create))
                        using (var jsonStream = new FileStream(jsonPath, FileMode.Create))
                        using (var jsonWriter = new JsonTextWriter(new StreamWriter(jsonStream)))
                        {
                            SaveINI(ini, fileType);
                            mprWriter.Write(ini.ToString());
                            SaveMapPreview(tgaStream);
                            SaveJSON(jsonWriter);
                        }
                    }
                    break;
                case FileType.MEG:
                case FileType.PGM:
                    {
                        using (var iniStream = new MemoryStream())
                        using (var tgaStream = new MemoryStream())
                        using (var jsonStream = new MemoryStream())
                        using (var jsonWriter = new JsonTextWriter(new StreamWriter(jsonStream)))
                        using (var megafileBuilder = new MegafileBuilder(@"", path))
                        {
                            var ini = new INI();
                            SaveINI(ini, fileType);
                            var iniWriter = new StreamWriter(iniStream);
                            iniWriter.Write(ini.ToString());
                            iniWriter.Flush();
                            iniStream.Position = 0;

                            SaveMapPreview(tgaStream);
                            tgaStream.Position = 0;

                            SaveJSON(jsonWriter);
                            jsonWriter.Flush();
                            jsonStream.Position = 0;

                            var mprFile = Path.ChangeExtension(Path.GetFileName(path), ".mpr").ToUpper();
                            var tgaFile = Path.ChangeExtension(Path.GetFileName(path), ".tga").ToUpper();
                            var jsonFile = Path.ChangeExtension(Path.GetFileName(path), ".json").ToUpper();

                            megafileBuilder.AddFile(mprFile, iniStream);
                            megafileBuilder.AddFile(tgaFile, tgaStream);
                            megafileBuilder.AddFile(jsonFile, jsonStream);
                            megafileBuilder.Write();
                        }
                    }
                    break;
                default:
                    throw new NotSupportedException();
            }

            return true;
        }

        private void SaveINI(INI ini, FileType fileType)
        {
            if (extraSections != null)
            {
                ini.Sections.AddRange(extraSections);
            }

            INI.WriteSection(new MapContext(Map, false), ini.Sections.Add("Basic"), Map.BasicSection);
            INI.WriteSection(new MapContext(Map, false), ini.Sections.Add("Map"), Map.MapSection);

            if (fileType != FileType.PGM)
            {
                INI.WriteSection(new MapContext(Map, false), ini.Sections.Add("Steam"), Map.SteamSection);
            }

            ini["Basic"]["NewINIFormat"] = "3";

            var smudgeSection = ini.Sections.Add("SMUDGE");
            foreach (var (cell, smudge) in Map.Smudge.Where(item => (item.Value.Type.Flag & SmudgeTypeFlag.Bib) == SmudgeTypeFlag.None))
            {
                smudgeSection[cell.ToString()] = string.Format("{0},{1},{2}", smudge.Type.Name.ToUpper(), cell, smudge.Data);
            }

            var terrainSection = ini.Sections.Add("TERRAIN");
            foreach (var (location, terrain) in Map.Technos.OfType<Terrain>())
            {
                Map.Metrics.GetCell(location, out int cell);
                terrainSection[cell.ToString()] = terrain.Type.Name.ToUpper();
            }

            var cellTriggersSection = ini.Sections.Add("CellTriggers");
            foreach (var (cell, cellTrigger) in Map.CellTriggers)
            {
                cellTriggersSection[cell.ToString()] = cellTrigger.Trigger;
            }

            int nameToIndex<T>(IList<T> list, string name)
            {
                var index = list.TakeWhile(x => !x.Equals(name)).Count();
                return (index < list.Count) ? index : -1;
            }

            string nameToIndexString<T>(IList<T> list, string name) => nameToIndex(list, name).ToString();

            var teamTypesSection = ini.Sections.Add("TeamTypes");
            foreach (var teamType in Map.TeamTypes)
            {
                var classes = teamType.Classes
                    .Select(c => string.Format("{0}:{1}", c.Type.Name.ToUpper(), c.Count))
                    .ToArray();
                var missions = teamType.Missions
                    .Select(m => string.Format("{0}:{1}", nameToIndexString(Map.TeamMissionTypes, m.Mission), m.Argument))
                    .ToArray();

                int flags = 0;
                if (teamType.IsRoundAbout) flags |= 0x01;
                if (teamType.IsSuicide) flags |= 0x02;
                if (teamType.IsAutocreate) flags |= 0x04;
                if (teamType.IsPrebuilt) flags |= 0x08;
                if (teamType.IsReinforcable) flags |= 0x10;

                var tokens = new List<string>
                {
                    teamType.House.ID.ToString(),
                    flags.ToString(),
                    teamType.RecruitPriority.ToString(),
                    teamType.InitNum.ToString(),
                    teamType.MaxAllowed.ToString(),
                    (teamType.Origin - 1).ToString(),
                    nameToIndexString(Map.Triggers, teamType.Trigger),
                    classes.Length.ToString(),
                    string.Join(",", classes),
                    missions.Length.ToString(),
                    string.Join(",", missions)
                };

                teamTypesSection[teamType.Name] = string.Join(",", tokens.Where(t => !string.IsNullOrEmpty(t)));
            }

            var infantrySection = ini.Sections.Add("INFANTRY");
            var infantryIndex = 0;
            foreach (var (location, infantryGroup) in Map.Technos.OfType<InfantryGroup>())
            {
                for (var i = 0; i < infantryGroup.Infantry.Length; ++i)
                {
                    var infantry = infantryGroup.Infantry[i];
                    if (infantry == null)
                    {
                        continue;
                    }

                    var key = infantryIndex.ToString("D3");
                    infantryIndex++;

                    Map.Metrics.GetCell(location, out int cell);
                    infantrySection[key] = string.Format("{0},{1},{2},{3},{4},{5},{6},{7}",
                        infantry.House.Name,
                        infantry.Type.Name,
                        infantry.Strength,
                        cell,
                        i,
                        infantry.Mission,
                        infantry.Direction.ID,
                        infantry.Trigger
                    );
                }
            }

            var structuresSection = ini.Sections.Add("STRUCTURES");
            var structureIndex = 0;
            foreach (var (location, building) in Map.Buildings.OfType<Building>().Where(x => x.Occupier.IsPrebuilt))
            {
                var key = structureIndex.ToString("D3");
                structureIndex++;

                Map.Metrics.GetCell(location, out int cell);
                structuresSection[key] = string.Format("{0},{1},{2},{3},{4},{5},{6},{7}",
                    building.House.Name,
                    building.Type.Name,
                    building.Strength,
                    cell,
                    building.Direction.ID,
                    building.Trigger,
                    building.Sellable ? 1 : 0,
                    building.Rebuild ? 1 : 0
                );
            }

            var baseSection = ini.Sections.Add("Base");
            var baseBuildings = Map.Buildings.OfType<Building>().Where(x => x.Occupier.BasePriority >= 0).OrderBy(x => x.Occupier.BasePriority).ToArray();
            baseSection["Player"] = Map.BasicSection.BasePlayer;
            baseSection["Count"] = baseBuildings.Length.ToString();
            var baseIndex = 0;
            foreach (var (location, building) in baseBuildings)
            {
                var key = baseIndex.ToString("D3");
                baseIndex++;

                Map.Metrics.GetCell(location, out int cell);
                baseSection[key] = string.Format("{0},{1}",
                    building.Type.Name.ToUpper(),
                    cell
                );
            }

            var unitsSection = ini.Sections.Add("UNITS");
            var unitIndex = 0;
            foreach (var (location, unit) in Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsUnit))
            {
                var key = unitIndex.ToString("D3");
                unitIndex++;

                Map.Metrics.GetCell(location, out int cell);
                unitsSection[key] = string.Format("{0},{1},{2},{3},{4},{5},{6}",
                    unit.House.Name,
                    unit.Type.Name,
                    unit.Strength,
                    cell,
                    unit.Direction.ID,
                    unit.Mission,
                    unit.Trigger
                );
            }

            var aircraftSection = ini.Sections.Add("AIRCRAFT");
            var aircraftIndex = 0;
            foreach (var (location, aircraft) in Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsAircraft))
            {
                var key = aircraftIndex.ToString("D3");
                aircraftIndex++;

                Map.Metrics.GetCell(location, out int cell);
                aircraftSection[key] = string.Format("{0},{1},{2},{3},{4},{5}",
                    aircraft.House.Name,
                    aircraft.Type.Name,
                    aircraft.Strength,
                    cell,
                    aircraft.Direction.ID,
                    aircraft.Mission
                );
            }

            var shipsSection = ini.Sections.Add("SHIPS");
            var shipsIndex = 0;
            foreach (var (location, ship) in Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsVessel))
            {
                var key = shipsIndex.ToString("D3");
                shipsIndex++;

                Map.Metrics.GetCell(location, out int cell);
                shipsSection[key] = string.Format("{0},{1},{2},{3},{4},{5},{6}",
                    ship.House.Name,
                    ship.Type.Name,
                    ship.Strength,
                    cell,
                    ship.Direction.ID,
                    ship.Mission,
                    ship.Trigger
                );
            }

            var triggersSection = ini.Sections.Add("Trigs");
            foreach (var trigger in Map.Triggers)
            {
                if (string.IsNullOrEmpty(trigger.Name))
                {
                    continue;
                }

                var action2TypeIndex = nameToIndex(Map.ActionTypes, trigger.Action2.ActionType);
                var actionControl = (action2TypeIndex > 0) ? TriggerMultiStyleType.And : TriggerMultiStyleType.Only;

                var tokens = new List<string>
                {
                    ((int)trigger.PersistantType).ToString(),
                    !string.IsNullOrEmpty(trigger.House) ? (Map.HouseTypes.Where(h => h.Equals(trigger.House)).FirstOrDefault()?.ID.ToString() ?? "-1") : "-1",
                    ((int)trigger.EventControl).ToString(),
                    ((int)actionControl).ToString(),
                    nameToIndexString(Map.EventTypes, trigger.Event1.EventType),
                    nameToIndexString(Map.TeamTypes, trigger.Event1.Team),
                    trigger.Event1.Data.ToString(),
                    nameToIndexString(Map.EventTypes, trigger.Event2.EventType),
                    nameToIndexString(Map.TeamTypes, trigger.Event2.Team),
                    trigger.Event2.Data.ToString(),
                    nameToIndexString(Map.ActionTypes, trigger.Action1.ActionType),
                    nameToIndexString(Map.TeamTypes, trigger.Action1.Team),
                    nameToIndexString(Map.Triggers, trigger.Action1.Trigger),
                    trigger.Action1.Data.ToString(),
                    action2TypeIndex.ToString(),
                    nameToIndexString(Map.TeamTypes, trigger.Action2.Team),
                    nameToIndexString(Map.Triggers, trigger.Action2.Trigger),
                    trigger.Action2.Data.ToString()
                };

                triggersSection[trigger.Name] = string.Join(",", tokens);
            }

            var waypointsSection = ini.Sections.Add("Waypoints");
            for (var i = 0; i < Map.Waypoints.Length; ++i)
            {
                var waypoint = Map.Waypoints[i];
                if (waypoint.Cell.HasValue)
                {
                    waypointsSection[i.ToString()] = waypoint.Cell.Value.ToString();
                }
            }

            foreach (var house in Map.Houses)
            {
                if ((house.Type.ID < 0) || !house.Enabled)
                {
                    continue;
                }

                INI.WriteSection(new MapContext(Map, true), ini.Sections.Add(house.Type.Name), house);
            }

            ini.Sections.Remove("Briefing");
            if (!string.IsNullOrEmpty(Map.BriefingSection.Briefing))
            {
                var briefingSection = ini.Sections.Add("Briefing");
                briefingSection["Text"] = Map.BriefingSection.Briefing.Replace(Environment.NewLine, "@");
            }

            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    for (var y = 0; y < Map.Metrics.Height; ++y)
                    {
                        for (var x = 0; x < Map.Metrics.Width; ++x)
                        {
                            var template = Map.Templates[x, y];
                            if (template != null)
                            {
                                writer.Write(template.Type.ID);
                            }
                            else
                            {
                                writer.Write(ushort.MaxValue);
                            }
                        }
                    }

                    for (var y = 0; y < Map.Metrics.Height; ++y)
                    {
                        for (var x = 0; x < Map.Metrics.Width; ++x)
                        {
                            var template = Map.Templates[x, y];
                            if (template != null)
                            {
                                writer.Write((byte)template.Icon);
                            }
                            else
                            {
                                writer.Write(byte.MaxValue);
                            }
                        }
                    }
                }

                ini.Sections.Remove("MapPack");
                CompressLCWSection(ini.Sections.Add("MapPack"), stream.ToArray());
            }

            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    for (var i = 0; i < Map.Metrics.Length; ++i)
                    {
                        var overlay = Map.Overlay[i];
                        if (overlay != null)
                        {
                            writer.Write(overlay.Type.ID);
                        }
                        else
                        {
                            writer.Write((sbyte)-1);
                        }
                    }
                }

                ini.Sections.Remove("OverlayPack");
                CompressLCWSection(ini.Sections.Add("OverlayPack"), stream.ToArray());
            }
        }

        private void SaveMapPreview(Stream stream)
        {
            Map.GenerateMapPreview().Save(stream);
        }

        private void SaveJSON(JsonTextWriter writer)
        {
            writer.WriteStartObject();
            writer.WritePropertyName("MapTileX");
            writer.WriteValue(Map.MapSection.X);
            writer.WritePropertyName("MapTileY");
            writer.WriteValue(Map.MapSection.Y);
            writer.WritePropertyName("MapTileWidth");
            writer.WriteValue(Map.MapSection.Width);
            writer.WritePropertyName("MapTileHeight");
            writer.WriteValue(Map.MapSection.Height);
            writer.WritePropertyName("Theater");
            writer.WriteValue(Map.MapSection.Theater.Name.ToUpper());
            writer.WritePropertyName("Waypoints");
            writer.WriteStartArray();
            foreach (var waypoint in Map.Waypoints.Where(w => (w.Flag == WaypointFlag.PlayerStart) && w.Cell.HasValue))
            {
                writer.WriteValue(waypoint.Cell.Value);
            }
            writer.WriteEndArray();
            writer.WriteEndObject();
        }

        private bool Validate()
        {
            StringBuilder sb = new StringBuilder("Error(s) during map validation:");

            bool ok = true;
            int numAircraft = Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsAircraft).Count();
            int numBuildings = Map.Buildings.OfType<Building>().Where(x => x.Occupier.IsPrebuilt).Count();
            int numInfantry = Map.Technos.OfType<InfantryGroup>().Sum(item => item.Occupier.Infantry.Count(i => i != null));
            int numTerrain = Map.Technos.OfType<Terrain>().Count();
            int numUnits = Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsUnit).Count();
            int numVessels = Map.Technos.OfType<Unit>().Where(u => u.Occupier.Type.IsVessel).Count();
            int numWaypoints = Map.Waypoints.Count(w => w.Cell.HasValue);

            if (numAircraft > Constants.MaxAircraft)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of aircraft exceeded ({0} > {1})", numAircraft, Constants.MaxAircraft));
                ok = false;
            }

            if (numBuildings > Constants.MaxBuildings)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of structures exceeded ({0} > {1})", numBuildings, Constants.MaxBuildings));
                ok = false;
            }

            if (numInfantry > Constants.MaxInfantry)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of infantry exceeded ({0} > {1})", numInfantry, Constants.MaxInfantry));
                ok = false;
            }

            if (numTerrain > Constants.MaxTerrain)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of terrain objects exceeded ({0} > {1})", numTerrain, Constants.MaxTerrain));
                ok = false;
            }

            if (numUnits > Constants.MaxUnits)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of units exceeded ({0} > {1})", numUnits, Constants.MaxUnits));
                ok = false;
            }

            if (numVessels > Constants.MaxVessels)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of ships exceeded ({0} > {1})", numVessels, Constants.MaxVessels));
                ok = false;
            }

            if (Map.TeamTypes.Count > Constants.MaxTeams)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of team types exceeded ({0} > {1})", Map.TeamTypes.Count, Constants.MaxTeams));
                ok = false;
            }

            if (Map.Triggers.Count > Constants.MaxTriggers)
            {
                sb.Append(Environment.NewLine + string.Format("Maximum number of triggers exceeded ({0} > {1})", Map.Triggers.Count, Constants.MaxTriggers));
                ok = false;
            }

            if (!Map.BasicSection.SoloMission && (numWaypoints < 2))
            {
                sb.Append(Environment.NewLine + "Skirmish/Multiplayer maps need at least 2 waypoints for player starting locations.");
                ok = false;
            }

            var homeWaypoint = Map.Waypoints.Where(w => w.Equals("Home")).FirstOrDefault();
            if (Map.BasicSection.SoloMission && !homeWaypoint.Cell.HasValue)
            {
                sb.Append(Environment.NewLine + string.Format("Single-player maps need the Home waypoint to be placed.", Map.Triggers.Count, Constants.MaxTriggers));
                ok = false;
            }

            if (!ok)
            {
                MessageBox.Show(sb.ToString(), "Validation Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return ok;
        }

        private void BasicSection_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "BasePlayer":
                    {
                        UpdateBasePlayerHouse();
                    }
                    break;
            }
        }

        private void MapSection_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "Theater":
                    {
                        Map.InitTheater(GameType);
                    }
                    break;
            }
        }

        private void UpdateBasePlayerHouse()
        {
            var basePlayer = Map.HouseTypes.Where(h => h.Equals(Map.BasicSection.BasePlayer)).FirstOrDefault() ?? Map.HouseTypes.First();
            foreach (var (_, building) in Map.Buildings.OfType<Building>())
            {
                if (!building.IsPrebuilt)
                {
                    building.House = basePlayer;
                }
            }
        }

        private void CompressLCWSection(INISection section, byte[] decompressedBytes)
        {
            using (var stream = new MemoryStream())
            using (var writer = new BinaryWriter(stream))
            {
                foreach (var decompressedChunk in decompressedBytes.Split(8192))
                {
                    var compressedChunk = WWCompression.LcwCompress(decompressedChunk);
                    writer.Write((ushort)compressedChunk.Length);
                    writer.Write((ushort)decompressedChunk.Length);
                    writer.Write(compressedChunk);
                }

                writer.Flush();
                stream.Position = 0;

                var values = Convert.ToBase64String(stream.ToArray()).Split(70).ToArray();
                for (var i = 0; i < values.Length; ++i)
                {
                    section[(i + 1).ToString()] = values[i];
                }
            }
        }

        private byte[] DecompressLCWSection(INISection section, int bytesPerCell)
        {
            var sb = new StringBuilder();
            foreach (var (key, value) in section)
            {
                sb.Append(value);
            }

            var compressedBytes = Convert.FromBase64String(sb.ToString());
            int readPtr = 0;
            int writePtr = 0;
            var decompressedBytes = new byte[Map.Metrics.Width * Map.Metrics.Height * bytesPerCell];

            while ((readPtr + 4) <= compressedBytes.Length)
            {
                uint uLength;
                using (var reader = new BinaryReader(new MemoryStream(compressedBytes, readPtr, 4)))
                {
                    uLength = reader.ReadUInt32();
                }
                var length = (int)(uLength & 0x0000FFFF);
                readPtr += 4;
                var dest = new byte[8192];
                var readPtr2 = readPtr;
                var decompressed = WWCompression.LcwDecompress(compressedBytes, ref readPtr2, dest, 0);
                Array.Copy(dest, 0, decompressedBytes, writePtr, decompressed);
                readPtr += length;
                writePtr += decompressed;
            }
            return decompressedBytes;
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    MapImage?.Dispose();
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}
