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
//using Application;
using Gtk;
using GConf;

namespace sermon2
{

public partial class MainWindow: Gtk.Window
{   
    const string AppBase = "/apps/sermon2";
 
    public SerPortConf   spconf;
    public TaggerConf    tgconf;
    public LopConf       lopconf;
    public SerPort[]     ports;
    public Tagger[]      taggers;
    
    private Random       prng;
    private GConf.Client gcclient;
    private TreeStore    configTreeStore;
    private TreeViewColumn configTreeColumn;
    private TreeIter[]   treeIters;
    private string[]     objNames={"serial port", "tagger", "LOP", "streamview", "LOPview"}; 
    public MainWindow (): base (Gtk.WindowType.Toplevel)
    {
        Build ();
        ports  =  new SerPort[0];
        prng = new Random();
        configTreeStore = new TreeStore(typeof(string));
        configTreeView.Model = configTreeStore;
        configTreeColumn = new TreeViewColumn("Object", new CellRendererText(), "text", 0);
        configTreeView.AppendColumn(configTreeColumn);
        treeIters = new TreeIter[objNames.Length];
        gcclient = new GConf.Client();
        for(int i=0; i<objNames.Length; ++i){
            
            treeIters[i] = configTreeStore.AppendValues(objNames[i]);
        }
        try{
            string[] sa;
            sa = (string[])(gcclient.Get(AppBase+"/objects"));
            foreach(string s in sa){
                System.Console.WriteLine("found object: "+s);
                if(s.StartsWith("serialport-")){
                    try{
                        AddSerialPort(s, false);
                    }catch{
                    }
                }
            }
        }catch{
            System.Console.WriteLine("Ooops, no objects found");    
        }
    }
    
    
    protected void OnDeleteEvent (object sender, DeleteEventArgs a)
    {
        Gtk.Application.Quit();  
        a.RetVal = true;
    }

    protected virtual void OnButton1Released (object sender, System.EventArgs e)
    {
        sermon2.streamview sv;
        sv = new sermon2.streamview();
        
    }
    
    public void AddSerialPort(string id, bool newname){
        Array.Resize(ref ports, ports.Length +1);
        ports[ports.Length-1] = new SerPort(id);
        if(newname){
            ports[ports.Length-1].config.name = "port "
                          + (ports.Length-1).ToString();
           ports[ports.Length-1].config.SaveConfig();
        }
        configTreeStore.AppendValues(treeIters[0], 
               ports[ports.Length-1].config.name);
    }

    protected virtual void OnNewObjButtonReleased (object sender, System.EventArgs e)
    {
        if(scrolledwindow1.Child!=null)
                    scrolledwindow1.Remove(scrolledwindow1.Child);
        switch(objectAddComboBox.Active){
            case 0: /* serial port config */
                AddSerialPort("serialport-"+prng.Next().ToString(), true);
                break;
            case 1: /* tagger config */    
                scrolledwindow1.Add(tgconf);
                configTreeStore.AppendValues(treeIters[objectAddComboBox.Active], "tagger 0");
                break;
            case 2: /* lop config */    
                scrolledwindow1.Add(lopconf);
                configTreeStore.AppendValues(treeIters[objectAddComboBox.Active], "lop 0");
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
        GConf.Client client = new Client();
        System.Object o;
        o = new System.Object();
        o = client.Get("/apps");
        System.Console.WriteLine("o has type: {0}", o.GetType());
        return;
        SerPort p1,p2;
        Tagger  t1,t2;
        TextTag tag1, tag2;
        streamview sv;
        p1 = new SerPort("p1");
        p2 = new SerPort("p2");
        tag1 = new TextTag("tag1");
        tag1.Foreground = "#FF0000";
        tag2 = new TextTag("tag2");
        tag2.Foreground = "#0000FF";
        sv = new streamview();
        t1 = new Tagger("tagger 1");
        t1.tagregister += sv.RegisterTag;
        t2 = new Tagger("tagger 2");
        t2.tagregister += sv.RegisterTag;
        t1.DefTag = tag1;
        t2.DefTag = tag2;
        p1.byterx_event += (streambyterx_t)(t1.OnDataRX);
        p2.byterx_event += (streambyterx_t)(t2.OnDataRX);
        t1.tagedbyterx += sv.OnDataReciveTagged;
    }

    protected virtual void OnAboutActionActivated (object sender, System.EventArgs e){
        AboutDialog abd;
        abd = new AboutDialog();
    }

    protected virtual void OnConfigTreeViewRowActivated (object o, Gtk.RowActivatedArgs args){
         System.Console.WriteLine(args.Path.ToString());
         foreach (int i in args.Path.Indices){
            System.Console.Write("-> " + i.ToString());
         }
         if(scrolledwindow1.Child!=null)
            scrolledwindow1.Remove(scrolledwindow1.Child);
         System.Console.Write("\n");
         if(args.Path.Indices.Length==2 && args.Path.Indices[0]==0){
            scrolledwindow1.Add(spconf= new SerPortConf(
                    ports[args.Path.Indices[1]].config));
                     
         }
    }
    
    
}

}

