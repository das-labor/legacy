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
		string   _name;
		int      _id;
		decimal  _deposit;
		decimal  _escrow;
		
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
			protected set { _deposit = value; }
		}
		
		public decimal Escrow {
			          get { return _escrow; }
			protected set { _escrow = value; }
		}

		public Person()
		{
			Deposit = 0.0m;
			Escrow  = 0.0m;
			Name    = "New Person";
		}
	}
}
