namespace PlotterCom {
    partial class ViewerForm {
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
            this.statusStrip_Unten = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel_Unten = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this.hpglViewer = new PlotterCom.HPGLViewer();
            this.toolStrip_Oben = new System.Windows.Forms.ToolStrip();
            this.toolStripButton_Beenden = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButton_Scale = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton_HQ = new System.Windows.Forms.ToolStripButton();
            this.statusStrip_Unten.SuspendLayout();
            this.toolStripContainer.ContentPanel.SuspendLayout();
            this.toolStripContainer.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer.SuspendLayout();
            this.toolStrip_Oben.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip_Unten
            // 
            this.statusStrip_Unten.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel_Unten});
            this.statusStrip_Unten.Location = new System.Drawing.Point(0, 576);
            this.statusStrip_Unten.Name = "statusStrip_Unten";
            this.statusStrip_Unten.Size = new System.Drawing.Size(724, 22);
            this.statusStrip_Unten.TabIndex = 1;
            this.statusStrip_Unten.Text = "statusStrip1";
            // 
            // toolStripStatusLabel_Unten
            // 
            this.toolStripStatusLabel_Unten.Name = "toolStripStatusLabel_Unten";
            this.toolStripStatusLabel_Unten.Size = new System.Drawing.Size(46, 17);
            this.toolStripStatusLabel_Unten.Text = "Bereit...";
            // 
            // toolStripContainer
            // 
            this.toolStripContainer.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer.ContentPanel
            // 
            this.toolStripContainer.ContentPanel.AutoScroll = true;
            this.toolStripContainer.ContentPanel.Controls.Add(this.hpglViewer);
            this.toolStripContainer.ContentPanel.Size = new System.Drawing.Size(724, 551);
            this.toolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer.LeftToolStripPanelVisible = false;
            this.toolStripContainer.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer.Name = "toolStripContainer";
            this.toolStripContainer.RightToolStripPanelVisible = false;
            this.toolStripContainer.Size = new System.Drawing.Size(724, 576);
            this.toolStripContainer.TabIndex = 2;
            this.toolStripContainer.Text = "toolStripContainer1";
            // 
            // toolStripContainer.TopToolStripPanel
            // 
            this.toolStripContainer.TopToolStripPanel.Controls.Add(this.toolStrip_Oben);
            // 
            // hpglViewer
            // 
            this.hpglViewer.AnsichtSkalieren = false;
            this.hpglViewer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.hpglViewer.Hintergrundfarbe = System.Drawing.Color.FromArgb(((int)(((byte)(250)))), ((int)(((byte)(250)))), ((int)(((byte)(230)))));
            this.hpglViewer.Location = new System.Drawing.Point(0, 0);
            this.hpglViewer.MinimumSize = new System.Drawing.Size(120, 82);
            this.hpglViewer.Name = "hpglViewer";
            this.hpglViewer.Size = new System.Drawing.Size(724, 551);
            this.hpglViewer.TabIndex = 0;
            this.hpglViewer.ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            // 
            // toolStrip_Oben
            // 
            this.toolStrip_Oben.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip_Oben.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton_Beenden,
            this.toolStripSeparator1,
            this.toolStripButton_Scale,
            this.toolStripButton_HQ});
            this.toolStrip_Oben.Location = new System.Drawing.Point(3, 0);
            this.toolStrip_Oben.Name = "toolStrip_Oben";
            this.toolStrip_Oben.Size = new System.Drawing.Size(87, 25);
            this.toolStrip_Oben.TabIndex = 0;
            // 
            // toolStripButton_Beenden
            // 
            this.toolStripButton_Beenden.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton_Beenden.Image = global::HPGLViewer.Properties.Resources.exit;
            this.toolStripButton_Beenden.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton_Beenden.Name = "toolStripButton_Beenden";
            this.toolStripButton_Beenden.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton_Beenden.Text = "Beenden";
            this.toolStripButton_Beenden.Click += new System.EventHandler(this.toolStripButton_Beenden_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButton_Scale
            // 
            this.toolStripButton_Scale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton_Scale.Image = global::HPGLViewer.Properties.Resources.window_fullscreen;
            this.toolStripButton_Scale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton_Scale.Name = "toolStripButton_Scale";
            this.toolStripButton_Scale.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton_Scale.Text = "Ansicht skalieren";
            this.toolStripButton_Scale.Click += new System.EventHandler(this.toolStripButton_Scale_Click);
            // 
            // toolStripButton_HQ
            // 
            this.toolStripButton_HQ.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton_HQ.Image = global::HPGLViewer.Properties.Resources.icons;
            this.toolStripButton_HQ.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton_HQ.Name = "toolStripButton_HQ";
            this.toolStripButton_HQ.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton_HQ.Text = "In hoher Qualität zeichnen";
            this.toolStripButton_HQ.Click += new System.EventHandler(this.toolStripButton_HQ_Click);
            // 
            // ViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(724, 598);
            this.Controls.Add(this.toolStripContainer);
            this.Controls.Add(this.statusStrip_Unten);
            this.KeyPreview = true;
            this.Name = "ViewerForm";
            this.Text = "HPGL Viewer";
            this.Load += new System.EventHandler(this.ViewerForm_Load);
            this.Shown += new System.EventHandler(this.ViewerForm_Shown);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ViewerForm_KeyDown);
            this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.ViewerForm_PreviewKeyDown);
            this.statusStrip_Unten.ResumeLayout(false);
            this.statusStrip_Unten.PerformLayout();
            this.toolStripContainer.ContentPanel.ResumeLayout(false);
            this.toolStripContainer.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer.TopToolStripPanel.PerformLayout();
            this.toolStripContainer.ResumeLayout(false);
            this.toolStripContainer.PerformLayout();
            this.toolStrip_Oben.ResumeLayout(false);
            this.toolStrip_Oben.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private PlotterCom.HPGLViewer hpglViewer;
        private System.Windows.Forms.StatusStrip statusStrip_Unten;
        private System.Windows.Forms.ToolStripContainer toolStripContainer;
        private System.Windows.Forms.ToolStrip toolStrip_Oben;
        private System.Windows.Forms.ToolStripButton toolStripButton_Scale;
        private System.Windows.Forms.ToolStripButton toolStripButton_Beenden;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel_Unten;
        private System.Windows.Forms.ToolStripButton toolStripButton_HQ;
    }
}

