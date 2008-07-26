// MainWindow.cs
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

public partial class MainWindow: Gtk.Window
{    
    public SerPortConf[] spconfs;
    public TaggerConf[]  tgconfs;
    public LopConf[]     lopconfs;

    public MainWindow (): base (Gtk.WindowType.Toplevel)
    {
        Build ();
        spconfs = new SerPortConf[0];
        tgconfs = new TaggerConf[0];
        lopconfs = new LopConf[0];
    }
    
    protected void OnDeleteEvent (object sender, DeleteEventArgs a)
    {
        Application.Quit ();
        a.RetVal = true;
    }

    protected virtual void OnButton1Released (object sender, System.EventArgs e)
    {
        sermon2.streamview sv;
        sv = new sermon2.streamview();
        
    }

    protected virtual void OnNewObjButtonReleased (object sender, System.EventArgs e)
    {
        switch(objectAddComboBox.Active){
            case 0: /* serial port config */
                Array.Resize(ref spconfs, spconfs.Length+1);
                spconfs[spconfs.Length-1] = new SerPortConf((uint)(spconfs.Length-1));
                vbox1.Add(spconfs[spconfs.Length-1]);
                break;
            case 1: /* tagger config */    
                Array.Resize(ref tgconfs, tgconfs.Length+1);
                tgconfs[tgconfs.Length-1] = new TaggerConf((uint)(tgconfs.Length-1));
                vbox1.Add(tgconfs[tgconfs.Length-1]);
                break;
            case 2: /* lop config */    
                Array.Resize(ref lopconfs, lopconfs.Length+1);
                lopconfs[lopconfs.Length-1] = new LopConf((uint)(lopconfs.Length-1));
                vbox1.Add(lopconfs[lopconfs.Length-1]);
                break;
                
            default: break;
        }
    /*
        Array.Resize(ref spconfs, spconfs.Length+1);
        spconfs[spconfs.Length-1] = new SerPortConf(String.Format("port {0}", spconfs.Length-1));
        Array.Resize(ref tgconfs, tgconfs.Length+1);
        tgconfs[tgconfs.Length-1] = new TaggerConf();
                
        vbox1.Add(spconfs[spconfs.Length-1]);
        vbox1.ShowAll();
       */
    }

    protected virtual void OnButton24Released (object sender, System.EventArgs e)
    {
        
    }
}

}

