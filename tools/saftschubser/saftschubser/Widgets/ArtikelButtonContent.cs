using System;
using SaftSchubserDb;

namespace saftschubser
{
	public partial class ArtikelButtonContent : Gtk.Bin
	{
		Artikel _a;
		
		public Artikel A {
			get {
				return _a;
			}
			set {
				_a = value;
				
				nameLabel.Markup    = string.Format( "<span size=\"x-large\"><b>{0}</b></span>", _a.Name );
				preisLabel.Markup   = string.Format( "<span size=\"x-large\"><b>{0:c}</b></span>", _a.Preis );
				commentLabel.Markup = string.Format( "<span size=\"smaller\">Bestand: {0}</span>", _a.Bestand );
			}
		}
		
		public ArtikelButtonContent(Artikel a)
		{
			this.Build();
			
			A = a;
		}
	}
}
