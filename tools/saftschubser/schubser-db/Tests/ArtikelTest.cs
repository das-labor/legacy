using System;
using NUnit.Framework;

using System.Collections.Generic;

namespace SchubserDb
{
	[TestFixture()]
	public class ArtikelTest
	{
		
		[Test()]
		public void CreateDatabase()
		{
			Artikel mate = ArtikelDB.CreateArtikel( "Mate" );
			mate.Bestand   = 0;
			mate.Preis     = 1.50m;			
			mate.ProKasten = 20;
			
			Artikel fritz = ArtikelDB.CreateArtikel( "Fritz" );
			fritz.Bestand   = 24;
			fritz.Preis     = 1.00m;
			fritz.ProKasten = 24;
			
			ArtikelDB.Commit();
		}

		[Test()]
		public void VisibleArtikel()
		{
			List<Artikel> list = ArtikelDB.VisibleArtikel();
			ListResult( list );
		}
		
		
		// Utility Functions
		public static void ListResult(List<Artikel> list)
		{
			Console.WriteLine(list.Count);
			foreach (Artikel i in list)
			{
				Console.WriteLine(i);
			}
		}
	}
}