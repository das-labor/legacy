using System;
using Gtk;

using Db4objects.Db4o;
using Db4objects.Db4o.Query;

using SaftSchubserDb;

namespace SchubserAdmin {
	
	class MainClass
	{
		public static void Main (string[] args)
		{
			// Open Database
			IObjectContainer db = Db4oFactory.OpenFile("/tmp/schubser.db");
			db.Close();
			
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
			
			
		}
	}
}