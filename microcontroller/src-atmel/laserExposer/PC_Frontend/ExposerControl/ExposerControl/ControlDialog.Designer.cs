namespace ExposerControl
{
    partial class ControlDialog
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
            this.port_comboBox = new System.Windows.Forms.ComboBox();
            this.connect_button = new System.Windows.Forms.Button();
            this.port_status_textBox = new System.Windows.Forms.Label();
            this.laserOnButton = new System.Windows.Forms.Button();
            this.laserOffButton = new System.Windows.Forms.Button();
            this.scannerOffButton = new System.Windows.Forms.Button();
            this.scannerOnButton = new System.Windows.Forms.Button();
            this.stepperOffButton = new System.Windows.Forms.Button();
            this.stepperCommandButton = new System.Windows.Forms.Button();
            this.stepperNumericUpdown = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.stepperNumericUpdown)).BeginInit();
            this.SuspendLayout();
            // 
            // port_comboBox
            // 
            this.port_comboBox.FormattingEnabled = true;
            this.port_comboBox.Location = new System.Drawing.Point(182, 10);
            this.port_comboBox.Name = "port_comboBox";
            this.port_comboBox.Size = new System.Drawing.Size(121, 21);
            this.port_comboBox.TabIndex = 16;
            // 
            // connect_button
            // 
            this.connect_button.Location = new System.Drawing.Point(90, 10);
            this.connect_button.Name = "connect_button";
            this.connect_button.Size = new System.Drawing.Size(86, 23);
            this.connect_button.TabIndex = 14;
            this.connect_button.Text = "connect";
            this.connect_button.UseVisualStyleBackColor = true;
            this.connect_button.Click += new System.EventHandler(this.connect_button_Click);
            // 
            // port_status_textBox
            // 
            this.port_status_textBox.Location = new System.Drawing.Point(12, 17);
            this.port_status_textBox.Name = "port_status_textBox";
            this.port_status_textBox.Size = new System.Drawing.Size(50, 13);
            this.port_status_textBox.TabIndex = 15;
            this.port_status_textBox.Text = "label1";
            // 
            // laserOnButton
            // 
            this.laserOnButton.Location = new System.Drawing.Point(90, 71);
            this.laserOnButton.Name = "laserOnButton";
            this.laserOnButton.Size = new System.Drawing.Size(86, 23);
            this.laserOnButton.TabIndex = 18;
            this.laserOnButton.Text = "Laser On";
            this.laserOnButton.UseVisualStyleBackColor = true;
            this.laserOnButton.Click += new System.EventHandler(this.laserOnButton_Click);
            // 
            // laserOffButton
            // 
            this.laserOffButton.Location = new System.Drawing.Point(182, 71);
            this.laserOffButton.Name = "laserOffButton";
            this.laserOffButton.Size = new System.Drawing.Size(86, 23);
            this.laserOffButton.TabIndex = 19;
            this.laserOffButton.Text = "Laser Off";
            this.laserOffButton.UseVisualStyleBackColor = true;
            this.laserOffButton.Click += new System.EventHandler(this.laserOffButton_Click);
            // 
            // scannerOffButton
            // 
            this.scannerOffButton.Location = new System.Drawing.Point(182, 100);
            this.scannerOffButton.Name = "scannerOffButton";
            this.scannerOffButton.Size = new System.Drawing.Size(86, 23);
            this.scannerOffButton.TabIndex = 21;
            this.scannerOffButton.Text = "Scanner Off";
            this.scannerOffButton.UseVisualStyleBackColor = true;
            this.scannerOffButton.Click += new System.EventHandler(this.scannerOffButton_Click);
            // 
            // scannerOnButton
            // 
            this.scannerOnButton.Location = new System.Drawing.Point(90, 100);
            this.scannerOnButton.Name = "scannerOnButton";
            this.scannerOnButton.Size = new System.Drawing.Size(86, 23);
            this.scannerOnButton.TabIndex = 20;
            this.scannerOnButton.Text = "Scanner On";
            this.scannerOnButton.UseVisualStyleBackColor = true;
            this.scannerOnButton.Click += new System.EventHandler(this.scannerOnButton_Click);
            // 
            // stepperOffButton
            // 
            this.stepperOffButton.Location = new System.Drawing.Point(182, 181);
            this.stepperOffButton.Name = "stepperOffButton";
            this.stepperOffButton.Size = new System.Drawing.Size(107, 23);
            this.stepperOffButton.TabIndex = 22;
            this.stepperOffButton.Text = "stepper off";
            this.stepperOffButton.UseVisualStyleBackColor = true;
            this.stepperOffButton.Click += new System.EventHandler(this.stepperOffButton_Click);
            // 
            // stepperCommandButton
            // 
            this.stepperCommandButton.Location = new System.Drawing.Point(182, 152);
            this.stepperCommandButton.Name = "stepperCommandButton";
            this.stepperCommandButton.Size = new System.Drawing.Size(107, 23);
            this.stepperCommandButton.TabIndex = 23;
            this.stepperCommandButton.Text = "stepper command";
            this.stepperCommandButton.UseVisualStyleBackColor = true;
            this.stepperCommandButton.Click += new System.EventHandler(this.stepperCommandButton_Click);
            // 
            // stepperNumericUpdown
            // 
            this.stepperNumericUpdown.Location = new System.Drawing.Point(90, 155);
            this.stepperNumericUpdown.Maximum = new decimal(new int[] {
            4800,
            0,
            0,
            0});
            this.stepperNumericUpdown.Minimum = new decimal(new int[] {
            3,
            0,
            0,
            -2147483648});
            this.stepperNumericUpdown.Name = "stepperNumericUpdown";
            this.stepperNumericUpdown.Size = new System.Drawing.Size(86, 20);
            this.stepperNumericUpdown.TabIndex = 24;
            // 
            // ControlDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(344, 273);
            this.Controls.Add(this.stepperNumericUpdown);
            this.Controls.Add(this.stepperCommandButton);
            this.Controls.Add(this.stepperOffButton);
            this.Controls.Add(this.scannerOffButton);
            this.Controls.Add(this.scannerOnButton);
            this.Controls.Add(this.laserOffButton);
            this.Controls.Add(this.laserOnButton);
            this.Controls.Add(this.port_comboBox);
            this.Controls.Add(this.port_status_textBox);
            this.Controls.Add(this.connect_button);
            this.Name = "ControlDialog";
            this.Text = "ControlDialog";
            ((System.ComponentModel.ISupportInitialize)(this.stepperNumericUpdown)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox port_comboBox;
        private System.Windows.Forms.Button connect_button;
        private System.Windows.Forms.Label port_status_textBox;
        private System.Windows.Forms.Button laserOnButton;
        private System.Windows.Forms.Button laserOffButton;
        private System.Windows.Forms.Button scannerOffButton;
        private System.Windows.Forms.Button scannerOnButton;
        private System.Windows.Forms.Button stepperOffButton;
        private System.Windows.Forms.Button stepperCommandButton;
        private System.Windows.Forms.NumericUpDown stepperNumericUpdown;

    }
}