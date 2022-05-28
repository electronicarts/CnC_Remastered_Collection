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

namespace MobiusEditor.Tools.Dialogs
{
    public partial class CellTriggersToolDialog : Form
    {
        public ComboBox TriggerCombo => triggerCombo;

        public CellTriggersToolDialog()
        {
            InitializeComponent();

            Location = Properties.Settings.Default.ToolDialogPosition;
        }

        protected override void OnMove(EventArgs e)
        {
            base.OnMove(e);

            Properties.Settings.Default.ToolDialogPosition = Location;
            Properties.Settings.Default.Save();
        }
    }
}
