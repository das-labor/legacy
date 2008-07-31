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
using System.IO.Ports;
namespace sermon2
{
    public delegate void dataChanged_t(object sender);
    
    public class SerPortConfig : ConfigBase
    {
        public string portName;
        public uint baudRate;
        public uint dataBits;
        public Parity parity;
        public StopBits stopBits;
        public bool hwFlowControl;
        public bool xonxoffFilter;
        public event dataChanged_t dataChanged;
        public SerPortConfig(string id): base(id)
        {
            this.id = id;
            LoadConfig();
        }
        
        public void SaveConfig(){
            writeconfig("name", name);
            writeconfig("portname", portName);
            writeconfig("baudrate", baudRate);
            writeconfig("databits", dataBits);
            writeconfig("parity",   (uint)parity);
            writeconfig("stopbits", (uint)stopBits);
            writeconfig("XonXoff_filter", xonxoffFilter);
            writeconfig("hwflowcontrol", hwFlowControl);
        }
        
        public void LoadConfig(){
            name     = readconfig("name", "default_port_name");
            portName = readconfig("portname", "/dev/ttyS0");
            baudRate = readconfig("baudrate", (uint)9600);
            dataBits = readconfig("databits", (uint)8);
            parity   = (Parity)readconfig("parity", (uint)Parity.None);
            stopBits = (StopBits)readconfig("stopbits", (uint)StopBits.One);
            xonxoffFilter = readconfig("XonXoff_filter", false);
            hwFlowControl = readconfig("hwflowcontrol", false);
        }
        
        public void OnGUI_Changed(){
            SaveConfig();
            if(dataChanged!=null){
                System.Console.WriteLine("data changed 1");
                dataChanged(this);                
            }
        }
        
        public void OnGConf_Changed(){
            LoadConfig();
            if(dataChanged!=null){
                System.Console.WriteLine("data changed 2");
                dataChanged(this);                
            }
        }
        
    }
}
