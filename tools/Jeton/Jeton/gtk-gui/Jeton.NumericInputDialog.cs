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
    
    
    public partial class NumericInputDialog {
        
        private Gtk.Button button354;
        
        private Gtk.Button button353;
        
        protected virtual void Build() {
            Stetic.Gui.Initialize(this);
            // Widget Jeton.NumericInputDialog
            this.Name = "Jeton.NumericInputDialog";
            this.WindowPosition = ((Gtk.WindowPosition)(4));
            this.Modal = true;
            this.BorderWidth = ((uint)(8));
            this.HasSeparator = false;
            // Internal child Jeton.NumericInputDialog.VBox
            Gtk.VBox w1 = this.VBox;
            w1.Name = "dialog1_VBox";
            w1.BorderWidth = ((uint)(2));
            // Internal child Jeton.NumericInputDialog.ActionArea
            Gtk.HButtonBox w2 = this.ActionArea;
            w2.Name = "dialog1_ActionArea";
            w2.Spacing = 6;
            w2.BorderWidth = ((uint)(5));
            w2.LayoutStyle = ((Gtk.ButtonBoxStyle)(4));
            // Container child dialog1_ActionArea.Gtk.ButtonBox+ButtonBoxChild
            this.button354 = new Gtk.Button();
            this.button354.CanFocus = true;
            this.button354.Name = "button354";
            this.button354.UseStock = true;
            this.button354.UseUnderline = true;
            this.button354.Label = "gtk-cancel";
            this.AddActionWidget(this.button354, -6);
            Gtk.ButtonBox.ButtonBoxChild w3 = ((Gtk.ButtonBox.ButtonBoxChild)(w2[this.button354]));
            w3.Expand = false;
            w3.Fill = false;
            // Container child dialog1_ActionArea.Gtk.ButtonBox+ButtonBoxChild
            this.button353 = new Gtk.Button();
            this.button353.WidthRequest = 100;
            this.button353.HeightRequest = 60;
            this.button353.CanFocus = true;
            this.button353.Name = "button353";
            this.button353.UseStock = true;
            this.button353.UseUnderline = true;
            this.button353.Label = "gtk-ok";
            this.AddActionWidget(this.button353, -5);
            Gtk.ButtonBox.ButtonBoxChild w4 = ((Gtk.ButtonBox.ButtonBoxChild)(w2[this.button353]));
            w4.Position = 1;
            w4.Fill = false;
            if ((this.Child != null)) {
                this.Child.ShowAll();
            }
            this.DefaultWidth = 424;
            this.DefaultHeight = 300;
            this.Show();
        }
    }
}
