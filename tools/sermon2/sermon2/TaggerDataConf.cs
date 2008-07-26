// TaggerDataConf2.cs
// 
// Copyright (C) 2008 Daniel Otte
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

using System;

namespace sermon2
{
    
    
    public partial class TaggerDataConf : Gtk.Bin
    {
        
        public TaggerDataConf()
        {
            this.Build();
            foregroundColorButton.Sensitive = foregroundCheckButton.Active;
            backgroundColorButton.Sensitive = backgroundCheckButton.Active;
            backgroundColorButton.Color = new Gdk.Color(0xff, 0xff, 0xff);
        }
        
        public TaggerDataConf(String s) : this()
        {
            frameLabel.Markup = "<b>" + s + "</b>";
        }
        
        public TaggerDataConf(uint i) : this()
        {
            frameLabel.Markup = "<b> data tagger " + i.ToString() + "</b>";
        }

        protected virtual void OnForegroundCheckButtonReleased (object sender, System.EventArgs e)
        {
           foregroundColorButton.Sensitive = foregroundCheckButton.Active;
        }

        protected virtual void OnBackgroundCheckButtonReleased (object sender, System.EventArgs e)
        {
            backgroundColorButton.Sensitive = backgroundCheckButton.Active;
        }

        protected virtual void OnRemButtonReleased (object sender, System.EventArgs e)
        {
            ((Gtk.Container)(this.Parent)).Remove(this);
        }
    }
}
