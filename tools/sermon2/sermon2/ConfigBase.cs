// ConfigBase.cs
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
using Gdk;
using GConf;

namespace sermon2
{
    public delegate void dataChanged_t(object changer, object changed);
   
    
    abstract public class ConfigBase
    {
        public const string AppBase="/apps/sermon2";
     
        private string keyBase;
        private string keyName;
        private string keyFullName;
        private string keyFullName_;
        private GConf.Client client;
        public  string name;
        public  string id;
        public event dataChanged_t dataChanged;
        public ConfigBase(String baseName, String Name)
        {
            client = new GConf.Client();
            keyBase = baseName.TrimEnd('/');
            keyName = Name.TrimEnd('/').TrimStart('/');
            keyFullName = keyBase + "/" +keyName;
            keyFullName_ = keyFullName+"/";
            string[] sa;
            try{
                sa=(string[])client.Get(AppBase+"/objects");
            }catch{
                sa = new string[0];
                client.Set(AppBase+"/objects", sa);
            }
            
            if(Array.FindIndex(sa, Name.Equals) == -1){
                Array.Resize(ref sa, sa.Length+1);
                sa[sa.Length-1] = Name;
                client.Set(AppBase+"/objects", sa);
            }
            client.AddNotify(keyFullName, 
                 new NotifyEventHandler(OnGConf_Changed));
           // client.AddNotify(keyFullName, 
           //      new NotifyEventHandler(beep));
            System.Console.WriteLine("Listening on "+keyFullName);
        }
        
        public void beep(object sender, NotifyEventArgs args){
            System.Console.WriteLine("beep");
        }
        
        public ConfigBase(String Name) : this(AppBase, Name)
        {
        }
        
        public void writeconfig(String name, String val){
            client.Set(keyFullName_+name, val);
        }
        
        public String readconfig(String name, String def){
            String s;
            try{
                s = (String)client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
 
        public void writeconfig(String name, String[] val){
            client.Set(keyFullName_+name, val);
        }
        
        public String[] readconfig(String name, String[] def){
            String[] s;
            try{
                s = (String[])client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        public void writeconfig(String name, bool val){
            client.Set(keyFullName_+name, val);
        }

        public bool readconfig(String name, bool def){
            bool s;
            try{
                s = (bool)client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        public void writeconfig(String name, bool[] val){
            client.Set(keyFullName_+name, val);
        }

        public bool[] readconfig(String name, bool[] def){
            bool[] s;
            try{
                s = (bool[])client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        public void writeconfig(String name, int val){
            client.Set(keyFullName_+name, val);
        }

        public int readconfig(String name, int def){
            int s;
            try{
                s = (int)client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        public void writeconfig(String name, int[] val){
            client.Set(keyFullName_+name, val);
        }

        public int[] readconfig(String name, int[] def){
            int[] s;
            try{
                s = (int[])client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        public void writeconfig(String name, Color val){
            int[] ca=new int[3];
            ca[0] = (int)val.Red;
            ca[1] = (int)val.Green;
            ca[2] = (int)val.Red;
            writeconfig(name, ca);
        }
        
        public Color readconfig(String name, Color def){
            Color c;
            c = new Color();
            int[] ca=new int[3];
            ca[0] = (int)def.Red;
            ca[1] = (int)def.Green;
            ca[2] = (int)def.Red;
            ca = readconfig(name, ca);
            try{
                c.Red   = (ushort)ca[0];
                c.Green = (ushort)ca[1];
                c.Blue  = (ushort)ca[2];
            }catch{
                ca[0] = (int)def.Red;
                ca[1] = (int)def.Green;
                ca[2] = (int)def.Red;
                writeconfig(name, ca);
                c = def;                    
            }
            return c;
        }
        
        public void writeconfig(String name, uint val){
            client.Set(keyFullName_+name, (int)val);
        }

        public uint readconfig(String name, uint def){
            uint s;
            try{
                s = (uint)client.Get(keyFullName_+name);
            }catch{
                writeconfig(name, def);
                s = def;
            }
            return s;
        }
        
        abstract public void SaveConfig();
        
        abstract public void LoadConfig();
        
        abstract public void OnGConf_Changed(object sender, NotifyEventArgs args);
        
        abstract public void OnGUI_Changed(object sender,  NotifyEventArgs args);
        
        public void OnDataChanged(object changer, object changed){
            if(dataChanged!= null)
                dataChanged(changer, changed);
        }
    }
}
