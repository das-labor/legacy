using System;
using Gtk;

using Db4objects.Db4o;
using Db4objects.Db4o.Query;

using JetonDb;

namespace JetonAdmin {
	
	class MainClass
	{
		public static void Main (string[] args)
		{
			JetonCtrl.Open();
			
			// Open Database
			
			Artikel mate = JetonCtrl.CreateArtikel();
			mate.Name      = "Mate";
			mate.Bestand   = 13;
			mate.Preis     = 1.50m;			
			mate.ProKasten = 20;
			
			Artikel fritz = JetonCtrl.CreateArtikel();
			fritz.Name      = "Fritz";
			fritz.Bestand   = 11;
			fritz.Preis     = 1.00m;
			fritz.ProKasten = 24;
			JetonCtrl.SaveArtikel(fritz);
			
			Artikel hermann =JetonCtrl.CreateArtikel();
			hermann.Name      = "Hermann";
			hermann.Bestand   = 4;
			hermann.Preis     = 1.00m;
			hermann.ProKasten = 24;
			JetonCtrl.SaveArtikel(hermann);
			
			Artikel schoka = JetonCtrl.CreateArtikel();
			schoka.Name      = "SchokaKola";
			schoka.Bestand   = 5;
			schoka.Preis     = 2.50m;
			schoka.ProKasten = 12;
			JetonCtrl.SaveArtikel(schoka);


			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
			
			JetonCtrl.Close();
		}
	}
}
