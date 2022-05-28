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
using System.Windows.Forms;

namespace MobiusEditor.Controls
{
    public partial class PropertiesComboBox : ComboBox
    {
        private bool savedAutoClose;

        private ToolStripDropDown DropDownHost
        {
            get
            {
                var parent = Parent;
                while ((parent != null) && !(parent is ToolStripDropDown))
                {
                    parent = parent.Parent;
                }
                return parent as ToolStripDropDown;
            }
        }

        public PropertiesComboBox()
        {
            InitializeComponent();
        }

        protected override void OnDropDownClosed(EventArgs e)
        {
            var dropDownHost = DropDownHost;
            if (dropDownHost != null)
            {
                dropDownHost.AutoClose = savedAutoClose;
            }

            base.OnDropDownClosed(e);
        }

        protected override void OnDropDown(EventArgs e)
        {
            var dropDownHost = DropDownHost;
            if (dropDownHost != null)
            {
                savedAutoClose = dropDownHost.AutoClose;
                dropDownHost.AutoClose = false;
            }

            base.OnDropDown(e);
        }
    }
}
