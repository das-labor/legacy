using System;
using Gtk;


namespace Jeton
{
	public partial class KasseScreen : Gtk.Bin
	{
		
		public KasseScreen()
		{
			this.Build();
			eventbox1.ModifyBg( Gtk.StateType.Normal, new Gdk.Color(255, 255, 255) );
		}

		protected virtual void OnFehlbetragClicked (object sender, System.EventArgs e)
		{
			Dialog win = new NumericInputDialog();
			win.ShowAll();
			win.Run();
			win.Destroy();
		}
	}
}
