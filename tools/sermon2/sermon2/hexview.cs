// hexview.cs
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
    public enum base_enum {dec=10, hex=16};
    
    public partial class hexview : Gtk.Bin
    {
    
        public  base_enum   addrBase=base_enum.hex;
        public  uint        addrSize=8;
        public  TextTag[]   tags;
        private string      mainFont;
        private uint        datawidth=16;
        private bool        datawidth_auto;
        private ulong       bytecount=0;
        private string      addrFormat="0x{0:X8}\n";
        private TextIter    endAddrTViter, endHexTViter, endAsciiTViter;
        private Adjustment  xAdj, yAdj;
        
        public hexview(){
            this.Build();
            endAddrTViter  = new TextIter();
            endHexTViter   = new TextIter();
            endAsciiTViter = new TextIter();
            endAddrTViter  = addrTV.Buffer.EndIter;
            endHexTViter   = hexTV.Buffer.EndIter;
            endAsciiTViter = asciiTV.Buffer.EndIter;
          //  addrTV.
          //  addrTV.Accessible.VScrollbar = vscrollbar1;
          //  hexTV.Accessible.VScrollbar = vscrollbar1;
          //  asciiTV.Accessible.VScrollbar = vscrollbar1;
            xAdj=new Adjustment(0, 0, 1000, 1, 100, 10);
            yAdj=new Adjustment(0, 0, 100, 1, 10, 10);
            
            addrTV.SetScrollAdjustments(xAdj,yAdj);
            hexTV.SetScrollAdjustments(xAdj,yAdj);
            asciiTV.SetScrollAdjustments(xAdj,yAdj);
       //     vscrollbar1.Adjustment = yAdj;
            MainFont       = "Monospace 7";
        }
        
        public String MainFont{
            get{
                return mainFont;
            }
            set{
                mainFont = value;
                addrTV.ModifyFont(Pango.FontDescription.FromString(mainFont));
                hexTV.ModifyFont(Pango.FontDescription.FromString(mainFont));
                asciiTV.ModifyFont(Pango.FontDescription.FromString(mainFont));
            }
        }
        
        public void Clear(){
            addrTV.Buffer.Clear();
            hexTV.Buffer.Clear();
            asciiTV.Buffer.Clear();
        }
        
        public void Reset(ulong offset){
            this.Clear();
            bytecount=offset;
        }
        
        public void Reset(){
            this.Reset(0);
        }
        
        public ulong Bytecount{
            get{
                return bytecount;
            }
        }
        
        public Adjustment YAdj{
            get{
                return yAdj;
            }
            set{
                yAdj=value;
                addrTV.SetScrollAdjustments(xAdj, yAdj);
                hexTV.SetScrollAdjustments(xAdj, yAdj);
                asciiTV.SetScrollAdjustments(xAdj, yAdj);
            }
        }
        
        private void IncAdj(){
                yAdj.Upper+=10;
                addrTV.SetScrollAdjustments(xAdj, yAdj);
                hexTV.SetScrollAdjustments(xAdj, yAdj);
                asciiTV.SetScrollAdjustments(xAdj, yAdj);
           //     vscrollbar1.Adjustment = yAdj;
                vscrollbar1.SetRange(0, yAdj.Upper);
        }
        
        public TextTag Tags{
            set{
                Array.Resize(ref tags, tags.Length);
                tags[tags.Length-1] = value;
                hexTV.Buffer.TagTable.Add(value);
                asciiTV.Buffer.TagTable.Add(value);
            }
        }
        
        public void updateAddrFormat(){
            addrFormat = "0"+((addrBase==base_enum.hex)?"x":"d")+"{0:}"
                            +((addrBase==base_enum.hex)?"X":"")
                            +addrSize.ToString()+"}\n";
        }
        
        public void addData(byte[] data, TextTag[] tags){
            string s, ascii;
            uint i=0;
            if(!endAddrTViter.IsEnd){
                endAddrTViter=addrTV.Buffer.EndIter;
            }
            if(!endHexTViter.IsEnd){
                endHexTViter=hexTV.Buffer.EndIter;
            }
            if(!endAsciiTViter.IsEnd){
                endAsciiTViter=asciiTV.Buffer.EndIter;
            } 
         //   if(bytecount%datawidth==0){
         //       s = string.Format(addrFormat, bytecount);
         //       addrTV.Buffer.Insert(ref endAddrTViter, s);
         //   }
            s="";
            ascii="";
            for(i=0; i<data.Length; ++i, bytecount++){
                if(bytecount%datawidth==0){
                    s = string.Format(addrFormat, bytecount);
                    addrTV.Buffer.Insert(ref endAddrTViter, s);
                }
                /* fill up the line */
                s = string.Format(" {0:X2}", data[i]);
                ascii += (data[i]>=32&&data[i]<=127)?(char)data[i]:'.';
                if((bytecount%datawidth) == (datawidth-1)){
                    s+='\n';
                    ascii+='\n';
                    IncAdj();
                }
                hexTV.Buffer.InsertWithTags(ref endHexTViter, s, tags);
                asciiTV.Buffer.InsertWithTags(ref endAsciiTViter, ascii, tags);
                s=""; ascii="";
            }
            
        }

        protected virtual void OnSizeAllocated (object o, Gtk.SizeAllocatedArgs args)
        {
        /*
            int w;
            Gdk.Rectangle a;
            w = (hexTV.Allocation.Width+asciiTV.Allocation.Width);
            a = hexTV.Allocation;
            a.Height = 400;
            a.Width = (w * 2) /3;
            hexTV.Allocation = a;
            a.X += a.Width;
            a.Width /= 2;
            asciiTV.Allocation=a;
        */
        }

        protected virtual void OnVscrollbar1MoveSlider (object o, Gtk.MoveSliderArgs args)
        {
         //   Adjustment a = new Adjustment(0,0,0,0,0,0);
         //   addrTV.SetScrollAdjustments(a, vscrollbar1.Adjustment);
         // addrTV.ScrollToIter(endAddrTViter, 0.25, false, 0, 0);
         // System.Console.WriteLine("xyz");
        }

        protected virtual void OnAddrTVScrollAdjustmentsSet (object o, Gtk.ScrollAdjustmentsSetArgs args)
        {
         //   vscrollbar1.Adjustment = args.Vadjustment;
            System.Console.WriteLine("---\nValue: {0}\nUpper: {1}\nLower: {2}\npInc: {3}\npSize: {4}",
                args.Vadjustment.Value,
                args.Vadjustment.Upper,
                args.Vadjustment.Lower,
                args.Vadjustment.PageIncrement,
                args.Vadjustment.PageSize);
          //  xAdj = args.Hadjustment;
          //  yAdj = args.Vadjustment;
            System.Console.WriteLine("-SB-\nValue1: {0}\n"
                                      +    "Value2: {0}\n"
                                      +    "Upper: {1}\n"
                                      +    "Lower: {2}\n"
                                      +    "pInc: {3}\n"
                                      +    "pSize: {4}",
                vscrollbar1.Value,
                vscrollbar1.Adjustment.Value,
                vscrollbar1.Adjustment.Upper,
                vscrollbar1.Adjustment.Lower,
                vscrollbar1.Adjustment.PageIncrement,
                vscrollbar1.Adjustment.PageSize);
        
        }
        

        protected virtual void OnVscrollbar1ChangeValue (object o, Gtk.ChangeValueArgs args)
        {
        
           
   //         asciiTV.SetScrollAdjustments(x,y);
              addrTV.SetScrollAdjustments(xAdj, vscrollbar1.Adjustment);
              hexTV.SetScrollAdjustments(xAdj, vscrollbar1.Adjustment);
              asciiTV.SetScrollAdjustments(xAdj, vscrollbar1.Adjustment);
              
            
        }
        
    }
}
