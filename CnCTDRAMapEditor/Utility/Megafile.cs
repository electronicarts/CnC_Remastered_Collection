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
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

namespace MobiusEditor.Utility
{
    [StructLayout(LayoutKind.Sequential, Pack = 2)]
    struct SubFileData
    {
        public ushort Flags;
        public uint CRCValue;
        public int SubfileIndex;
        public uint SubfileSize;
        public uint SubfileImageDataOffset;
        public ushort SubfileNameIndex;

        public static readonly uint Size = (uint)Marshal.SizeOf(typeof(SubFileData));
    }

    public class Megafile : IEnumerable<string>, IEnumerable, IDisposable
    {
        private readonly MemoryMappedFile megafileMap;

        private readonly string[] stringTable;

        private readonly Dictionary<string, SubFileData> fileTable = new Dictionary<string, SubFileData>();

        public Megafile(string megafilePath)
        {
            megafileMap = MemoryMappedFile.CreateFromFile(
                new FileStream(megafilePath, FileMode.Open, FileAccess.Read, FileShare.Read) , null, 0, MemoryMappedFileAccess.Read, HandleInheritability.None, false
            );

            var numFiles = 0U;
            var numStrings = 0U;
            var stringTableSize = 0U;
            var fileTableSize = 0U;

            var readOffset = 0U;
            using (var magicNumberReader = new BinaryReader(megafileMap.CreateViewStream(readOffset, 4, MemoryMappedFileAccess.Read)))
            {
                var magicNumber = magicNumberReader.ReadUInt32();
                if ((magicNumber == 0xFFFFFFFF) || (magicNumber == 0x8FFFFFFF))
                {
                    // Skip header size and version
                    readOffset += 8;
                }
            }

            readOffset += 4U;
            using (var headerReader = new BinaryReader(megafileMap.CreateViewStream(readOffset, 12, MemoryMappedFileAccess.Read)))
            {
                numFiles = headerReader.ReadUInt32();
                numStrings = headerReader.ReadUInt32();
                stringTableSize = headerReader.ReadUInt32();
                fileTableSize = numFiles * SubFileData.Size;
            }

            readOffset += 12U;
            using (var stringReader = new BinaryReader(megafileMap.CreateViewStream(readOffset, stringTableSize, MemoryMappedFileAccess.Read)))
            {
                stringTable = new string[numStrings];

                for (var i = 0U; i < numStrings; ++i)
                {
                    var stringSize = stringReader.ReadUInt16();
                    stringTable[i] = new string(stringReader.ReadChars(stringSize));
                }
            }

            readOffset += stringTableSize;
            using (var subFileAccessor = megafileMap.CreateViewAccessor(readOffset, fileTableSize, MemoryMappedFileAccess.Read))
            {
                for (var i = 0U; i < numFiles; ++i)
                {
                    subFileAccessor.Read(i * SubFileData.Size, out SubFileData subFile);
                    var fullName = stringTable[subFile.SubfileNameIndex];
                    fileTable[fullName] = subFile;
                }
            }
        }

        public Stream Open(string path)
        {
            if (!fileTable.TryGetValue(path, out SubFileData subFile))
            {
                return null;
            }

            return megafileMap.CreateViewStream(subFile.SubfileImageDataOffset, subFile.SubfileSize, MemoryMappedFileAccess.Read);
        }

        public IEnumerator<string> GetEnumerator()
        {
            foreach (var file in stringTable)
            {
                yield return file;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    megafileMap.Dispose();
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
