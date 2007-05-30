// project created on 4/5/2007 at 11:04 AM
using System;
using System.Threading;
using Gtk;
using NUnit.Framework;
using CTapi;

namespace LabCardCreator
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Thread.Sleep(1000);		
			CT ct = new CT(1);
			// ct.Reset();
			Thread.Sleep(1000);
			
			while(true) {
				Thread.Sleep(500);
				
				if (ct.GetStatus()) {
					System.Console.WriteLine( "Karte Drinne");
				} else {
					System.Console.WriteLine( "Karte Drinne");
				}
			}
		
		
			/*
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
			*/
		}
	}
}