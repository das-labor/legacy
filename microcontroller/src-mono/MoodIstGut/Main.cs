// project created on 15.10.2005 at 17:47
using System;
using Gtk;
using Glade;


public class GladeApp
{
	
	[Widget] Gtk.ColorSelection ledfoo1;
	[Widget] Gtk.ColorSelection ledfoo2;
	[Widget] Gtk.ColorSelection ledfoo3;
	[Widget] Gtk.ColorSelection ledfoo4;
	Mood MyMood;
	public static void Main (string[] args)
	{
		new GladeApp (args);
	}

	public GladeApp (string[] args) 
	{
		Application.Init ();

		Glade.XML gxml = new Glade.XML (null, "gui.glade", "window1", null);
		gxml.Autoconnect (this);
		
		ledfoo1.ColorChanged += new EventHandler(OnColorChanged);
		ledfoo2.ColorChanged += new EventHandler(OnColorChanged);
		ledfoo3.ColorChanged += new EventHandler(OnColorChanged);
		ledfoo4.ColorChanged += new EventHandler(OnColorChanged);
		
		CanConnection conn = new CanConnection("rl", 2342 );
		MyMood = new Mood(0x1);
		Application.Run ();
	}

	// Connect the Signals defined in Glade
	private void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
	{
		Application.Quit ();
		a.RetVal = true;
	}
	private void OnColorChanged(object sender, EventArgs a){
		int mood=0;
		Gdk.Color foo=ledfoo1.CurrentColor;
		if (sender == ledfoo1){
			mood = 0;
			foo=ledfoo1.CurrentColor;
		}
		if (sender == ledfoo2){
			mood = 1;
			foo=ledfoo2.CurrentColor;
		}
		double factor =	0.000984615;
		
		int red   = (int)(foo.Red * factor);
		int blue  = (int)(foo.Blue * factor);
		int green = (int)(foo.Green * factor);
		MyMood.SetMood(mood,blue,green,red,0x00);
		
	}
}

