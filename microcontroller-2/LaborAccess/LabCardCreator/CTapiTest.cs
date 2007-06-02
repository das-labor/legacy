using System;
using System.Threading;

using NUnit.Framework;
using CTapi;

[TestFixture]
public class CtTests
{
	// Configure COM port
	public const int pn = 1;
	public CT ct;
	

	public CtTests()
	{
		// Aquire CT_api object
		ct = new CT(pn);
	}

	[Test]
	public void ResetTest()
	{
		ct.Reset();
	}
	
	[Test]
	public void GetStatus()
	{
		if (ct.GetStatus())
			Console.WriteLine( "Karte im Slot!" );
		else
			Console.WriteLine( "Keine Karte im Slot!" );
	}
	
}
