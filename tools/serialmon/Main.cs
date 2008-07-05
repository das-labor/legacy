// Main.cs
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
using Gtk;
using GConf;

namespace serialmon
{	
	struct serialportcfg{
		public string portname;
		public uint   baudrate;
		public Parity parity;
		public uint   databits;
		public StopBits stopbits;
		public Gdk.Color color;
		public bool active;
	}
	
	class MainClass
	{
		static GConf.Client conf;
		static string KEY_BASE = "/apps/serialmon/";
		public static uint rxbytecnt = 0;
		public static uint datafixwidth = 8;
		public static uint datagran = 8;
		public static bool dataautowidth = false;
		public static uint addrwidth = 8;
		public static char addrBase = 'X';
		public static string addrformat;
		public static int[] baudrates;
		public static string mainfont;
		public static System.IO.Ports.SerialPort[] ports;
		public static serialportcfg[] portcfg;
		public static bool connected = false;
		public static MainWindow win;
		public static ConfigWindow cw;
		public static bool terminate=false;
		
		public static System.Object lockvar = "";
		
		public static void autoread(ref uint var, string key, uint def){
		    try{
		        var = (uint)((int)conf.Get(KEY_BASE + key));
		    }
		    catch{
		        var = def;
		        conf.Set(KEY_BASE + key, (int)def);
		        System.Console.WriteLine("initial setting of {0} to {1}", key, def);
		    }
		}
		
		public static void autoread(ref int var, string key, int def){
		    try{
		        var = (int)conf.Get(KEY_BASE + key);
		    }
		    catch{
		        var = def;
		        conf.Set(KEY_BASE + key, def);
		        System.Console.WriteLine("initial setting of {0} to {1}", key, def);
		    }
		}
		
		public static void autoread(ref string var, string key, string def){
		    try{
		        var = (string)conf.Get(KEY_BASE + key);
		    }
		    catch{
		        var = def;
		        conf.Set(KEY_BASE + key, def);
		        System.Console.WriteLine("initial setting of {0} to {1}", key, def);
		    }
		}
		
		public static void autoread(ref bool var, string key, bool def){
		    try{
		        var = (bool)conf.Get(KEY_BASE + key);
		    }
		    catch{
		        var = def;
		        conf.Set(KEY_BASE + key, def);
		        System.Console.WriteLine("initial setting of {0} to {1}", key, def);
		    }
		}
		
		public static void autoread(ref int[] var, string key, int[] def){
		    try{
		        var = (int[])conf.Get(KEY_BASE + key);
		    }
		    catch{
		        var = def;
		        conf.Set(KEY_BASE + key, def);
		        System.Console.WriteLine("initial setting of {0} to {1}", key, def);
		    }
		}
			
		public static void readconfig(){
			uint i;	
			
			autoread(ref baudrates, "baudrates", new int[]{
					    50, 75, 110, 134, 150, 200, 300, 600, 1200,
        			    1800, 2400, 4800, 9600, 19200, 38400, 57600,
                        115200, 230400, 460800, 500000, 576000, 921600,
                        1000000, 1152000, 1500000, 2000000, 2500000,
                        3000000, 3500000, 4000000 });
            autoread(ref mainfont, "mainfont", "Monospace 8");
            autoread(ref datafixwidth, "datafixwidth", 16);
            autoread(ref datagran, "datagran", 8);
            autoread(ref dataautowidth, "dataautowidth", false);
            string ts="";
            autoread(ref ts, "addrbase", "X");
            addrBase=ts[0];
            autoread(ref addrwidth, "addrwidth", 4);
            
			for(i = 0; i<=1; i++){
			    int t=0;
				byte[] colorarray = new byte[3];
				
				autoread(ref portcfg[i].active, "Port" + i + "/active", true);
				autoread(ref portcfg[i].portname, "Port" + i + "/device",   System.IO.Ports.SerialPort.GetPortNames()[i]);
				autoread(ref portcfg[i].baudrate, "Port" + i + "/baudrate", 9600);
				autoread(ref t,   "Port" + i + "/parity",   (int)Parity.None);
				portcfg[i].parity = (Parity)t;
				autoread(ref portcfg[i].databits, "Port" + i + "/databits", 8);
				autoread(ref t, "Port" + i + "/stopbits", (int)StopBits.One);
                portcfg[i].stopbits = (StopBits)t;

	            try{
	                int[]  temp;
	                temp = (int[])conf.Get(KEY_BASE + "Port" + i + "/color");
	                colorarray[0] = (byte)(temp[0]);
	                colorarray[1] = (byte)(temp[1]);
	                colorarray[2] = (byte)(temp[2]);
	       		}
				catch{
					if(i==0)
						colorarray[0] = 0xff;
					if(i==1)
						colorarray[2] = 0xff;
					conf.Set(KEY_BASE + "Port" + i + "/color", new int[]{colorarray[0], colorarray[1], colorarray[2]});
					System.Console.WriteLine("initial setting of color for port{0}",i); 
				}				
				portcfg[i].color = new Gdk.Color(colorarray[0], colorarray[1], colorarray[2]);
			}
			
			addrformat = "0" + char.ToLower(addrBase) + "{0:" + addrBase + addrwidth + "}:";
			
			if(cw != null){
			    cw.configSetup();
			}
		}

		public static void writeconfig(){
			uint i;		
			conf.Set(KEY_BASE + "mainfont", (string)mainfont);
            conf.Set(KEY_BASE + "datafixwidth", (int)datafixwidth);
            conf.Set(KEY_BASE + "datagran", (int)datagran);
            conf.Set(KEY_BASE + "dataautowidth", (bool)dataautowidth);
            conf.Set(KEY_BASE + "addrbase", ""+addrBase);
            conf.Set(KEY_BASE + "addrwidth", (int)addrwidth);
			for(i = 0; i<=1; i++){	
				conf.Set(KEY_BASE + "Port" + i + "/device", (string)portcfg[i].portname);
				conf.Set(KEY_BASE + "Port" + i + "/baudrate", (int)portcfg[i].baudrate);
				conf.Set(KEY_BASE + "Port" + i + "/parity", (int)portcfg[i].parity);
				conf.Set(KEY_BASE + "Port" + i + "/databits", (int)portcfg[i].databits);
				conf.Set(KEY_BASE + "Port" + i + "/stopbits", (int)portcfg[i].stopbits);
				conf.Set(KEY_BASE + "Port" + i + "/active", (bool)portcfg[i].active);
				conf.Set(KEY_BASE + "Port" + i + "/color", new int[] 
				         {(portcfg[i].color.Red >> 8), 
					      (portcfg[i].color.Green >> 8), 
					      (portcfg[i].color.Blue >> 8)});
			}
			win.update_textview();
			addrformat = "0" + char.ToLower(addrBase) + "{0:" + addrBase + addrwidth + "}:";
		}

		public static void GConf_Changed(object o, NotifyEventArgs a){
			MainClass.readconfig();
			System.Console.WriteLine("configuration changed from external");
		}
		
		
		
		public static void Main (string[] args)
		{			
			conf = new GConf.Client();
			portcfg = new serialportcfg[2];
			ports = new SerialPort[2];
			Application.Init ();
			readconfig();
			conf.AddNotify(KEY_BASE.TrimEnd(new char[]{'/','\\'}), new GConf.NotifyEventHandler(GConf_Changed));
			win = new MainWindow ();
			win.Show ();
			
			while(!terminate){
		        if(connected){
		            win.serialpoll();
		        }
    			Application.RunIteration();
    		}
		//	Application.Run ();
		}
	}
}