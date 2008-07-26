// NumericInputDialog.cs created with MonoDevelop
// User: joerg at 13:24 26.07.2008
//
// To change standard headers go to Edit->Preferences->Coding->Standard Headers
//


using System;
using Gtk;

namespace Jeton
{
	
	public partial class NumericInputDialog : Gtk.Dialog
	{
		public decimal val = 0;
		public bool currencyMode = true;
		public decimal maxval = 999.99m;
		
		public NumericInputDialog(string info)
		{
			this.Build();
			
			infoLabel.Markup = info;
			Button btn;
			uint row, col;
			
			for(uint i=0; i< 10; i++){
				row = (i-1)/3;
				col = (i-1)%3;
				if(i == 0){
					row=3; col=1;
				}
				
				btn = new Button(i.ToString());
				btn.SetUsize(80, 80);

				btn.Name = i.ToString();
				btn.Clicked += new EventHandler(OnNumberClicked);
				
				ButtonTable.Attach(btn, col, col+1, row, row+1);
			
			}

			row=3; col=2;
			btn = new Button("gtk-go-back");
			btn.SetUsize(80, 80);			
			btn.Clicked += new EventHandler(OnBackspaceClicked);
			ButtonTable.Attach(btn, col, col+1, row, row+1);

			
			Update();
			
		}

		void Update(){
			if(currencyMode){
				valueLabel.Markup = "<span size=\"xx-large\"><b>" + string.Format("{0:c}", val) + "</b></span>";
			}else{
				valueLabel.Markup = "<span size=\"xx-large\"><b>" + string.Format("{0:d}", (int)val) + "</b></span>";
			}
		}		
		void OnNumberClicked(object sender, EventArgs a){
			decimal newval = val;
			Button btn = sender as Button;
			if(currencyMode){
				//Console.WriteLine(btn.Name);
				newval *= 10;
				newval += decimal.Parse (btn.Name) /100;
			}else{
				newval *= 10;
				newval += decimal.Parse (btn.Name);
			}
			if(newval <= maxval)
				val = newval;
			
			Update();
		}
		
		void OnBackspaceClicked(object sender, EventArgs a){
			if(currencyMode){
				val = ((decimal)(int)(val*10))/100 ;
			}else{
				val = (int)val/10;
			}
			Update();
		}
	
	}
}

