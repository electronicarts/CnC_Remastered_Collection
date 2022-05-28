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
using MobiusEditor.Interface;
using MobiusEditor.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MobiusEditor.Controls
{
    public partial class TypeComboBox : ComboBox
    {
        [Category("Behavior")]
        public Image MissingThumbnail { get; set; } = SystemIcons.Error.ToBitmap();

        public IEnumerable<IBrowsableType> Types
        {
            get => Items.Cast<TypeItem<IBrowsableType>>().Select(t => t.Type);
            set
            {
                DataSource = value.Select(t => new TypeItem<IBrowsableType>(t.DisplayName, t)).ToArray();
                DropDownHeight = Math.Max(DropDownHeight, value.Max(t => (t.Thumbnail?.Height ?? MissingThumbnail.Height) * 3));
                Invalidate();
            }
        }

        public IBrowsableType SelectedType => SelectedValue as IBrowsableType;

        public TypeComboBox()
        {
            InitializeComponent();

            DisplayMember = "Name";
            ValueMember = "Type";
        }

        protected override void OnMeasureItem(MeasureItemEventArgs e)
        {
            base.OnMeasureItem(e);

            var typeItem = Items[e.Index] as TypeItem<IBrowsableType>;
            if (typeItem?.Type != null)
            {
                e.ItemHeight = typeItem.Type.Thumbnail?.Height ?? MissingThumbnail.Height;
            }
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            base.OnDrawItem(e);

            e.DrawBackground();

            if ((e.Index >= 0) && (e.Index < Items.Count))
            {
                var typeItem = Items[e.Index] as TypeItem<IBrowsableType>;
                if (typeItem?.Type != null)
                {
                    StringFormat stringFormat = new StringFormat
                    {
                        LineAlignment = StringAlignment.Center
                    };

                    var textColor = ((e.State & DrawItemState.Selected) == DrawItemState.Selected) ? SystemBrushes.HighlightText : SystemBrushes.WindowText;
                    var textSize = e.Graphics.MeasureString(typeItem.Name, Font, e.Bounds.Width, stringFormat);
                    e.Graphics.DrawString(typeItem.Name, Font, textColor, e.Bounds, stringFormat);

                    if ((e.State & DrawItemState.ComboBoxEdit) == DrawItemState.None)
                    {
                        var thumbnail = typeItem.Type.Thumbnail ?? MissingThumbnail;
                        var thumbnailWidth = (int)Math.Min(e.Bounds.Width - textSize.Width, thumbnail.Width);
                        var thumbnailSize = new Size(thumbnailWidth, thumbnailWidth * thumbnail.Height / thumbnail.Width);
                        var thumbnailBounds = new Rectangle(new Point(e.Bounds.Right - thumbnailSize.Width, e.Bounds.Top), thumbnailSize);
                        e.Graphics.DrawImage(thumbnail, thumbnailBounds);
                    }
                }
            }

            e.DrawFocusRectangle();
        }
    }
}
