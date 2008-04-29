using System;
using System.Collections.Generic;

using Db4objects.Db4o;

namespace SchubserDb
{
	/// <summary>
	/// Representiert eine Person mit einer Karte
	/// </summary>
	public class Person
	{
		// static Members
		public static int NextId = 0;
			
		// Members
		string   _name;
		int      _id;
		decimal  _guthaben;
		decimal  _treuhaendisch;
		
		/// <value>
		/// 
		/// </value>
		public string Name {
			set { _name = value; }
			get { return _name; }
		}
		
		public int Id {
			get { return _id; }
		}
		
		public decimal Guthaben {
			get { return _guthaben; }
			set { _guthaben = value; }
		}
		
		public decimal Treuhaendisch {
			get { return _treuhaendisch; }
			set { _treuhaendisch = value; }
		}
		
		/// <summary>
		/// 
		/// </summary>
		public Person(int id)
		{
			_id = id;
		}
		
		public Person(string name)
		{
			_name = name;
		}
	}
}
