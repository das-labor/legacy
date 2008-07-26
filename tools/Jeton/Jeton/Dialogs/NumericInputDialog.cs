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
		
		public NumericInputDialog()
		{
			this.Build();
			
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
			valueLabel.Markup = "<span size=\"xx-large\"><b>" + string.Format("{0:c}", val) + "</b></span>";
		}
		
		void OnNumberClicked(object sender, EventArgs a){
			Button btn = sender as Button;
			//Console.WriteLine(btn.Name);
			val *= 10;
			val += decimal.Parse (btn.Name) /100;
			Update();
		}
		
		void OnBackspaceClicked(object sender, EventArgs a){
			val = ((decimal)(int)(val*10))/100 ;
			Update();
		}
	
	}
}

