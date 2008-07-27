using System;
using Gtk;

using JetonDb;

namespace Jeton
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			JetonCtrl.Open();
			
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
			
			JetonCtrl.Close();			
		}
	}
}
