using System;
using System.Collections.Generic;
using JetonDb;

namespace Jeton
{
	public partial class KaufenScreen : Gtk.Bin
	{
		public KaufenScreen()
		{
			this.Build();
			
			hpaned1.Position = 600;
		}
		
		public void OnArtikelChoosen(object sender, Artikel a)
		{
			bestellWidget.AddArtikel(a);
			// Console.WriteLine( a.ToString() );
		}
		
		
		
	}
}
