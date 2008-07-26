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
        
        public SerPortConf()
        {
            this.Build();
            
            foreach(String s in System.IO.Ports.SerialPort.GetPortNames()){
                portNameComboBoxEntry.AppendText(s);
            }
            baudRateComboBox.Active = 12; /* 9600 baud */
            dataBitsComboBox.Active = 3;  /* 8 bits */
            parityComboBox.Active = (int)System.IO.Ports.Parity.None;
            stopBitsComboBox.Active = (int)System.IO.Ports.StopBits.One;
        }
        
        public SerPortConf(string name) : this()
        {
            nameLabel.Markup = "<b>" + name + "</b>";
        }
        
        public SerPortConf(uint i) : this()
        {
            nameLabel.Markup = "<b>port " + i.ToString() + "</b>";
        }

        protected virtual void OnDeleteButtonReleased (object sender, System.EventArgs e)
        {
            ((Gtk.Container)(this.Parent)).Remove(this);
        }
    }
}
