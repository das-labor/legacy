using System;
using System.Runtime.InteropServices;


namespace OpenCT 
{
	struct ct_info_t 
	{
	}
	

	class OpenCT
	{
		// private static extern int ct_status(const ct_info_t **);
		// private static extern int ct_reader_info(uint reader, ct_info_t)   
		
		[DllImport("openct")]
		private static extern IntPtr ct_reader_connect(uint reader);
		[DllImport("openct")]		
		private static extern void ct_reader_disconnect(IntPtr h);
		
		// private static extern int  ct_reader_status    (IntPtr h, ct_info_t *);
		// private static extern int  ct_card_status      (IntPtr h, uint slot, int *status);
		// private static extern int  ct_card_set_protocol(IntPtr h, uint slot, uint protocol);
		[DllImport("openct")]		
		private static extern int  ct_card_reset       (IntPtr h, uint slot, byte[] atr, int atr_len);
		// private static extern int  ct_card_lock        (IntPtr h, uint slot, int type, IntPtr lh);
		// private static extern int  ct_card_unlock      (IntPtr h, uint slot, ct_lock_handle);
		// private static extern int  ct_card_request     (IntPtr h, uint slot, uint timeout, string msg, void *atr, size_t atr_len);
		// private static extern int  ct_card_transact    (IntPtr h, uint slot, void *apdu, size_t apdu_len, void *recv_buf, size_t recv_len);
		// private static extern int  ct_card_verify      (IntPtr h, uint slot,
		// 									uint timeout, const char *prompt,
		//									uint pin_encoding,
		//									uint pin_length,
		//									uint pin_offset,
		//									void *send_buf, int send_len,
		//									void *recv_buf, int recv_len);

		[DllImport("openct")]		
		private static extern int  ct_card_read_memory (IntPtr h, uint slot, ushort addr, byte[] recv_buf, int recv_len);
		[DllImport("openct")]		
		private static extern int  ct_card_write_memory(IntPtr h, uint slot, ushort addr, byte[] send_buf, int send_len);

		// [DllImport("openct")]
		// private static extern int  ct_status_clear(uint lala);
		
		// extern ct_info_t *	ct_status_alloc_slot(int *);
		// extern int		ct_status_update(ct_info_t *);


		//////////////////////////////////////////////////////////////////////
		// Member variables
		private IntPtr ctHandle;
		private uint   ctSlot;
		
		
		//////////////////////////////////////////////////////////////////////
		// Static Methods
		public static void Info(int reader)
		{
			// ct_reader_info(reader);
		}
		
		
		//////////////////////////////////////////////////////////////////////
		// Constructor 

		// <summary>
		// </summary>
		public OpenCT(int reader, int slot)
		{
			ctHandle = ct_reader_connect( (uint)reader);
			ctSlot = (uint) slot;
		}

		//////////////////////////////////////////////////////////////////////
		// Destructor 
		~OpenCT()
		{
			ct_reader_disconnect(ctHandle);
		}
		
		public void Close()
		{
		}

		//////////////////////////////////////////////////////////////////////
		// Methods 
		//public void SetProtocol(int p)
		//{
		//	ct_card_set_protocol(ctHandle, ctSlot, p);
		//}
		
		// <summary>
		//  Reset card and return ATR
		// </summary>
		public byte[] Reset()
		{
			byte[] buf = new byte[64];
		
			ct_card_reset(ctHandle, ctSlot, buf, buf.Length);
			return buf;
		}
		
		// <summary>
		//  Read specified memory from synchronous memory card
		// </summary>
		public byte[] ReadMemory(int offset, int size)
		{
			byte[] buf = new byte[size];
			
			ct_card_read_memory(ctHandle, ctSlot, (ushort)offset, buf, buf.Length); 
			return buf;
		}
		
		// <summary>
		//  Write byte array to synchronous memory card at specified location
		// </summary>
		public void WriteMemory(int offset, byte[] data)
		{
			ct_card_write_memory(ctHandle, ctSlot, (ushort)offset, data, data.Length);			
		}
}

}