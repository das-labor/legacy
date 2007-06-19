using System;
using System.Text;
using CTapi;
using Gtk;

namespace LabCardCreator
{
	public class EraseDialog : Gtk.Dialog
	{
		protected CT Ct;
		protected bool ContentFilled = false;
		
		/********************************************************************
		 * Gtk Widgets
		 */
		protected Gtk.Label    MsgLabel;
		protected Gtk.Expander ContentExpander;
		protected Gtk.TextView ContentView;
	
		/********************************************************************
		 * Constructor
		 */
		public EraseDialog(CT ct, string msg)
		{
			Ct = ct;
		
			Stetic.Gui.Build(this, typeof(LabCardCreator.EraseDialog));
			
			if (msg != null)
				MsgLabel.Markup = msg;
		}

		/********************************************************************
		 * Event Handler
		 */
		protected virtual void OnDeleteClicked(object sender, System.EventArgs e)
		{
		}

		protected virtual void OnCancelClicked(object sender, System.EventArgs e)
		{
		}

		protected virtual void OnExpander(object sender, System.EventArgs e)
		{
			if ( !ContentFilled ) {
				int    lines = 15;
				byte[] data;

				// Fill Content View with data...
				Gtk.TextBuffer buffer = ContentView.Buffer;
				try {
					data = Ct.Read(0, lines * 16);

					for(int l=0; l<lines; l++) {
						StringBuilder sb = new StringBuilder(80);
						
						sb.AppendFormat( "{0:x4}: ", l*16 );
						for(int i=0; i<16; i++)
							sb.AppendFormat( "{0:x2} ", data[l*16+i] );
							
						sb.Append( "\n" );
						buffer.Insert( buffer.EndIter, sb.ToString() );					
					}
					
					ContentFilled = true;					
				} catch (CTException ex) {
					System.Console.Error.WriteLine(ex);
				}
			}
		}
	}
}
