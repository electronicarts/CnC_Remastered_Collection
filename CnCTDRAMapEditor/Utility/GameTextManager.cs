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
using System.Text;

namespace MobiusEditor.Utility
{
    public class GameTextManager
    {
        private readonly Dictionary<string, string> gameText = new Dictionary<string, string>();

        public string this[string textId] => gameText.TryGetValue(textId, out string text) ? text : textId;

        public GameTextManager(MegafileManager megafileManager, string gameTextFile)
        {
            using (var stream = megafileManager.Open(gameTextFile))
            using (var reader = new BinaryReader(stream))
            using (var unicodeReader = new BinaryReader(stream, Encoding.Unicode))
            using (var asciiReader = new BinaryReader(stream, Encoding.ASCII))
            {
                var numStrings = reader.ReadUInt32();
                var stringSizes = new (uint textSize, uint idSize)[numStrings];
                var strings = new string[numStrings];

                for (var i = 0; i < numStrings; ++i)
                {
                    reader.ReadUInt32();
                    stringSizes[i] = (reader.ReadUInt32(), reader.ReadUInt32());
                }

                for (var i = 0; i < numStrings; ++i)
                {
                    strings[i] = new string(unicodeReader.ReadChars((int)stringSizes[i].textSize));
                }

                for (var i = 0; i < numStrings; ++i)
                {
                    var textId = new string(asciiReader.ReadChars((int)stringSizes[i].idSize));
                    gameText[textId] = strings[i];
                }
            }
        }
    }
}
