// lop.cs
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
    
    
    public class lop
    {
        bool escaped;
        byte[] msgbuffer;
        const byte reset =0x06;
        const byte escape=0x05;
        const byte esc_reset = 0x01;
        const byte esc_esc   = 0x02;
        const byte esc_xon   = 0x03;
        const byte esc_xoff  = 0x04;
        const byte type_msg  = 0x14;
        const byte type_sync = 0x15;
        
        byte[] escaped_codes={escape, reset, 0x11, 0x13};
        private enum state_t {idle, stream, message};
        private state_t rxstate;
        private uint byteidx;
        private uint msglength;
        
        
        public delegate void streambyterx_t(byte b, object sender);
        public delegate void streamsyncrx_t(object sender);
        public delegate void messagerx_t(byte[] msg, object sender);
        
        public event streambyterx_t streambyterx;
        public event streamsyncrx_t streamsyncrx;
        public event messagerx_t    messagerx;
        
        
        public lop(){
            escaped=false;   
            rxstate=state_t.stream;
        }
        
        public void rxbyte(byte b){
            if(escaped){
                escaped = false;
                switch(b){
                    case type_sync:
                        if(streamsyncrx!=null)
                            streamsyncrx(this);
                        return;
                    case type_msg:
                        rxstate=state_t.message;
                        byteidx=0;
                        msglength=0;
                        return;
                    default: break;
                }
                if(1<=b && b<=4){
                    layer2(escaped_codes[b-1]);
                    return;
                }
                System.Console.WriteLine("Ooops got escaped 0x{0:X2}", b);
            } else {
                if(b==escape){
                    escaped = true;
                } else {
                    layer2(b);
                }
            }
        }
        
        private void layer2(byte b){
            if(rxstate==state_t.stream){
                if(streambyterx!=null){
                    streambyterx(b, this);
                }
            }
            
            if(rxstate==state_t.message){
                switch (byteidx){
                    case 0: msglength=(uint)b*256;
                        break;
                    case 1: msglength+=b;
                        msgbuffer = new byte[msglength];
                        break;
                    default:
                        msgbuffer[byteidx-2]=b;
                        break;
                }
                byteidx++;
                if(msglength+2==byteidx){
                    /* recieved last byte of message */
                    if(messagerx!=null){
                        messagerx(msgbuffer, this);
                    }
                    rxstate = state_t.stream;
                }
            }
        }
        
    }
}
