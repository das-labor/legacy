using System;
using System.Collections.Generic;

using Gtk;


using SaftSchubserDb;

namespace saftschubser
{
	public partial class KaufenScreen : Gtk.Bin
	{
		public KaufenScreen()
		{
			this.Build();
			
			artikelChooser.ArtikelChoosen += new ArtikelChoosenHandler( OnArtikelChoosen );
			
//			foreach(Artikel a in list) {
//				Console.WriteLine( a );
//			}
		}
		
		public void OnArtikelChoosen(object sender, Artikel a)
		{
			Console.WriteLine( a.ToString() );
		}
		
	}
}
