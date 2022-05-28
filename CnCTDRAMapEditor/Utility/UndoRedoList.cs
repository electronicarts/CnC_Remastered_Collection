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
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MobiusEditor.Utility
{
    public class UndoRedoList<T>
    {
        private const int DefaultMaxUndoRedo = 50;

        private readonly List<(Action<T> Undo, Action<T> Redo)> undoRedoActions = new List<(Action<T> Undo, Action<T> Redo)>();
        private readonly int maxUndoRedo;
        private int undoRedoPosition = 0;

        public event EventHandler<EventArgs> Tracked;
        public event EventHandler<EventArgs> Undone;
        public event EventHandler<EventArgs> Redone;

        public bool CanUndo => undoRedoPosition > 0;

        public bool CanRedo => undoRedoActions.Count > undoRedoPosition;

        public UndoRedoList(int maxUndoRedo)
        {
            this.maxUndoRedo = maxUndoRedo;
        }

        public UndoRedoList()
            : this(DefaultMaxUndoRedo)
        {
        }

        public void Clear()
        {
            undoRedoActions.Clear();
            undoRedoPosition = 0;
            OnTracked();
        }

        public void Track(Action<T> undo, Action<T> redo)
        {
            if (undoRedoActions.Count > undoRedoPosition)
            {
                undoRedoActions.RemoveRange(undoRedoPosition, undoRedoActions.Count - undoRedoPosition);
            }

            undoRedoActions.Add((undo, redo));

            if (undoRedoActions.Count > maxUndoRedo)
            {
                undoRedoActions.RemoveRange(0, undoRedoActions.Count - maxUndoRedo);
            }

            undoRedoPosition = undoRedoActions.Count;
            OnTracked();
        }

        public void Undo(T context)
        {
            if (!CanUndo)
            {
                throw new InvalidOperationException();
            }

            undoRedoPosition--;
            undoRedoActions[undoRedoPosition].Undo(context);
            OnUndone();
        }

        public void Redo(T context)
        {
            if (!CanRedo)
            {
                throw new InvalidOperationException();
            }

            undoRedoActions[undoRedoPosition].Redo(context);
            undoRedoPosition++;
            OnRedone();
        }

        protected virtual void OnTracked()
        {
            Tracked?.Invoke(this, new EventArgs());
        }

        protected virtual void OnUndone()
        {
            Undone?.Invoke(this, new EventArgs());
        }

        protected virtual void OnRedone()
        {
            Redone?.Invoke(this, new EventArgs());
        }
    }
}
