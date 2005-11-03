// project created on 27.10.2005 at 20:19
using System;
using Gtk;
using Glade;

public class GladeApp
{
	// Dies ist nur ein dummer Kommentar
	[Widget] private Gtk.Button buttonAdd;
	[Widget] private Gtk.TreeView treeview1;
	private TreeStore store;
	

	public static void Main (string[] args)
	{
		new GladeApp (args);
	}

	public GladeApp (string[] args) 
	{
		Application.Init ();
		
		Glade.XML gxml = new Glade.XML (null, "gui.glade", "window1", null);
		gxml.Autoconnect (this);
		treeview1.HeadersVisible = true;
		buttonAdd.Clicked += new EventHandler(OnAddClicked);
		TreeViewColumn col = new TreeViewColumn();
		CellRenderer cell = new CellRendererText();
		col.Title = "Befehl";
		col.PackStart(cell, true);
		col.AddAttribute(cell, "text", 0);
		treeview1.AppendColumn(col);
		store = new TreeStore(typeof(string));
		treeview1.Model = store;
		
		Application.Run ();			
	}
	
	private void OnAddClicked(object sender, EventArgs args)
	{
		store.AppendValues("LineTo 342 3423");
	}

	// Connect the Signals defined in Glade
	private void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
	{
		Application.Quit ();
		a.RetVal = true;
	}
}
//02366-43437
