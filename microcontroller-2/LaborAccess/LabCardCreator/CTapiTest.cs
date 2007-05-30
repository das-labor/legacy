using NUnit.Framework;
using CTapi;

[TestFixture]
public class CtTests
{
	[Test]
	public void ResetTest()
	{
		CT ct = new CT(1);
		ct.Reset();
	}
	
	[Test]
	public void RequestICC()
	{
		CT ct = new CT(1);
		ct.GetStatus();
	}
	
}
