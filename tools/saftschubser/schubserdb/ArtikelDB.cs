using System;
using System.Collections.Generic;

using Db4objects.Db4o;
using Db4objects.Db4o.Query;

namespace SaftSchubserDb
{
	public static class ArtikelDB
	{
		static IObjectContainer _db;
		static Dictionary<Artikel, bool> _dirtyList; 
		
		public static IObjectContainer Db {
			get {
				if (_db == null) {
					_db = Db4oFactory.OpenFile("/tmp/schubser.db");
				}
				
				return _db;
			}
			set {
				_db = value;
			}
		}

		static Dictionary<Artikel, bool> DirtyList {
			get {
				if (_dirtyList == null) {
					_dirtyList = new Dictionary<Artikel,bool>();
				}
				
				return _dirtyList;
			}
			set {
				_dirtyList = value;
			}
		}
		
		public static  Artikel CreateArtikel( string name )
		{
			Artikel a = new Artikel();
			a.Name = name;
			
			return a;
		}
		
		public static void Dirty(Artikel a)
		{
			DirtyList[a] = true;
		}
		
		public static void Commit()
		{
			//flush Db
			Artikel proto = new Artikel();
			IObjectSet res = Db.Get(proto);
			foreach (Artikel a in res) {
				Db.Delete(a);
			}
			
			foreach ( KeyValuePair<Artikel,bool> pair in DirtyList ) 
			{
				Db.Set( pair.Key );
			}
			Db.Commit();
		}
		
		public static List<Artikel> VisibleArtikel()
		{
			List<Artikel> list = new List<Artikel>();
			Artikel proto = new Artikel();
			
			IObjectSet res = Db.Get(proto);
			foreach (Artikel a in res) {
				list.Add( a );
			}
			
			return list;
		}
	}
}

