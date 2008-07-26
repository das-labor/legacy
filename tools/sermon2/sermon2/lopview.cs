// lopview.cs
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
    
    
    public partial class lopview : Gtk.Window
    {
        
        public lop[] lops;
        private uint lopcount=2;
        public TextTag[] tags;
        public TextTag[] texttags;
        public TextTag   syntag;
        public TextTag   textsyntag;
        private int tempi=0;
        private long lasttime=-1;
        
        public lopview() : base(Gtk.WindowType.Toplevel) {
            int i;

            Build();
            tags = new TextTag[2];
            tags[0] = new TextTag("t0");
            tags[1] = new TextTag("t1");
            syntag = new TextTag("sync");
            texttags = new TextTag[2];
            texttags[0] = new TextTag("tt0");
            texttags[1] = new TextTag("tt1");
            textsyntag = new TextTag("tsync");
            
            tags[0].ForegroundGdk = new Gdk.Color(0xff, 0x00, 0x00);
            tags[1].ForegroundGdk = new Gdk.Color(0x00, 0x00, 0xff);
            syntag.BackgroundGdk =  new Gdk.Color(0x00, 0xff, 0x00);
            texttags[0].ForegroundGdk = new Gdk.Color(0xff, 0x00, 0x00);
            texttags[1].ForegroundGdk = new Gdk.Color(0x00, 0x00, 0xff);
            textsyntag.BackgroundGdk =  new Gdk.Color(0x00, 0xff, 0x00);
            textview1.Buffer.TagTable.Add(tags[0]);
            textview1.Buffer.TagTable.Add(tags[1]);
            textview1.Buffer.TagTable.Add(syntag);
            textview2.Buffer.TagTable.Add(texttags[0]);
            textview2.Buffer.TagTable.Add(texttags[1]);
            textview2.Buffer.TagTable.Add(textsyntag);
            treeview1.Model = new ListStore(typeof(long), typeof(string), typeof(Gdk.Color));
            treeview1.AppendColumn("timestamp", new CellRendererText (), "text", 0);
            treeview1.AppendColumn("message", new CellRendererText (), "text", 1, "foreground-gdk", 2);   
            lops = new lop[2];
            for(i=0; i<lopcount; ++i){
                lops[i] = new lop();
                lops[i].messagerx += insert_message;
                lops[i].streambyterx += insert_stream;
                lops[i].streamsyncrx += insert_sync;
            }
 
        }
        
       public lopview(string fontdesc) :
              this()
       {
           textview1.ModifyFont(
		    Pango.FontDescription.FromString(fontdesc));
		   textview2.ModifyFont(
		    Pango.FontDescription.FromString(fontdesc));
		   treeview1.ModifyFont(
		    Pango.FontDescription.FromString(fontdesc)); 
       }
       
        public void insert_message(byte[] b, uint portno){
        string s = "";
        int i;
        for(i=0; i<b.Length; ++i){
            s += string.Format("{0:X2} ", b[i]);
            if(i%16==15){
                s += "\n";
            }
        }
        ((Gtk.ListStore)treeview1.Model).AppendValues((lasttime==-1)?0:System.DateTimeOffset.Now.Ticks-lasttime, s, this.tags[portno].ForegroundGdk);
        lasttime = System.DateTimeOffset.Now.Ticks;
    }
        
        public void insert_message(byte[] b, object sender){
            int i;
            i = Array.FindIndex(lops, sender.Equals);
            if(i==-1){
                System.Console.WriteLine("Ooops! could not associate sender of message");
            }
            insert_message(b, (uint)i);
        }
                
        public void insert_stream(byte b, object sender){
            int i;
            i = Array.FindIndex(lops, sender.Equals);
            if(i==-1){
                System.Console.WriteLine("Ooops! could not associate sender of streambyte {0:X2}", b);
            }
            insert_stream(b, (uint)i);
        }
        
        public void insert_sync(object sender){
            int i;
            i = Array.FindIndex(lops, sender.Equals);
            if(i==-1){
                System.Console.WriteLine("Ooops! could not associate sender of sync");
            }
            insert_sync((uint)i);
        }
        
        public void insert_stream(byte b, uint portno){
            TextIter enditer;
            string s;
            s = string.Format(" {0:X2}", b);
            if(portno>=lopcount){
                System.Console.WriteLine("Ooops! recieved 0x{0:X2} from port {1}", b, portno);
            }
            enditer = this.textview1.Buffer.EndIter;
            this.textview1.Buffer.InsertWithTags(ref enditer, s, tags[portno]);
            
            string c="";
            if(b>=32 && b<=127){
                c += (char)b;
            } else {
                c += '.';
            }
            enditer = this.textview2.Buffer.EndIter;
            this.textview2.Buffer.InsertWithTags(ref enditer, c, texttags[portno]);    
        }
        
        
        public void insert_sync(uint portno){
            TextIter enditer;
            if(portno>=lopcount){
                System.Console.WriteLine("Ooops! recieved sync from port {0}", portno);
            }
            enditer = this.textview1.Buffer.EndIter;
            this.textview1.Buffer.InsertWithTags(ref enditer, "S", new TextTag[] {syntag, tags[portno]});
        }
        
        
        public void rxbyte(byte b, uint portno){
            if(portno<lopcount){
                if(lops[portno]!=null)
                    lops[portno].rxbyte(b);
            }
        }

        protected virtual void OnButton3Released (object sender, System.EventArgs e)
        {   byte[] msg =  {0xDE, 0xAD, 0xBE, 0xEF};
            byte[] ctab = {0x00, 0x00, 0x00};
            string s = "";
            int i;
            ctab[tempi++%3]=0xff;
            Gdk.Color color= new Gdk.Color(ctab[0], ctab[1], ctab[2]);
            
            for(i=0; i<msg.Length; ++i){
                s += string.Format("{0:X2} ", msg[i]);
            }
            ((Gtk.ListStore)treeview1.Model).AppendValues(System.DateTimeOffset.Now.Ticks, s, color );
        }
        
    }
}
