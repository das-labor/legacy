using System;
using System.Collections.Generic;
using Gtk;
using SchubserDb;

namespace SchubserPos
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
