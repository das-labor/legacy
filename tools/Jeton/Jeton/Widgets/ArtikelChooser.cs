using System;
using System.Collections.Generic;
using Gtk;
using JetonDb;

namespace Jeton
{
	public delegate void ArtikelChoosenHandler(object sender, Artikel a);  
	
	[System.ComponentModel.Category("Jeton")]
	[System.ComponentModel.ToolboxItem(true)]
	public partial class ArtikelChooser : Gtk.Bin
	{
		List<Artikel> _artikel;
		uint          _offset;


		// Accessor
		public uint Offset {
			get {
				return _offset;
			}
			set {
				_offset = value;
			}
		}

		public ArtikelChooser()
		{
			this.Build();
			
			
			JetonCtrl.CreateArtikel();
			_artikel = JetonDb.JetonCtrl.GetVisibleArtikel();
			
			Update();
		}

		public void Update()
		{
			foreach(Widget w in btnTable.Children) {
				btnTable.Remove(w);
			}
			
			for (uint y=0; y<btnTable.NRows ; y++) {
				for (uint x=0; x<btnTable.NColumns; x++) {
					Button b = CreateArtikelButton(x,y);
					b.Clicked += new System.EventHandler( OnBtnClicked );
					btnTable.Attach( b, x, x+1, y, y+1 );
				}
			}
			btnTable.ShowAll();
			
			upBtn.Sensitive   = (Offset != 0);
			downBtn.Sensitive = (GetXYArtikel(1, btnTable.NRows) != null); 
		}
		
		// 
		public Button CreateArtikelButton(uint x, uint y)
		{
			Button  b;
			Artikel a = GetXYArtikel(x, y);
			
			if (a != null) {
				ArtikelButtonContent c = new ArtikelButtonContent(a);
				b = new Button(c);
				b.CanFocus  = false;				
			} else {
				b = new Button();
				b.Sensitive = false;
				b.CanFocus  = false;
				//b = null;
			}
			
			return b;
		}
		
		// Utility Methods
		Artikel GetXYArtikel(uint x, uint y)
		{
			uint idx;
			
			idx = btnTable.NColumns * (Offset + y) + x;
			if (idx >= _artikel.Count)
				return null;
			else
				return _artikel[(int)idx];
		}

		protected virtual void OnBtnClicked (object sender, System.EventArgs e)
		{
			foreach(Widget w in btnTable.Children) {
				if (w == sender) {
					Button b = (Button)w;
					ArtikelButtonContent c = (ArtikelButtonContent)(b.Children[0]);
					
					if (ArtikelChoosen != null)
						ArtikelChoosen(this, c.A);
				}
			}
		}
		
		protected virtual void OnUpBtnClicked (object sender, System.EventArgs e)
		{
			Offset--;
			Update();
		}

		protected virtual void OnDownBtnClicked (object sender, System.EventArgs e)
		{
			Offset++;
			Update();
		}

		public event ArtikelChoosenHandler ArtikelChoosen;
	}
}
