namespace ExposerControl
{
    partial class MainWindow
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
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openPdfDialog = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.stepperReadyCheckBox = new System.Windows.Forms.CheckBox();
            this.servoLockedCheckBox = new System.Windows.Forms.CheckBox();
            this.doorCheckBox = new System.Windows.Forms.CheckBox();
            this.controlButton = new System.Windows.Forms.Button();
            this.editCalibrationButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.continueButton = new System.Windows.Forms.Button();
            this.goButton = new System.Windows.Forms.Button();
            this.timeEstimateLabel = new System.Windows.Forms.Label();
            this.delayNumeric = new System.Windows.Forms.NumericUpDown();
            this.timerLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lineNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.resetButton = new System.Windows.Forms.Button();
            this.openImageFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.sizeLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.delayNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lineNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(1165, 554);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1165, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.openImageToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.openToolStripMenuItem.Text = "Open pdf";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // openImageToolStripMenuItem
            // 
            this.openImageToolStripMenuItem.Name = "openImageToolStripMenuItem";
            this.openImageToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.openImageToolStripMenuItem.Text = "Open Image";
            this.openImageToolStripMenuItem.Click += new System.EventHandler(this.openImageToolStripMenuItem_Click);
            // 
            // openPdfDialog
            // 
            this.openPdfDialog.Filter = "pdf Dateien|*.pdf";
            this.openPdfDialog.Title = "Select an eagle exported pdf file";
            this.openPdfDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.pictureBox1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.groupBox2);
            this.splitContainer1.Panel2.Controls.Add(this.groupBox1);
            this.splitContainer1.Size = new System.Drawing.Size(1165, 698);
            this.splitContainer1.SplitterDistance = 554;
            this.splitContainer1.TabIndex = 2;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.stepperReadyCheckBox);
            this.groupBox2.Controls.Add(this.servoLockedCheckBox);
            this.groupBox2.Controls.Add(this.doorCheckBox);
            this.groupBox2.Controls.Add(this.controlButton);
            this.groupBox2.Controls.Add(this.editCalibrationButton);
            this.groupBox2.Controls.Add(this.connectButton);
            this.groupBox2.Location = new System.Drawing.Point(516, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(379, 116);
            this.groupBox2.TabIndex = 25;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Control";
            // 
            // stepperReadyCheckBox
            // 
            this.stepperReadyCheckBox.AutoSize = true;
            this.stepperReadyCheckBox.Location = new System.Drawing.Point(133, 68);
            this.stepperReadyCheckBox.Name = "stepperReadyCheckBox";
            this.stepperReadyCheckBox.Size = new System.Drawing.Size(90, 17);
            this.stepperReadyCheckBox.TabIndex = 5;
            this.stepperReadyCheckBox.Text = "stepper ready";
            this.stepperReadyCheckBox.UseVisualStyleBackColor = true;
            // 
            // servoLockedCheckBox
            // 
            this.servoLockedCheckBox.AutoSize = true;
            this.servoLockedCheckBox.Location = new System.Drawing.Point(133, 45);
            this.servoLockedCheckBox.Name = "servoLockedCheckBox";
            this.servoLockedCheckBox.Size = new System.Drawing.Size(87, 17);
            this.servoLockedCheckBox.TabIndex = 4;
            this.servoLockedCheckBox.Text = "servo locked";
            this.servoLockedCheckBox.UseVisualStyleBackColor = true;
            // 
            // doorCheckBox
            // 
            this.doorCheckBox.AutoSize = true;
            this.doorCheckBox.Location = new System.Drawing.Point(133, 22);
            this.doorCheckBox.Name = "doorCheckBox";
            this.doorCheckBox.Size = new System.Drawing.Size(81, 17);
            this.doorCheckBox.TabIndex = 3;
            this.doorCheckBox.Text = "door closed";
            this.doorCheckBox.UseVisualStyleBackColor = true;
            // 
            // controlButton
            // 
            this.controlButton.Location = new System.Drawing.Point(7, 49);
            this.controlButton.Name = "controlButton";
            this.controlButton.Size = new System.Drawing.Size(83, 23);
            this.controlButton.TabIndex = 2;
            this.controlButton.Text = "control";
            this.controlButton.UseVisualStyleBackColor = true;
            this.controlButton.Click += new System.EventHandler(this.controlButton_Click);
            // 
            // editCalibrationButton
            // 
            this.editCalibrationButton.Location = new System.Drawing.Point(7, 78);
            this.editCalibrationButton.Name = "editCalibrationButton";
            this.editCalibrationButton.Size = new System.Drawing.Size(84, 23);
            this.editCalibrationButton.TabIndex = 1;
            this.editCalibrationButton.Text = "edit calibration";
            this.editCalibrationButton.UseVisualStyleBackColor = true;
            this.editCalibrationButton.Click += new System.EventHandler(this.editCalibrationButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(6, 19);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(84, 23);
            this.connectButton.TabIndex = 0;
            this.connectButton.Text = "connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.sizeLabel);
            this.groupBox1.Controls.Add(this.continueButton);
            this.groupBox1.Controls.Add(this.goButton);
            this.groupBox1.Controls.Add(this.timeEstimateLabel);
            this.groupBox1.Controls.Add(this.delayNumeric);
            this.groupBox1.Controls.Add(this.timerLabel);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.lineNumericUpDown);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.resetButton);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(497, 116);
            this.groupBox1.TabIndex = 24;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Exposure";
            // 
            // continueButton
            // 
            this.continueButton.Location = new System.Drawing.Point(6, 50);
            this.continueButton.Name = "continueButton";
            this.continueButton.Size = new System.Drawing.Size(86, 23);
            this.continueButton.TabIndex = 24;
            this.continueButton.Text = "Continue";
            this.continueButton.UseVisualStyleBackColor = true;
            this.continueButton.Click += new System.EventHandler(this.continueButton_Click);
            // 
            // goButton
            // 
            this.goButton.Location = new System.Drawing.Point(6, 19);
            this.goButton.Name = "goButton";
            this.goButton.Size = new System.Drawing.Size(86, 23);
            this.goButton.TabIndex = 14;
            this.goButton.Text = "Go";
            this.goButton.UseVisualStyleBackColor = true;
            this.goButton.Click += new System.EventHandler(this.goButton_Click);
            // 
            // timeEstimateLabel
            // 
            this.timeEstimateLabel.AutoSize = true;
            this.timeEstimateLabel.Location = new System.Drawing.Point(438, 55);
            this.timeEstimateLabel.Name = "timeEstimateLabel";
            this.timeEstimateLabel.Size = new System.Drawing.Size(34, 13);
            this.timeEstimateLabel.TabIndex = 23;
            this.timeEstimateLabel.Text = "00:00";
            // 
            // delayNumeric
            // 
            this.delayNumeric.Location = new System.Drawing.Point(188, 49);
            this.delayNumeric.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.delayNumeric.Name = "delayNumeric";
            this.delayNumeric.Size = new System.Drawing.Size(120, 20);
            this.delayNumeric.TabIndex = 15;
            this.delayNumeric.Value = new decimal(new int[] {
            400,
            0,
            0,
            0});
            this.delayNumeric.ValueChanged += new System.EventHandler(this.delayNumeric_ValueChanged);
            // 
            // timerLabel
            // 
            this.timerLabel.AutoSize = true;
            this.timerLabel.Location = new System.Drawing.Point(438, 21);
            this.timerLabel.Name = "timerLabel";
            this.timerLabel.Size = new System.Drawing.Size(34, 13);
            this.timerLabel.TabIndex = 22;
            this.timerLabel.Text = "00:00";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(116, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(27, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Line";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(356, 55);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 13);
            this.label4.TabIndex = 21;
            this.label4.Text = "time estimate";
            // 
            // lineNumericUpDown
            // 
            this.lineNumericUpDown.Location = new System.Drawing.Point(188, 19);
            this.lineNumericUpDown.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.lineNumericUpDown.Name = "lineNumericUpDown";
            this.lineNumericUpDown.Size = new System.Drawing.Size(120, 20);
            this.lineNumericUpDown.TabIndex = 17;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(356, 21);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 20;
            this.label3.Text = "timer";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(116, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Delay (ms)";
            // 
            // resetButton
            // 
            this.resetButton.Location = new System.Drawing.Point(6, 78);
            this.resetButton.Name = "resetButton";
            this.resetButton.Size = new System.Drawing.Size(85, 23);
            this.resetButton.TabIndex = 19;
            this.resetButton.Text = "Reset";
            this.resetButton.UseVisualStyleBackColor = true;
            this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
            // 
            // openImageFileDialog
            // 
            this.openImageFileDialog.Title = "Select 600dpi image";
            this.openImageFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openImageFileDialog_FileOk);
            // 
            // sizeLabel
            // 
            this.sizeLabel.AutoSize = true;
            this.sizeLabel.Location = new System.Drawing.Point(356, 83);
            this.sizeLabel.Name = "sizeLabel";
            this.sizeLabel.Size = new System.Drawing.Size(25, 13);
            this.sizeLabel.TabIndex = 25;
            this.sizeLabel.Text = "size";
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1165, 722);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "Exposer Control";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainWindow_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.delayNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lineNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openPdfDialog;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button goButton;
        private System.Windows.Forms.NumericUpDown delayNumeric;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown lineNumericUpDown;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button resetButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label timeEstimateLabel;
        private System.Windows.Forms.Label timerLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button editCalibrationButton;
        private System.Windows.Forms.Button controlButton;
        private System.Windows.Forms.CheckBox doorCheckBox;
        private System.Windows.Forms.CheckBox stepperReadyCheckBox;
        private System.Windows.Forms.CheckBox servoLockedCheckBox;
        private System.Windows.Forms.Button continueButton;
        private System.Windows.Forms.OpenFileDialog openImageFileDialog;
        private System.Windows.Forms.ToolStripMenuItem openImageToolStripMenuItem;
        private System.Windows.Forms.Label sizeLabel;
    }
}

