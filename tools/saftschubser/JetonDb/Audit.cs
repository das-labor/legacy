using System;
using System.Collections.Generic;

namespace JetonDb
{
	public class Audit
	{
		protected static void AppendLog( Person p, string msg )
		{
			string ts = DateTime.Now.ToString( "yyyy-MM-dd HH:mm:ss" );
			string line = string.Format( "{0} [{1}] {2}", ts, p.Name, msg );
			
			System.Console.Error.WriteLine(line);
		}
		
		public static void Debug( Person p, string msg)  
		{
			AppendLog(p, msg);
		}
		
		public static void Error( Person p, string msg)
		{
			AppendLog(p, msg);			
		}
		
		public static void StockOperation( Person p, string msg )
		{
			AppendLog(p, msg);			
		}
		
		public static void EscowOperation( Person p, string msg )
		{
			AppendLog(p, msg);			
		}
		
		public static void Buy( Person p, string msg )
		{
			AppendLog(p, msg);			
		}
	}
}
