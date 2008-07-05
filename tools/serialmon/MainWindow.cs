// MainWindow.cs
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
using System.Timers;
using System.Threading;
using Gtk;
namespace serialmon {

public partial class MainWindow: Gtk.Window
{	
    public uint datawidth;
	private static TextTag p1Tag, p2Tag, addrTag;
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
		p1Tag = new TextTag("p1");
		p2Tag = new TextTag("p2");
		addrTag  = new TextTag("addr");
		update_textview();
		textview1.Buffer.TagTable.Add(p1Tag);
		textview1.Buffer.TagTable.Add(p2Tag);
		textview1.Buffer.TagTable.Add(addrTag);
	}


    public void update_textview(){
        textview1.ModifyFont(
		    Pango.FontDescription.FromString(serialmon.MainClass.mainfont));
        calcwidth();        
    }

	public void calcwidth()
	{
        if(MainClass.dataautowidth){
        System.Console.WriteLine("tvwidth = {0}; fontwidth = {1}; scale = {2}; stretch = {3} ", 
            textview1.Allocation.Width, 
            Pango.FontDescription.FromString(MainClass.mainfont).Size,
            Pango.Scale.PangoScale,
            Pango.FontDescription.FromString(MainClass.mainfont).Stretch);
	 	
    	datawidth = (uint)(textview1.Allocation.Width * Pango.Scale.PangoScale * 1.3 / Pango.FontDescription.FromString(MainClass.mainfont).Size);
        System.Console.WriteLine(" 1datawidth = {0}", datawidth);
	 		datawidth -= 3 + MainClass.addrwidth + 5;
        System.Console.WriteLine(" 2datawidth = {0}", datawidth);
	 		datawidth /= 3;
        System.Console.WriteLine(" 3datawidth = {0}", datawidth);
	 		datawidth = (datawidth / MainClass.datagran) * MainClass.datagran;
        } else {
            datawidth = MainClass.datafixwidth;
        }
        System.Console.WriteLine("datawidth = {0}", datawidth);
	}

	
	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
        serialmon.MainClass.terminate= true;	
	//	Application.Quit ();
		a.RetVal = true;
	}


	protected virtual void OnAboutButtonReleased (object sender, System.EventArgs e)
	{
	    about aboutwin = new about();
	    aboutwin.Show();
	}

	protected virtual void OnConfigButtonReleased (object sender, System.EventArgs e)
	{
		serialmon.MainClass.cw = new serialmon.ConfigWindow();
		serialmon.MainClass.cw.configSetup();
		configButton.Sensitive = false;
	}
	
    public void configfinished(){
        textview1.ModifyFont(Pango.FontDescription.FromString(serialmon.MainClass.mainfont));
	    configButton.Sensitive = true;
	}
	
	protected virtual void OnClearButtonReleased (object sender, System.EventArgs e)
	{
		textview1.Buffer.Clear();
		MainClass.rxbytecnt = 0;
		calcwidth();
	}

    protected virtual void OnSerialDataRx (object sender, SerialDataReceivedEventArgs e)
	{
	    TextTag tTag = null;   
	    if(sender==serialmon.MainClass.ports[0]){
	        tTag = p1Tag;
	    }
	    if(sender==serialmon.MainClass.ports[1]){
	        tTag = p2Tag;
	    }
	    if(tTag == null){
	        System.Console.WriteLine("unknown sender");
	        return;
	    }
	    
	   // if (e.EventType == SerialData.){
	        System.Console.WriteLine("bla ({0})", e.ToString());
	        return;
	  //  }
	}
        
    public void serialpollloop(){
        while(true){
            serialpoll();
        }
    }
    
    public void insertbyte(byte b, int port, ref TextIter enditer){
            TextTag[] t = {p1Tag, p2Tag};
            string s = string.Format(" {0:X2}", b);
            
            if(MainClass.rxbytecnt%datawidth==0){
                if(MainClass.rxbytecnt!=0){
                    textview1.Buffer.Insert(ref enditer, "\n");
                }
                string saddr = string.Format(MainClass.addrformat, MainClass.rxbytecnt);
                textview1.Buffer.InsertWithTags(ref enditer, saddr, addrTag);
            }
            
            textview1.Buffer.InsertWithTags(ref enditer, s, t[port]);
            MainClass.rxbytecnt += 1;
    }
    
    public void serialpoll(){
        bool x = true;
        TextIter enditer;
		enditer = textview1.Buffer.EndIter;
		if(MainClass.portcfg[0].active)
        while(serialmon.MainClass.ports[0].BytesToRead>0){
            insertbyte((byte)serialmon.MainClass.ports[0].ReadByte(), 0, ref enditer);
            x = true;
        }
        
		if(MainClass.portcfg[1].active)
        while(serialmon.MainClass.ports[1].BytesToRead>0){
            insertbyte((byte)serialmon.MainClass.ports[1].ReadByte(), 1, ref enditer);
            x = true;
        }
        if(x){
            textview1.QueueDraw();
            textview1.ShowNow();
        }
    }
    
    
    
    void OnMainButtonReleased (object sender, System.EventArgs e)
	{
	    int i;
	    if(serialmon.MainClass.connected==true){
	        for(i=0; i<2; ++i){
	            if(serialmon.MainClass.portcfg[i].active && serialmon.MainClass.ports[i] != null){
	               try{
	                  serialmon.MainClass.ports[i].Close();
	                  serialmon.MainClass.ports[i] = null;
	               }
	               catch(Exception e1){
	                   System.Console.WriteLine("error closing port{0} ({1}): {2}", 
	                                            i, serialmon.MainClass.portcfg[i].portname,
	                                            e1.ToString());
	               }
	            }	            
	        }
	        mainButton.Label = "connect";
	        serialmon.MainClass.connected = false;
	    } else {
	    	for(i=0; i<2; ++i){
	            if(serialmon.MainClass.portcfg[i].active)
	                try{
	                    serialmon.MainClass.ports[i] = new SerialPort(
	                       serialmon.MainClass.portcfg[i].portname,	           
	                       (int)serialmon.MainClass.portcfg[i].baudrate,
	                       serialmon.MainClass.portcfg[i].parity,
	                       (int)serialmon.MainClass.portcfg[i].databits,
	                       serialmon.MainClass.portcfg[i].stopbits);
	                    serialmon.MainClass.ports[i].ReadTimeout=1;
	                    serialmon.MainClass.ports[i].Open();
	                    serialmon.MainClass.ports[i].DataReceived += (SerialDataReceivedEventHandler)OnSerialDataRx;
	                    System.Console.WriteLine("opened port{0} ({1} @ {2}) successful",
	                            i, serialmon.MainClass.portcfg[i].portname, serialmon.MainClass.portcfg[i].baudrate);
	                }
	                catch(Exception e2){
	                    System.Console.WriteLine("error opening port{0} ({1}): {2}", 
	                                             i, serialmon.MainClass.portcfg[i].portname,
	                                             e2.ToString());    
	                    serialmon.MainClass.ports[i]=null;
	                }
	        }
	        p1Tag.ForegroundGdk = serialmon.MainClass.portcfg[0].color;
	        p2Tag.ForegroundGdk = serialmon.MainClass.portcfg[1].color;
	        mainButton.Label = "disconnect";
	        serialmon.MainClass.connected = true;
	    }
	}

    protected virtual void OnTextview (object o, Gtk.SizeAllocatedArgs args)
    {
        calcwidth();
    }
}

}