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
using System.Drawing;
using System.Windows.Forms;

namespace MobiusEditor.Controls
{
    public partial class ImageTooltip : ToolTip
    {
        public Size MaxSize { get; set; } = Size.Empty;

        public ImageTooltip()
        {
            InitializeComponent();

            OwnerDraw = true;
            Popup += ImageTooltip_Popup;
            Draw += ImageTooltip_Draw;
        }

        private void ImageTooltip_Popup(object sender, PopupEventArgs e)
        {
            if (e.AssociatedControl.Tag is Image image)
            {
                var size = image.Size;
                if (MaxSize.Width > 0)
                {
                    size.Width = Math.Min(MaxSize.Width, size.Width);
                }
                if (MaxSize.Height > 0)
                {
                    size.Height = Math.Min(MaxSize.Height, size.Height);
                }

                e.ToolTipSize = size;
            }
            else
            {
                e.Cancel = true;
            }
        }

        private void ImageTooltip_Draw(object sender, DrawToolTipEventArgs e)
        {
            if (e.AssociatedControl.Tag is Image image)
            {
                e.Graphics.DrawImage(image, e.Bounds);
            }
        }
    }
}
