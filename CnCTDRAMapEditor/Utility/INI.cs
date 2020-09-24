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
using System.Collections.Specialized;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;

namespace MobiusEditor.Utility
{
    static class INIHelpers
    {
        public static readonly Regex SectionRegex = new Regex(@"^\s*\[([^\]]*)\]", RegexOptions.Compiled);
        public static readonly Regex KeyValueRegex = new Regex(@"^\s*(.*?)\s*=([^;]*)", RegexOptions.Compiled);
        public static readonly Regex CommentRegex = new Regex(@"^\s*(#|;)", RegexOptions.Compiled);

        public static readonly Func<INIDiffType, string> DiffPrefix = t =>
        {
            switch (t)
            {
                case INIDiffType.Added:
                    return "+";
                case INIDiffType.Removed:
                    return "-";
                case INIDiffType.Updated:
                    return "@";
            }
            return string.Empty;
        };
    }

    public class INIKeyValueCollection : IEnumerable<(string Key, string Value)>, IEnumerable
    {
        private readonly OrderedDictionary KeyValues;

        public string this[string key]
        {
            get
            {
                if (!KeyValues.Contains(key))
                {
                    throw new KeyNotFoundException(key);
                }
                return KeyValues[key] as string;
            }
            set
            {
                if (key == null)
                {
                    throw new ArgumentNullException("key");
                }
                KeyValues[key] = value;
            }
        }

        public INIKeyValueCollection()
        {
            KeyValues = new OrderedDictionary(StringComparer.OrdinalIgnoreCase);
        }

        public int Count => KeyValues.Count;

        public bool Contains(string key) => KeyValues.Contains(key);

        public T Get<T>(string key) where T : struct
        {
            var converter = TypeDescriptor.GetConverter(typeof(T));
            return (T)converter.ConvertFromString(this[key]);
        }

        public void Set<T>(string key, T value) where T : struct
        {
            var converter = TypeDescriptor.GetConverter(typeof(T));
            this[key] = converter.ConvertToString(value);
        }

        public bool Remove(string key)
        {
            if (!KeyValues.Contains(key))
            {
                return false;
            }
            KeyValues.Remove(key);
            return true;
        }

        public IEnumerator<(string Key, string Value)> GetEnumerator()
        {
            foreach (DictionaryEntry entry in KeyValues)
            {
                yield return (entry.Key as string, entry.Value as string);
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }

    public class INISection : IEnumerable<(string Key, string Value)>, IEnumerable
    {
        public readonly INIKeyValueCollection Keys;

        public string Name { get; private set; }

        public string this[string key] { get => Keys[key]; set => Keys[key] = value; }

        public bool Empty => Keys.Count == 0;

        public INISection(string name)
        {
            Keys = new INIKeyValueCollection();
            Name = name;
        }

        public void Parse(TextReader reader)
        {
            while (true)
            {
                var line = reader.ReadLine();
                if (line == null)
                {
                    break;
                }

                var m = INIHelpers.KeyValueRegex.Match(line);
                if (m.Success)
                {
                    Keys[m.Groups[1].Value] = m.Groups[2].Value;
                }
            }
        }

        public void Parse(string iniText)
        {
            using (var reader = new StringReader(iniText))
            {
                Parse(reader);
            }
        }

        public IEnumerator<(string Key, string Value)> GetEnumerator()
        {
            return Keys.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            var lines = new List<string>(Keys.Count);
            foreach (var item in Keys)
            {
                lines.Add(string.Format("{0}={1}", item.Key, item.Value));
            }
            return string.Join(Environment.NewLine, lines);
        }
    }

    public class INISectionCollection : IEnumerable<INISection>, IEnumerable
    {
        private readonly OrderedDictionary Sections;

        public INISection this[string name] => Sections.Contains(name) ? (Sections[name] as INISection) : null;

        public INISectionCollection()
        {
            Sections = new OrderedDictionary(StringComparer.OrdinalIgnoreCase);
        }

        public int Count => Sections.Count;

        public bool Contains(string section) => Sections.Contains(section);

        public INISection Add(string name)
        {
            if (!Sections.Contains(name))
            {
                var section = new INISection(name);
                Sections[name] = section;
            }
            return this[name];
        }

        public bool Add(INISection section)
        {
            if ((section == null) || Sections.Contains(section.Name))
            {
                return false;
            }
            Sections[section.Name] = section;
            return true;
        }

        public void AddRange(IEnumerable<INISection> sections)
        {
            foreach (var section in sections)
            {
                Add(section);
            }
        }

        public bool Remove(string name)
        {
            if (!Sections.Contains(name))
            {
                return false;
            }
            Sections.Remove(name);
            return true;
        }

        public INISection Extract(string name)
        {
            if (!Sections.Contains(name))
            {
                return null;
            }
            var section = this[name];
            Sections.Remove(name);
            return section;
        }

        public IEnumerator<INISection> GetEnumerator()
        {
            foreach (DictionaryEntry entry in Sections)
            {
                yield return entry.Value as INISection;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }

    public partial class INI : IEnumerable<INISection>, IEnumerable
    {
        public readonly INISectionCollection Sections;

        public INISection this[string name] { get => Sections[name]; }

        public INI()
        {
            Sections = new INISectionCollection();
        }

        public void Parse(TextReader reader)
        {
            INISection currentSection = null;

            while (true)
            {
                var line = reader.ReadLine();
                if (line == null)
                {
                    break;
                }

                var m = INIHelpers.SectionRegex.Match(line);
                if (m.Success)
                {
                    currentSection = Sections.Add(m.Groups[1].Value);
                }

                if (currentSection != null)
                {
                    if (INIHelpers.CommentRegex.Match(line).Success)
                    {
                        continue;
                    }

                    currentSection.Parse(line);
                }
            }
        }

        public void Parse(string iniText)
        {
            using (var reader = new StringReader(iniText))
            {
                Parse(reader);
            }
        }

        public IEnumerator<INISection> GetEnumerator()
        {
            foreach (var section in Sections)
            {
                yield return section;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            var sections = new List<string>(Sections.Count);
            foreach (var item in Sections)
            {
                var lines = new List<string>
                {
                    string.Format("[{0}]", item.Name)
                };
                if (!item.Empty)
                {
                    lines.Add(item.ToString());
                }
                sections.Add(string.Join(Environment.NewLine, lines));
            }
            return string.Join(Environment.NewLine + Environment.NewLine, sections) + Environment.NewLine;
        }
    }

    [Flags]
    public enum INIDiffType
    {
        None = 0,
        Added = 1,
        Removed = 2,
        Updated = 4,
        AddedOrUpdated = 5
    }

    public class INISectionDiff : IEnumerable<string>, IEnumerable
    {
        public readonly INIDiffType Type;

        private readonly Dictionary<string, INIDiffType> keyDiff;

        public INIDiffType this[string key]
        {
            get
            {
                INIDiffType diffType;
                if (!keyDiff.TryGetValue(key, out diffType))
                {
                    return INIDiffType.None;
                }
                return diffType;
            }
        }

        private INISectionDiff()
        {
            keyDiff = new Dictionary<string, INIDiffType>();
            Type = INIDiffType.None;
        }

        internal INISectionDiff(INIDiffType type, INISection section)
            : this()
        {
            foreach (var keyValue in section.Keys)
            {
                keyDiff[keyValue.Key] = type;
            }

            Type = type;
        }

        internal INISectionDiff(INISection leftSection, INISection rightSection)
            : this(INIDiffType.Removed, leftSection)
        {
            foreach (var keyValue in rightSection.Keys)
            {
                var key = keyValue.Key;
                if (keyDiff.ContainsKey(key))
                {
                    if (leftSection[key] == rightSection[key])
                    {
                        keyDiff.Remove(key);
                    }
                    else
                    {
                        keyDiff[key] = INIDiffType.Updated;
                        Type = INIDiffType.Updated;
                    }
                }
                else
                {
                    keyDiff[key] = INIDiffType.Added;
                    Type = INIDiffType.Updated;
                }
            }

            Type = (keyDiff.Count > 0) ? INIDiffType.Updated : INIDiffType.None;
        }

        public IEnumerator<string> GetEnumerator()
        {
            return keyDiff.Keys.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            foreach (var item in keyDiff)
            {
                sb.AppendLine(string.Format("{0} {1}", INIHelpers.DiffPrefix(item.Value), item.Key));
            }
            return sb.ToString();
        }
    }

    public class INIDiff : IEnumerable<string>, IEnumerable
    {
        private readonly Dictionary<string, INISectionDiff> sectionDiffs;

        public INISectionDiff this[string key]
        {
            get
            {
                if (!sectionDiffs.TryGetValue(key, out INISectionDiff sectionDiff))
                {
                    return null;
                }
                return sectionDiff;
            }
        }

        private INIDiff()
        {
            sectionDiffs = new Dictionary<string, INISectionDiff>(StringComparer.OrdinalIgnoreCase);
        }

        public INIDiff(INI leftIni, INI rightIni)
            : this()
        {
            foreach (var leftSection in leftIni)
            {
                sectionDiffs[leftSection.Name] = rightIni.Sections.Contains(leftSection.Name) ?
                    new INISectionDiff(leftSection, rightIni[leftSection.Name]) :
                    new INISectionDiff(INIDiffType.Removed, leftSection);
            }

            foreach (var rightSection in rightIni)
            {
                if (!leftIni.Sections.Contains(rightSection.Name))
                {
                    sectionDiffs[rightSection.Name] = new INISectionDiff(INIDiffType.Added, rightSection);
                }
            }

            sectionDiffs = sectionDiffs.Where(x => x.Value.Type != INIDiffType.None).ToDictionary(x => x.Key, x => x.Value);
        }

        public bool Contains(string key) => sectionDiffs.ContainsKey(key);

        public IEnumerator<string> GetEnumerator()
        {
            return sectionDiffs.Keys.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            foreach (var item in sectionDiffs)
            {
                sb.AppendLine(string.Format("{0} {1}", INIHelpers.DiffPrefix(item.Value.Type), item.Key));
                using (var reader = new StringReader(item.Value.ToString()))
                {
                    while (true)
                    {
                        var line = reader.ReadLine();
                        if (line == null)
                        {
                            break;
                        }

                        sb.AppendLine(string.Format("\t{0}", line));
                    }
                }
            }
            return sb.ToString();
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class NonSerializedINIKeyAttribute : Attribute
    {
    }

    public partial class INI
    {
        public static void ParseSection<T>(ITypeDescriptorContext context, INISection section, T data)
        {
            var propertyDescriptors = TypeDescriptor.GetProperties(data);
            var properties = data.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance).Where(p => p.GetSetMethod() != null);
            foreach (var property in properties)
            {
                if (property.GetCustomAttribute<NonSerializedINIKeyAttribute>() != null)
                {
                    continue;
                }

                if (section.Keys.Contains(property.Name))
                {
                    var converter = propertyDescriptors.Find(property.Name, false)?.Converter ?? TypeDescriptor.GetConverter(property.PropertyType);
                    if (converter.CanConvertFrom(context, typeof(string)))
                    {
                        try
                        {
                            property.SetValue(data, converter.ConvertFromString(context, section[property.Name]));
                        }
                        catch (FormatException)
                        {
                            if (property.PropertyType == typeof(bool))
                            {
                                var value = section[property.Name].ToLower();
                                if (value == "no")
                                {
                                    property.SetValue(data, false);
                                }
                                else if (value == "yes")
                                {
                                    property.SetValue(data, true);
                                }
                                else
                                {
                                    throw;
                                }
                            }
                            else
                            {
                                throw;
                            }
                        }
                    }
                }
            }
        }

        public static void WriteSection<T>(ITypeDescriptorContext context, INISection section, T data)
        {
            var propertyDescriptors = TypeDescriptor.GetProperties(data);
            var properties = data.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance).Where(p => p.GetGetMethod() != null);
            foreach (var property in properties)
            {
                if (property.GetCustomAttribute<NonSerializedINIKeyAttribute>() != null)
                {
                    continue;
                }

                var value = property.GetValue(data);
                if (property.PropertyType.IsValueType || (value != null))
                {
                    var converter = propertyDescriptors.Find(property.Name, false)?.Converter ?? TypeDescriptor.GetConverter(property.PropertyType);
                    if (converter.CanConvertTo(context, typeof(string)))
                    {
                        section[property.Name] = converter.ConvertToString(context, value);
                    }
                }
            }
        }

        public static void ParseSection<T>(INISection section, T data) => ParseSection(null, section, data);

        public static void WriteSection<T>(INISection section, T data) => WriteSection(null, section, data);
    }
}
