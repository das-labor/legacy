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
		public void NewDb()
		{
			// Choose a temporary Database
			string path = Path.GetTempFileName();
			JetonCtrl.Open( path );
		}
		
		/// <summary>
		/// 
		/// </summary>
		[Test] public void CreateAndSavePersons()
		{
			NewDb();
			
			Person p1 = JetonCtrl.CreatePerson();
			p1.Name = "User 1";
			JetonCtrl.SavePerson(p1);
			
			Person p2 = JetonCtrl.CreatePerson();
			p2.Name = "User 2";
			JetonCtrl.SavePerson(p2);
			
			List<Person> list = JetonCtrl.GetAllPersons();
			Assert.AreEqual( list.Count, 2 );
			foreach( Person p in list ) {
				Console.WriteLine( p ); 
			}
		}
		
		[Test] public void TestPersonPermissions()
		{
			NewDb();
			
			// create & save person
			Person p1 = JetonCtrl.CreatePerson();
			p1.Name = "Admin";
			p1.Permissions.Add( Perm.Laborant );
			Console.WriteLine( "permisstions: {0:x}", p1.Permissions.data );			
			p1.Permissions.Add( Perm.Treuhaender );			
			p1.Permissions.Add( Perm.God );
			Console.WriteLine( p1.Permissions.ToString() );
			
			// Check Permissions
			Assert.IsTrue(  p1.Permissions.Contains( Perm.Laborant ) );
			Assert.IsTrue(  p1.Permissions.Contains( Perm.Treuhaender ) );
			Assert.IsTrue(  p1.Permissions.Contains( Perm.God ) );
			Assert.IsFalse( p1.Permissions.Contains( Perm.CardCreator ) );
			Assert.IsFalse( p1.Permissions.Contains( Perm.Satan ) );			

			
			JetonCtrl.SavePerson( p1 );
			
			// and load again
			List<Person> list = JetonCtrl.GetAllPersons();
			Assert.AreEqual( list.Count, 1 );

			Person p2 = list[0];
			Assert.AreEqual( p2.Name, "Admin" );
			Assert.IsTrue(  p2.Permissions.Contains( Perm.Laborant ) );
			Assert.IsTrue(  p2.Permissions.Contains( Perm.Treuhaender ) );
			Assert.IsTrue(  p2.Permissions.Contains( Perm.God ) );
			Assert.IsFalse( p2.Permissions.Contains( Perm.CardCreator ) );
			Assert.IsFalse( p2.Permissions.Contains( Perm.Satan ) );			
			Console.WriteLine( p2.Permissions );
		}
		
		
		/// <summary>
		/// 
		/// </summary>
		[Test] public void CreateAndSaveArtikel()
		{
			NewDb();
			
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
			Assert.AreEqual( 2, aList.Count );
			
			// There should be two articles
			List<Artikel> vaList = JetonCtrl.GetArtikel();
			Assert.AreEqual( 2, vaList.Count );
		}
		
		[Test] public void PermsTest()
		{
			Perms p = new Perms();
			
			p.Add( Perm.CardCreator );
			Console.WriteLine( p.data );
			
		}
		
	}
}
