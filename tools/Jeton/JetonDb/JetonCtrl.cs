using System;
using System.Collections.Generic;
using System.IO;

using Db4objects.Db4o;
using Db4objects.Db4o.Query;

namespace JetonDb
{
	/// <summary>
	///  This class encapsulates most operatins (transactions) on the actual 
	///  database. In terms of MVC, this is ths Controller.
	/// </summary>
	public class JetonCtrl
	{
		public static string DbPath {
			get {
				if (_db != null) {
					_db.Rollback();
					_db.Close();
					_db = null;
				}
				
				if (_dbPath == null)
					_dbPath = System.Environment.GetEnvironmentVariable("JETON_DB");
				
				if (_dbPath == null)
					_dbPath = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
					_dbPath = Path.Combine( System.Environment.GetFolderPath( System.Environment.SpecialFolder.Personal ),
						                       ".jeton.db" );
				
				return _dbPath;
			}
			set {
				_dbPath = value;
			}
		}
		private static string _dbPath;		
		
		/// <value>
		///  Actual connection to the Db4O Database Backend 
		/// </value>
		public static IObjectContainer Db {
			get {
				if (_db == null) {
					_db = Db4oFactory.OpenFile(DbPath);
				}				
				
				return _db;
			}
			set {
				_db = value;
			}
		}
		private static IObjectContainer _db;
		
		/// <summary>
		/// 
		/// </summary>
		public static Artikel CreateArtikel()
		{
			return new Artikel();
		}
		
		/// <summary>
		///   
		/// </summary>
		public static void SaveArtikel(Artikel i)
		{
			Db.Set(i);
			Db.Commit();
		}
		
		/// <summary>
		///  Search and return all Artikel 
		/// </summary>
		public static List<Artikel> GetArtikel()
		{
			List<Artikel> list = new List<Artikel>();
			Artikel proto = new Artikel();
			
			IObjectSet res = Db.Get(proto);
			foreach (Artikel a in res) {
				list.Add( a );
			}
			
			return list;
		}

		/// <summary>
		///  
		/// </summary>
		public static List<Artikel> GetVisibleArtikel()
		{
			return GetArtikel();
		}

		
		///////////////////////////////////////////////////////////		
		
		/// <summary>
		///  
		/// </summary>
		public static Person CreatePerson()
		{
			return new Person();
		}
		
		/// <summary>
		///  
		/// </summary>
		public static void SavePerson(Person p)
		{
			Db.Set(p);
			Db.Commit();
		}
		
		/// <summary>
		///  Search and return all Artikel 
		/// </summary>
		public static List<Person> GetAllPersons()
		{
			List<Person> list = new List<Person>();
			Person proto = new Person();
			
			IObjectSet res = Db.Get(proto);
			foreach (Person a in res) {
				list.Add( a );
			}
			
			return list;
		}
		
		///////////////////////////////////////////////////////////		

		public static void BuyItems(Person p, List<Artikel> list)
		{
		}

		public static void TransferEscrow(Person from, Person to, decimal val)
		{
		}
		
		public static void RelieveEscrow(Person p, decimal val)
		{
		}
		

	}
}
