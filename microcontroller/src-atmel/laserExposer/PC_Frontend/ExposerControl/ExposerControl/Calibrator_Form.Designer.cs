namespace ExposerControl
{
    partial class Calibrator_Form
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.patternButton = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.saveCalibrationButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.startNumericUpdown = new System.Windows.Forms.NumericUpDown();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.startNumericUpdown)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.patternButton);
            this.splitContainer1.Panel1.Controls.Add(this.button1);
            this.splitContainer1.Panel1.Controls.Add(this.saveCalibrationButton);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.startNumericUpdown);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel1);
            this.splitContainer1.Size = new System.Drawing.Size(685, 395);
            this.splitContainer1.SplitterDistance = 88;
            this.splitContainer1.TabIndex = 0;
            // 
            // patternButton
            // 
            this.patternButton.Location = new System.Drawing.Point(360, 38);
            this.patternButton.Name = "patternButton";
            this.patternButton.Size = new System.Drawing.Size(123, 23);
            this.patternButton.TabIndex = 5;
            this.patternButton.Text = "100 mil pattern";
            this.patternButton.UseVisualStyleBackColor = true;
            this.patternButton.Click += new System.EventHandler(this.patternButton_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(489, 9);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(123, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Read calibration";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // saveCalibrationButton
            // 
            this.saveCalibrationButton.Location = new System.Drawing.Point(360, 9);
            this.saveCalibrationButton.Name = "saveCalibrationButton";
            this.saveCalibrationButton.Size = new System.Drawing.Size(123, 23);
            this.saveCalibrationButton.TabIndex = 3;
            this.saveCalibrationButton.Text = "Save calibration";
            this.saveCalibrationButton.UseVisualStyleBackColor = true;
            this.saveCalibrationButton.Click += new System.EventHandler(this.saveCalibrationButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Stepper start";
            // 
            // startNumericUpdown
            // 
            this.startNumericUpdown.Location = new System.Drawing.Point(112, 12);
            this.startNumericUpdown.Maximum = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.startNumericUpdown.Minimum = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.startNumericUpdown.Name = "startNumericUpdown";
            this.startNumericUpdown.Size = new System.Drawing.Size(120, 20);
            this.startNumericUpdown.TabIndex = 0;
            this.startNumericUpdown.Value = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.startNumericUpdown.ValueChanged += new System.EventHandler(this.startNumericUpdown_ValueChanged);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(685, 303);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // Calibrator_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(685, 395);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Calibrator_Form";
            this.Text = "Calibrator_Form";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.startNumericUpdown)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown startNumericUpdown;
        private System.Windows.Forms.Button saveCalibrationButton;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button patternButton;

    }
}