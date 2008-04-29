using System;
using Gtk;

using SaftSchubserDb;

namespace SchubserAdmin {

	public partial class MainWindow: Gtk.Window
	{	
		public MainWindow (): base (Gtk.WindowType.Toplevel)
		{
			Build ();
		}
		
		protected void OnDeleteEvent (object sender, DeleteEventArgs a)
		{
			Application.Quit ();
			a.RetVal = true;
		}
		
		protected virtual void CardAdd (object sender, System.EventArgs e)
		{
			//Person np = SaftDb.NewPerson();
			//
			//np.Name     = "Kassenwart";
			//np.Guthaben = 23.42m;
			//
			//ShowPerson(np);
		}
		/*
		void ShowPerson(Person p)
		{
			cardIdEntry.Text       = p.Id.ToString();
			cardIdEntry.IsEditable = false;
			
			cardNameEntry.Text     = p.Name;
			cardCreditEntry.Text   = p.Guthaben.ToString();
			cardTreuhandEntry.Text = p.Treuhaendisch.ToString();
		}
		*/
		
	}
	
}