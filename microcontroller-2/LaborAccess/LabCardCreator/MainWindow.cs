using System;
using System.Threading;
using Mono.Security;
using Gtk;
using CTapi;

public class MainWindow: Gtk.Window
{
	// GTK Widgets
	protected Gtk.Entry nickEntry;
	protected Gtk.Entry nameEntry;
	protected Gtk.Entry numberEntry;
	protected Gtk.CheckButton adminCheck;
	protected Gtk.CheckButton door0Check;
	protected Gtk.CheckButton door1Check;

	// CardReader stuff
	protected CT ct;
	
	// Constructor
	public MainWindow (): base ("")
	{
		Stetic.Gui.Build (this, typeof(MainWindow));
	}

	protected bool CheckCardStatus()
	{
		if (ct == null)
			return false;
	
		try {
			if (ct.GetStatus()) {
				Console.WriteLine( "Karte is drinne" );
			} else {
				Console.WriteLine( "Nix Karte drinne" );			
			};
		} catch {
			ct = null;
			return false;
		}
		
		return true;
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	protected virtual void OnQuitActivated(object sender, System.EventArgs e)
	{
		Application.Quit ();
	}

	protected virtual void OnConnectActivated(object sender, System.EventArgs e)
	{
		ct = new CT(1);
		
		GLib.Timeout.Add (500, new GLib.TimeoutHandler(CheckCardStatus));
		Console.Error.WriteLine("Connected to Card Terminal");
	}

	protected virtual void OnDisconnectActivated(object sender, System.EventArgs e)
	{
	}

	protected virtual void OnAboutChanged(object o, Gtk.ChangedArgs args)
	{
	}

	protected virtual void OnViewHexdumpActivated(object sender, System.EventArgs e)
	{
	}

	protected virtual void OnEraseActivated(object sender, System.EventArgs e)
	{
	}

	protected virtual void OnWriteBtnClicked(object sender, System.EventArgs e)
	{
		ASN1 root  = new ASN1(0xE3);
		ASN1 sid   = new ASN1(0xC0); root.Add( sid );	
		ASN1 lid   = new ASN1(0xC1); root.Add( lid );	
		ASN1 token = new ASN1(0xC2); root.Add( token );		
		ASN1 nick  = new ASN1(0xC3); root.Add( nick );
		ASN1 name  = new ASN1(0xC4); root.Add( name );
		ASN1 perm  = new ASN1(0xC5); root.Add( perm );
		
		sid.Value   = new byte[4];
		lid.Value   = new byte[2];
		token.Value = new byte[8];
		
		nick.Value = System.Text.Encoding.ASCII.GetBytes( nickEntry.Text );
		name.Value = System.Text.Encoding.ASCII.GetBytes( nameEntry.Text );
		
		// root.SaveToFile( "/home/gast/file.asn" );
		// Console.WriteLine( root.ToString() );
	}
	
	protected virtual void OnAboutActivated(object sender, System.EventArgs e)
	{
	}

}