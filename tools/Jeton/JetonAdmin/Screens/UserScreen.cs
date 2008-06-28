using System;
using System.Collections.Generic;
using Gtk;
using JetonDb;

namespace JetonAdmin
{
	public partial class UserScreen : Gtk.Bin
	{
		ListStore cardList;
		Person    currentPerson;
		
		public UserScreen()
		{
			this.Build();
			
			cardList = new ListStore( typeof(Person), typeof(string) );
			UpdateCardList();
			
			cardListView.AppendColumn( "Name", new CellRendererText(), "text", 1 );
			cardListView.Model = cardList;
			cardListView.Selection.Changed += OnSelectionChanged;
			
			Db2Gui();
		}
		
		void UpdateCardList()
		{
			foreach( Person p in JetonCtrl.GetAllPersons()) {
				Console.WriteLine( "Adding {0}", p.Name );
				cardList.AppendValues( p, p.Name );
			}
		}

		void OnCardAdd (object sender, System.EventArgs e)
		{
			TreeIter iter;
			
			Person p = JetonCtrl.CreatePerson();
			p.Name = "Neuer Laborant";
			
			iter = cardList.AppendValues( p, p.Name );
			cardListView.Selection.SelectIter(iter);
		}

		void OnCardDelete (object sender, System.EventArgs e)
		{
		}
		
		void OnSelectionChanged(object o, EventArgs args)
		{
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				currentPerson = cardList.GetValue (iter, 0) as Person;
			} else {
				currentPerson = null;
			}
			Db2Gui();
        }
		
		void OnChanged (object sender, System.EventArgs e)
		{
			// Update CardListView
			/*
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				cardList.SetValue(iter, 1, cardNameEntry.Text); 
			}
			*/

			undoBtn.Sensitive = true;			
		}

		void Gui2Db()
		{
			Console.WriteLine( "Db2Gui" );
			
			currentPerson.Name    = cardNameEntry.Text;
			currentPerson.Deposit = decimal.Parse( cardCreditEntry.Text );
			currentPerson.Escrow  = decimal.Parse( cardEscrowEntry.Text );
			
			JetonCtrl.SavePerson( currentPerson );
			undoBtn.Sensitive = false;
		}
		
		void Db2Gui()
		{
			if (currentPerson == null) {
				cardIdEntry.Sensitive     = false;
				cardNameEntry.Sensitive   = false;
				cardCreditEntry.Sensitive = false;
				cardEscrowEntry.Sensitive = false;
			} else {
				cardIdEntry.Sensitive     = true;
				cardNameEntry.Sensitive   = true;
				cardCreditEntry.Sensitive = true;
				cardEscrowEntry.Sensitive = true;
				
				cardIdEntry.Text     = currentPerson.Id.ToString();
				cardNameEntry.Text   = currentPerson.Name;
				cardCreditEntry.Text = currentPerson.Deposit.ToString();
				cardEscrowEntry.Text = currentPerson.Escrow.ToString();
			}
			undoBtn.Sensitive = false;			
		}

		void OnUndoClicked (object sender, System.EventArgs e)
		{
			Db2Gui();
		}

	}
}
