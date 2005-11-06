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

	/*Ping*/
	[DllImport("liblap.so")]
	public static extern void lap_ping(int addr);
	 /*Pong*/
	 [DllImport("liblap.so")]
	 public static extern int lap_getpong(int addr);
	

	/* Lampe */
	[DllImport("liblap.so")]
	public static extern int lampe_set_lampe(int addr, int lampe, int val);
	
	/* Mood */
	[DllImport("liblap.so")]
	public static extern int mood_set_mood(int addr,int mood, int val1, int val2,int val3,int val4);
	[DllImport("liblap.so")]
	public static extern int mood_get_mood(int addr,int mood,int moodled);
	
	/* Borg */	
	[DllImport("liblap.so")]
	public static extern int borg_scroll_borg(int addr,string String);
	[DllImport("liblap.so")]
	public static extern int borg_fire_borg(int addr);
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

class Lampe {
	int _addr;	

	public Lampe(int addr) {
		_addr = addr;
	}
	
	public void lampe(int nr, int val)
	{
		LibLap.lampe_set_lampe(_addr, nr, val);
	}
}

class Mood{
	int _addr;	

	public Mood(int addr) 
	{
		_addr = addr;
	}
	
	public void SetMood(int mood,int val1,int val2, int val3, int val4)
	{
		LibLap.mood_set_mood(_addr,mood, val1, val2, val3, val4);
	}
	
	public int GetMood(int mood,int moodled){
		int moody;
		moody = LibLap.mood_get_mood(_addr,mood,moodled);
		return moody;
	}
}
	
class Borg{
	int _addr;
	
	public Borg(int addr)
	{
		_addr = addr;
	}
	
	public void SetScroll(string String)
	{
		LibLap.borg_scroll_borg(_addr,String);
		LibLap.borg_fire_borg(_addr);
	}
}
class Ping{
	int _addr;
	
	public Ping(int addr){
		_addr = addr;
	}
	public void PingDevice(){
		LibLap.lap_ping(_addr);
	}
	public int GetPong(){
		return LibLap.lap_getpong(_addr);
	}
}