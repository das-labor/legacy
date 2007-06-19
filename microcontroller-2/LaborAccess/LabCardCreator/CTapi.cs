using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text;

namespace CTapi 
{
	/****************************************************************************
	 * My own Exception Class
	 */
	public class CTException : ApplicationException
	{
		public CTException(string msg)
			: base(msg)
		{
		}
		
		public CTException(string msg, Exception inner)
			: base(msg, inner)
		{
		}
	}

	/***************************************************************************
	 * Interface to libtowitoko CT_api
	 */
	public class CT 
	{
		/*********************************************************************
		 * actual P/Invoke for native library
		 */
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
		                                     
	
		/*********************************************************************
		 * Utility Methoids
	 	 */
		public string Hexify(byte[] bytes, int lr)
		{
			StringBuilder sb = new StringBuilder(80);
			
			for(int i=0; i<lr; i++) {
				sb.AppendFormat( "{0:x2} ", bytes[i] );				
			}
			return sb.ToString();
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
			int count, ret=0;

			// Sometimes CT_init failes without reason
			// Try it 3 times...
			for(count=0; count<3; count++) {
				ret = CT_init(ctn, (ushort)pn);

				if (ret==0)
					break;
				Thread.Sleep(500);
				Console.WriteLine( "Opening CardTerminal ({0} try failed)", count+1 );
			}

			if (ret != 0)
				throw new CTException( String.Format ("Could not open Card Terminal (ret={0:d})", ret) );
		}
		
		~CT()
		{
			CT_close(ctn);
		}
		
		// <summary>
		//   Reset Card Terminal
		// </summary>
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
		
			throw new CTException( 
				String.Format("Could not reset Card Terminal (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
			);
		}
		
		// <summary>
		//   Return card inserted status
		// </summary>
		public bool CardInserted()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x13, 0x00, 0x80, 0x00 };  //  
			ushort lr = 20;			
			byte[] rsp = new byte[lr];

			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);

			if ( (rsp[1] != 0x90) || (rsp[2] != 0x00) ) {
				// Console.Error.WriteLine( String.Format("Could not get status (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) ) );
				throw new CTException( 
				 	String.Format("Could not get status (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
				);
			}
				
			if (rsp[0] == 0x00) {
				return false;
			} else if (rsp[0] == 0x05) { 
				return true;
			} else {
				throw new CTException( 
					String.Format("Get status return bogus stuff (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
				);
			};
		}

		// <summary>
		//   Request ICC
		// </summary>
		public bool RequestICC()
		{
			sbyte ret;
			byte   dad = ADDR_CT;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x20, 0x12, 0x01, 0x01, 0x00 };  //  
			ushort lr = 20;			
			byte[] rsp = new byte[lr];

			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);
		
			if ( (ret == 0) && (rsp[lr-2] == 0x90) && (rsp[lr-1] == 0x00) )
				return true;
			
			throw new CTException( 
				String.Format("Could not request ICC (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
			);
		}
		
		// <summary>		
		//    Select MainFile
		// </summary>
		public void SelectFile()
		{
			sbyte  ret;
			byte   dad = ADDR_ICC1;
			byte   sad = ADDR_HOST;
			byte[] cmd = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00 };  
			ushort lr  = (ushort)10;			
			byte[] rsp = new byte[lr];

			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);
			
			if ( (ret != 0) || (rsp[lr-2] != 0x90) || (rsp[lr-1] != 0x00) ) {
				throw new CTException( 
					String.Format("Could not SelectFile (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
				);
			}
		}
		
		// <summary>
		//   Read bytes form memory card
		// </summary>
		public byte[] Read(int addr, int size)
		{
			sbyte  ret;
			byte   dad = ADDR_ICC1;
			byte   sad = ADDR_HOST;
			byte[] cmd;  
			ushort lr  = (ushort)(size+5);			
			byte[] rsp = new byte[lr];
			
			// Allocate and setup cmd buffer 
			if (size < 256) {
				cmd = new byte[5];
				cmd[0] = 0x00;
				cmd[1] = 0xB0;
				cmd[2] = (byte)(addr >> 8);
				cmd[3] = (byte)(addr & 0x00FF);
				cmd[4] = (byte)size;			
			} else {
				throw new CTException( 
					String.Format("Cannot read more than 255 bytes at once.")
				);
			}
			
			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);

			if ( (ret != 0) || (rsp[lr-2] != 0x90) || (rsp[lr-1] != 0x00) ) {
				throw new CTException( 
					String.Format("Could not read bytes (ret={0:d} rsp=[{1}])", ret, Hexify(rsp, lr) )
				);
			}
			
			if (lr != size+2) {
				throw new CTException( 
					String.Format("Could not read bytes; strange lr (ret={0:d} size={1:d} rsp=[{1}])", ret, size, Hexify(rsp, lr) )
				);
			}

			// extract payload
			byte[] data = new byte[size];
			Array.Copy(rsp, data, size);
			
			return data;
		}
		
		// <summary>
		//   Write bytes onto memory card
		// </summary>
		public void Write(int addr, byte[] data)
		{
			sbyte  ret;
			byte   dad = ADDR_ICC1;
			byte   sad = ADDR_HOST;
			byte[] cmd;
			
			ushort lr  = (ushort)255;			
			byte[] rsp = new byte[lr];
			
			// Allocate and setup cmd buffer			
			if ( data.Length < 256 ) {
				cmd = new byte[data.Length + 5];
				cmd[0] = 0x00;
				cmd[1] = 0xD6;
				cmd[2] = (byte)(addr >> 8); 				
				cmd[3] = (byte)(addr & 0x00FF);
				cmd[4] = (byte)(data.Length);
				Array.Copy( data, 0, cmd, 5, data.Length );
			} else {
				cmd = new byte[data.Length + 7];
				cmd[0] = 0x00;
				cmd[1] = 0xD6;
				cmd[2] = (byte)(addr >> 8); 				
				cmd[3] = (byte)(addr & 0x00FF);
				cmd[4] = 0;
				cmd[5] = (byte)(data.Length >> 8);
				cmd[6] = (byte)(data.Length & 0x00FF);
				Array.Copy( data, 0, cmd, 5, data.Length );
			}
			
			ret = CT_data(ctn, ref dad, ref sad, (ushort)cmd.Length, cmd, ref lr, rsp);

			if ( (ret != 0) || (rsp[lr-2] != 0x90) || (rsp[lr-1] != 0x00) ) {
				throw new CTException( 
				 	String.Format("Could not write bytes (addr={0:d} length={1:d} ret={2:d} rsp=[{3}])", 
				 	               addr, data.Length, ret, Hexify(rsp, lr) )
				);
			}
			// Console.Error.WriteLine( "ReqRead: ret={0}; lr={1}, res={2}", ret, lr, Hexify(rsp, lr));			
			// Console.Error.WriteLine( "AnsWrite: ret={0}; lr={1}, res={2}", ret, lr, Hexify(rsp, lr));
			
			return;
		}
	
	}
}