// project created on 05.10.2005 at 23:05
using System;
using Gtk;
using Glade;

public class GladeApp
{
	public static void Main (string[] args)
	{
		new GladeApp (args);
	}

	[Widget] Gtk.VScale scale1;
	[Widget] Gtk.VScale scale2;
	[Widget] Gtk.VScale scale3;
	[Widget] Gtk.VScale scale4;
	
	Lampe lampe;

	/*************************************************************************
	 * Constructor
	 */
	public GladeApp (string[] args) 
	{
		Application.Init ();

		Glade.XML gxml = new Glade.XML (null, "gui.glade", "window1", null);
		gxml.Autoconnect (this);
		
		CanConnection conn = new CanConnection("rl", 2342 );

		lampe = new Lampe(0x35);
		
		scale1.ValueChanged += new EventHandler(OnScaleChange);
		scale1.Adjustment.Lower = 0;
		scale1.Adjustment.Upper = 0x40;

		scale2.ValueChanged += new EventHandler(OnScaleChange);
		scale2.Adjustment.Lower = 0;
		scale2.Adjustment.Upper = 0x40;

		scale3.ValueChanged += new EventHandler(OnScaleChange);
		scale3.Adjustment.Lower = 0;
		scale3.Adjustment.Upper = 0x40;


		scale4.ValueChanged += new EventHandler(OnScaleChange);
		scale4.Adjustment.Lower = 0;
		scale4.Adjustment.Upper = 0x40;

	
		Application.Run ();
	}

	void OnScaleChange(object sender, EventArgs a)
	{
		if (sender == scale1) {
			System.Console.WriteLine( "scale1" );
			lampe.lampe(0, (int)scale1.Adjustment.Value);
		} else if (sender == scale2) {
			System.Console.WriteLine( "scale2" );
			lampe.lampe(1, (int)scale2.Adjustment.Value);
		} else if (sender == scale3) {
			System.Console.WriteLine( "scale3" );
			lampe.lampe(2, (int)scale3.Adjustment.Value);
		} else if (sender == scale4) {
			System.Console.WriteLine( "scale4" );
			lampe.lampe(3, (int)scale4.Adjustment.Value);
		}
	}
	

	// Connect the Signals defined in Glade
	void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
	{
		Application.Quit ();
		a.RetVal = true;
	}
}

