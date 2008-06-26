using System;
using System.Collections.Generic;
using Gtk;
using JetonDb;

namespace JetonAdmin
{
	public partial class CardScreen : Gtk.Bin
	{
		ListStore cardList;
		Person    currentPerson;
		
		public CardScreen()
		{
			this.Build();
			
			cardList = new ListStore( typeof(Person), typeof(string) );
			UpdateCardList();
			
			cardListView.AppendColumn( "Name", new CellRendererText(), "text", 1 );
			cardListView.Model = cardList;
			cardListView.Selection.Changed += OnSelectionChanged;
			
			UpdatePanel();
		}
		
		void UpdateCardList()
		{
			foreach( Person p in JetonCtrl.GetAllPersons()) {
				Console.WriteLine( "Adding {0}", p.Name );
				cardList.AppendValues( p, p.Name );
			}
		}
		
		void UpdatePanel()
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
		}
		
		void OnSelectionChanged(object o, EventArgs args)
		{
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				currentPerson = cardList.GetValue (iter, 0) as Person;

			} else {
				currentPerson = null;
			}
			UpdatePanel();
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

		void OnNameEntryChanged (object sender, System.EventArgs e)
		{
			// Update CardListView
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				cardList.SetValue(iter, 1, cardNameEntry.Text); 
			}
		}

		void OnSaveClicked (object sender, System.EventArgs e)
		{
			Console.WriteLine( "EditDone" );
			
			currentPerson.Name    = cardNameEntry.Text;
			currentPerson.Deposit = decimal.Parse( cardCreditEntry.Text );
			currentPerson.Escrow  = decimal.Parse( cardEscrowEntry.Text );
			
			JetonCtrl.SavePerson( currentPerson );
		}

	}
}
