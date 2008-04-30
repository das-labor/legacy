using System;
using NUnit.Framework;

namespace JetonDb
{
	[TestFixture()]
	public class JetonDbTest
	{
		[SetUp]
		public void SetUp()
		{
		}
		
		[Test()]
		public void CreateAndSavePersons()
		{
			Person p1 = JetonCtrl.CreatePerson();
			p1.Name = "User 1";
			JetonCtrl.SavePerson(p1);
			
			Person p2 = JetonCtrl.CreatePerson();
			p2.Name = "User 2";
			JetonCtrl.SavePerson(p2);
		}
		
		[Test]
		public void CreateAndSaveItems()
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
			

		}
		
	}
}
