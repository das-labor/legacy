using System;
using NUnit.Framework;

namespace JetonDb
{
	[TestFixture()]
	public class AuditTest
	{
		Person Me;
		
		[SetUp]
		public void Init()
		{
			Me = new Person();
			Me.Name = "TEO";
		}
		
		[Test()]
		public void TestCase()
		{
			Audit.Debug( Me, "Debug Message" );
			Audit.Error( Me, "Fatal Error" );
		}
	}
}
