using System;
using Gtk;
using CTapi;
using JetonDb;
using I2cUsb;

namespace Jeton
{
	public partial class MainWindow: Gtk.Window
	{	
		// Members
		Gtk.Widget kaufenScreen;
		Gtk.Widget kasseScreen;
		Gtk.Widget bestandScreen;
		Gtk.Widget labCtrlScreen;
		Gtk.Widget adminScreen;

		bool       ignoreModeSwitchEvents;
		
	
		public MainWindow (): base (Gtk.WindowType.Toplevel)
		{
			Build ();
			
			ignoreModeSwitchEvents = false;

			// alle möglichen Screens erzeugen und einhängen
			kaufenScreen  = new KaufenScreen();
			kasseScreen   = new KasseScreen();
			bestandScreen = new BestandScreen();
			labCtrlScreen = new LabCtrlScreen();
			adminScreen   = new AdminScreen();
			
			// Darstellung aktualisieren
			DisplayScreen( kaufenScreen );
			UpdateStatusDisplay();
			Fullscreen();
			
			// Open CardTerminal
			//ActiveUser au = ActiveUser.Instance;
			//au.OpenCT(1);
			//au.ActiveUserChanged   += new ActiveUserChangedHandler( OnActiveUserChanged );
			//au.CardTerminalProblem += new CardTerminalProblemHandler( OnCardTerminalProblem );
			
			I2cUsb.I2cUsb i = new I2cUsb.I2cUsb();
			byte[] foo = i.ReadCard(0,0x80);
			
			CT ct = new CT(1);
			ct.Hexify(foo,0x80);
			
			
		}

		/// <value>
		///  Set currently visible screen
		/// </value>
		Gtk.Widget currentScreen;
		public void DisplayScreen(Gtk.Widget screen) {
			// remove old screen
			if (currentScreen != null) {
				currentScreen.Visible = false;
				vbox1.Remove( currentScreen );
			};
				
			currentScreen = screen;
				
			vbox1.Add( screen );
			screen.Visible = true;
				
			UpdateButtons();
		}
		

		/// <summary>
		///  Update button-bar appereance 
		/// </summary>
		private void UpdateButtons()
		{
			ignoreModeSwitchEvents = true;

			// ToggleButtons aktualisieren 
			kaufenBtn.Active  = (currentScreen == kaufenScreen);
			kasseBtn.Active   = (currentScreen == kasseScreen);
			bestandBtn.Active = (currentScreen == bestandScreen);
			labCtrlBtn.Active = (currentScreen == labCtrlScreen);
			adminBtn.Active   = (currentScreen == adminScreen);
			
			ignoreModeSwitchEvents = false;
		}
		
		private void UpdateStatusDisplay()
		{
			// headingLabel
			headingLabel.Markup  = "[ <b>Anonymous</b> // Bar Kasse ]";
			headingLabel.Justify = Justification.Left;
		}
		
		private void OnActiveUserChanged()
		{
			System.Console.WriteLine( "New ActiveUser" );
			
			/*
			 Person p = JetonCtrl.CreatePerson();
			p.Name = "Admin";
			
			ActiveUser au = ActiveUser.Instance;
			 au.CreateCard( p );
			 */
		}

		/////////////////////////////////////////////////////////////
		/// Event-Handler ///////////////////////////////////////////
		
		private void OnCardTerminalProblem()
		{
			System.Console.WriteLine( "!! CardTerminal Problem !!" );
		}
		
		private void OnDeleteEvent (object sender, DeleteEventArgs a)
		{
			Application.Quit ();
			a.RetVal = true;
		}

	
		protected virtual void OnModeBtnClicked (object sender, System.EventArgs e)
		{
			if (ignoreModeSwitchEvents)	return;

			if (sender == kaufenBtn)
				DisplayScreen( kaufenScreen );
			else if (sender == kasseBtn)
				DisplayScreen( kasseScreen );				
			else if (sender == bestandBtn)
				DisplayScreen( bestandScreen );				
			else if (sender == labCtrlBtn)
				DisplayScreen( labCtrlScreen );				
			else if (sender == adminBtn)
				DisplayScreen( adminScreen );				
			else 
				System.Console.WriteLine( "Unknown OnModeBtnClicked sender!" );
		}

		
	}
}
	