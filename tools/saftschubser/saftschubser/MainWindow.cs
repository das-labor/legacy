// MainWindow.cs created with MonoDevelop
// User: mati at 00:52 26.04.2008
//
// To change standard headers go to Edit->Preferences->Coding->Standard Headers
//
using System;
using Gtk;

namespace saftschubser
{

public partial class MainWindow: Gtk.Window
{	
	Gtk.Widget mainScreen; 
			
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
		kaufenScreen = new Kauf();
		//bestandScreen = new Bestand();
		//labCtrlScreen = new LabCtrl();
	}
	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;k.
	}

	protected virtual void kaufenClicked (object sender, System.EventArgs e)
	{
		vbox1.Remove
		vbox1.Add( new Kauf());
		vbox1.ShowAll();
	}
}
	
}