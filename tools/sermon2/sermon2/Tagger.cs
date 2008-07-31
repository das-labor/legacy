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
        private String name;
        private bool dataTag;
        private TextTag defTag;
        private TextTag[] dataTags;
        public TaggerConfig config;
        public tagedbyterx_t tagedbyterx;
        public tagregister_t tagregister;
        public Tagger(String name)
        {
            this.name = name;
            dataTags = new TextTag[256];
            dataTag = false;
            
        }
        
        public TextTag DefTag{
            get{
                return defTag;
            }
            set{
                defTag = value;
                tagregister(defTag, this);
            }
        }
        
        public bool DataTag{
            get{
                return dataTag;
            }
            set{
                dataTag = value;
            }
        }
        
        public String Name{
            get{
                return this.name;
            }
        }
        public void SetDataTag(byte data, TextTag tag){
            dataTags[data] = tag;
            tagregister(tag, this);
        }
        
        public void OnDataRX(byte data, object sender){
            TextTag[] tags;
            if(dataTag && dataTags[data] != null){
                tags= new TextTag[2];
                tags[0] = defTag;
                tags[1] = dataTags[data];
            } else {
                tags= new TextTag[1];
                tags[0] = defTag;
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
