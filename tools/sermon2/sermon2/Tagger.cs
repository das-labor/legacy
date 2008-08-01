// Tagger.cs
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
    
    
    public delegate void tagedbyterx_t(byte b, TextTag[] tags,object sender);
    public delegate void tagregister_t(TextTag tag,object sender);
    
    public class Tagger
    {
        public TaggerConfig config;
        public event tagedbyterx_t tagedbyterx;
        public event tagregister_t tagregister;
        
        public Tagger(string id){
            config = new TaggerConfig(id);   
        }
        
        public Tagger(string id, string name):this(id){
            config.name = name;
            config.SaveConfig();
        }
                
        public void OnDataRX(byte data, object sender){
            TextTag[] tags;
            if(config.dataTag && config.dataTags[data] != null){
                tags= new TextTag[2];
                tags[0] = config.defTag;
                tags[1] = config.dataTags[data];
            } else {
                tags= new TextTag[1];
                tags[0] = config.defTag;
            }
            tagedbyterx(data, tags, this);
        }
        /*
        public void OnDataRX(byte[] data, object sender){
            foreach(byte d in data){
                OnDataRX(d);
            }
        }
        */        
        
    }
}
