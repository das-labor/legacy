using System;
using System.Collections.Generic;
using Gtk;
using JetonDb;

namespace Jeton
{
	[System.ComponentModel.Category("Jeton")]
	[System.ComponentModel.ToolboxItem(true)]

	public partial class BestellWidget : Gtk.Bin
	{
		const int Rows = 12;
		const string ArtikelMarkup = "<span size=\"xx-large\">{0}</span>";
		const string PreisMarkup   = "<span size=\"xx-large\">{0:c}</span>";
		const string SummeMarkup   = "<span size=\"xx-large\"><b>{0:c}</b></span>";
		
		Label sumLabel;

		Label[]   artikelLabel;
		Label[]   preisLabel;
		Button[]  delBtn;
		
		List<Artikel> _posten;
		public List<Artikel> Posten {
			get {
				return _posten;
			}
			set {
				_posten = value;
			}
		}
		
		public decimal Summe {
			get {
				decimal sum = 0.0m;
				
				foreach(Artikel a in Posten) {
					sum += a.Preis;
				}
				return sum;
			}
		}
			
		public BestellWidget()
		{
			this.Build();
			
			eventBox.ModifyBg( Gtk.StateType.Normal, new Gdk.Color(255, 255, 255) );
			
			table.NRows = Rows + 2;
			table.NColumns = 3;
			
			HSeparator s = new HSeparator();
			table.Attach( s, 0, 3, Rows, Rows+1 );
			
			sumLabel = new Label();
			sumLabel.Xalign = 1.0f;
			sumLabel.Markup = "<span size=\"xx-large\"><b>Summe:</b></span>";
			table.Attach( sumLabel,0, 1, Rows+1, Rows+2);
			
			sumLabel = new Label();
			sumLabel.Markup = "<span size=\"xx-large\"><b>0,00 €</b></span>";
			sumLabel.Xalign = 1.0f;
			table.Attach( sumLabel, 1, 2, Rows+1, Rows+2);
			
			artikelLabel = new Label[Rows];
			preisLabel   = new Label[Rows];
			delBtn       = new Button[Rows];
			
			for (uint y=0; y<Rows ; y++) {
				artikelLabel[y] = new Label();
				artikelLabel[y].Xalign = 1.0f;
				preisLabel[y] = new Label();
				preisLabel[y].Xalign = 1.0f;
				Image delImg = new Image( Gtk.Stock.Delete, Gtk.IconSize.Button );			
				delBtn[y] = new Button(delImg);
				delBtn[y].Clicked += new EventHandler( this.OnDelClicked );
				
				table.Attach( artikelLabel[y], 0, 1, y, y+1 );
				table.Attach( preisLabel[y], 1, 2, y, y+1 );
				table.Attach( delBtn[y], 2, 3, y, y+1 );
			}
			
			Posten = new List<Artikel>();
			
			Update();
		}

		public void Update()
		{
			for(int y=0; y<Rows; y++) {
				Artikel a = GetYArtikel(y);
				if (a == null) {
					artikelLabel[y].Markup = "";
					preisLabel[y].Markup = "";
					delBtn[y].Sensitive = false;					
				} else {
					artikelLabel[y].Markup = string.Format( ArtikelMarkup, a.Name );
					preisLabel[y].Markup   = string.Format( PreisMarkup, a.Preis );
					delBtn[y].Sensitive = true;
				}
			}
			
			sumLabel.Markup = string.Format( SummeMarkup, Summe );
			
			applyBtn.Sensitive  = (Posten.Count > 0);
			cancelBtn.Sensitive = (Posten.Count > 0);
		}
		
		protected Artikel GetYArtikel(int y)
		{
			int p = Rows-y-1;
			
			if (p >= Posten.Count)
				return null;
			
			return Posten[p];
		}
		
		public void AddArtikel(Artikel a)
		{
			Posten.Add(a);
			Update();
		}
		
		public void DelArtikel(Artikel a)
		{
			Posten.Remove(a);
			Update();
		}
		
		protected virtual void OnDelClicked(object sender, System.EventArgs e)
		{
			for(int y=0; y<Rows; y++) {
				if (sender != delBtn[y]) continue;
				
				Artikel a = GetYArtikel(y);
				DelArtikel(a);
			}
		}

		protected virtual void OnApplyClicked (object sender, System.EventArgs e)
		{
			
		}

		protected virtual void OnCancelClicked (object sender, System.EventArgs e)
		{
			Posten.Clear();
			Update();
		}
		
	}
}
