using System;
using System.Collections.Generic;

using Db4objects.Db4o;

namespace JetonDb
{
	/// <summary>
	///  Representiert eine Person mit einer Karte
	/// </summary>
	public class Person
	{
		// static Members
		//public static int NextId = 0;
			
		// Members
		UInt16      _id;	
		UInt64      _token;
		string      _name;
		decimal     _deposit;
		decimal     _escrow;
		Perms       _permissions;
		
		
		
		public Person()
		{
			_permissions = new Perms();
		}
		
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
		
		public decimal Deposit {
			get { return _deposit; }
			set { _deposit = value; }
		}
		
		public decimal Escrow {
			get { return _escrow; }
			set { _escrow = value; }
		}
		
		public UInt64 Token {
			get { return _token; }
			set { _token = value; }
		}
		
		public Perms Permissions {
			get { return _permissions; }
			set { _permissions = value; }
		}
	}
}
