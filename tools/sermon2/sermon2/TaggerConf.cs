// TaggerConf.cs
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
    
    
    public partial class TaggerConf : Gtk.Bin
    {
        private TaggerDataConf[] tgdc;
        public TaggerConf()
        {
            this.Build();
            backgroundCButton.Color = new Gdk.Color(0xff, 0xff, 0xff);     
            tgdc = new TaggerDataConf[0];
            foregroundCButton.Sensitive = foregroundCheckButton.Active;
            backgroundCButton.Sensitive = backgroundCheckButton.Active;
            frame2.Sensitive = dataCheckButton.Active;
        }

        public TaggerConf(String s) : this()
        {
            frameLabel.Markup = "<b>" + s + "</b>";
        }
        
        public TaggerConf(uint i) : this()
        {
            frameLabel.Markup = "<b>tagger " + i.ToString() + "</b>";
        }

        protected virtual void OnAddTaggerDataConfbuttonReleased (object sender, System.EventArgs e)
        {
            Array.Resize(ref tgdc, tgdc.Length+1);
            System.Console.WriteLine("a");
            tgdc[tgdc.Length-1] = new TaggerDataConf();
            System.Console.WriteLine("b");
            vbox1.Add(tgdc[tgdc.Length-1]);
            System.Console.WriteLine("c");
          //  vbox2.
            vbox1.ShowAll();
            
            System.Console.WriteLine("d");
        }

        protected virtual void OnRemoveButtonReleased (object sender, System.EventArgs e)
        {
            ((Gtk.Container)(this.Parent)).Remove(this);
        }

        protected virtual void OnForegroundCheckButtonReleased (object sender, System.EventArgs e)
        {
            foregroundCButton.Sensitive = foregroundCheckButton.Active;
        }

        protected virtual void OnBackgroundCheckButtonReleased (object sender, System.EventArgs e)
        {
            backgroundCButton.Sensitive = backgroundCheckButton.Active;
        }

        protected virtual void OnDataCheckButtonReleased (object sender, System.EventArgs e)
        {
            frame2.Sensitive = dataCheckButton.Active;
        }
        
    }
}
