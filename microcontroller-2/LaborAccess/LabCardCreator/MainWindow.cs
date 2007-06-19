using System;
using System.Threading;
using Mono.Security;
using CTapi;

namespace LabCardCreator {

public class MainWindow: Gtk.Window
{
	// GTK Widgets
	protected Gtk.Button      writeButton;	
	protected Gtk.Entry       nickEntry;
	protected Gtk.Entry       nameEntry;
	protected Gtk.Entry       numberEntry;
	protected Gtk.CheckButton adminCheck;
	protected Gtk.CheckButton door0Check;
	protected Gtk.CheckButton door1Check;
	protected Gtk.Statusbar   Statusbar;
	
	protected Gtk.Action ConnectCardTerminal;
	protected Gtk.Action DisconnectCardTerminal;	

	// CardReader stuff
	CT ct;
	bool CardInSlot = false;
	
	int  StatusErrorCounter = 0;
	uint SBCtxId;
	
	// Constructor
	public MainWindow (): base ("")
	{
		Stetic.Gui.Build (this, typeof(MainWindow));
		
		SBCtxId = Statusbar.GetContextId( "Card Status" );
		Statusbar.Push(SBCtxId, "");

		// Try to open CardTerminal		
		try {
			ct = new CT(1);
			OnCTConnected();
		} catch ( CTException ex ) {
			Console.Error.WriteLine( ex );
			CTError();
		}
		
	}

	void SetStatusText(string msg)
	{
		Statusbar.Pop( SBCtxId );
		Statusbar.Push( SBCtxId, msg );
		// Console.Error.WriteLine(msg);
	}

	// CTError
	protected void CTError()
	{
		ct = null;
		OnCTDisconnected();
	}
	
	// CT Connected // Disconnected
	protected void OnCTConnected()
	{
		ConnectCardTerminal.Sensitive    = false;
		DisconnectCardTerminal.Sensitive = true;		
	
		CardInSlot = false;
		GLib.Timeout.Add (1000, new GLib.TimeoutHandler(CheckCardStatus));
		
		SetStatusText( "Connected to CardTerminal" );
	}
	
	protected void OnCTDisconnected()
	{
		ConnectCardTerminal.Sensitive    = true;
		DisconnectCardTerminal.Sensitive = false;		
		OnCardRemoved();

		CardInSlot = false;
		
		SetStatusText( "Disconnected from CardTerminal" );		
	}
	
	// Card Inserted // Removed
	protected void OnCardInserted()
	{
		writeButton.Sensitive = true;
		nickEntry.Sensitive   = true;
		nameEntry.Sensitive   = true;
		door0Check.Sensitive  = true;
		door1Check.Sensitive  = true;
		
		// Check for Empty Or Labor Card
		byte[] data;
		
		try {
			ct.SelectFile();
			data = ct.Read(0, 80); 

			// Parse card data
			byte[] data2 = new byte[80-4];
			
			Array.Copy(data, 4, data2, 0, 80-4);
			Console.WriteLine( "Card content: " + ct.Hexify(data2, data2.Length) );
			// ASN1 root = new ASN1(data2);
			// System.Console.WriteLine( root );
			// 
			// EraseDialog erase = new EraseDialog(ct, 
			//     "Inserted card is not a Labor Card.\n\n" +   
			//     "<b>Delete card content and format as Labor Card?</b>" );
			// erase.Run();
			
			System.Console.WriteLine( ct.Hexify(data, data.Length) );
		} catch (CTException e) {
			System.Console.Error.WriteLine(e);
			CTError();
		}
		
		SetStatusText( "Card inserted" );		
	}
	
	protected void OnCardRemoved()
	{
		writeButton.Sensitive = false;	
		nickEntry.Sensitive   = false;
		nameEntry.Sensitive   = false;
		door0Check.Sensitive  = false;
		door1Check.Sensitive  = false;
		
		SetStatusText( "Card removed" );				
	}

	// Check for card insertion / removes
	// Called every second
	protected bool CheckCardStatus()
	{
		bool card;
		if (ct == null)
			return false;   // stop checking
	
		try {
			card = ct.CardInserted();
			
			StatusErrorCounter = 0;
		
			if (card && !CardInSlot ) {
				if ( !ct.RequestICC() )
					return true;
				CardInSlot = true;					
			
				OnCardInserted();
			} else if (!card && CardInSlot) {
				OnCardRemoved();
				CardInSlot = false;
			};
		} catch (CTException ex) {
			Console.Error.WriteLine( ex );		
			
			StatusErrorCounter++;
			if (StatusErrorCounter == 2) {
				CTError();
				return false;
			}
		}
		
		return true;       // recheck in 1s
	}

	protected void OnDeleteEvent (object sender, Gtk.DeleteEventArgs a)
	{
		Gtk.Application.Quit ();
		a.RetVal = true;
	}

	protected virtual void OnQuitActivated(object sender, System.EventArgs e)
	{
		Gtk.Application.Quit ();
	}

	protected virtual void OnConnectActivated(object sender, System.EventArgs e)
	{
		try {
			ct = new CT(1);
			OnCTConnected();
		} catch ( CTException ex ) {
			Console.Error.WriteLine( ex );
			CTError();
		}
	}

	protected virtual void OnDisconnectActivated(object sender, System.EventArgs e)
	{
		try {
			// ct.Dispose();
			ct = null;
		} catch ( CTException ex ) {
			Console.Error.WriteLine( ex );
			CTError();
		}
		
		OnCTDisconnected();
	}

	protected virtual void OnAboutChanged(object o, Gtk.ChangedArgs args)
	{
	}

	protected virtual void OnViewHexdumpActivated(object sender, System.EventArgs e)
	{
	}

	protected virtual void OnEraseActivated(object sender, System.EventArgs e)
	{
		Gtk.ResponseType answer;
	
		EraseDialog erase = new EraseDialog(ct, 
		     "<b>Delete card content and format as Labor Card?</b>" );

		answer = (Gtk.ResponseType)erase.Run();
		erase.Destroy ();
		
		if (answer == Gtk.ResponseType.Accept) {
			// Erase
		}
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
		
		sid.Value   = new byte[2];
		lid.Value   = new byte[2];
		token.Value = new byte[8];
		nick.Value  = System.Text.Encoding.ASCII.GetBytes( nickEntry.Text );
		name.Value  = System.Text.Encoding.ASCII.GetBytes( nameEntry.Text );
		perm.Value  = new byte[1];
		
		byte[] asn1data = root.GetBytes();

		bool success = false;
		int  trycount = 0;
		do {
			byte[] hdr;
			byte[] writedata;
			byte[] readback;

			trycount++;
		
			try {
				// Read header from card
				ct.SelectFile();
				hdr = ct.Read(0, 4);
				
				// Concat Header and ASN1 data
				writedata = new byte[hdr.Length + asn1data.Length];
				hdr.CopyTo(writedata, 0);
				asn1data.CopyTo(writedata, hdr.Length);
				
				// Write complete dataset
				ct.Write(0, writedata);
				
				// Wait 500ms
				Thread.Sleep(500);
				
				// Readback (hopefully) same data
				readback = ct.Read(0, writedata.Length);
				
				// Compare
				success = true;
				for(int i=0; i<readback.Length; i++) {
					if (writedata[i] != readback[i]) {
						success = false;
						break;
					}
				}

			} catch (CTException ex) {
				Console.Error.WriteLine(ex);			
				SetStatusText(
					String.Format("Error writing to card, retrying... ({0}", trycount)
				);
			}
		} while ( !success && trycount < 5);
		
		if (success) {
			SetStatusText(
				String.Format("Wrote {0} byte to card.", asn1data.Length)
			);
		} else {
			SetStatusText(
				String.Format("Could not write to card -- PERMANENT ERROR -- Keine Lust mehr")
			);
		}
		
		
		
		// root.SaveToFile( "/home/gast/file.asn" );
		// Console.WriteLine( root.ToString() );
	}
	
	protected virtual void OnAboutActivated(object sender, System.EventArgs e)
	{
	}
}

}