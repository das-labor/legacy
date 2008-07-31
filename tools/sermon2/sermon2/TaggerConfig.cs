// TaggerConfig.cs
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

namespace sermon2
{
    
    
    public class TaggerConfig : ConfigBase
    {
        private bool dataTag;
        private TextTag defTag;
        private TextTag[] dataTags;
        
        public TaggerConfig(string id):base(id){
            this.id = id;
        }
        
        public void SaveConfig(){
            writeconfig("tagdata", dataTag);
            writeconfig("foreground_set", defTag.ForegroundSet);
            writeconfig("foreground", defTag.ForegroundGdk);
            writeconfig("background_set", defTag.BackgroundSet);
            writeconfig("background", defTag.BackgroundGdk);
            for(int i=0; i<256; ++i){
                if(dataTags[i]!=null){
                  writeconfig("foreground_" + string.Format("{0:X2}", i)+"_set", defTag.ForegroundSet);
                  writeconfig("foreground_" + string.Format("{0:X2}", i), defTag.ForegroundGdk);
                  writeconfig("background_" + string.Format("{0:X2}", i)+"_set", defTag.BackgroundSet);
                  writeconfig("background_" + string.Format("{0:X2}", i), defTag.BackgroundGdk);
                }
            }
            
        }
        
        public void LoadConfig(){
            dataTag = readconfig("tagdata", false);
            defTag.ForegroundSet = readconfig("foreground_set", true);
            defTag.ForegroundGdk = readconfig("foreground", new Gdk.Color(0xff, 0x00, 0x00));
            defTag.BackgroundSet = readconfig("background_set", false);
            defTag.BackgroundGdk = readconfig("background", new Gdk.Color(0x00, 0x00, 0x00));
            for(int i=0; i<256; ++i){
              try{
                dataTags[i].ForegroundSet =
                  readconfig("foreground_" + string.Format("{0:X2}", i)+"_set", defTag.ForegroundSet);
                dataTags[i].ForegroundGdk =
                  readconfig("foreground_" + string.Format("{0:X2}", i), defTag.ForegroundGdk);
                dataTags[i].BackgroundSet =
                  readconfig("background_" + string.Format("{0:X2}", i)+"_set", defTag.BackgroundSet);
                dataTags[i].BackgroundGdk =
                  readconfig("background_" + string.Format("{0:X2}", i), defTag.BackgroundGdk);
              }catch{
                dataTags[i]=null;
              }
            }
        }     
    }
}
