// streamview.cs
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
using Gtk;

namespace sermon2
{
    
    
    public partial class streamview : Gtk.Window
    {
        
        public streamview() : 
                base(Gtk.WindowType.Toplevel){
            this.Build();
        }
    
        public void RegisterTag(TextTag tag, object sender){
            hexview1.RegisterTag(tag, this);
        }
        
        protected virtual void OnClearActionActivated (object sender, System.EventArgs e)
        {
            hexview1.Clear();
        }

        protected virtual void OnLoremIpsumActionActivated (object sender, System.EventArgs e)
        {
            string loremipsum = 
            "Lorem ipsum dolor sit amet, consectetur adipisci elit. Nunc ac ante sed ante imperdiet auctor."+
            "Fusce dignissim, magna eu feugiat tincidunt, nibh metus tincidunt augue, quis ullamcorper lorem"+
            "pede a ante. Proin congue nisl a arcu. Donec et elit. Etiam ac eros nec metus molestie aliquam."+
            "Nullam vestibulum molestie magna. In varius quam in nulla luctus tristique. Nam et eros."+
            "Sed vitae sem a velit mattis dapibus. Sed blandit, sapien auctor adipiscing viverra, purus urna "+
            "fermentum wisi, id luctus tortor augue et ligula. In quis libero. Sed urna arcu, malesuada in, "+
            "adipiscing vitae, vehicula vitae, magna. Phasellus sit amet nisl at erat aliquet eleifend. Quisque "+
            "malesuada porta elit. Nulla nec orci ac leo posuere eleifend. Aliquam ultrices vulputate velit. "+
            "Vestibulum vitae ipsum. Vestibulum pede erat, cursus nec, porttitor ac, accumsan ut, neque.";
            int i;
            for(i=0; i<1000; ++i){
                hexview1.addData(new byte[]{(byte)(loremipsum[i%loremipsum.Length])}, new Gtk.TextTag[]{});
            }
        }
        
        public void OnDataReciveTagged(byte data, TextTag[] tags, object sender){
            hexview1.addData(new byte[] {data}, tags);
        }
        
    }
}
