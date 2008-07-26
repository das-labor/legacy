// TextualInputDialog.cs created with MonoDevelop
// User: tixiv at 18:25 26.07.2008
//
// To change standard headers go to Edit->Preferences->Coding->Standard Headers
//

using System;
using Gtk;

namespace Jeton
{
	
	
	public partial class TextualInputDialog : Gtk.Dialog
	{
		Button _shiftButton;
		public string text = "";
					
		public TextualInputDialog(string info)
		{
			this.Build();
			infoLabel.Markup = info;
			BuildKeyboard();
		}
		
		
		void BuildKeyboard(){
			string keys =
				    "1234567890ß<backspace>\n" +
					"<#25>qwertzuiopü\n" +
					"<#40>asdfghjklöä\n" +
					"<shift>yxcvbnm,.-\n" +
					"<#0><space><#0>\n";

			string key;
			uint row=0, col=0;
			
			HBox hbox = new HBox();
			Button btn;
			
			while(keys.Length != 0){
				if(keys.StartsWith("<")){
					key = keys.Substring(1, keys.IndexOf(">")-1);
					keys = keys.Remove(0,keys.IndexOf(">")+1);
				}else{
					key = keys.Substring(0,1);
					keys = keys.Remove(0,1);
				}
				
				if(key.StartsWith("#")){
					int width = int.Parse( key.Substring(1) );
					Label lbl = new Label();
					lbl.SetSizeRequest(width, 50);
					if(width != 0){
						hbox.PackStart(lbl, false, false, 0);
					}else{
						hbox.PackStart(lbl, true, true, 0);
					}
				}else{
				
					switch(key){
					case "\n":
						keyboardVBox.Add(hbox);	
						hbox = new HBox();
						break;
					case "shift":{
						Image img = new Image( "gtk-go-up",  IconSize.Button);								
						btn = new ToggleButton();
						btn.Image = img;
						btn.SetSizeRequest(65,50);
						hbox.PackStart(btn, false, false, 0);
						_shiftButton = btn;
						break;
					}
					case "space":
						btn = new Button();
						btn.SetSizeRequest(300,50);
						btn.Name = " ";
						btn.Clicked += new EventHandler(OnKeyClicked);
						hbox.PackStart(btn, false, false, 0);
						break;
					case "backspace":{
						Image img = new Image( "gtk-go-back",  IconSize.Button);
						btn = new Button();
						btn.Image = img;
						btn.SetSizeRequest(50,50);
						btn.Clicked += new EventHandler(OnBackspaceClicked);
						hbox.PackStart(btn, false, false, 0);
						break;
					}	
					default:
						btn = new Button(key);
						btn.SetSizeRequest(50,50);
						btn.Name = key;
						btn.Clicked += new EventHandler(OnKeyClicked);
						hbox.PackStart(btn, false, false, 0);
						break;
					}
				}
				//Console.WriteLine(key);
			}
		}
		
		void OnKeyClicked(object sender, EventArgs a){
			Button btn = sender as Button;
			text += btn.Name;
			Update();
		}
		
		void OnBackspaceClicked(object sender, EventArgs a){
			if(text.Length > 0){
				text = text.Remove(text.Length-1,1);
				Update();
			}
		}
				
		
		void Update(){
			textLabel.Markup = "<span size=\"xx-large\"><b>" + text + "</b></span>";		
		}
		
	}
}
