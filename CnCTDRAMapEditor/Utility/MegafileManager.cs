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

namespace MobiusEditor.Utility
{
    public class MegafileManager : IEnumerable<string>, IEnumerable, IDisposable
    {
        private readonly string looseFilePath;

        private readonly List<Megafile> megafiles = new List<Megafile>();

        private readonly HashSet<string> filenames = new HashSet<string>();

        public MegafileManager(string looseFilePath)
        {
            this.looseFilePath = looseFilePath;
        }

        public bool Load(string megafilePath)
        {
            if (!File.Exists(megafilePath))
            {
                return false;
            }

            var megafile = new Megafile(megafilePath);
            filenames.UnionWith(megafile);
            megafiles.Add(megafile);
            return true;
        }

        public bool Exists(string path)
        {
            return File.Exists(Path.Combine(looseFilePath, path)) || filenames.Contains(path.ToUpper());
        }

        public Stream Open(string path)
        {
            string loosePath = Path.Combine(looseFilePath, path);
            if (File.Exists(loosePath))
            {
                return File.Open(loosePath, FileMode.Open, FileAccess.Read);
            }

            foreach (var megafile in megafiles)
            {
                var stream = megafile.Open(path.ToUpper());
                if (stream != null)
                {
                    return stream;
                }
            }

            return null;
        }

        public IEnumerator<string> GetEnumerator()
        {
            return filenames.GetEnumerator();
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
                    megafiles.ForEach(m => m.Dispose());
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
