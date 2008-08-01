// SerPortConf.cs
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
 
    public partial class SerPortConf : Gtk.Bin
    {
        static uint[] baudTable = new uint[]{
                50,    75,   110,   134,   150,
               200,   300,   600,  1200,  1800,
              2400,  4800,  9600, 19200, 38400,
             57600, 115200, 230400  };
        static string[] portNameTable = new string[0];
        
        private SerPortConfig config;
        public SerPortConf(SerPortConfig config)
        {
            int i;
            this.Build();
            this.config = config;
            for(i=0; i<baudTable.Length; ++i){
                baudRateComboBox.AppendText(baudTable[i].ToString());
            }
            for(i=0; i<portNameTable.Length; ++i){
                portNameComboBoxEntry.AppendText(portNameTable[i]);
            }
            foreach(String s in System.IO.Ports.SerialPort.GetPortNames()){
                i = Array.FindIndex(portNameTable, s.Equals);
                if(i==-1){
                    Array.Resize(ref portNameTable, portNameTable.Length+1);
                    portNameTable[portNameTable.Length-1] = s;
                    portNameComboBoxEntry.AppendText(s);
                }
            }
            config.LoadConfig();
            SetGuiFromConfig();
            config.dataChanged += gconf_changed;
            System.Console.WriteLine("xwidth = " + baudRateComboBox.Allocation.X);
        }
        
        public void gconf_changed(object changer, object changed){
            config.LoadConfig();
            SetGuiFromConfig();
        }
        
        public void SetGuiFromConfig(){
            nameLabel.Markup = "<b>" 
                               + config.id 
                               + " / " 
                               + config.name 
                               + "</b>";            
            int i;
            
            i = Array.FindIndex(portNameTable, config.portName.Equals);
            if(i!=-1){
                portNameComboBoxEntry.Active = i;
            } else {
                Array.Resize(ref portNameTable, portNameTable.Length+1);
                portNameTable[portNameTable.Length-1] = config.portName;
                portNameComboBoxEntry.AppendText(config.portName);
                portNameComboBoxEntry.Active = portNameTable.Length-1;
            }
            int baudidx=0;
            while((baudTable[baudidx]!=config.baudRate) 
                  && (baudidx<baudTable.Length)){
                  baudidx++;
            }
            if(baudidx==baudTable.Length){
                /* we simply add this new baudrate */
                Array.Resize(ref baudTable, baudTable.Length+1);
                baudTable[baudidx]=config.baudRate;
                baudRateComboBox.AppendText(baudTable[baudidx].ToString());
            }
            baudRateComboBox.Active = baudidx;
            dataBitsComboBox.Active = (int)config.dataBits - 5;  
            parityComboBox.Active = (int)config.parity;
            stopBitsComboBox.Active = (int)config.stopBits;
            xonxoffCheckButton.Active = config.xonxoffFilter;
            hwFlowControlCheckButton.Active = config.hwFlowControl;
        }

        protected virtual void OnPortNameComboBoxEntryChanged (object sender, System.EventArgs e)
        {
            config.portName = portNameComboBoxEntry.ActiveText;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnDataBitsComboBoxChanged (object sender, System.EventArgs e)
        {
            config.dataBits = (uint)(dataBitsComboBox.Active+5);
            config.OnGUI_Changed(this);
        }

        protected virtual void OnStopBitsComboBoxChanged (object sender, System.EventArgs e)
        {
            config.stopBits = (System.IO.Ports.StopBits)stopBitsComboBox.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnBaudRateComboBoxChanged (object sender, System.EventArgs e)
        {
            config.baudRate = uint.Parse(baudRateComboBox.ActiveText);
            config.OnGUI_Changed(this);
        }

        protected virtual void OnParityComboBoxChanged (object sender, System.EventArgs e)
        {
            config.parity = (System.IO.Ports.Parity)parityComboBox.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnXonxoffCheckButtonReleased (object sender, System.EventArgs e)
        {
            config.xonxoffFilter = xonxoffCheckButton.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnHwFlowControlCheckButtonReleased (object sender, System.EventArgs e)
        {
            config.hwFlowControl = hwFlowControlCheckButton.Active;
            config.OnGUI_Changed(this);
        }
    }
}
