// project created on 15.10.2005 at 20:13
using System;
using Gtk;
using Glade;

public class GladeApp
{
	Lampe lampe;
	Mood  mood;
	Borg  borg;
	
	[Widget] Gtk.Entry	entry1;
	[Widget] Gtk.Button button1;
	[Widget] Gtk.ColorSelection ledfoo1;
	[Widget] Gtk.ColorSelection ledfoo2;
	[Widget] Gtk.VScale lampe1Scale;
	[Widget] Gtk.VScale lampe2Scale;
	[Widget] Gtk.VScale lampe3Scale;
	[Widget] Gtk.VScale lampe4Scale;
	
	public static void Main (string[] args)
	{
		new GladeApp (args);
	}

	// .ctor
	public GladeApp (string[] args) 
	{
		Application.Init ();
		
		Glade.XML gxml = new Glade.XML (null, "gui.glade", "window1", null);
		gxml.Autoconnect (this);
		CanConnection conn = new CanConnection("rl", 2342 );
		
		InitMoods();
		InitLampe();
		InitBorg();
	
		Application.Run ();
	}

	// Quit event
	private void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
	{
		Application.Quit ();
		a.RetVal = true;
	}
	
	// Lampe Scale events
	private void OnScaleChange(object sender, EventArgs a)
	{
		if (sender == lampe1Scale) {
			lampe.lampe(0, (int)lampe1Scale.Adjustment.Value);
		} else if (sender == lampe2Scale) {
			lampe.lampe(1, (int)lampe2Scale.Adjustment.Value);
		} else if (sender == lampe3Scale) {
			lampe.lampe(3, (int)lampe3Scale.Adjustment.Value);
		} else if (sender == lampe4Scale) {
			lampe.lampe(2, (int)lampe4Scale.Adjustment.Value);
		}
	}
	
	// Mood color changer
	private void OnColorChanged(object sender, EventArgs a){
		double factor =	0.000984615;
		int    module=0;
		
		Gdk.Color foo=ledfoo1.CurrentColor;
		if(sender == ledfoo1){
			foo=ledfoo1.CurrentColor;
			module=0;
		}
		if(sender == ledfoo2){
			foo=ledfoo2.CurrentColor;
			module=1;
		}
		int red   = (int)(foo.Red * factor);
		int blue  = (int)(foo.Blue * factor);
		int green = (int)(foo.Green * factor);
		mood.SetMood(module,blue,green,red,0x00);
		
	}
	
	// Borg button
	public void OnButton(object sender, EventArgs a){
		borg.SetScroll(entry1.Text );
	}

	// Initialize GUI 
	private void InitMoods()
	{
		mood = new Mood(0x1);

		//int blue = MyMood.GetMood(0,2);
		//System.Console.WriteLine(blue);
		
		ledfoo1.ColorChanged += new EventHandler(OnColorChanged);
		ledfoo2.ColorChanged += new EventHandler(OnColorChanged);
	}
	
	private void InitLampe() 
	{
		lampe  = new Lampe(0x35);
	
		lampe1Scale.ValueChanged  += new EventHandler(OnScaleChange);
		lampe1Scale.Adjustment.Lower = 0;
		lampe1Scale.Adjustment.Upper = 0x40;

		lampe2Scale.ValueChanged += new EventHandler(OnScaleChange);
		lampe2Scale.Adjustment.Lower = 0;
		lampe2Scale.Adjustment.Upper = 0x40;

		lampe3Scale.ValueChanged += new EventHandler(OnScaleChange);
		lampe3Scale.Adjustment.Lower = 0;
		lampe3Scale.Adjustment.Upper = 0x40;

		lampe4Scale.ValueChanged += new EventHandler(OnScaleChange);
		lampe4Scale.Adjustment.Lower = 0;
		lampe4Scale.Adjustment.Upper = 0x40;
	}
	
	private void InitBorg()
	{
		borg   = new Borg(0x24);
	
		button1.Clicked += new EventHandler(OnButton);
	}
}

		