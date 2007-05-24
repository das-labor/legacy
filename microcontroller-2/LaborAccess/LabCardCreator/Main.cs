// project created on 4/5/2007 at 11:04 AM
using System;
using Gtk;
using NUnit.Framework;

namespace LabCardCreator
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
		}
	}
	
	[TestFixture]
	public class Tests
	{
		[Test]
		public void Test1()
		{
			Assert.Fail();
		}
	}
	
}