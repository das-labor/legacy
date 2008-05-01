using System;

namespace Jeton
{
	
	
	public partial class KasseScreen : Gtk.Bin
	{
		
		public KasseScreen()
		{
			this.Build();
			eventbox1.ModifyBg( Gtk.StateType.Normal, new Gdk.Color(255, 255, 255) );
		}
	}
}
