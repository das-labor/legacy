using Gtk;
using Gdk;
using System;
using System.Text;
/** 
 * Stores a Point
 */
struct Point {
	public float x;
	public float y;
	
	public Point(float x, float y)  {
		this.x = x;
		this.y = y;
	}
}

class Display : DrawingArea {
	private float zoom;
	private int chainPosX;
	private int chainPosY;
	private int drawLevel;
	private int chainLevel;
	
	private ListStore store;
	private Pixmap pixmap;
	private Point CurrentPoint;
	private StringBuilder chain;
	
	public Display(float zoom, int height, int width, ListStore liststore) {
		this.zoom = zoom;
		this.store = liststore;
		this.SetSizeRequest((int)(zoom*(float)height), (int) (zoom*(float)width));
		CurrentPoint = new Point(0.0f, 0.0f);
		chainLevel = 10;
		drawLevel = 6;
		chain = new StringBuilder();
	}
	
	public float Zoom {
		get {
			return zoom;
		}
		set {
			if (value > 0.01 && value < 10)
				zoom = value;
		}
	}


	protected override bool OnExposeEvent (Gdk.EventExpose args)
	{
		using (Graphics g = Gtk.DotNet.Graphics.FromDrawable (args.Window)){
			Pen p = new Pen (Color.Black, 1.0f);
			
			foreach (object[] row in store) {
				string a = row[0]+"" ;
				string[] ps = a.Split(new Char [] {' '});
				switch (ps[0].ToLower().ToCharArray()[0]){
					case 's': {
						CurrentPoint.x = Single.Parse(ps[1]);
						CurrentPoint.y = Single.Parse(ps[2]);
						break;
					} 
					case 'l': {
						drawLineTo(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])), g);
						break;
					}
					case 'c': {
						drawBezier(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])),
							       new Point(Single.Parse(ps[3]), Single.Parse(ps[4])),
							       new Point(Single.Parse(ps[5]), Single.Parse(ps[6])),
							       g);
						break;		                                  
					}
				}
			}
		}
		return true;
	}
	
	
	protected override void OnConfigure(object o, ConfigureEventArgs args) {
		EventConfigure eventConfigure = args.Event;
		Gdk.Window window = eventConfigure.Window;
		pixmap = new Pixmap (window,
							Allocation.Width,
							Allocation.Height,
							-1);  	
		// Initialize the pixmap to white
		pixmap.DrawRectangle (Style.WhiteGC, true, 0, 0,
					Allocation.Width, Allocation.Height);
		// We've handled the configure event, no need for further processing.
		args.RetVal = true;
	}
	
	protected override void OnExpose(object o, ExposeEventArgs args)
	{
		Gdk.GC gc1;
		Gdk.Color color = new Gdk.Color();
		
		EventExpose eventExpose = args.Event;
		Gdk.Window window = eventExpose.Window;
		gc1 = new Gdk.GC(window);

		color.Red   = 0;
		color.Green = 0;
		color.Blue  = 0;
		gc1.RgbFgColor = color;
		
		color.Red = 65535;
		color.Green = 65535;
		color.Blue = 65535;
		gc1.RgbBgColor = color;
		
		window.DrawLine(gc1, 20, 20, 350, 399);
		
		
		using (Graphics g = args.Window){
			Pen p = new Pen (Color.Black, 1.0f);
			
			foreach (object[] row in store) {
				string a = row[0]+"" ;
				string[] ps = a.Split(new Char [] {' '});
				switch (ps[0].ToLower().ToCharArray()[0]){
					case 's': {
						CurrentPoint.x = Single.Parse(ps[1]);
						CurrentPoint.y = Single.Parse(ps[2]);
						break;
					} 
					case 'l': {
						drawLineTo(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])), g);
						break;
					}
					case 'c': {
						drawBezier(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])),
							       new Point(Single.Parse(ps[3]), Single.Parse(ps[4])),
							       new Point(Single.Parse(ps[5]), Single.Parse(ps[6])),
							       g);
						break;		                                  
					}
				}
			}
		}
	}
	
	private void drawLineTo(Point p, Graphics g) {
		Pen pen = new Pen(Color.Black, 1.0f);
		g.DrawLine(pen, (int)(zoom*CurrentPoint.x+0.5), 
		                (int)(zoom*CurrentPoint.y+0.5),
		                (int)(zoom*p.x+0.5), 
		                (int)(zoom*p.y+0.5));
		CurrentPoint = p;
	}
	
	private void drawBezier(Point p2, Point p3, Point p4, Graphics g) {
		drawBezierRec(CurrentPoint, p2, p3, p4, drawLevel, g);
		CurrentPoint = p4;
	}
	
	private void drawBezierRec(Point p1, Point p2, Point p3, Point p4, int level, Graphics g) {
	    if (level == 0) {
	        drawLineTo(p4, g);
	    } else {
	        Point l1 = p1;
	        Point l2 = midpoint(p1, p2);
	        Point h  = midpoint(p2, p3);
	        Point r3 = midpoint(p3, p4);
	        Point r4 = p4;
	        Point l3 = midpoint(l2, h);
	        Point r2 = midpoint(r3, h);
	        Point l4 = midpoint(l3, r2);
	        Point r1 = l4;
	        drawBezierRec(l1, l2, l3, l4, level-1, g);
	        drawBezierRec(r1, r2, r3, r4, level-1, g);
	    }
	}
	
	
	private Point midpoint(Point p1, Point p2) {
		return new Point((p1.x+p2.x)/2, (p1.y+p2.y)/2);
	} 	
	
	public string getChainCode() {
		foreach (object[] row in store) {
			string a = row[0] + "";
			string[] ps = a.Split(new Char [] {' '});
			switch (ps[0].ToLower().ToCharArray()[0]) {
				case 's': {
					CurrentPoint.x = Single.Parse(ps[1]);
					CurrentPoint.y = Single.Parse(ps[2]);
					startChain((int)CurrentPoint.x, (int)CurrentPoint.y);
					break;
				} 
				case 'l': {
					chainLineTo(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])));
					break;
				}
				case 'c': {
					chainBezier(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])),
						        new Point(Single.Parse(ps[3]), Single.Parse(ps[4])),
						        new Point(Single.Parse(ps[5]), Single.Parse(ps[6])));
					break;		                                  
				}
			}
		}
		return this.chain.ToString();
	}

	private void chainLineTo(Point p) {
	    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	    dx = (int)(p.x+0.5) - (int)(CurrentPoint.x+0.5);
	    dy = (int)(p.y+0.5) - (int)(CurrentPoint.y+0.5);
	    dxabs = dx >= 0 ? dx: -dx; //abs
	    dyabs = dy >= 0 ? dy: -dy; //abs
	    sdx = dx >= 0 ? 1: -1;     //sign
	    sdy = dy >= 0 ? 1: -1;     //sign
	    x = dyabs >> 1;
	    y = dxabs >> 1;
	    px = (int)(CurrentPoint.x+0.5);
	    py = (int)(CurrentPoint.y+0.5);
		addToChain(px, py);
	    if (dxabs >= dyabs) { // the line is more horizontal than vertical
	        for (i = 0; i < dxabs; i++) {
	            y += dyabs;
	            if (y >= dxabs) {
	                y -= dxabs;
	                py += sdy;
	            }
	            px += sdx;
	            addToChain(px, py);
	        }
	    } else { // the line is more vertical than horizontal
	        for (i = 0; i < dyabs; i++) {
	            x += dxabs;
	            if (x >= dyabs) {
	                x -= dyabs;
	                px += sdx;
	            }
	            py += sdy;
	            addToChain(px, py);
	        }
	    }
	    CurrentPoint = p;
	}
	
	private void chainBezier(Point p2, Point p3, Point p4) {
		chainBezierRec(CurrentPoint, p2, p3, p4, chainLevel);
		CurrentPoint = p4;
	}
 	
	private void chainBezierRec(Point p1, Point p2, Point p3, Point p4, int level) {
	    if (level == 0) {
	        chainLineTo(p4);
	    } else {
	        Point l1 = p1;
	        Point l2 = midpoint(p1, p2);
	        Point h  = midpoint(p2, p3);
	        Point r3 = midpoint(p3, p4);
	        Point r4 = p4;
	        Point l3 = midpoint(l2, h);
	        Point r2 = midpoint(r3, h);
	        Point l4 = midpoint(l3, r2);
	        Point r1 = l4;
	        chainBezierRec(l1, l2, l3, l4, level-1);
	        chainBezierRec(r1, r2, r3, r4, level-1);
	    }
	}
	
 	private void startChain(int px, int py) {
		chain = new StringBuilder();
		chain.Append(px);
		chain.Append(';');
		chain.Append(py);
		
		chainPosX = px;
	    chainPosY = py;
	}
 
 	private void addToChain(int px, int py) {
	    int dx = px - chainPosX;
	    int dy = py - chainPosY;
	    char addChain = ' ';
	    bool skip = false;	
	    if (dy == -1) {
	        switch (dx) {
	            case -1: addChain = 'H'; break;
	            case  0: addChain = 'A'; break;
	            case  1: addChain = 'B'; break;
	            default: addChain = 'X'; break;
	        }
	    } else if (dy == 0) {
	        switch (dx) {
	            case -1: addChain = 'G'; break;
	            case  0: skip     = true; break;
	            case  1: addChain = 'C'; break;
	            default: addChain = 'X'; break;
	        }
	    } else if (dy == 1) {
	        switch (dx) {
	            case -1: addChain = 'F'; break;
	            case  0: addChain = 'E'; break;
	            case  1: addChain = 'D'; break;
	            default: addChain = 'X'; break;
	        }
	    } else {
	        addChain = 'X';
	    }

	    if (!skip) {
	    	chain.Append(addChain);
	    }
	    chainPosX = px;
	    chainPosY = py;
	}
}


/*

// Connect the Signals defined in Glade
private void OnWindowDeleteEvent (object sender, DeleteEventArgs a) 
{
	Application.Quit ();
	a.RetVal = true;
}

Point CurrentPoint;
	
void drawChain(HDC hdc) {
	char *ch = chain;
	int px = atoi(ch), py;
	while (*++ch != ',');
	py = atoi(++ch);
	while (*ch) {
		SetPixel(hdc, px, py, 0);
		switch (*ch) {
			case 'A':       py--; break;
			case 'B': px++; py--; break;
			case 'C': px++;       break;
			case 'D': px++; py++; break;
			case 'E':       py++; break;
			case 'F': px--; py++; break;
			case 'G': px--;       break;
			case 'H': px--; py--; break;
		}
		ch++;
	}
}
*/
