// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Mono Runtime Version: 2.0.50727.42
// 
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------

namespace saftschubser {
    
    
    public partial class MainWindow {
        
        private Gtk.VBox vbox1;
        
        private Gtk.HButtonBox hbuttonbox1;
        
        private Gtk.HBox hbox1;
        
        private Gtk.Button button1;
        
        private Gtk.Button button2;
        
        private Gtk.Button button3;
        
        private Gtk.Button button4;
        
        private Gtk.Button button5;
        
        private Gtk.Label headingLabel;
        
        protected virtual void Build() {
            Stetic.Gui.Initialize(this);
            // Widget saftschubser.MainWindow
            this.Name = "saftschubser.MainWindow";
            this.Title = Mono.Unix.Catalog.GetString("MainWindow");
            this.WindowPosition = ((Gtk.WindowPosition)(4));
            // Container child saftschubser.MainWindow.Gtk.Container+ContainerChild
            this.vbox1 = new Gtk.VBox();
            this.vbox1.Name = "vbox1";
            this.vbox1.Spacing = 6;
            // Container child vbox1.Gtk.Box+BoxChild
            this.hbuttonbox1 = new Gtk.HButtonBox();
            this.hbuttonbox1.Name = "hbuttonbox1";
            this.vbox1.Add(this.hbuttonbox1);
            Gtk.Box.BoxChild w1 = ((Gtk.Box.BoxChild)(this.vbox1[this.hbuttonbox1]));
            w1.Position = 0;
            w1.Expand = false;
            w1.Fill = false;
            // Container child vbox1.Gtk.Box+BoxChild
            this.hbox1 = new Gtk.HBox();
            this.hbox1.Name = "hbox1";
            this.hbox1.Homogeneous = true;
            this.hbox1.Spacing = 6;
            this.hbox1.BorderWidth = ((uint)(9));
            // Container child hbox1.Gtk.Box+BoxChild
            this.button1 = new Gtk.Button();
            this.button1.CanFocus = true;
            this.button1.UseUnderline = true;
            // Container child button1.Gtk.Container+ContainerChild
            Gtk.Alignment w2 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w3 = new Gtk.HBox();
            w3.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w4 = new Gtk.Image();
            w4.Pixbuf = Stetic.IconLoader.LoadIcon(this, "stock_home", Gtk.IconSize.Menu, 16);
            w3.Add(w4);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w6 = new Gtk.Label();
            w6.LabelProp = Mono.Unix.Catalog.GetString("Kaufen");
            w6.UseUnderline = true;
            w3.Add(w6);
            w2.Add(w3);
            this.button1.Add(w2);
            this.hbox1.Add(this.button1);
            Gtk.Box.BoxChild w10 = ((Gtk.Box.BoxChild)(this.hbox1[this.button1]));
            w10.Position = 0;
            w10.Expand = false;
            // Container child hbox1.Gtk.Box+BoxChild
            this.button2 = new Gtk.Button();
            this.button2.CanFocus = true;
            this.button2.Name = "button2";
            this.button2.UseUnderline = true;
            // Container child button2.Gtk.Container+ContainerChild
            Gtk.Alignment w11 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w12 = new Gtk.HBox();
            w12.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w13 = new Gtk.Image();
            w13.Pixbuf = Stetic.IconLoader.LoadIcon(this, "stock_book_green", Gtk.IconSize.Menu, 16);
            w12.Add(w13);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w15 = new Gtk.Label();
            w15.LabelProp = Mono.Unix.Catalog.GetString("Kasse");
            w15.UseUnderline = true;
            w12.Add(w15);
            w11.Add(w12);
            this.button2.Add(w11);
            this.hbox1.Add(this.button2);
            Gtk.Box.BoxChild w19 = ((Gtk.Box.BoxChild)(this.hbox1[this.button2]));
            w19.Position = 1;
            w19.Expand = false;
            // Container child hbox1.Gtk.Box+BoxChild
            this.button3 = new Gtk.Button();
            this.button3.CanFocus = true;
            this.button3.Name = "button3";
            this.button3.UseUnderline = true;
            // Container child button3.Gtk.Container+ContainerChild
            Gtk.Alignment w20 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w21 = new Gtk.HBox();
            w21.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w22 = new Gtk.Image();
            w22.Pixbuf = Stetic.IconLoader.LoadIcon(this, "stock_data-sources-new", Gtk.IconSize.Menu, 16);
            w21.Add(w22);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w24 = new Gtk.Label();
            w24.LabelProp = Mono.Unix.Catalog.GetString("Bestand");
            w24.UseUnderline = true;
            w21.Add(w24);
            w20.Add(w21);
            this.button3.Add(w20);
            this.hbox1.Add(this.button3);
            Gtk.Box.BoxChild w28 = ((Gtk.Box.BoxChild)(this.hbox1[this.button3]));
            w28.Position = 2;
            w28.Expand = false;
            // Container child hbox1.Gtk.Box+BoxChild
            this.button4 = new Gtk.Button();
            this.button4.CanFocus = true;
            this.button4.Name = "button4";
            this.button4.UseUnderline = true;
            // Container child button4.Gtk.Container+ContainerChild
            Gtk.Alignment w29 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w30 = new Gtk.HBox();
            w30.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w31 = new Gtk.Image();
            w31.Pixbuf = Stetic.IconLoader.LoadIcon(this, "stock_contrast", Gtk.IconSize.Menu, 16);
            w30.Add(w31);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w33 = new Gtk.Label();
            w33.LabelProp = Mono.Unix.Catalog.GetString("LabControl");
            w33.UseUnderline = true;
            w30.Add(w33);
            w29.Add(w30);
            this.button4.Add(w29);
            this.hbox1.Add(this.button4);
            Gtk.Box.BoxChild w37 = ((Gtk.Box.BoxChild)(this.hbox1[this.button4]));
            w37.Position = 3;
            w37.Expand = false;
            // Container child hbox1.Gtk.Box+BoxChild
            this.button5 = new Gtk.Button();
            this.button5.CanFocus = true;
            this.button5.Name = "button5";
            this.button5.UseUnderline = true;
            // Container child button5.Gtk.Container+ContainerChild
            Gtk.Alignment w38 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w39 = new Gtk.HBox();
            w39.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w40 = new Gtk.Image();
            w40.Pixbuf = Stetic.IconLoader.LoadIcon(this, "gtk-dialog-authentication", Gtk.IconSize.Menu, 16);
            w39.Add(w40);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w42 = new Gtk.Label();
            w42.LabelProp = Mono.Unix.Catalog.GetString("Admin");
            w42.UseUnderline = true;
            w39.Add(w42);
            w38.Add(w39);
            this.button5.Add(w38);
            this.hbox1.Add(this.button5);
            Gtk.Box.BoxChild w46 = ((Gtk.Box.BoxChild)(this.hbox1[this.button5]));
            w46.Position = 4;
            w46.Expand = false;
            this.vbox1.Add(this.hbox1);
            Gtk.Box.BoxChild w47 = ((Gtk.Box.BoxChild)(this.vbox1[this.hbox1]));
            w47.Position = 1;
            w47.Expand = false;
            w47.Fill = false;
            // Container child vbox1.Gtk.Box+BoxChild
            this.headingLabel = new Gtk.Label();
            this.headingLabel.Name = "headingLabel";
            this.headingLabel.LabelProp = Mono.Unix.Catalog.GetString("label1");
            this.vbox1.Add(this.headingLabel);
            Gtk.Box.BoxChild w48 = ((Gtk.Box.BoxChild)(this.vbox1[this.headingLabel]));
            w48.Position = 2;
            w48.Expand = false;
            w48.Fill = false;
            this.Add(this.vbox1);
            if ((this.Child != null)) {
                this.Child.ShowAll();
            }
            this.DefaultWidth = 715;
            this.DefaultHeight = 542;
            this.Show();
            this.DeleteEvent += new Gtk.DeleteEventHandler(this.OnDeleteEvent);
            this.button1.Clicked += new System.EventHandler(this.kaufenClicked);
        }
    }
}
