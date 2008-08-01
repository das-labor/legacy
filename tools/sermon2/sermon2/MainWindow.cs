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
    private string[]     objNames={"serial port", "tagger", "LOP", "QPort", "streamview", "LOPview"}; 
    public MainWindow (): base (Gtk.WindowType.Toplevel)
    {
        Build ();
        ports   = new SerPort[0];
        taggers = new Tagger[0];
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
                if(s.StartsWith("tagger-")){
                    try{
                        AddTagger(s, false);
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
        sv.ShowAll();
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
        ports[ports.Length-1].config.dataChanged += SerialPortObjectDataChanged;
    }

    public void AddTagger(string id, bool newname){
        Array.Resize(ref taggers, taggers.Length +1);
        taggers[taggers.Length-1] = new Tagger(id);
        if(newname){
            taggers[taggers.Length-1].config.name = "tagger "
                          + (taggers.Length-1).ToString();
           taggers[taggers.Length-1].config.SaveConfig();
        }
        configTreeStore.AppendValues(treeIters[1], 
               taggers[taggers.Length-1].config.name);
        taggers[taggers.Length-1].config.dataChanged += TaggerObjectDataChanged;
    }
    protected virtual void OnNewObjButtonReleased (object sender, System.EventArgs e)
    {
        if(scrolledwindow2.Child!=null)
                    scrolledwindow2.Remove(scrolledwindow2.Child);
        switch(objectAddComboBox.Active){
            case 0: /* serial port config */
                AddSerialPort("serialport-"+prng.Next().ToString(), true);
                break;
            case 1: /* tagger config */    
                AddTagger("tagger-"+prng.Next().ToString(), true);
                break;
            case 2: /* lop config */    
                scrolledwindow2.Add(lopconf);
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
     /*   
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
    */
    }

    public void SerialPortObjectDataChanged(object changer, object changed){
        int i;
        for(i=0; ((ConfigBase)changed).id!=ports[i].config.id && i<ports.Length; ++i)
            ;
        if(i==ports.Length)
            return;
        TreeIter iter;
        iter = new TreeIter();
        System.Console.WriteLine("tree-update [serport] "+ System.DateTimeOffset.Now.ToString());
        configTreeStore.GetIter(out iter, new TreePath(new int[]{0, i}));
        configTreeStore.SetValue(iter, 0, ports[i].config.name);
    }

    public void TaggerObjectDataChanged(object changer, object changed){
        int i;
        for(i=0; ((ConfigBase)changed).id!=taggers[i].config.id && i<taggers.Length; ++i)
            ;
        if(i==taggers.Length)
            return;
        TreeIter iter;
        iter = new TreeIter();
        System.Console.WriteLine("tree-update [tagger] "+ System.DateTimeOffset.Now.ToString());
        configTreeStore.GetIter(out iter, new TreePath(new int[]{1, i}));
        configTreeStore.SetValue(iter, 0, taggers[i].config.name);
    }

    protected virtual void OnAboutActionActivated (object sender, System.EventArgs e){
        AboutDialog abd;
        abd = new AboutDialog();
        abd.ShowAll();
    }

    protected virtual void OnConfigTreeViewRowActivated (object o, Gtk.RowActivatedArgs args){
        
         if(scrolledwindow2.Child!=null)
            scrolledwindow2.Remove(scrolledwindow2.Child);
         if(commonConfScrolledWindow.Child!=null)
            commonConfScrolledWindow.Remove(commonConfScrolledWindow.Child);
         if(args.Path.Indices.Length==2 && args.Path.Indices[0]==0){
            scrolledwindow2.Add(spconf= new SerPortConf(
                    ports[args.Path.Indices[1]].config));
            commonConfScrolledWindow.Add(new ObjectCommonEdit( 
                    ports[args.Path.Indices[1]].config));
         }
         if(args.Path.Indices.Length==2 && args.Path.Indices[0]==1){
            scrolledwindow2.Add(tgconf= new TaggerConf(
                    taggers[args.Path.Indices[1]].config));
            commonConfScrolledWindow.Add(new ObjectCommonEdit( 
                    taggers[args.Path.Indices[1]].config));
         }
    }
    
    
}

}

