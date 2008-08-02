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
		ToggleButton _shiftButton;
		public string text = "";
		public int maxLength = 50;
		string keys_lowercase =
				    "1234567890ß<backspace>\n" +
					"<#25>qwertzuiopü\n" +
					"<#40>asdfghjklöä\n" +
					"<shift>yxcvbnm,.-\n" +
					"<#0><space><#0>\n";

		string keys_uppercase =
				    "!\"§$%&/()=?<backspace>\n" +
					"<#25>QWERTZUIOPÜ\n" +
					"<#40>ASDFGHJKLÖÄ\n" +
					"<shift>YXCVBNM;:_\n" +
					"<#0><space><#0>\n";

		public TextualInputDialog(string info)
		{
			this.Build();
			infoLabel.Markup = info;
			BuildKeyboard();
			Update();
		}
		
		
		void BuildKeyboard(){
			string keys = keys_lowercase;
			string key;
			//uint row=0, col=0;
			
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
						ToggleButton tbtn = new ToggleButton();
						tbtn.Image = img;
						tbtn.SetSizeRequest(65,50);
						tbtn.Toggled += new  EventHandler(OnShiftToggled);
						hbox.PackStart(tbtn, false, false, 0);
						_shiftButton = tbtn;
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
			}
		}
		
		void SetKeys(string keys){
			string key;
			uint row=0, col=0;
			
			HBox hbox = keyboardVBox.Children[0] as HBox;
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
					
				}else{
					switch(key){
					case "\n":
						col=0;
						row++;
						if(row < 5){
							hbox = keyboardVBox.Children[row] as HBox;
						}
						continue;
					case "shift":
						break;
					case "space":
						break;
					case "backspace":
						break;	
					default:
						if( col < hbox.Children.GetLength( 0)  ){
							btn = hbox.Children[col] as Button;
							btn.Name = key;
							btn.Label = key;
							
						}						
						break;
					}
					
				}				
				col++;			
			}

		}
		
		void OnKeyClicked(object sender, EventArgs a){
			Button btn = sender as Button;
			if(text.Length < maxLength){
				text += btn.Name;
			}
			_shiftButton.Active = false;
			Update();
		}
		
		void OnBackspaceClicked(object sender, EventArgs a){
			Console.WriteLine("foobar");
			if(text.Length > 0){
				text = text.Remove(text.Length-1,1);
				Update();
			}
		}
		
		void OnShiftToggled(object sender,  EventArgs a){
			ToggleButton btn = sender as ToggleButton;
			if(btn.Active){
				SetKeys(keys_uppercase);
			}else{
				SetKeys(keys_lowercase);	
			}
		}
		
		void Update(){
			textLabel.Markup = "<span size=\"xx-large\"><b>" + text + "</b></span>";		
		}
		
	}
}
