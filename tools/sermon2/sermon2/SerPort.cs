// serport.cs
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
using System.Threading;

namespace sermon2
{
    
    public delegate void streambyterx_t(byte b, object sender);
    
    public class SerPort
    {
        public  event streambyterx_t byterx_event;
        private SerialPort port;
        private Thread t;
        public SerPortConfig config;
        
        public SerPort(string id)
        {   
            config = new SerPortConfig(id);
        }
        
        public SerPort(string id, string name):this(id)
        {   
            config.name = name;
            config.SaveConfig();
        }
        
        public void Open(){
            port = new SerialPort(
                    config.portName,
                    (int)config.baudRate,
                    config.parity,
                    (int)config.dataBits,
                    config.stopBits);
            t = new Thread(new ThreadStart(pollloop));        
            port.Open();
            t.Start();
        }
        
        public void Close(){
          //  t.Suspend();
            t.Abort();
            port.Close();
        }
        
        private void pollloop(){
            while(true){
                while(port.BytesToRead==0){
                }
                byterx_event((byte)port.ReadByte(), this);
            }
        }
    }
}
