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
	[Widget] Gtk.Entry	entry2;
	[Widget] Gtk.SpinButton spinmood1;
	
	[Widget] Gtk.Label	label5;
	[Widget] Gtk.Label  label6;
	[Widget] Gtk.Button button1;
	[Widget] Gtk.Button button2;
	[Widget] Gtk.ColorSelection ledfoo1;
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
		CanConnection conn = new CanConnection("roulette", 2342 );
		

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
		int module = (int)spinmood1.Value-1 ;
		Gdk.Color foo=ledfoo1.CurrentColor;
		mood.SetMood(module, foo.Red/256, foo.Green/256, foo.Blue/256, 0x00);
	}
	
	// Borg button
	public void OnButton(object sender, EventArgs a){
		borg.SetScroll(entry1.Text );
	}

	public void OnPingButton(object sender, EventArgs a){
		int toping = 0;
		try{
			toping = Int16.Parse(entry2.Text);
		}
		catch(Exception e){

		}
		Ping ping = new Ping(toping);
		ping.PingDevice();
	}

	private void InitMoods()
	{
		mood = new Mood(0xff);
				
		ledfoo1.ColorChanged += new EventHandler(OnColorChanged);
		
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

		