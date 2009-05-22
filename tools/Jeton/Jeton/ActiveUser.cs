using System;
using System.Threading;
using Mono.Security;   // for ASN.1 parsing
using CTapi;
using JetonDb;

namespace Jeton
{
	public delegate void ActiveUserChangedHandler();
	public delegate void OtherCardInsertedHandler();	
	public delegate void CardTerminalProblemHandler();

	/// <summary>
	///  Manange the Card Terminal an provide up-to-date information on
	///  which user is currently logged in.
	/// </summary>
	public sealed class ActiveUser
	{
		/// <value>
		///  Singleton Pattern: return the one and only instance
		/// </value>
		public static ActiveUser Instance
		{
			get { return instance; }
		}
		static readonly ActiveUser instance = new ActiveUser();		
		
		// Protected member variables
		CT    ct;
		int   errorCounter;
		
		/// <value>
		///  Does the CardTerminal currently havre a card inserted?  
		/// </value>
		public bool CardInSlot {
			        get { return _cardInSlot; }
			private set { _cardInSlot = value; }
		}
		bool _cardInSlot;

		/// <value>
		///  Active Person when no card is inserted or card terminal is 
		///  out of order. 
		/// </value>
		public Person DefaultUser {
			get { return _defaultUser; }
			set { _defaultUser = value; }
		}
		Person _defaultUser;
		
		/// <value>
		///  Current active user. Defaults to DefaultUser when no card is
		///  inserted.
		/// </value>
		public Person CurrentUser {
			        get { return _currentUser; }
			private set { _currentUser = value; }
		}
		Person _currentUser;

		/// <summary>
		/// 
		/// </summary>
		public void InsertOtherCard( OtherCardInsertedHandler handler, uint timeout)
		{
			_otherCardHandler = handler;
			
			GLib.Timeout.Add(timeout, new GLib.TimeoutHandler(OnOtherCardTimeout));
		}
		OtherCardInsertedHandler _otherCardHandler;
		
		private bool OnOtherCardTimeout()
		{
			_otherCardHandler = null;
			return false;
		}
		
		/// <summary>
		///  Constructor opens the CartTerminal 
		/// </summary>
		private ActiveUser()
		{
			errorCounter = 0;
			ct = null;
		}

		/// <summary>
		///  Open the CardTerminal   
		/// </summary>
		public bool OpenCT(int port)
		{
			/*
			// Open CardTerminal
			try {
				ct = new CT(port);
				OnCtConnected();
				return true;
			} catch ( CTException ex ) {
				System.Console.WriteLine( ex );
				return false;
			}
			*/
			return true;
		}
		
		/// <summary>
		///  Close the connection to the CardTerminal
		/// </summary>
		public void CloseCT()
		{
			OnCtDisconnect();
		}
		
		/// <summary>
		///  Notify client when the active user changes because a card was inserted/removed. This
		///  event does not happen, when a card war inserted/removed while such event was expected
		///  (e.g. CreateNewCard)   
		/// </summary>
		public event ActiveUserChangedHandler ActiveUserChanged;
		
		/// <summary>
		///  Notify client when the connection to the card terminal was lost.
		/// </summary>
		public event CardTerminalProblemHandler CardTerminalProblem;
		
		/// <summary>
		///  Called when CT was sucsessfully opened 
		/// </summary>
		private void OnCtConnected()
		{
			// Check for new card once a second
			GLib.Timeout.Add(1000, new GLib.TimeoutHandler(CheckCardStatus));
		}
		
		/// <summary>
		///  Called when connection to CT was teared down
		/// </summary>
		private void OnCtDisconnect()
		{
			ct = null;
		}
		
		/// <summary>
		///  Called regulary (1 Hz) by GLib.Timeout
		/// </summary>
		private bool CheckCardStatus()
		{
			if (ct == null)
				return false;    // stop checking
			
			try {
				bool card;
				
				card = ct.CardInserted();
				
				if (card && !CardInSlot ) {
					if ( !ct.RequestICC() )
						return true;
					CardInSlot = true;
					
					Thread.Sleep(200);					
					OnCardInserted();
				} else if (!card && CardInSlot) {
					OnCardRemoved();
					CardInSlot = false;
				};
			} catch (CTException ex) {
				Console.Error.WriteLine( ex );
				
				errorCounter++;
				if (errorCounter == 2) {
					// XXX
					return false;
				}
			}
			
			return true;       // recheck in 1s
		}
		
		private void OnCardInserted()
		{
			try {
				byte [] data;
				
				// Read ASN1 data from card
				ct.SelectFile();
				data = ct.Read(0, 80-4);
			
				Console.WriteLine( "Card content: " + ct.Hexify(data, data.Length) );
			
				// Decode ASN1 struct
				// ASN1 root = new ASN1( data );
				// Console.WriteLine( root );
			} catch (CTException ex) {
				Console.WriteLine( ex );
			} catch (Exception ex) {
				Console.WriteLine( "ASN1 parsing exception:" );
				Console.WriteLine( ex );
			}
			
			if (ActiveUserChanged != null)
				ActiveUserChanged();
		}
		
		private void OnCardRemoved()
		{
			if (ActiveUserChanged != null)
				ActiveUserChanged();
		}
		
		public void CreateCard(Person person)
		{
			ASN1 root   = new ASN1( 0xE3 );
			ASN1 jeton  = new ASN1( 0xE4 ); root.Add( jeton );
			ASN1 uid    = new ASN1( 0xC1 ); jeton.Add( uid );
			ASN1 token0 = new ASN1( 0xC2 ); jeton.Add( token0 );
			ASN1 token1 = new ASN1( 0xC3 ); jeton.Add( token1 );			
			
			byte[] uid_val = new byte[2];
			uid_val[0] = (byte)((person.Id >> 8) & 0xFF);
			uid_val[1] = (byte)((person.Id >> 0) & 0xFF);
			uid.Value = uid_val;
			
			byte[] token_val = new byte[4];
			token0.Value = token_val;
			token1.Value = token_val;			
			
			byte[] data = root.GetBytes();
			
			Console.WriteLine( "Writing to card: " + ct.Hexify( data, data.Length ) );
			
			ct.SelectFile();
			Thread.Sleep(200);								
			ct.Write(4, data);
		}
	}
}
