
using System;

namespace LabCardCreator
{
	
	
	public class AboutDialog : Gtk.Dialog
	{
		
		public AboutDialog()
		{
			Stetic.Gui.Build(this, typeof(LabCardCreator.AboutDialog));
		}
	}
}
