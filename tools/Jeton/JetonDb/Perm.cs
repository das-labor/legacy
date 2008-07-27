// Perm.cs created with MonoDevelop
// User: joerg at 22:29 26.07.2008
//
// To change standard headers go to Edit->Preferences->Coding->Standard Headers
//

using System;

namespace JetonDb
{
	public enum Perm {
		Laborant, Treuhaender, CardCreator, God, Satan
	}
	
	public class Perms
	{
		// member
		public int data;
		
		private int BitPattern(Perm p)
		{
			int idx = (int)p + 1;
			int pattern = (1 << idx);

			return pattern;
		}
		
		public Perms()
		{
			data = 0;
		}
		
        public void Add(Perm p)
        {
			data = data | BitPattern(p);
		}

		public void Remove(Perm p)
		{
			data = data & ~BitPattern(p);
        }
				
		public void Set(Perm p, bool include)
		{
			if (include)
				Add(p);
			else
				Remove(p);
		}
		
        public bool Contains(Perm p)
        {
			return ( (data & BitPattern(p)) != 0 ); 
		}
		
		public override string ToString()
		{
			return String.Format( "{0:x}", data ); 
		}
	}
}
