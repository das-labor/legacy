// project created on 27.10.2005 at 20:19
using System;
using System.Drawing;
using Gtk;


public class StryroDesign {
	private ListStore store;
	
	private Window window;
	private VBox vbox1;
	private HandleBox handlebox1;
	private MenuBar menubar;
	private HPaned hpan;
	
	private VBox vbox2;
	private ScrolledWindow swin1;
	private Viewport vport1;
	private TreeView treeview1;
	private VButtonBox vbuttonbox1;
	private Button buttonAdd;
	private Entry entry1;
	
	private ScrolledWindow swin;
	private Viewport vport2;
	private Display drawing;
		
	public static void Main (string[] args) {
		new StryroDesign (args);
	}

	public StryroDesign (string[] args) { 
		Application.Init ();		
		store = new ListStore(typeof(string));
		
		window = new Window("Styro Design");
		window.SetSizeRequest(800, 600);
		window.DeleteEvent += new DeleteEventHandler(Window_Delete);
		
		vbox1 = new VBox();
		window.Add(vbox1);
		handlebox1 = new HandleBox();
		//vbox1.Add(handlebox1);
		menubar = new MenuBar();
		handlebox1.Add(menubar);
		hpan = new HPaned();
		vbox1.Add(hpan);
		
		vbox2 = new VBox();
		hpan.Add(vbox2);
		swin1 = new ScrolledWindow();
		vbox2.Add(swin1);
		vport1 = new Viewport();
		swin1.Add(vport1);
		treeview1 = new TreeView();
		vport1.Add(treeview1);
		vbuttonbox1 = new VButtonBox();
		vbuttonbox1.BorderWidth = 5;
		vbuttonbox1.Spacing = 5;
		vbox2.Add(vbuttonbox1);
		vbox2.SetChildPacking(vbuttonbox1, false, false, 5, Gtk.PackType.Start);
		
		entry1 = new Entry();
		vbuttonbox1.Add(entry1);
		
		buttonAdd = new Button();
		buttonAdd.Label = "Hinzufügen";
		vbuttonbox1.Add(buttonAdd);
		vbuttonbox1.Layout = ButtonBoxStyle.Start;
		          
		swin = new ScrolledWindow();
		hpan.Add(swin);
		vport2 = new Viewport();
		swin.Add(vport2);
		drawing = new Display(0.3f, 2000, 2000, store);
		vport2.Add(drawing);
		
		treeview1.HeadersVisible = true;
		buttonAdd.Clicked += new EventHandler(OnAddClicked);
		TreeViewColumn col = new TreeViewColumn(); 
		CellRenderer cell = new CellRendererText();
		
		col.Title = "Befehl";
		col.PackStart(cell, true);
		col.AddAttribute(cell, "text", 0);
		treeview1.AppendColumn(col);

		treeview1.Model = store;
		
		window.ShowAll();
		Application.Run();			
	}
	

	private void OnAddClicked(object sender, EventArgs args) {	
		store.AppendValues(entry1.Text);
	}
	
	
	void Window_Delete (object obj, DeleteEventArgs args)
	{
		Application.Quit ();
		args.RetVal = true;
	}
	
}

