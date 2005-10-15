// created on 05.10.2005 at 23:21

using System;
using System.Runtime.InteropServices;

class LibLap {
	/* CANN */
	[DllImport("liblap.so")]
	public static extern IntPtr cann_connect(string host, int port);

	[DllImport("liblap.so")]
	public static extern void cann_close(IntPtr conn);

	/* CAN */
	[DllImport("liblap.so")]
	public static extern IntPtr can_init(IntPtr conn);

	[DllImport("liblap.so")]
	public static extern void can_setfilter();
	
//	[DllImport("liblap.so")]
//	public static extern void can_setmode();
	
	[DllImport("liblap.so")]
	public static extern void can_transmit(IntPtr msg);

	[DllImport("liblap.so")]
	public static extern IntPtr can_get_nb();
	
	[DllImport("liblap.so")]
	public static extern void can_free(IntPtr msg);

	/* Lampe not needed here*/
	[DllImport("liblap.so")]
	public static extern int mood_set_mood(int addr,int mood, int val1, int val2,int val3,int val4);
}
		

class CanConnection {
	private IntPtr _conn;
	
	public CanConnection(string host, int port)
	{
		_conn = LibLap.cann_connect( host, port );
		LibLap.can_init(_conn);
	}
	
//	public ~CanConnection()
//	{
//		LibLap.cann_close(_conn);
//	}

}

/*class Lampe {
	int _addr;	

	public Lampe(int addr) {
		_addr = addr;
	}
	
	public void lampe(int nr, int val)
	{
		LibLap.lampe_set_lampe(_addr, nr, val);
	}
}*/

class Mood{
	int _addr;	

	public Mood(int addr) {
		_addr = addr;
	}
	
	public void SetMood(int mood,int val1,int val2, int val3, int val4)
	{
		LibLap.mood_set_mood(_addr, mood, val1, val2, val3, val4);
	}

}	
