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
    
    
    public partial class BestellWidget {
        
        private Gtk.Frame frame1;
        
        private Gtk.EventBox eventBox;
        
        private Gtk.Alignment GtkAlignment2;
        
        private Gtk.VBox vbox3;
        
        private Gtk.Table table;
        
        private Gtk.HButtonBox hbuttonbox2;
        
        private Gtk.Button cancelBtn;
        
        private Gtk.Button applyBtn;
        
        private Gtk.Label GtkLabel2;
        
        protected virtual void Build() {
            Stetic.Gui.Initialize(this);
            // Widget Jeton.BestellWidget
            Stetic.BinContainer.Attach(this);
            this.Name = "Jeton.BestellWidget";
            // Container child Jeton.BestellWidget.Gtk.Container+ContainerChild
            this.frame1 = new Gtk.Frame();
            this.frame1.Name = "frame1";
            this.frame1.ShadowType = ((Gtk.ShadowType)(1));
            this.frame1.BorderWidth = ((uint)(35));
            // Container child frame1.Gtk.Container+ContainerChild
            this.eventBox = new Gtk.EventBox();
            this.eventBox.Name = "eventBox";
            // Container child eventBox.Gtk.Container+ContainerChild
            this.GtkAlignment2 = new Gtk.Alignment(0F, 0F, 1F, 1F);
            this.GtkAlignment2.Name = "GtkAlignment2";
            this.GtkAlignment2.LeftPadding = ((uint)(12));
            this.GtkAlignment2.RightPadding = ((uint)(12));
            this.GtkAlignment2.BottomPadding = ((uint)(12));
            // Container child GtkAlignment2.Gtk.Container+ContainerChild
            this.vbox3 = new Gtk.VBox();
            this.vbox3.Name = "vbox3";
            this.vbox3.Spacing = 6;
            // Container child vbox3.Gtk.Box+BoxChild
            this.table = new Gtk.Table(((uint)(3)), ((uint)(3)), false);
            this.table.Name = "table";
            this.table.RowSpacing = ((uint)(6));
            this.table.ColumnSpacing = ((uint)(6));
            this.table.BorderWidth = ((uint)(5));
            this.vbox3.Add(this.table);
            Gtk.Box.BoxChild w1 = ((Gtk.Box.BoxChild)(this.vbox3[this.table]));
            w1.Position = 0;
            // Container child vbox3.Gtk.Box+BoxChild
            this.hbuttonbox2 = new Gtk.HButtonBox();
            this.hbuttonbox2.Name = "hbuttonbox2";
            // Container child hbuttonbox2.Gtk.ButtonBox+ButtonBoxChild
            this.cancelBtn = new Gtk.Button();
            this.cancelBtn.WidthRequest = 150;
            this.cancelBtn.HeightRequest = 50;
            this.cancelBtn.CanFocus = true;
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.UseUnderline = true;
            // Container child cancelBtn.Gtk.Container+ContainerChild
            Gtk.Alignment w2 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w3 = new Gtk.HBox();
            w3.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w4 = new Gtk.Image();
            w4.Pixbuf = Stetic.IconLoader.LoadIcon(this, "gtk-delete", Gtk.IconSize.Button, 20);
            w3.Add(w4);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w6 = new Gtk.Label();
            w6.LabelProp = Mono.Unix.Catalog.GetString("Alle _entfernen");
            w6.UseUnderline = true;
            w3.Add(w6);
            w2.Add(w3);
            this.cancelBtn.Add(w2);
            this.hbuttonbox2.Add(this.cancelBtn);
            Gtk.ButtonBox.ButtonBoxChild w10 = ((Gtk.ButtonBox.ButtonBoxChild)(this.hbuttonbox2[this.cancelBtn]));
            w10.Expand = false;
            w10.Fill = false;
            // Container child hbuttonbox2.Gtk.ButtonBox+ButtonBoxChild
            this.applyBtn = new Gtk.Button();
            this.applyBtn.WidthRequest = 150;
            this.applyBtn.CanFocus = true;
            this.applyBtn.Name = "applyBtn";
            this.applyBtn.UseUnderline = true;
            // Container child applyBtn.Gtk.Container+ContainerChild
            Gtk.Alignment w11 = new Gtk.Alignment(0.5F, 0.5F, 0F, 0F);
            // Container child GtkAlignment.Gtk.Container+ContainerChild
            Gtk.HBox w12 = new Gtk.HBox();
            w12.Spacing = 2;
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Image w13 = new Gtk.Image();
            w13.Pixbuf = Stetic.IconLoader.LoadIcon(this, "gtk-apply", Gtk.IconSize.Button, 20);
            w12.Add(w13);
            // Container child GtkHBox.Gtk.Container+ContainerChild
            Gtk.Label w15 = new Gtk.Label();
            w15.LabelProp = Mono.Unix.Catalog.GetString("_Kaufen");
            w15.UseUnderline = true;
            w12.Add(w15);
            w11.Add(w12);
            this.applyBtn.Add(w11);
            this.hbuttonbox2.Add(this.applyBtn);
            Gtk.ButtonBox.ButtonBoxChild w19 = ((Gtk.ButtonBox.ButtonBoxChild)(this.hbuttonbox2[this.applyBtn]));
            w19.Position = 1;
            w19.Expand = false;
            w19.Fill = false;
            this.vbox3.Add(this.hbuttonbox2);
            Gtk.Box.BoxChild w20 = ((Gtk.Box.BoxChild)(this.vbox3[this.hbuttonbox2]));
            w20.Position = 1;
            w20.Expand = false;
            w20.Fill = false;
            this.GtkAlignment2.Add(this.vbox3);
            this.eventBox.Add(this.GtkAlignment2);
            this.frame1.Add(this.eventBox);
            this.GtkLabel2 = new Gtk.Label();
            this.GtkLabel2.Name = "GtkLabel2";
            this.GtkLabel2.LabelProp = "";
            this.GtkLabel2.UseMarkup = true;
            this.frame1.LabelWidget = this.GtkLabel2;
            this.Add(this.frame1);
            if ((this.Child != null)) {
                this.Child.ShowAll();
            }
            this.Show();
            this.cancelBtn.Clicked += new System.EventHandler(this.OnCancelClicked);
            this.applyBtn.Clicked += new System.EventHandler(this.OnApplyClicked);
        }
    }
}
