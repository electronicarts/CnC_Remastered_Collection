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
using System.Drawing;
using System.Numerics;
using System.Xml;

namespace MobiusEditor.Utility
{
    public class TeamColor
    {
        private readonly TeamColorManager teamColorManager;
        private readonly MegafileManager megafileManager;

        public string Variant { get; private set; }

        public string Name { get; private set; }

        private Color? lowerBounds;
        public Color LowerBounds => lowerBounds.HasValue ? lowerBounds.Value : ((Variant != null) ? teamColorManager[Variant].LowerBounds : default);

        private Color? upperBounds;
        public Color UpperBounds => upperBounds.HasValue ? upperBounds.Value : ((Variant != null) ? teamColorManager[Variant].UpperBounds : default);

        private float? fudge;
        public float Fudge => fudge.HasValue ? fudge.Value : ((Variant != null) ? teamColorManager[Variant].Fudge : default);

        private Vector3? hsvShift;
        public Vector3 HSVShift => hsvShift.HasValue ? hsvShift.Value : ((Variant != null) ? teamColorManager[Variant].HSVShift : default);

        private Vector3? inputLevels;
        public Vector3 InputLevels => inputLevels.HasValue ? inputLevels.Value : ((Variant != null) ? teamColorManager[Variant].InputLevels : default);

        private Vector2? outputLevels;
        public Vector2 OutputLevels => outputLevels.HasValue ? outputLevels.Value : ((Variant != null) ? teamColorManager[Variant].OutputLevels : default);

        private Vector3? overallInputLevels;
        public Vector3 OverallInputLevels => overallInputLevels.HasValue ? overallInputLevels.Value : ((Variant != null) ? teamColorManager[Variant].OverallInputLevels : default);

        private Vector2? overallOutputLevels;
        public Vector2 OverallOutputLevels => overallOutputLevels.HasValue ? overallOutputLevels.Value : ((Variant != null) ? teamColorManager[Variant].OverallOutputLevels : default);

        private Color? radarMapColor;
        public Color RadarMapColor => radarMapColor.HasValue ? radarMapColor.Value : ((Variant != null) ? teamColorManager[Variant].RadarMapColor : default);

        public TeamColor(TeamColorManager teamColorManager, MegafileManager megafileManager)
        {
            this.teamColorManager = teamColorManager;
            this.megafileManager = megafileManager;
        }

        public void Load(string xml)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(xml);

            var node = xmlDoc.FirstChild;
            Name = node.Attributes["Name"].Value;
            Variant = node.Attributes["Variant"]?.Value;

            var lowerBoundsNode = node.SelectSingleNode("LowerBounds");
            if (lowerBoundsNode != null)
            {
                lowerBounds = Color.FromArgb(
                    (int)(float.Parse(lowerBoundsNode.SelectSingleNode("R").InnerText) * 255),
                    (int)(float.Parse(lowerBoundsNode.SelectSingleNode("G").InnerText) * 255),
                    (int)(float.Parse(lowerBoundsNode.SelectSingleNode("B").InnerText) * 255)
                );
            }

            var upperBoundsNode = node.SelectSingleNode("UpperBounds");
            if (upperBoundsNode != null)
            {
                upperBounds = Color.FromArgb(
                    (int)(float.Parse(upperBoundsNode.SelectSingleNode("R").InnerText) * 255),
                    (int)(float.Parse(upperBoundsNode.SelectSingleNode("G").InnerText) * 255),
                    (int)(float.Parse(upperBoundsNode.SelectSingleNode("B").InnerText) * 255)
                );
            }

            var fudgeNode = node.SelectSingleNode("Fudge");
            if (fudgeNode != null)
            {
                fudge = float.Parse(fudgeNode.InnerText);
            }

            var hsvShiftNode = node.SelectSingleNode("HSVShift");
            if (hsvShiftNode != null)
            {
                hsvShift = new Vector3(
                    float.Parse(hsvShiftNode.SelectSingleNode("X").InnerText),
                    float.Parse(hsvShiftNode.SelectSingleNode("Y").InnerText),
                    float.Parse(hsvShiftNode.SelectSingleNode("Z").InnerText)
                );
            }

            var inputLevelsNode = node.SelectSingleNode("InputLevels");
            if (inputLevelsNode != null)
            {
                inputLevels = new Vector3(
                    float.Parse(inputLevelsNode.SelectSingleNode("X").InnerText),
                    float.Parse(inputLevelsNode.SelectSingleNode("Y").InnerText),
                    float.Parse(inputLevelsNode.SelectSingleNode("Z").InnerText)
                );
            }

            var outputLevelsNode = node.SelectSingleNode("OutputLevels");
            if (outputLevelsNode != null)
            {
                outputLevels = new Vector2(
                    float.Parse(outputLevelsNode.SelectSingleNode("X").InnerText),
                    float.Parse(outputLevelsNode.SelectSingleNode("Y").InnerText)
                );
            }

            var overallInputLevelsNode = node.SelectSingleNode("OverallInputLevels");
            if (overallInputLevelsNode != null)
            {
                overallInputLevels = new Vector3(
                    float.Parse(overallInputLevelsNode.SelectSingleNode("X").InnerText),
                    float.Parse(overallInputLevelsNode.SelectSingleNode("Y").InnerText),
                    float.Parse(overallInputLevelsNode.SelectSingleNode("Z").InnerText)
                );
            }

            var overallOutputLevelsNode = node.SelectSingleNode("OverallOutputLevels");
            if (outputLevelsNode != null)
            {
                overallOutputLevels = new Vector2(
                    float.Parse(overallOutputLevelsNode.SelectSingleNode("X").InnerText),
                    float.Parse(overallOutputLevelsNode.SelectSingleNode("Y").InnerText)
                );
            }

            var radarMapColorNode = node.SelectSingleNode("RadarMapColor");
            if (radarMapColorNode != null)
            {
                radarMapColor = Color.FromArgb(
                    (int)(float.Parse(radarMapColorNode.SelectSingleNode("R").InnerText) * 255),
                    (int)(float.Parse(radarMapColorNode.SelectSingleNode("G").InnerText) * 255),
                    (int)(float.Parse(radarMapColorNode.SelectSingleNode("B").InnerText) * 255)
                );
            }
        }

        public void Flatten()
        {
            lowerBounds = LowerBounds;
            upperBounds = UpperBounds;
            fudge = Fudge;
            hsvShift = HSVShift;
            inputLevels = InputLevels;
            outputLevels = OutputLevels;
            overallInputLevels = OverallInputLevels;
            overallOutputLevels = OverallOutputLevels;
            radarMapColor = RadarMapColor;
        }
    }
}
