using System;

namespace JetonDb
{
	public class Artikel
	{
		// Members
		string    _name;
		decimal   _preis;
		int       _bestand;
		int       _proKasten;
		
		// Properties
		public string Name {
			get {
				return _name;
			}
			set {
				_name = value;
				// ArtikelDB.Dirty(this);
			}
		}

		public decimal Preis {
			get {
				return _preis;
			}
			set {
				_preis = value;
				// ArtikelDB.Dirty(this);				
			}
		}

		public int Bestand {
			get {
				return _bestand;
			}
			set {
				_bestand = value;
				// ArtikelDB.Dirty(this);				
			}
		}

		public int ProKasten {
			get {
				return _proKasten;
			}
			set {
				_proKasten = value;
				// ArtikelDB.Dirty(this);				
			}
		}
		
		public override string ToString()
		{
			return string.Format( "Item: ({0}, Price: {1:c}, InStock: {2}, PerPackage: {3}",
			                      Name, Preis, Bestand, ProKasten );
		}
		
		// Constructor
		//protected Artikel(name)
		//{
		//	Name = name;
		//}
	}
}

