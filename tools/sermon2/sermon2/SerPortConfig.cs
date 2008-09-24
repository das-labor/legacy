// serortConfig.cs
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
using GConf;
using System.IO.Ports;
namespace sermon2
{
     
    public class SerPortConfig : ConfigBase
    {
        public string portName;
        public uint baudRate;
        public uint dataBits;
        public Parity parity;
        public StopBits stopBits;
        public bool hwFlowControl;
        public bool xonxoffFilter;
        
        public SerPortConfig(string id): base(id)
        {
            this.id = id;
            LoadConfig();
        }
        
        override public void SaveConfig_core(){
            writeconfig("name", name);
            writeconfig("portname", portName);
            writeconfig("baudrate", baudRate);
            writeconfig("databits", dataBits);
            writeconfig("parity",   (uint)parity);
            writeconfig("stopbits", (uint)stopBits);
            writeconfig("XonXoff_filter", xonxoffFilter);
            writeconfig("hwflowcontrol", hwFlowControl);
        }
        
        override public void LoadConfig(){
            name     = readconfig("name", "default_port_name");
            portName = readconfig("portname", "/dev/ttyS0");
            baudRate = readconfig("baudrate", (uint)9600);
            dataBits = readconfig("databits", (uint)8);
            parity   = (Parity)readconfig("parity", (uint)Parity.None);
            stopBits = (StopBits)readconfig("stopbits", (uint)StopBits.One);
            xonxoffFilter = readconfig("XonXoff_filter", false);
            hwFlowControl = readconfig("hwflowcontrol", false);
        }
        
        public void OnGUI_Changed(object changer){
            SaveConfig();
            OnDataChanged(changer, this);                
        }
        
        public void OnGConf_Changed(object changer){
            if(noGconfLoad)
                return;
            System.Console.WriteLine("OnGConf_Changed " + System.DateTimeOffset.Now.ToString());
            OnDataChanged(changer, this);             
        }
        
        
        override public void OnGConf_Changed(object sender, NotifyEventArgs args){
            if(noGconfLoad)
                return;
            System.Console.WriteLine("OnGConf_Changed " + System.DateTimeOffset.Now.ToString());
            OnDataChanged(this, this);             
        }
        
        override public void OnGUI_Changed(object sender,  NotifyEventArgs args){
            SaveConfig();
            OnDataChanged(sender, this);                
        }
        
    }
}
