using System;
using Gtk;


namespace Jeton
{
	[System.ComponentModel.Category("Jeton")]
	[System.ComponentModel.ToolboxItem(true)]
	public partial class KasseScreen : Gtk.Bin
	{
		
		public KasseScreen()
		{
			this.Build();
			eventbox1.ModifyBg( Gtk.StateType.Normal, new Gdk.Color(255, 255, 255) );
		}

		protected virtual void OnFehlbetragClicked (object sender, System.EventArgs e)
		{
			TextualInputDialog win = new TextualInputDialog ("Bitte Nickname eingeben!");
			
			win.ShowAll ();
			win.Run();			
			
			
//			NumericInputDialog win = new NumericInputDialog("Wieviel Geld befindet sich in der Kasse ?");
	//		win.currencyMode = true;
	//		win.ShowAll();
	//		win.Run();
			
			
			win.Destroy();
		}
	}
}
