using System;
using System.IO;
using System.Collections.Generic;
using NUnit.Framework;

namespace JetonDb
{
	[TestFixture] 
	public class JetonDbTest
	{
		/// <summary>
		/// Choose temporary file for unit test database operations 
		/// </summary>
		[SetUp]	public void SetUp()
		{
			// Choose a temporary Database
			string path = Path.GetTempFileName();
			JetonCtrl.DbPath = path;
		}
		
		/// <summary>
		/// Delete temporary database
		/// </summary>
		[TearDown] public void TearDown()
		{
			File.Delete( JetonCtrl.DbPath );
		}
		
		/// <summary>
		/// 
		/// </summary>
		[Test] public void CreateAndSavePersons()
		{
			Person p1 = JetonCtrl.CreatePerson();
			p1.Name = "User 1";
			JetonCtrl.SavePerson(p1);
			
			Person p2 = JetonCtrl.CreatePerson();
			p2.Name = "User 2";
			JetonCtrl.SavePerson(p2);
		}

		/// <summary>
		/// 
		/// </summary>
		[Test] public void CreateAndSaveArtikel()
		{
			Artikel mate = JetonCtrl.CreateArtikel();
			mate.Name       =  "Mate";
			mate.Bestand    = 0;
			mate.Preis      = 1.50m;			
			mate.ProKasten  = 20;
			JetonCtrl.SaveArtikel(mate);
			
			Artikel fritz = JetonCtrl.CreateArtikel();
			fritz.Name       = "Fritz";
			fritz.Bestand    = 24;
			fritz.Preis      = 1.00m;
			fritz.ProKasten  = 24;
			JetonCtrl.SaveArtikel(fritz);
			
			// There should be two articles
			List<Artikel> aList = JetonCtrl.GetArtikel();
			Assert.AreEqual( aList.Count, 2 );
			
			// There should be two articles
			List<Artikel> vaList = JetonCtrl.GetArtikel();
			Assert.AreEqual( vaList.Count, 2 );
		}
		
	}
}
