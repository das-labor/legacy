// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Mono Runtime Version: 2.0.50727.42
// 
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------

namespace Jeton {
    
    
    public partial class KasseScreen {
        
        private Gtk.VBox vbox1;
        
        private Gtk.Frame frame1;
        
        private Gtk.Alignment GtkAlignment;
        
        private Gtk.EventBox eventbox1;
        
        private Gtk.VBox vbox2;
        
        private Gtk.HBox hbox1;
        
        private Gtk.Label label1;
        
        private Gtk.Label standLabel;
        
        private Gtk.HBox hbox2;
        
        private Gtk.Button errorBtn;
        
        private Gtk.Label GtkLabel1;
        
        protected virtual void Build() {
            Stetic.Gui.Initialize(this);
            // Widget Jeton.KasseScreen
            Stetic.BinContainer.Attach(this);
            this.Name = "Jeton.KasseScreen";
            // Container child Jeton.KasseScreen.Gtk.Container+ContainerChild
            this.vbox1 = new Gtk.VBox();
            this.vbox1.Name = "vbox1";
            this.vbox1.Spacing = 6;
            // Container child vbox1.Gtk.Box+BoxChild
            this.frame1 = new Gtk.Frame();
            this.frame1.Name = "frame1";
            this.frame1.ShadowType = ((Gtk.ShadowType)(1));
            this.frame1.BorderWidth = ((uint)(63));
            // Container child frame1.Gtk.Container+ContainerChild
            this.GtkAlignment = new Gtk.Alignment(0F, 0F, 1F, 1F);
            this.GtkAlignment.Name = "GtkAlignment";
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            this.eventbox1 = new Gtk.EventBox();
            this.eventbox1.Name = "eventbox1";
            // Container child eventbox1.Gtk.Container+ContainerChild
            this.vbox2 = new Gtk.VBox();
            this.vbox2.Name = "vbox2";
            this.vbox2.Spacing = 6;
            this.vbox2.BorderWidth = ((uint)(9));
            // Container child vbox2.Gtk.Box+BoxChild
            this.hbox1 = new Gtk.HBox();
            this.hbox1.Name = "hbox1";
            this.hbox1.Spacing = 6;
            // Container child hbox1.Gtk.Box+BoxChild
            this.label1 = new Gtk.Label();
            this.label1.Name = "label1";
            this.label1.Xalign = 1F;
            this.label1.LabelProp = Mono.Unix.Catalog.GetString("<span size=\"xx-large\"><b>Kassenstand:</b></span>");
            this.label1.UseMarkup = true;
            this.hbox1.Add(this.label1);
            Gtk.Box.BoxChild w1 = ((Gtk.Box.BoxChild)(this.hbox1[this.label1]));
            w1.Position = 0;
            // Container child hbox1.Gtk.Box+BoxChild
            this.standLabel = new Gtk.Label();
            this.standLabel.Name = "standLabel";
            this.standLabel.LabelProp = Mono.Unix.Catalog.GetString("<span size=\"xx-large\"><b>0,00 €</b></span>");
            this.standLabel.UseMarkup = true;
            this.hbox1.Add(this.standLabel);
            Gtk.Box.BoxChild w2 = ((Gtk.Box.BoxChild)(this.hbox1[this.standLabel]));
            w2.Position = 1;
            this.vbox2.Add(this.hbox1);
            Gtk.Box.BoxChild w3 = ((Gtk.Box.BoxChild)(this.vbox2[this.hbox1]));
            w3.Position = 0;
            w3.Expand = false;
            w3.Fill = false;
            // Container child vbox2.Gtk.Box+BoxChild
            this.hbox2 = new Gtk.HBox();
            this.hbox2.Name = "hbox2";
            this.hbox2.Spacing = 6;
            // Container child hbox2.Gtk.Box+BoxChild
            this.errorBtn = new Gtk.Button();
            this.errorBtn.WidthRequest = 300;
            this.errorBtn.HeightRequest = 80;
            this.errorBtn.CanFocus = true;
            this.errorBtn.Name = "errorBtn";
            this.errorBtn.UseUnderline = true;
            this.errorBtn.BorderWidth = ((uint)(10));
            // Container child errorBtn.Gtk.Container+ContainerChild
            Gtk.Alignment w4 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w5 = new Gtk.HBox();
            w5.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w6 = new Gtk.Image();
            w6.Pixbuf = Stetic.IconLoader.LoadIcon(this, "gtk-no", Gtk.IconSize.Button, 20);
            w5.Add(w6);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w8 = new Gtk.Label();
            w8.LabelProp = Mono.Unix.Catalog.GetString("Fehlbetrag Melden");
            w8.UseUnderline = true;
            w5.Add(w8);
            w4.Add(w5);
            this.errorBtn.Add(w4);
            this.hbox2.Add(this.errorBtn);
            Gtk.Box.BoxChild w12 = ((Gtk.Box.BoxChild)(this.hbox2[this.errorBtn]));
            w12.PackType = ((Gtk.PackType)(1));
            w12.Position = 0;
            w12.Expand = false;
            w12.Fill = false;
            w12.Padding = ((uint)(1));
            this.vbox2.Add(this.hbox2);
            Gtk.Box.BoxChild w13 = ((Gtk.Box.BoxChild)(this.vbox2[this.hbox2]));
            w13.Position = 1;
            w13.Expand = false;
            w13.Fill = false;
            this.eventbox1.Add(this.vbox2);
            this.GtkAlignment.Add(this.eventbox1);
            this.frame1.Add(this.GtkAlignment);
            this.GtkLabel1 = new Gtk.Label();
            this.GtkLabel1.Name = "GtkLabel1";
            this.GtkLabel1.LabelProp = Mono.Unix.Catalog.GetString("\n");
            this.GtkLabel1.UseMarkup = true;
            this.frame1.LabelWidget = this.GtkLabel1;
            this.vbox1.Add(this.frame1);
            Gtk.Box.BoxChild w17 = ((Gtk.Box.BoxChild)(this.vbox1[this.frame1]));
            w17.Position = 0;
            w17.Expand = false;
            w17.Fill = false;
            this.Add(this.vbox1);
            if ((this.Child != null)) {
                this.Child.ShowAll();
            }
            this.Show();
        }
    }
}
