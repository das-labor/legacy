using System;
using System.Runtime.InteropServices;


namespace CTapi 
{

	class CTapi 
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
		private const byte ADDR_HOST    = 0x02;
		private const byte ADDR_REMHOST = 0x05;  
		private const byte ADDR_CT      = 0x01;		
		private const byte ADDR_ICC1    = 0x00;		
		private const byte ADDR_ICC2    = 0x02;
		private const byte ADDR_ICC3    = 0x03;
		private const byte ADDR_ICC4    = 0x04;		
		
		private ushort ctn = 1; 
		
		CTapi(int pn)
		{
			int ret;

			ret = CT_init(ctn, (ushort)pn);
		}
		
		~CTapi()
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
			
			// XXX Fehlerbehandlung XXX
		}
		
		public bool RequestICC()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x12, 0x01, 0x00, 0x00 };  // RequestICC, ICC1, retrun ATR 
			ushort lr = 4;			
			byte[] rsp = new byte[lr];
			
			ret = CT_data(ctn, ref dad, ref sad, cmd.Length, cmd, ref lr, rsp);			
		
			if (ret == 0)
				return true;
			
			System.Console.WriteLine(ret);
		}
		
		public byte[] Read(int addr, int size)
		{
		}
	}
	
}