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
using System.Dynamic;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.Utility
{
    public class TrackablePropertyDescriptor<T> : PropertyDescriptor
    {
        private readonly T obj;
        private readonly PropertyInfo propertyInfo;
        private readonly Dictionary<string, object> propertyValues;

        public override Type ComponentType => obj.GetType();

        public override bool IsReadOnly => false;

        public override Type PropertyType => propertyInfo.PropertyType;

        public TrackablePropertyDescriptor(string name, T obj, PropertyInfo propertyInfo, Dictionary<string, object> propertyValues)
            : base(name, null)
        {
            this.obj = obj;
            this.propertyInfo = propertyInfo;
            this.propertyValues = propertyValues;
        }

        public override bool CanResetValue(object component)
        {
            return propertyValues.ContainsKey(Name);
        }

        public override object GetValue(object component)
        {
            if (propertyValues.TryGetValue(Name, out object result))
            {
                return result;
            }
            return propertyInfo.GetValue(obj);
        }

        public override void ResetValue(object component)
        {
            propertyValues.Remove(Name);
        }

        public override void SetValue(object component, object value)
        {
            if (Equals(propertyInfo.GetValue(obj), value))
            {
                propertyValues.Remove(Name);
            }
            else
            {
                propertyValues[Name] = value;
            }
        }

        public override bool ShouldSerializeValue(object component)
        {
            return false;
        }
    }

    public class PropertyTracker<T> : DynamicObject, ICustomTypeDescriptor
    {
        private readonly Dictionary<string, PropertyInfo> trackableProperties;
        private readonly Dictionary<string, object> propertyValues = new Dictionary<string, object>();

        public T Object { get; private set; }
        
        public PropertyTracker(T obj)
        {
            Object = obj;

            trackableProperties = Object.GetType()
                .GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(p => (p.GetGetMethod() != null) && (p.GetSetMethod() != null))
                .ToDictionary(k => k.Name, v => v);
        }

        public void Revert() => propertyValues.Clear();

        public void Commit()
        {
            foreach (var propertyValue in propertyValues)
            {
                trackableProperties[propertyValue.Key].SetValue(Object, propertyValue.Value);
            }
            propertyValues.Clear();
        }

        public IDictionary<string, object> GetUndoValues() => propertyValues.ToDictionary(kv => kv.Key, kv => trackableProperties[kv.Key].GetValue(Object));

        public IDictionary<string, object> GetRedoValues() => new Dictionary<string, object>(propertyValues);

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            if (!trackableProperties.TryGetValue(binder.Name, out PropertyInfo property))
            {
                result = null;
                return false;
            }

            if (!propertyValues.TryGetValue(binder.Name, out result))
            {
                result = property.GetValue(Object);
            }
            return true;
        }

        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            if (!trackableProperties.TryGetValue(binder.Name, out PropertyInfo property))
            {
                return false;
            }

            if (Equals(property.GetValue(Object), value))
            {
                propertyValues.Remove(binder.Name);
            }
            else
            {
                propertyValues[binder.Name] = value;
            }
            return true;
        }

        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(Object.GetType());
        }

        public string GetClassName()
        {
            return TypeDescriptor.GetClassName(Object.GetType());
        }

        public string GetComponentName()
        {
            return TypeDescriptor.GetComponentName(Object.GetType());
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(Object.GetType());
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(Object.GetType());
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(Object.GetType());
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(Object.GetType(), editorBaseType);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(Object.GetType());
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(Object.GetType(), attributes);
        }

        public PropertyDescriptorCollection GetProperties()
        {
            var propertyDescriptors = trackableProperties.Select(kv =>
            {
                return new TrackablePropertyDescriptor<T>(kv.Key, Object, kv.Value, propertyValues);
            }).ToArray();
            return new PropertyDescriptorCollection(propertyDescriptors);
        }

        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return Object;
        }
    }
}
