using System;
using System.Threading;
using System.Text;

using NUnit.Framework;
using CTapi;

[TestFixture]
public class CtTests
{
	// Configure COM port
	public const int pn = 1;
	public CT ct;
	
	/****************************************************************************
	 * Utility Methoids
	 */
	string Hexify(byte[] bytes)
	{
		StringBuilder sb = new StringBuilder(80);
		
		for(int i=0; i<bytes.Length; i++) {
			sb.AppendFormat( "{0:x2} ", bytes[i] );				
		}
		return sb.ToString();
	}
	
	/****************************************************************************
	 * Constructor
	 */
	public CtTests()
	{
		// Aquire CT_api object
		ct = new CT(pn);
	}

	/****************************************************************************
	 * Tests
	 */


	[Test]
	public void ResetTest()
	{
		ct.Reset();
	}
	
	[Test]
	public void CardInserted()
	{
		if (ct.CardInserted())
			Console.WriteLine( "Karte im Slot!" );
		else
			Console.WriteLine( "Keine Karte im Slot!" );
	}
	
	[Test]
	public void Read()
	{
		ct.RequestICC();
		Thread.Sleep(200);
		ct.SelectFile();
		Thread.Sleep(200);
		
		Console.WriteLine( "ReadResult: " + Hexify( ct.Read(10, 10) ) );
		Console.WriteLine( "ReadResult: " + Hexify( ct.Read( 5, 10) ) );
		Console.WriteLine( "ReadResult: " + Hexify( ct.Read( 0, 10) ) );		
	}
	
	[Test]
	public void Write()
	{
		byte[] data = { 0x00, 0xA7, 0xFF, 0x51 };
		
		ct.Reset();
		ct.RequestICC();
		Thread.Sleep(200);
		ct.SelectFile();
		Thread.Sleep(200);
		
//		data = ct.Read(40, 10); Console.WriteLine( "ReadResult: " + Hexify(data) );
/*		
		for(int i=0; i<data.Length; i++)
			data[i] = (byte)(data[i]+i+1);		
			// data[i] = 0xff;
*/

		ct.Write(0, data);
		
		data = ct.Read(0, 0x10); Console.WriteLine( "ReadResult: " + Hexify(data) );		
	}
}
