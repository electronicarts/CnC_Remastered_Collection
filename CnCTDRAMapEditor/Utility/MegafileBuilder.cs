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
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace MobiusEditor.Utility
{
    public class MegafileBuilder : IDisposable
    {
        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    Out.Dispose();
                }

                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion

        private const float Version = 0.99f;

        public string RootPath { get; private set; }

        private Stream Out { get; set; }

        private List<(string, object)> Files = new List<(string, object)>();

        public MegafileBuilder(string rootPath, string outFile)
        {
            RootPath = rootPath.ToUpper();
            Out = new FileStream(outFile, FileMode.Create);
        }

        public void AddFile(string path)
        {
            if (File.Exists(path))
            {
                Files.Add((Path.GetFileName(path), path));
            }
        }

        public void AddFile(string path, Stream stream)
        {
            Files.Add((Path.GetFileName(path), stream));
        }

        public void AddDirectory(string path)
        {
            AddDirectory(path, "*.*");
        }

        public void AddDirectory(string path, string searchPattern)
        {
            var uriPath = new Uri(path);
            foreach (var file in Directory.GetFiles(path, searchPattern, SearchOption.AllDirectories))
            {
                var relativePath = Uri.UnescapeDataString(uriPath.MakeRelativeUri(new Uri(file)).ToString()).Replace('/', Path.DirectorySeparatorChar);
                Files.Add((relativePath, file));
            }
        }

        public void Write()
        {
            var headerSize = sizeof(uint) * 6U;
            headerSize += SubFileData.Size * (uint)Files.Count;

            var strings = new List<string>();
            Func<string, ushort> stringIndex = (string value) =>
            {
                var index = strings.IndexOf(value);
                if (index < 0)
                {
                    index = strings.Count;
                    if (index > ushort.MaxValue)
                    {
                        throw new IndexOutOfRangeException();
                    }
                    strings.Add(value);
                }
                return (ushort)index;
            };

            var files = new List<(ushort index, uint crc, Stream stream, bool dispose)>();
            foreach (var (filename, source) in Files)
            {
                var name = Encoding.ASCII.GetBytes(filename);
                var crc = CRC.Calculate(name);

                if (source is string)
                {
                    var file = source as string;
                    if (File.Exists(file))
                    {
                        files.Add((stringIndex(Path.Combine(RootPath, filename).ToUpper()), crc, new FileStream(file, FileMode.Open, FileAccess.Read), true));
                    }
                }
                else if (source is Stream)
                {
                    files.Add((stringIndex(Path.Combine(RootPath, filename).ToUpper()), crc, source as Stream, false));
                }
            }
            files = files.OrderBy(x => x.crc).ToList();

            var stringsSize = sizeof(ushort) * (uint)strings.Count;
            stringsSize += (uint)strings.Sum(x => x.Length);
            headerSize += stringsSize;

            var subfileImageOffset = headerSize;
            using (var writer = new BinaryWriter(Out))
            {
                writer.Write(0xFFFFFFFF);
                writer.Write(Version);
                writer.Write(headerSize);

                writer.Write((uint)Files.Count);
                writer.Write((uint)strings.Count);
                writer.Write(stringsSize);

                foreach (var item in strings)
                {
                    writer.Write((ushort)item.Length);
                    writer.Write(item.ToCharArray());
                }

                using (var fileStream = new MemoryStream())
                {
                    for (var i = 0; i < files.Count; ++i)
                    {
                        var (index, crc, stream, dispose) = files[i];

                        var fileSize = (uint)(stream.Length - stream.Position);
                        var fileBytes = new byte[fileSize];
                        stream.Read(fileBytes, 0, fileBytes.Length);
                        fileStream.Write(fileBytes, 0, fileBytes.Length);

                        if (dispose)
                        {
                            stream.Dispose();
                        }

                        SubFileData data = new SubFileData
                        {
                            Flags = 0,
                            CRCValue = crc,
                            SubfileIndex = i,
                            SubfileSize = fileSize,
                            SubfileImageDataOffset = subfileImageOffset,
                            SubfileNameIndex = index
                        };

                        var ptr = Marshal.AllocHGlobal((int)SubFileData.Size);
                        Marshal.StructureToPtr(data, ptr, false);
                        var bytes = new byte[SubFileData.Size];
                        Marshal.Copy(ptr, bytes, 0, bytes.Length);
                        Marshal.FreeHGlobal(ptr);

                        writer.Write(bytes);

                        subfileImageOffset += data.SubfileSize;
                    }

                    fileStream.Seek(0, SeekOrigin.Begin);
                    fileStream.CopyTo(writer.BaseStream);
                }
            }
        }
    }
}
