using System;
using Gtk;

namespace SchubserPos
{

	public partial class MainWindow: Gtk.Window
	{	
		enum ScreenMode {
			Kaufen, Kasse, Bestand, LabCtrl, Admin
		};

		// Members
		Gtk.Widget kaufenScreen;
		Gtk.Widget kasseScreen;
		Gtk.Widget bestandScreen;
		Gtk.Widget labCtrlScreen;
		Gtk.Widget adminScreen;
		
		ScreenMode curMode;
		bool       ignoreModeSwitchEvents;
			
		public MainWindow (): base (Gtk.WindowType.Toplevel)
		{
			Build ();
			
			curMode = ScreenMode.Kaufen;
			ignoreModeSwitchEvents = false;

			// alle möglichen Screens erzeugen und einhängen
			kaufenScreen  = new KaufenScreen();
			kasseScreen   = new KasseScreen();
			bestandScreen = new BestandScreen();
			labCtrlScreen = new LabCtrlScreen();
			adminScreen   = new AdminScreen();
			
			vbox1.Add( kaufenScreen );
			vbox1.Add( kasseScreen );
			vbox1.Add( bestandScreen );
			vbox1.Add( labCtrlScreen );
			vbox1.Add( adminScreen );
			
			// Darstellung aktualisieren
			UpdateScreen();
			Fullscreen();
		}
		
		protected void OnDeleteEvent (object sender, DeleteEventArgs a)
		{
			Application.Quit ();
			a.RetVal = true;
		}

		/// <summary>
		///  Update screen appereance according to curMode
		/// </summary>
		protected void UpdateScreen()
		{
			ignoreModeSwitchEvents = true;
			
			// ToggleButtons aktualisieren 
			kaufenBtn.Active  = (curMode == ScreenMode.Kaufen);
			kasseBtn.Active   = (curMode == ScreenMode.Kasse);
			bestandBtn.Active = (curMode == ScreenMode.Bestand);
			labCtrlBtn.Active = (curMode == ScreenMode.LabCtrl);
			adminBtn.Active   = (curMode == ScreenMode.Admin);
				
			// MainArea
			kaufenScreen.Visible  = (curMode == ScreenMode.Kaufen);
			kasseScreen.Visible   = (curMode == ScreenMode.Kasse);			
			bestandScreen.Visible = (curMode == ScreenMode.Bestand);
			labCtrlScreen.Visible = (curMode == ScreenMode.LabCtrl);
			adminScreen.Visible   = (curMode == ScreenMode.Admin);
			
			// headingLabel
			headingLabel.Markup  = "[ <b>Anonymous</b> // Bar Kasse ]";
			headingLabel.Justify = Justification.Left;
			
			// vbox1.ShowAll();
			
			ignoreModeSwitchEvents = false;
		}

		protected virtual void OnModeBtnClicked (object sender, System.EventArgs e)
		{
			if (ignoreModeSwitchEvents)	return;
			
			if (sender == kaufenBtn)
				curMode = ScreenMode.Kaufen;
			else if (sender == kasseBtn)
				curMode = ScreenMode.Kasse;
			else if (sender == bestandBtn)
				curMode = ScreenMode.Bestand;
			else if (sender == labCtrlBtn)
				curMode = ScreenMode.LabCtrl;
			else if (sender == adminBtn)
				curMode = ScreenMode.Admin;
			else 
				System.Console.WriteLine( "Unknown OnModeBtnClicked sender!" );
			
			UpdateScreen();
		}

		protected virtual void OnCardInserted (object sender, System.EventArgs e)
		{
			//curUser = new Person();
		}
			
			
	}
}
	