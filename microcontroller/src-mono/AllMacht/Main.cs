// project created on 15.10.2005 at 20:13
using System;
using Gtk;
using Glade;

public class GladeApp
{
	Lampe lampe;
	Mood MyMood;
	
	[Widget] Gtk.ColorSelection ledfoo1;
	[Widget] Gtk.ColorSelection ledfoo2;
	[Widget] Gtk.VScale scale1;
	[Widget] Gtk.VScale scale2;
	[Widget] Gtk.VScale scale3;
	[Widget] Gtk.VScale scale4;
	public static void Main (string[] args)
	{
		new GladeApp (args);
	}

	public GladeApp (string[] args) 
	{
		Application.Init ();

		Glade.XML gxml = new Glade.XML (null, "gui.glade", "window1", null);
		gxml.Autoconnect (this);
		CanConnection conn = new CanConnection("rl", 2342 );
		
		lampe  = new Lampe(0x35);
		MyMood = new Mood(0x1);
		ledfoo1.ColorChanged += new EventHandler(OnColorChanged);
		ledfoo2.ColorChanged += new EventHandler(OnColorChanged);
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

	// Connect the Signals defined in Glade
	private void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
	{
		Application.Quit ();
		a.RetVal = true;
	}
	private void OnScaleChange(object sender, EventArgs a)
	{
		if (sender == scale1) {
			System.Console.WriteLine( "scale1" );
			lampe.lampe(0, (int)scale1.Adjustment.Value);
		} else if (sender == scale2) {
			System.Console.WriteLine( "scale2" );
			lampe.lampe(1, (int)scale2.Adjustment.Value);
		} else if (sender == scale3) {
			System.Console.WriteLine( "scale3" );
			lampe.lampe(3, (int)scale3.Adjustment.Value);
		} else if (sender == scale4) {
			System.Console.WriteLine( "scale4" );
			lampe.lampe(2, (int)scale4.Adjustment.Value);
		}
	}
	private void OnColorChanged(object sender, EventArgs a){
		double factor =	0.000984615;
		int mood=0;
		Gdk.Color foo=ledfoo1.CurrentColor;
		if(sender == ledfoo1){
			foo=ledfoo1.CurrentColor;
			mood=0;
		}
		if(sender == ledfoo2){
			foo=ledfoo2.CurrentColor;
			mood=1;
		}
		int red   = (int)(foo.Red * factor);
		int blue  = (int)(foo.Blue * factor);
		int green = (int)(foo.Green * factor);
		MyMood.SetMood(mood,blue,green,red,0x00);
		Console.Out.WriteLine(foo);
		
		
	}
}

		