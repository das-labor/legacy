// created on 11/16/2005 at 11:51 PM

[DllImport("libgdk-x11-2.0.so")]
internal static extern IntPtr gdk_cairo_create (IntPtr raw);

class Draw
public static Cairo.Graphics CreateDrawable (Gdk.Drawable drawable)
{
    Cairo.Graphics g = new Cairo.Graphics (gdk_cairo_create (drawable.Handle));
    if (g == null) 
        throw new Exception ("Couldn't create Cairo Graphics!");
 
    return g;
}

void OnDrawingAreaExposed (object o, ExposeEventArgs args)
{
    DrawingArea area = (DrawingArea) o;
    Cairo.Graphics g = Graphics.CreateDrawable (area.GdkWindow);
 
    // Perform some drawing
 
    ((IDisposable) gr.Target).Dispose ();                               
    ((IDisposable) g).Dispose ();
}