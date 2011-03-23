namespace PlotterCom
{
    partial class Skalierung
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Skalierung));
            this.trackBar_Skalierung = new System.Windows.Forms.TrackBar();
            this.checkBox_Auto = new System.Windows.Forms.CheckBox();
            this.label_MinSkalierung = new System.Windows.Forms.Label();
            this.label_MaxSkalierung = new System.Windows.Forms.Label();
            this.label_MitteSkalierung = new System.Windows.Forms.Label();
            this.button_OK = new System.Windows.Forms.Button();
            this.button_Abbruch = new System.Windows.Forms.Button();
            this.label_Absolut = new System.Windows.Forms.Label();
            this.label_AktSkalierung = new System.Windows.Forms.Label();
            this.textBox_Faktor = new System.Windows.Forms.TextBox();
            this.textBox_AbsolutX = new System.Windows.Forms.TextBox();
            this.textBox_AbsolutY = new System.Windows.Forms.TextBox();
            this.label_GrößeX = new System.Windows.Forms.Label();
            this.label_Plottergröße = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_PlotterY = new System.Windows.Forms.TextBox();
            this.textBox_PlotterX = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Skalierung)).BeginInit();
            this.SuspendLayout();
            // 
            // trackBar_Skalierung
            // 
            this.trackBar_Skalierung.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBar_Skalierung.Location = new System.Drawing.Point(12, 12);
            this.trackBar_Skalierung.Maximum = 100;
            this.trackBar_Skalierung.Name = "trackBar_Skalierung";
            this.trackBar_Skalierung.Size = new System.Drawing.Size(278, 45);
            this.trackBar_Skalierung.TabIndex = 0;
            this.trackBar_Skalierung.TickFrequency = 10;
            this.trackBar_Skalierung.Value = 50;
            this.trackBar_Skalierung.Scroll += new System.EventHandler(this.trackBarSkalierung_Scroll);
            // 
            // checkBox_Auto
            // 
            this.checkBox_Auto.AutoSize = true;
            this.checkBox_Auto.Location = new System.Drawing.Point(110, 133);
            this.checkBox_Auto.Name = "checkBox_Auto";
            this.checkBox_Auto.Size = new System.Drawing.Size(165, 17);
            this.checkBox_Auto.TabIndex = 1;
            this.checkBox_Auto.Text = "Gesamte Plotterfläche nutzen";
            this.checkBox_Auto.UseVisualStyleBackColor = true;
            this.checkBox_Auto.CheckedChanged += new System.EventHandler(this.checkBox_Auto_CheckedChanged);
            // 
            // label_MinSkalierung
            // 
            this.label_MinSkalierung.AutoSize = true;
            this.label_MinSkalierung.Location = new System.Drawing.Point(12, 41);
            this.label_MinSkalierung.Name = "label_MinSkalierung";
            this.label_MinSkalierung.Size = new System.Drawing.Size(27, 13);
            this.label_MinSkalierung.TabIndex = 2;
            this.label_MinSkalierung.Text = "10%";
            // 
            // label_MaxSkalierung
            // 
            this.label_MaxSkalierung.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label_MaxSkalierung.AutoSize = true;
            this.label_MaxSkalierung.Location = new System.Drawing.Point(254, 41);
            this.label_MaxSkalierung.Name = "label_MaxSkalierung";
            this.label_MaxSkalierung.Size = new System.Drawing.Size(39, 13);
            this.label_MaxSkalierung.TabIndex = 3;
            this.label_MaxSkalierung.Text = "1000%";
            // 
            // label_MitteSkalierung
            // 
            this.label_MitteSkalierung.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label_MitteSkalierung.Location = new System.Drawing.Point(55, 41);
            this.label_MitteSkalierung.Name = "label_MitteSkalierung";
            this.label_MitteSkalierung.Size = new System.Drawing.Size(193, 23);
            this.label_MitteSkalierung.TabIndex = 4;
            this.label_MitteSkalierung.Text = "100%";
            this.label_MitteSkalierung.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // button_OK
            // 
            this.button_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button_OK.Location = new System.Drawing.Point(12, 199);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(75, 23);
            this.button_OK.TabIndex = 5;
            this.button_OK.Text = "OK";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // button_Abbruch
            // 
            this.button_Abbruch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button_Abbruch.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button_Abbruch.Location = new System.Drawing.Point(94, 199);
            this.button_Abbruch.Name = "button_Abbruch";
            this.button_Abbruch.Size = new System.Drawing.Size(75, 23);
            this.button_Abbruch.TabIndex = 6;
            this.button_Abbruch.Text = "Abbruch";
            this.button_Abbruch.UseVisualStyleBackColor = true;
            this.button_Abbruch.Click += new System.EventHandler(this.button_Abbruch_Click);
            // 
            // label_Absolut
            // 
            this.label_Absolut.AutoSize = true;
            this.label_Absolut.Location = new System.Drawing.Point(9, 110);
            this.label_Absolut.Name = "label_Absolut";
            this.label_Absolut.Size = new System.Drawing.Size(93, 13);
            this.label_Absolut.TabIndex = 8;
            this.label_Absolut.Text = "Zeichnungsgröße:";
            // 
            // label_AktSkalierung
            // 
            this.label_AktSkalierung.AutoSize = true;
            this.label_AktSkalierung.Location = new System.Drawing.Point(8, 73);
            this.label_AktSkalierung.Name = "label_AktSkalierung";
            this.label_AktSkalierung.Size = new System.Drawing.Size(92, 13);
            this.label_AktSkalierung.TabIndex = 9;
            this.label_AktSkalierung.Text = "Skalierungsfaktor:";
            // 
            // textBox_Faktor
            // 
            this.textBox_Faktor.Location = new System.Drawing.Point(106, 70);
            this.textBox_Faktor.Name = "textBox_Faktor";
            this.textBox_Faktor.Size = new System.Drawing.Size(180, 20);
            this.textBox_Faktor.TabIndex = 10;
            this.textBox_Faktor.Text = "100.00%";
            this.textBox_Faktor.Leave += new System.EventHandler(this.textBox_Faktor_Leave);
            this.textBox_Faktor.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.textBox_Faktor_PreviewKeyDown);
            // 
            // textBox_AbsolutX
            // 
            this.textBox_AbsolutX.Location = new System.Drawing.Point(106, 107);
            this.textBox_AbsolutX.Name = "textBox_AbsolutX";
            this.textBox_AbsolutX.ReadOnly = true;
            this.textBox_AbsolutX.Size = new System.Drawing.Size(78, 20);
            this.textBox_AbsolutX.TabIndex = 11;
            // 
            // textBox_AbsolutY
            // 
            this.textBox_AbsolutY.Location = new System.Drawing.Point(208, 107);
            this.textBox_AbsolutY.Name = "textBox_AbsolutY";
            this.textBox_AbsolutY.ReadOnly = true;
            this.textBox_AbsolutY.Size = new System.Drawing.Size(78, 20);
            this.textBox_AbsolutY.TabIndex = 12;
            // 
            // label_GrößeX
            // 
            this.label_GrößeX.AutoSize = true;
            this.label_GrößeX.Location = new System.Drawing.Point(190, 110);
            this.label_GrößeX.Name = "label_GrößeX";
            this.label_GrößeX.Size = new System.Drawing.Size(12, 13);
            this.label_GrößeX.TabIndex = 13;
            this.label_GrößeX.Text = "x";
            // 
            // label_Plottergröße
            // 
            this.label_Plottergröße.AutoSize = true;
            this.label_Plottergröße.Location = new System.Drawing.Point(9, 159);
            this.label_Plottergröße.Name = "label_Plottergröße";
            this.label_Plottergröße.Size = new System.Drawing.Size(67, 13);
            this.label_Plottergröße.TabIndex = 14;
            this.label_Plottergröße.Text = "Plottergröße:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(190, 159);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 13);
            this.label1.TabIndex = 17;
            this.label1.Text = "x";
            // 
            // textBox_PlotterY
            // 
            this.textBox_PlotterY.Location = new System.Drawing.Point(208, 156);
            this.textBox_PlotterY.Name = "textBox_PlotterY";
            this.textBox_PlotterY.ReadOnly = true;
            this.textBox_PlotterY.Size = new System.Drawing.Size(78, 20);
            this.textBox_PlotterY.TabIndex = 16;
            // 
            // textBox_PlotterX
            // 
            this.textBox_PlotterX.Location = new System.Drawing.Point(106, 156);
            this.textBox_PlotterX.Name = "textBox_PlotterX";
            this.textBox_PlotterX.ReadOnly = true;
            this.textBox_PlotterX.Size = new System.Drawing.Size(78, 20);
            this.textBox_PlotterX.TabIndex = 15;
            // 
            // Skalierung
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.button_Abbruch;
            this.ClientSize = new System.Drawing.Size(302, 234);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_PlotterY);
            this.Controls.Add(this.textBox_PlotterX);
            this.Controls.Add(this.label_Plottergröße);
            this.Controls.Add(this.label_GrößeX);
            this.Controls.Add(this.textBox_AbsolutY);
            this.Controls.Add(this.textBox_AbsolutX);
            this.Controls.Add(this.textBox_Faktor);
            this.Controls.Add(this.label_AktSkalierung);
            this.Controls.Add(this.label_Absolut);
            this.Controls.Add(this.button_Abbruch);
            this.Controls.Add(this.button_OK);
            this.Controls.Add(this.label_MitteSkalierung);
            this.Controls.Add(this.label_MaxSkalierung);
            this.Controls.Add(this.label_MinSkalierung);
            this.Controls.Add(this.checkBox_Auto);
            this.Controls.Add(this.trackBar_Skalierung);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(318, 270);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(318, 270);
            this.Name = "Skalierung";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Skalierung";
            this.Load += new System.EventHandler(this.Skalierung_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Skalierung)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar trackBar_Skalierung;
        private System.Windows.Forms.CheckBox checkBox_Auto;
        private System.Windows.Forms.Label label_MinSkalierung;
        private System.Windows.Forms.Label label_MaxSkalierung;
        private System.Windows.Forms.Label label_MitteSkalierung;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.Button button_Abbruch;
        private System.Windows.Forms.Label label_Absolut;
        private System.Windows.Forms.Label label_AktSkalierung;
        private System.Windows.Forms.TextBox textBox_Faktor;
        private System.Windows.Forms.TextBox textBox_AbsolutX;
        private System.Windows.Forms.TextBox textBox_AbsolutY;
        private System.Windows.Forms.Label label_GrößeX;
        private System.Windows.Forms.Label label_Plottergröße;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_PlotterY;
        private System.Windows.Forms.TextBox textBox_PlotterX;
    }
}
