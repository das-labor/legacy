using System;
using System.Runtime.InteropServices;




namespace CTapi 
{

	public class CT 
	{
		/*********************************************************************/
		/* actual P/Invoke for native library */
		const string Library = "libtowitoko.so";
	
		[DllImport(Library)]
		private static extern sbyte CT_init(ushort ctn, ushort pn);

		[DllImport(Library)]		
		private static extern sbyte CT_close(ushort ctn);

		[DllImport(Library)]
		private static extern sbyte CT_data(ushort ctn, 
											 ref byte dad, ref byte sad,
		                                     ushort lc, byte[] cmd,
		                                     ref ushort lr, byte[] rsp);
		                                     
		/*********************************************************************/
		void DumpBytes(byte[] bytes)
		{
			foreach(byte i in bytes)
			{
				System.Console.Write( "{0:x2} ", i );				
			}
			System.Console.WriteLine("");
		}
		
		/*********************************************************************/

		private const byte ADDR_HOST    = 0x02;
		private const byte ADDR_REMHOST = 0x05;  
		private const byte ADDR_CT      = 0x01;		
		private const byte ADDR_ICC1    = 0x00;		
		private const byte ADDR_ICC2    = 0x02;
		private const byte ADDR_ICC3    = 0x03;
		private const byte ADDR_ICC4    = 0x04;		
		
		private ushort ctn = 1; 
		
		public CT(int pn)
		{
			int ret;

			ret = CT_init(ctn, (ushort)pn);
			
			if (ret != 0) {
				System.Console.WriteLine("CT_init: " + ret);			
				throw new ApplicationException( "Could not open Card Terminal" );
			}
		}
		
		~CT()
		{
			int ret;
			
			ret = CT_close(ctn);
		}
		
		public void Reset()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x11, 0x00, 0x00 };
			ushort lr = 4;			
			byte[] rsp = new byte[lr];
			
			ret = CT_data(ctn, ref dad, ref sad, 4, cmd, ref lr, rsp);
			
			if ( (ret==0) && (rsp[0]==0x90) && (rsp[1]==0x00) )
				return;
		
			throw new ApplicationException( "Could not reset Card Terminal" ); 
		}
		
		public bool RequestICC()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x12, 0x01, 0x00, 0x00 };  // RequestICC, ICC1, retrun ATR 
			ushort lr = 4;			
			byte[] rsp = new byte[lr];
			
			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);			
		
			if (ret != 0)
				throw new ArgumentException();
			
			System.Console.Write( "RequestICC: (ret={0:d} lr={1:d}) ", ret, lr );
			DumpBytes(rsp);
			return true;
		}
		
		public bool GetStatus()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x13, 0x00, 0x80, 0x00 };  //  
			ushort lr = 20;			
			byte[] rsp = new byte[lr];

			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);


			
			if ( (rsp[1] != 0x90) || (rsp[2] != 0x00) ) {
				System.Console.Write( "RequestICC: (ret={0:d} lr={1:d}) ", ret, lr );
				DumpBytes( rsp );
				throw new ArgumentException();
			}
				
			if (rsp[0] == 0x00)
				return false;
			else if (rsp[0] == 0x05) 
				return true;

			System.Console.Write( "RequestICC: (ret={0:d} lr={1:d}) ", ret, lr );
			DumpBytes( rsp );

			throw new ArgumentException();			
		}
		
		
		public byte[] Read(int addr, int size)
		{
			return new byte[0];
		}
	}
	
}