
using System;
using System.Runtime.InteropServices;


namespace I2cUsb
{
	
	
	public class I2cUsb
	{
		// actual P/Invoke for native library
		const string Library = "libI2cUsb";
			
		/* @description Connects to a rfmusb device
 		 * returns != 0 on error */
		[DllImport(Library)]
		static extern int i2cusb_Connect(ref System.IntPtr handle);

		/* @description Connects to a rfmusb device with the given vid, pid, vendor and product string
		 * returns != 0 on error */
		[DllImport(Library)]
		static extern int i2cusb_ConnectEx(ref System.IntPtr handle, int vid, int pid, string vendor, string product);
		
		/* @description Formats a given chunk of data accordingly and hands it over to the
		 * transmitting function. */
		[DllImport(Library)]
		static extern int i2cusb_WriteCard (System.IntPtr udhandle, int len, byte[] data);
		
		/* @description Receives a packet */
		[DllImport(Library)]
		static extern int i2cusb_ReadCard  (System.IntPtr udhandle, int len, ref byte[] data);
		
		/* @description Receives the card (present or not?) status */
		[DllImport(Library)]
		static extern int i2cusb_GetCardStatus (System.IntPtr udhandle, ref int cardStatus);
		
		/* @description Open the cash tray*/
		[DllImport(Library)]
		static extern int i2cusb_ShowMeTheMoney (System.IntPtr udhandle);
		
		[DllImport(Library)]
		static extern int i2cusb_SetScannerPower (System.IntPtr udhandle, int state);
		
		
		System.IntPtr handle;
		
		public I2cUsb()
		{
			if(i2cusb_Connect(ref handle) != 0){
				throw new ApplicationException("could not connect to I2cUsb");;
			}
			
		}
		
		public void WriteCard (int addr, byte[] data){
			
			
		}
		
		public byte[] ReadCard(int addr, int length){
			byte[] data = new byte[length];
			i2cusb_ReadCard(handle,length, ref data);
			return data;
		}
		
		
		
	}
}
