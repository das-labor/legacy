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

namespace Application
{
    
    public class ConfigBase
    {
        private String keyBase;
        private String keyName;
        private String keyFullName;
        private String keyFullName_;
        private GConf.Client client;
        public ConfigBase(String baseName, String Name)
        {
            client = new GConf.Client();
            keyBase = baseName.TrimEnd('/');
            keyName = Name.TrimEnd('/').TrimStart('/');
            keyFullName = keyBase + "/" +keyName;
            keyFullName_ = keyFullName+"/";
            client.AddNotify(keyFullName, 
                   new NotifyEventHandler(OnGConf_Changed));
        }
        
        public void write(String name, String val){
            client.Set(keyFullName_+name, val);
        }
        
        public String read(String name, String def){
            String s;
            try{
                s = (String)client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
 
        public void write(String name, String[] val){
            client.Set(keyFullName_+name, val);
        }
        
        public String[] read(String name, String[] def){
            String[] s;
            try{
                s = (String[])client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
        
        public void write(String name, bool val){
            client.Set(keyFullName_+name, val);
        }

        public bool read(String name, bool def){
            bool s;
            try{
                s = (bool)client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
        
        public void write(String name, bool[] val){
            client.Set(keyFullName_+name, val);
        }

        public bool[] read(String name, bool[] def){
            bool[] s;
            try{
                s = (bool[])client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
        
        public void write(String name, int val){
            client.Set(keyFullName_+name, val);
        }

        public int read(String name, int def){
            int s;
            try{
                s = (int)client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
        
        public void write(String name, int[] val){
            client.Set(keyFullName_+name, val);
        }

        public int[] read(String name, int[] def){
            int[] s;
            try{
                s = (int[])client.Get(keyFullName_+name);
            }catch{
                write(name, def);
                s = def;
            }
            return s;
        }
        
        public void write(String name, Color val){
            int[] ca=new int[3];
            ca[0] = (int)val.Red;
            ca[1] = (int)val.Green;
            ca[2] = (int)val.Red;
            write(name, ca);
        }
        
        public Color read(String name, Color def){
            Color c;
            c = new Color();
            int[] ca=new int[3];
            ca[0] = (int)def.Red;
            ca[1] = (int)def.Green;
            ca[2] = (int)def.Red;
            ca = read(name, ca);
            try{
                c.Red   = (ushort)ca[0];
                c.Green = (ushort)ca[1];
                c.Blue  = (ushort)ca[2];
            }catch{
                ca[0] = (int)def.Red;
                ca[1] = (int)def.Green;
                ca[2] = (int)def.Red;
                write(name, ca);
                c = def;                    
            }
            return c;
        }
        
        virtual public void OnGConf_Changed(object sender, NotifyEventArgs args){
        }
        
        virtual public void OnGUI_Changed(object sender,  NotifyEventArgs args){
        }
    }
}
