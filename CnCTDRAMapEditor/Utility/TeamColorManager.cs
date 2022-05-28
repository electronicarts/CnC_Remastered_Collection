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
using System.Linq;
using System.Xml;

namespace MobiusEditor.Utility
{
    public class TeamColorManager
    {
        private readonly Dictionary<string, TeamColor> teamColors = new Dictionary<string, TeamColor>();

        private readonly MegafileManager megafileManager;

        public TeamColor this[string key] => !string.IsNullOrEmpty(key) ? teamColors[key] : null;

        public TeamColorManager(MegafileManager megafileManager)
        {
            this.megafileManager = megafileManager;
        }

        public void Reset()
        {
            teamColors.Clear();
        }

        public void Load(string xmlPath)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(megafileManager.Open(xmlPath));

            foreach (XmlNode teamColorNode in xmlDoc.SelectNodes("/*/TeamColorTypeClass"))
            {
                var teamColor = new TeamColor(this, megafileManager);
                teamColor.Load(teamColorNode.OuterXml);

                teamColors[teamColorNode.Attributes["Name"].Value] = teamColor;
            }

            foreach (var teamColor in TopologicalSortTeamColors())
            {
                teamColor.Flatten();
            }
        }

        private IEnumerable<TeamColor> TopologicalSortTeamColors()
        {
            var nodes = teamColors.Values.ToList();
            HashSet<(TeamColor, TeamColor)> edges = new HashSet<(TeamColor, TeamColor)>();
            foreach (var node in nodes)
            {
                if (!string.IsNullOrEmpty(node.Variant))
                {
                    edges.Add((this[node.Variant], node));
                }
            }

            var sorted = new List<TeamColor>();
            var openSet = new HashSet<TeamColor>(nodes.Where(n => edges.All(e => !e.Item2.Equals(n))));
            while (openSet.Count > 0)
            {
                var node = openSet.First();
                openSet.Remove(node);
                sorted.Add(node);

                foreach (var edge in edges.Where(e => e.Item1.Equals(node)).ToArray())
                {
                    var node2 = edge.Item2;
                    edges.Remove(edge);

                    if (edges.All(edge2 => !edge2.Item2.Equals(node2)))
                    {
                        openSet.Add(node2);
                    }
                }
            }

            return (edges.Count == 0) ? sorted : null;
        }
    }
}
