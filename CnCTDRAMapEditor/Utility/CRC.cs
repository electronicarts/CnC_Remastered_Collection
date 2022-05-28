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

namespace MobiusEditor.Utility
{
    public class CRC
    {
        static CRC()
        {
            for (var i = 0U; i < 256U; ++i)
            {
                uint crc = i;
                for (var j = 0U; j < 8U; ++j)
                {
                    if ((crc & 1U) != 0U)
                    {
                        crc = (crc >> 1) ^ polynomial;
                    }
                    else
                    {
                        crc >>= 1;
                    }
                }
                crcTable[i] = crc;
            }
        }

        public static uint Calculate(byte[] bytes)
        {
            if (bytes == null)
            {
                throw new ArgumentNullException("bytes");
            }

            uint remainder = 0xFFFFFFFFU;
            for (var i = 0; i < bytes.Length; ++i)
            {
                uint index = (remainder & 0xFF) ^ bytes[i];
                remainder = (remainder >> 8) ^ crcTable[index];
            }
            return ~remainder;
        }

        private static readonly uint[] crcTable = new uint[256];
        private const uint polynomial = 0xEDB88320;
    }
}
