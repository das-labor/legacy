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
		bool      changed;
		
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
			// First chef if we need to save currentPerson?
			if (changed) 
				System.Console.WriteLine( "Need too save!" );
			
			// Load choosen Persion and display 
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				currentPerson = cardList.GetValue (iter, 0) as Person;
				writeToCardBtn.Sensitive = true;
				Db2Gui();
			} else {
				currentPerson = null;
				writeToCardBtn.Sensitive = false;
			}
		}
		
		void OnChanged (object sender, System.EventArgs e)
		{
			// Update CardListView
			TreeIter iter;
			
			if (cardListView.Selection.GetSelected(out iter)) {
				cardList.SetValue(iter, 1, cardNameEntry.Text); 
			}

			changed           = true;
			undoBtn.Sensitive = true;
			saveBtn.Sensitive = true;
		}
	
		protected virtual void OnRoleBtnClicked (object sender, System.EventArgs e)
		{
			changed           = true;
			undoBtn.Sensitive = true;
			saveBtn.Sensitive = true;
		}


		void Gui2Db()
		{
			if (currentPerson == null)
				return;
			
			currentPerson.Name    = cardNameEntry.Text;
			currentPerson.Deposit = decimal.Parse( cardCreditEntry.Text );
			currentPerson.Escrow  = decimal.Parse( cardEscrowEntry.Text );
			
			currentPerson.Permissions.Set( Perm.Laborant, cardPermLaborant.Active ); 
			currentPerson.Permissions.Set( Perm.Treuhaender, cardPermEscrow.Active );
			currentPerson.Permissions.Set( Perm.CardCreator, cardPermCreator.Active );			
			currentPerson.Permissions.Set( Perm.God, cardPermGod.Active ); 
			
			JetonCtrl.SavePerson( currentPerson );
			changed           = false;
			undoBtn.Sensitive = false;
			saveBtn.Sensitive = false;
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
				
				cardIdEntry.Text        = currentPerson.Id.ToString();
				cardNameEntry.Text      = currentPerson.Name;
				cardCreditEntry.Text    = currentPerson.Deposit.ToString();
				cardEscrowEntry.Text    = currentPerson.Escrow.ToString();

				cardPermLaborant.Active = currentPerson.Permissions.Contains( Perm.Laborant );
				cardPermEscrow.Active   = currentPerson.Permissions.Contains( Perm.Treuhaender );
				cardPermCreator.Active  = currentPerson.Permissions.Contains( Perm.CardCreator );
				cardPermGod.Active      = currentPerson.Permissions.Contains( Perm.God );				
			
			}
			
			changed = false;
			undoBtn.Sensitive = false;
			saveBtn.Sensitive = false;
		}
		

		void OnUndoClicked (object sender, System.EventArgs e)
		{
			Db2Gui();
		}

		protected virtual void OnSaveClicked (object sender, System.EventArgs e)
		{
			Gui2Db();
		}

		protected virtual void OnSaveToCardClicked(object sender, System.EventArgs e)
		{
			// ToDo 
		}

		
	}
}
