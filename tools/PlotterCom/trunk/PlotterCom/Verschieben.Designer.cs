namespace PlotterCom {
    partial class Verschieben {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent() {
            this.label_VerschX = new System.Windows.Forms.Label();
            this.label_VerschY = new System.Windows.Forms.Label();
            this.textBox_VerschX = new System.Windows.Forms.TextBox();
            this.textBox_VerschY = new System.Windows.Forms.TextBox();
            this.checkBox_Ursprung = new System.Windows.Forms.CheckBox();
            this.button_OK = new System.Windows.Forms.Button();
            this.button_Abbruch = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label_VerschX
            // 
            this.label_VerschX.AutoSize = true;
            this.label_VerschX.Location = new System.Drawing.Point(13, 13);
            this.label_VerschX.Name = "label_VerschX";
            this.label_VerschX.Size = new System.Drawing.Size(85, 13);
            this.label_VerschX.TabIndex = 0;
            this.label_VerschX.Text = "Verschiebung X:";
            // 
            // label_VerschY
            // 
            this.label_VerschY.AutoSize = true;
            this.label_VerschY.Location = new System.Drawing.Point(13, 40);
            this.label_VerschY.Name = "label_VerschY";
            this.label_VerschY.Size = new System.Drawing.Size(85, 13);
            this.label_VerschY.TabIndex = 1;
            this.label_VerschY.Text = "Verschiebung Y:";
            // 
            // textBox_VerschX
            // 
            this.textBox_VerschX.Location = new System.Drawing.Point(104, 10);
            this.textBox_VerschX.Name = "textBox_VerschX";
            this.textBox_VerschX.Size = new System.Drawing.Size(100, 20);
            this.textBox_VerschX.TabIndex = 2;
            this.textBox_VerschX.Text = "0,000mm";
            // 
            // textBox_VerschY
            // 
            this.textBox_VerschY.Location = new System.Drawing.Point(104, 37);
            this.textBox_VerschY.Name = "textBox_VerschY";
            this.textBox_VerschY.Size = new System.Drawing.Size(100, 20);
            this.textBox_VerschY.TabIndex = 3;
            this.textBox_VerschY.Text = "0,000mm";
            // 
            // checkBox_Ursprung
            // 
            this.checkBox_Ursprung.AutoSize = true;
            this.checkBox_Ursprung.Location = new System.Drawing.Point(16, 72);
            this.checkBox_Ursprung.Name = "checkBox_Ursprung";
            this.checkBox_Ursprung.Size = new System.Drawing.Size(148, 17);
            this.checkBox_Ursprung.TabIndex = 4;
            this.checkBox_Ursprung.Text = "In den Ursprung schieben";
            this.checkBox_Ursprung.UseVisualStyleBackColor = true;
            this.checkBox_Ursprung.CheckedChanged += new System.EventHandler(this.checkBox_Ursprung_CheckedChanged);
            // 
            // button_OK
            // 
            this.button_OK.Location = new System.Drawing.Point(16, 96);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(75, 23);
            this.button_OK.TabIndex = 5;
            this.button_OK.Text = "OK";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // button_Abbruch
            // 
            this.button_Abbruch.Location = new System.Drawing.Point(104, 96);
            this.button_Abbruch.Name = "button_Abbruch";
            this.button_Abbruch.Size = new System.Drawing.Size(75, 23);
            this.button_Abbruch.TabIndex = 6;
            this.button_Abbruch.Text = "Abbruch";
            this.button_Abbruch.UseVisualStyleBackColor = true;
            this.button_Abbruch.Click += new System.EventHandler(this.button_Abbruch_Click);
            // 
            // Verschieben
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(214, 128);
            this.Controls.Add(this.button_Abbruch);
            this.Controls.Add(this.button_OK);
            this.Controls.Add(this.checkBox_Ursprung);
            this.Controls.Add(this.textBox_VerschY);
            this.Controls.Add(this.textBox_VerschX);
            this.Controls.Add(this.label_VerschY);
            this.Controls.Add(this.label_VerschX);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(230, 164);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(230, 164);
            this.Name = "Verschieben";
            this.Text = "Verschieben";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_VerschX;
        private System.Windows.Forms.Label label_VerschY;
        private System.Windows.Forms.TextBox textBox_VerschX;
        private System.Windows.Forms.TextBox textBox_VerschY;
        private System.Windows.Forms.CheckBox checkBox_Ursprung;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.Button button_Abbruch;
    }
}