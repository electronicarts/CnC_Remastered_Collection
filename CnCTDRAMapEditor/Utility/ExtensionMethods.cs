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
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace MobiusEditor.Utility
{
    public static class ExtensionMethods
    {
        public static float ToLinear(this float v)
        {
            return (v < 0.04045f) ? (v * 25.0f / 323.0f) : (float)Math.Pow(((200.0f * v) + 11.0f) / 211.0f, 12.0f / 5.0f);
        }

        public static float ToLinear(this byte v)
        {
            return (v / 255.0f).ToLinear();
        }

        public static float ToSRGB(this float v)
        {
            return (v < 0.0031308) ? (v * 323.0f / 25.0f) : ((((float)Math.Pow(v, 5.0f / 12.0f) * 211.0f) - 11.0f) / 200.0f);
        }

        public static void SetDefault<T>(this T data)
        {
            var properties = data.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance).Where(p => p.GetSetMethod() != null);
            foreach (var property in properties)
            {
                if (property.GetCustomAttribute(typeof(DefaultValueAttribute)) is DefaultValueAttribute defaultValueAttr)
                {
                    property.SetValue(data, defaultValueAttr.Value);
                }
            }
        }

        public static void CopyTo<T>(this T data, T other)
        {
            var properties = data.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance).Where(p => (p.GetSetMethod() != null) && (p.GetGetMethod() != null));
            foreach (var property in properties)
            {
                var defaultValueAttr = property.GetCustomAttribute(typeof(DefaultValueAttribute)) as DefaultValueAttribute;
                property.SetValue(other, property.GetValue(data));
            }
        }

        public static IEnumerable<Point> Points(this Rectangle rectangle)
        {
            for (var y = rectangle.Top; y < rectangle.Bottom; ++y)
            {
                for (var x = rectangle.Left; x < rectangle.Right; ++x)
                {
                    yield return new Point(x, y);
                }
            }
        }

        public static IEnumerable<T> Yield<T>(this T item)
        {
            yield return item;
        }

        public static HashSet<T> ToHashSet<T>(this IEnumerable<T> source, IEqualityComparer<T> comparer = null)
        {
            return new HashSet<T>(source, comparer);
        }

        public static IEnumerable<byte[]> Split(this byte[] bytes, int length)
        {
            for (int i = 0; i < bytes.Length; i += length)
            {
                yield return bytes.Skip(i).Take(Math.Min(length, bytes.Length - i)).ToArray();
            }
        }

        public static IEnumerable<string> Split(this string str, int length)
        {
            for (int i = 0; i < str.Length; i += length)
            {
                yield return str.Substring(i, Math.Min(length, str.Length - i));
            }
        }

        public static Font GetAdjustedFont(this Graphics graphics, string text, Font originalFont, int width, int minSize, int maxSize, bool smallestOnFail)
        {
            if (minSize > maxSize)
            {
                throw new ArgumentOutOfRangeException("minSize");
            }

            for (var size = maxSize; size >= minSize; --size)
            {
                var font = new Font(originalFont.Name, size, originalFont.Style);
                var textSize = graphics.MeasureString(text, font);

                if (width > Convert.ToInt32(textSize.Width))
                {
                    return font;
                }
            }

            return smallestOnFail ? new Font(originalFont.Name, minSize, originalFont.Style) : originalFont;
        }

        public static Bitmap Sharpen(this Bitmap bitmap, double strength)
        {
            var sharpenImage = bitmap.Clone() as Bitmap;

            int width = bitmap.Width;
            int height = bitmap.Height;

            // Create sharpening filter.
            const int filterSize = 5;

            var filter = new double[,]
            {
                {-1, -1, -1, -1, -1},
                {-1,  2,  2,  2, -1},
                {-1,  2, 16,  2, -1},
                {-1,  2,  2,  2, -1},
                {-1, -1, -1, -1, -1}
            };

            double bias = 1.0 - strength;
            double factor = strength / 16.0;

            const int s = filterSize / 2;

            var result = new Color[bitmap.Width, bitmap.Height];

            // Lock image bits for read/write.
            if (sharpenImage != null)
            {
                BitmapData pbits = sharpenImage.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                // Declare an array to hold the bytes of the bitmap.
                int bytes = pbits.Stride * height;
                var rgbValues = new byte[bytes];

                // Copy the RGB values into the array.
                Marshal.Copy(pbits.Scan0, rgbValues, 0, bytes);

                int rgb;
                // Fill the color array with the new sharpened color values.
                for (int x = s; x < width - s; x++)
                {
                    for (int y = s; y < height - s; y++)
                    {
                        double red = 0.0, green = 0.0, blue = 0.0;

                        for (int filterX = 0; filterX < filterSize; filterX++)
                        {
                            for (int filterY = 0; filterY < filterSize; filterY++)
                            {
                                int imageX = (x - s + filterX + width) % width;
                                int imageY = (y - s + filterY + height) % height;

                                rgb = imageY * pbits.Stride + 3 * imageX;

                                red += rgbValues[rgb + 2] * filter[filterX, filterY];
                                green += rgbValues[rgb + 1] * filter[filterX, filterY];
                                blue += rgbValues[rgb + 0] * filter[filterX, filterY];
                            }

                            rgb = y * pbits.Stride + 3 * x;

                            int r = Math.Min(Math.Max((int)(factor * red + (bias * rgbValues[rgb + 2])), 0), 255);
                            int g = Math.Min(Math.Max((int)(factor * green + (bias * rgbValues[rgb + 1])), 0), 255);
                            int b = Math.Min(Math.Max((int)(factor * blue + (bias * rgbValues[rgb + 0])), 0), 255);

                            result[x, y] = Color.FromArgb(r, g, b);
                        }
                    }
                }

                // Update the image with the sharpened pixels.
                for (int x = s; x < width - s; x++)
                {
                    for (int y = s; y < height - s; y++)
                    {
                        rgb = y * pbits.Stride + 3 * x;

                        rgbValues[rgb + 2] = result[x, y].R;
                        rgbValues[rgb + 1] = result[x, y].G;
                        rgbValues[rgb + 0] = result[x, y].B;
                    }
                }

                // Copy the RGB values back to the bitmap.
                Marshal.Copy(rgbValues, 0, pbits.Scan0, bytes);
                // Release image bits.
                sharpenImage.UnlockBits(pbits);
            }

            return sharpenImage;
        }
    }
}
