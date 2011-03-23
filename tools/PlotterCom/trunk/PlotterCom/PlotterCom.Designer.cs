namespace PlotterCom {
    partial class Form_Hauptfenster {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.menuStrip_HauptMenü = new System.Windows.Forms.MenuStrip();
            this.dateiToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.einstellungenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem_Strich1 = new System.Windows.Forms.ToolStripSeparator();
            this.beendenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.überPlotterComToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tableLayoutPanel_Hauptlayout = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox_Öffnen = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel_Öffnen = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox_Speichern = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel_Speichern = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox_Optimieren = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel_Optimieren = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox_Senden = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel_Senden = new System.Windows.Forms.TableLayoutPanel();
            this.label_Senden = new System.Windows.Forms.Label();
            this.progressBar_Senden = new System.Windows.Forms.ProgressBar();
            this.label_ETA = new System.Windows.Forms.Label();
            this.statusStrip_unten = new System.Windows.Forms.StatusStrip();
            this.toolStripProgressBar_Unten = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialog_HPGL = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog_HPGL = new System.Windows.Forms.SaveFileDialog();
            this.button_Viewer = new System.Windows.Forms.Button();
            this.button_Öffnen = new System.Windows.Forms.Button();
            this.label_DateiGeladen = new System.Windows.Forms.Label();
            this.button_Abbruch = new System.Windows.Forms.Button();
            this.button_Schließen = new System.Windows.Forms.Button();
            this.button_Skalieren = new System.Windows.Forms.Button();
            this.button_Verschieben = new System.Windows.Forms.Button();
            this.button_Speichern = new System.Windows.Forms.Button();
            this.button_SpeichernAls = new System.Windows.Forms.Button();
            this.button_Optimieren = new System.Windows.Forms.Button();
            this.button_OptimierenAbbrechen = new System.Windows.Forms.Button();
            this.button_Dubletten = new System.Windows.Forms.Button();
            this.button_Verbinden = new System.Windows.Forms.Button();
            this.button_Analysieren = new System.Windows.Forms.Button();
            this.button_Senden = new System.Windows.Forms.Button();
            this.button_SendenAbbrechen = new System.Windows.Forms.Button();
            this.button_DirektSenden = new System.Windows.Forms.Button();
            this.button_UmschrRechteck = new System.Windows.Forms.Button();
            this.button_Netzwerk = new System.Windows.Forms.Button();
            this.menuStrip_HauptMenü.SuspendLayout();
            this.tableLayoutPanel_Hauptlayout.SuspendLayout();
            this.groupBox_Öffnen.SuspendLayout();
            this.tableLayoutPanel_Öffnen.SuspendLayout();
            this.groupBox_Speichern.SuspendLayout();
            this.tableLayoutPanel_Speichern.SuspendLayout();
            this.groupBox_Optimieren.SuspendLayout();
            this.tableLayoutPanel_Optimieren.SuspendLayout();
            this.groupBox_Senden.SuspendLayout();
            this.tableLayoutPanel_Senden.SuspendLayout();
            this.statusStrip_unten.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip_HauptMenü
            // 
            this.menuStrip_HauptMenü.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dateiToolStripMenuItem,
            this.infoToolStripMenuItem});
            this.menuStrip_HauptMenü.Location = new System.Drawing.Point(0, 0);
            this.menuStrip_HauptMenü.Name = "menuStrip_HauptMenü";
            this.menuStrip_HauptMenü.Size = new System.Drawing.Size(677, 24);
            this.menuStrip_HauptMenü.TabIndex = 2;
            this.menuStrip_HauptMenü.Text = "menuStrip1";
            // 
            // dateiToolStripMenuItem
            // 
            this.dateiToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.einstellungenToolStripMenuItem,
            this.toolStripMenuItem_Strich1,
            this.beendenToolStripMenuItem});
            this.dateiToolStripMenuItem.Name = "dateiToolStripMenuItem";
            this.dateiToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.dateiToolStripMenuItem.Text = "Datei";
            // 
            // einstellungenToolStripMenuItem
            // 
            this.einstellungenToolStripMenuItem.Name = "einstellungenToolStripMenuItem";
            this.einstellungenToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.einstellungenToolStripMenuItem.Text = "Einstellungen";
            this.einstellungenToolStripMenuItem.Click += new System.EventHandler(this.einstellungenToolStripMenuItem_Click);
            // 
            // toolStripMenuItem_Strich1
            // 
            this.toolStripMenuItem_Strich1.Name = "toolStripMenuItem_Strich1";
            this.toolStripMenuItem_Strich1.Size = new System.Drawing.Size(145, 6);
            // 
            // beendenToolStripMenuItem
            // 
            this.beendenToolStripMenuItem.Name = "beendenToolStripMenuItem";
            this.beendenToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.beendenToolStripMenuItem.Text = "Beenden";
            this.beendenToolStripMenuItem.Click += new System.EventHandler(this.beendenToolStripMenuItem_Click);
            // 
            // infoToolStripMenuItem
            // 
            this.infoToolStripMenuItem.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.infoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.überPlotterComToolStripMenuItem});
            this.infoToolStripMenuItem.Name = "infoToolStripMenuItem";
            this.infoToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.infoToolStripMenuItem.Text = "Info";
            // 
            // überPlotterComToolStripMenuItem
            // 
            this.überPlotterComToolStripMenuItem.Name = "überPlotterComToolStripMenuItem";
            this.überPlotterComToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.überPlotterComToolStripMenuItem.Text = "Über PlotterCom";
            this.überPlotterComToolStripMenuItem.Click += new System.EventHandler(this.überPlotterComToolStripMenuItem_Click);
            // 
            // tableLayoutPanel_Hauptlayout
            // 
            this.tableLayoutPanel_Hauptlayout.ColumnCount = 2;
            this.tableLayoutPanel_Hauptlayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Hauptlayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Hauptlayout.Controls.Add(this.groupBox_Öffnen, 0, 0);
            this.tableLayoutPanel_Hauptlayout.Controls.Add(this.groupBox_Speichern, 1, 0);
            this.tableLayoutPanel_Hauptlayout.Controls.Add(this.groupBox_Optimieren, 0, 2);
            this.tableLayoutPanel_Hauptlayout.Controls.Add(this.groupBox_Senden, 1, 1);
            this.tableLayoutPanel_Hauptlayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel_Hauptlayout.Location = new System.Drawing.Point(0, 24);
            this.tableLayoutPanel_Hauptlayout.Name = "tableLayoutPanel_Hauptlayout";
            this.tableLayoutPanel_Hauptlayout.RowCount = 4;
            this.tableLayoutPanel_Hauptlayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel_Hauptlayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel_Hauptlayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Hauptlayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 22F));
            this.tableLayoutPanel_Hauptlayout.Size = new System.Drawing.Size(677, 449);
            this.tableLayoutPanel_Hauptlayout.TabIndex = 3;
            // 
            // groupBox_Öffnen
            // 
            this.groupBox_Öffnen.Controls.Add(this.tableLayoutPanel_Öffnen);
            this.groupBox_Öffnen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox_Öffnen.Location = new System.Drawing.Point(3, 3);
            this.groupBox_Öffnen.Name = "groupBox_Öffnen";
            this.tableLayoutPanel_Hauptlayout.SetRowSpan(this.groupBox_Öffnen, 2);
            this.groupBox_Öffnen.Size = new System.Drawing.Size(332, 207);
            this.groupBox_Öffnen.TabIndex = 0;
            this.groupBox_Öffnen.TabStop = false;
            this.groupBox_Öffnen.Text = "Datei öffnen";
            // 
            // tableLayoutPanel_Öffnen
            // 
            this.tableLayoutPanel_Öffnen.ColumnCount = 3;
            this.tableLayoutPanel_Öffnen.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Öffnen.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel_Öffnen.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Viewer, 0, 1);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Öffnen, 0, 0);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.label_DateiGeladen, 1, 0);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Abbruch, 2, 0);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Schließen, 2, 1);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Skalieren, 0, 2);
            this.tableLayoutPanel_Öffnen.Controls.Add(this.button_Verschieben, 2, 2);
            this.tableLayoutPanel_Öffnen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel_Öffnen.Location = new System.Drawing.Point(3, 16);
            this.tableLayoutPanel_Öffnen.Name = "tableLayoutPanel_Öffnen";
            this.tableLayoutPanel_Öffnen.RowCount = 4;
            this.tableLayoutPanel_Öffnen.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Öffnen.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Öffnen.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Öffnen.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel_Öffnen.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel_Öffnen.Size = new System.Drawing.Size(326, 188);
            this.tableLayoutPanel_Öffnen.TabIndex = 0;
            // 
            // groupBox_Speichern
            // 
            this.groupBox_Speichern.Controls.Add(this.tableLayoutPanel_Speichern);
            this.groupBox_Speichern.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox_Speichern.Location = new System.Drawing.Point(341, 3);
            this.groupBox_Speichern.Name = "groupBox_Speichern";
            this.groupBox_Speichern.Size = new System.Drawing.Size(333, 79);
            this.groupBox_Speichern.TabIndex = 1;
            this.groupBox_Speichern.TabStop = false;
            this.groupBox_Speichern.Text = "Datei speichern";
            // 
            // tableLayoutPanel_Speichern
            // 
            this.tableLayoutPanel_Speichern.ColumnCount = 3;
            this.tableLayoutPanel_Speichern.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Speichern.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel_Speichern.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Speichern.Controls.Add(this.button_Speichern, 0, 0);
            this.tableLayoutPanel_Speichern.Controls.Add(this.button_SpeichernAls, 2, 0);
            this.tableLayoutPanel_Speichern.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel_Speichern.Location = new System.Drawing.Point(3, 16);
            this.tableLayoutPanel_Speichern.Name = "tableLayoutPanel_Speichern";
            this.tableLayoutPanel_Speichern.RowCount = 2;
            this.tableLayoutPanel_Speichern.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Speichern.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel_Speichern.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel_Speichern.Size = new System.Drawing.Size(327, 60);
            this.tableLayoutPanel_Speichern.TabIndex = 0;
            // 
            // groupBox_Optimieren
            // 
            this.groupBox_Optimieren.Controls.Add(this.tableLayoutPanel_Optimieren);
            this.groupBox_Optimieren.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox_Optimieren.Location = new System.Drawing.Point(3, 216);
            this.groupBox_Optimieren.Name = "groupBox_Optimieren";
            this.groupBox_Optimieren.Size = new System.Drawing.Size(332, 207);
            this.groupBox_Optimieren.TabIndex = 2;
            this.groupBox_Optimieren.TabStop = false;
            this.groupBox_Optimieren.Text = "HPGL optimieren";
            // 
            // tableLayoutPanel_Optimieren
            // 
            this.tableLayoutPanel_Optimieren.ColumnCount = 3;
            this.tableLayoutPanel_Optimieren.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Optimieren.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel_Optimieren.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Optimieren.Controls.Add(this.button_Optimieren, 0, 0);
            this.tableLayoutPanel_Optimieren.Controls.Add(this.button_OptimierenAbbrechen, 2, 0);
            this.tableLayoutPanel_Optimieren.Controls.Add(this.button_Dubletten, 0, 1);
            this.tableLayoutPanel_Optimieren.Controls.Add(this.button_Verbinden, 0, 2);
            this.tableLayoutPanel_Optimieren.Controls.Add(this.button_Analysieren, 2, 1);
            this.tableLayoutPanel_Optimieren.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel_Optimieren.Location = new System.Drawing.Point(3, 16);
            this.tableLayoutPanel_Optimieren.Name = "tableLayoutPanel_Optimieren";
            this.tableLayoutPanel_Optimieren.RowCount = 4;
            this.tableLayoutPanel_Optimieren.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Optimieren.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Optimieren.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Optimieren.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel_Optimieren.Size = new System.Drawing.Size(326, 188);
            this.tableLayoutPanel_Optimieren.TabIndex = 0;
            // 
            // groupBox_Senden
            // 
            this.groupBox_Senden.Controls.Add(this.tableLayoutPanel_Senden);
            this.groupBox_Senden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox_Senden.Location = new System.Drawing.Point(341, 88);
            this.groupBox_Senden.Name = "groupBox_Senden";
            this.tableLayoutPanel_Hauptlayout.SetRowSpan(this.groupBox_Senden, 2);
            this.groupBox_Senden.Size = new System.Drawing.Size(333, 335);
            this.groupBox_Senden.TabIndex = 3;
            this.groupBox_Senden.TabStop = false;
            this.groupBox_Senden.Text = "HPGL senden";
            // 
            // tableLayoutPanel_Senden
            // 
            this.tableLayoutPanel_Senden.ColumnCount = 3;
            this.tableLayoutPanel_Senden.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Senden.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel_Senden.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_Senden.Controls.Add(this.button_Senden, 0, 0);
            this.tableLayoutPanel_Senden.Controls.Add(this.button_SendenAbbrechen, 0, 1);
            this.tableLayoutPanel_Senden.Controls.Add(this.label_Senden, 0, 5);
            this.tableLayoutPanel_Senden.Controls.Add(this.progressBar_Senden, 0, 6);
            this.tableLayoutPanel_Senden.Controls.Add(this.button_DirektSenden, 2, 0);
            this.tableLayoutPanel_Senden.Controls.Add(this.button_UmschrRechteck, 2, 1);
            this.tableLayoutPanel_Senden.Controls.Add(this.label_ETA, 2, 5);
            this.tableLayoutPanel_Senden.Controls.Add(this.button_Netzwerk, 0, 3);
            this.tableLayoutPanel_Senden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel_Senden.Location = new System.Drawing.Point(3, 16);
            this.tableLayoutPanel_Senden.Name = "tableLayoutPanel_Senden";
            this.tableLayoutPanel_Senden.RowCount = 7;
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel_Senden.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel_Senden.Size = new System.Drawing.Size(327, 316);
            this.tableLayoutPanel_Senden.TabIndex = 0;
            // 
            // label_Senden
            // 
            this.label_Senden.AutoSize = true;
            this.label_Senden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label_Senden.Location = new System.Drawing.Point(3, 271);
            this.label_Senden.Name = "label_Senden";
            this.label_Senden.Size = new System.Drawing.Size(127, 20);
            this.label_Senden.TabIndex = 2;
            this.label_Senden.Text = "Bereit...";
            this.label_Senden.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // progressBar_Senden
            // 
            this.tableLayoutPanel_Senden.SetColumnSpan(this.progressBar_Senden, 3);
            this.progressBar_Senden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.progressBar_Senden.Location = new System.Drawing.Point(3, 294);
            this.progressBar_Senden.Name = "progressBar_Senden";
            this.progressBar_Senden.Size = new System.Drawing.Size(321, 19);
            this.progressBar_Senden.TabIndex = 1;
            // 
            // label_ETA
            // 
            this.label_ETA.AutoSize = true;
            this.label_ETA.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label_ETA.Location = new System.Drawing.Point(196, 271);
            this.label_ETA.Name = "label_ETA";
            this.label_ETA.Size = new System.Drawing.Size(128, 20);
            this.label_ETA.TabIndex = 5;
            this.label_ETA.Text = "ETA: 00:00:00";
            this.label_ETA.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // statusStrip_unten
            // 
            this.statusStrip_unten.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripProgressBar_Unten,
            this.toolStripStatusLabel});
            this.statusStrip_unten.Location = new System.Drawing.Point(0, 451);
            this.statusStrip_unten.Name = "statusStrip_unten";
            this.statusStrip_unten.Size = new System.Drawing.Size(677, 22);
            this.statusStrip_unten.TabIndex = 4;
            this.statusStrip_unten.Text = "statusStrip1";
            // 
            // toolStripProgressBar_Unten
            // 
            this.toolStripProgressBar_Unten.Name = "toolStripProgressBar_Unten";
            this.toolStripProgressBar_Unten.Size = new System.Drawing.Size(100, 16);
            this.toolStripProgressBar_Unten.Visible = false;
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(47, 17);
            this.toolStripStatusLabel.Text = "Bereit...";
            // 
            // openFileDialog_HPGL
            // 
            this.openFileDialog_HPGL.DefaultExt = "plt";
            this.openFileDialog_HPGL.Filter = "HPGL-Dateien|*.hpgl;*.hpg;*.plt;*.txt|Alle Dateien|*.*";
            this.openFileDialog_HPGL.RestoreDirectory = true;
            this.openFileDialog_HPGL.SupportMultiDottedExtensions = true;
            // 
            // saveFileDialog_HPGL
            // 
            this.saveFileDialog_HPGL.Filter = "HPGL-Datei|*.hpg;*.plt;*.hpgl;*.txt";
            // 
            // button_Viewer
            // 
            this.button_Viewer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Viewer.Image = global::PlotterCom.Properties.Resources.display;
            this.button_Viewer.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Viewer.Location = new System.Drawing.Point(3, 58);
            this.button_Viewer.Name = "button_Viewer";
            this.button_Viewer.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Viewer.Size = new System.Drawing.Size(127, 49);
            this.button_Viewer.TabIndex = 2;
            this.button_Viewer.Text = "Anzeigen";
            this.button_Viewer.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Viewer.UseVisualStyleBackColor = true;
            this.button_Viewer.Click += new System.EventHandler(this.button_Viewer_Click);
            // 
            // button_Öffnen
            // 
            this.button_Öffnen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Öffnen.Image = global::PlotterCom.Properties.Resources.fileopen;
            this.button_Öffnen.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Öffnen.Location = new System.Drawing.Point(3, 3);
            this.button_Öffnen.Name = "button_Öffnen";
            this.button_Öffnen.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Öffnen.Size = new System.Drawing.Size(127, 49);
            this.button_Öffnen.TabIndex = 0;
            this.button_Öffnen.Text = "Datei öffnen";
            this.button_Öffnen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Öffnen.UseVisualStyleBackColor = true;
            this.button_Öffnen.Click += new System.EventHandler(this.button_Öffnen_Click);
            // 
            // label_DateiGeladen
            // 
            this.label_DateiGeladen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label_DateiGeladen.Image = global::PlotterCom.Properties.Resources.button_cance;
            this.label_DateiGeladen.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label_DateiGeladen.Location = new System.Drawing.Point(136, 0);
            this.label_DateiGeladen.Name = "label_DateiGeladen";
            this.label_DateiGeladen.Size = new System.Drawing.Size(54, 55);
            this.label_DateiGeladen.TabIndex = 1;
            // 
            // button_Abbruch
            // 
            this.button_Abbruch.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Abbruch.Image = global::PlotterCom.Properties.Resources.button_cance;
            this.button_Abbruch.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Abbruch.Location = new System.Drawing.Point(196, 3);
            this.button_Abbruch.Name = "button_Abbruch";
            this.button_Abbruch.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Abbruch.Size = new System.Drawing.Size(127, 49);
            this.button_Abbruch.TabIndex = 4;
            this.button_Abbruch.Text = "Abbruch";
            this.button_Abbruch.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Abbruch.UseVisualStyleBackColor = true;
            this.button_Abbruch.Click += new System.EventHandler(this.button_Abrruch_Click);
            // 
            // button_Schließen
            // 
            this.button_Schließen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Schließen.Image = global::PlotterCom.Properties.Resources.editdelete;
            this.button_Schließen.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Schließen.Location = new System.Drawing.Point(196, 58);
            this.button_Schließen.Name = "button_Schließen";
            this.button_Schließen.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Schließen.Size = new System.Drawing.Size(127, 49);
            this.button_Schließen.TabIndex = 3;
            this.button_Schließen.Text = "Datei schließen";
            this.button_Schließen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Schließen.UseVisualStyleBackColor = true;
            this.button_Schließen.Click += new System.EventHandler(this.button_Schließen_Click);
            // 
            // button_Skalieren
            // 
            this.button_Skalieren.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Skalieren.Image = global::PlotterCom.Properties.Resources.window_fullscreen;
            this.button_Skalieren.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Skalieren.Location = new System.Drawing.Point(3, 113);
            this.button_Skalieren.Name = "button_Skalieren";
            this.button_Skalieren.Size = new System.Drawing.Size(127, 49);
            this.button_Skalieren.TabIndex = 6;
            this.button_Skalieren.Text = "Skalieren";
            this.button_Skalieren.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Skalieren.UseVisualStyleBackColor = true;
            this.button_Skalieren.Click += new System.EventHandler(this.button_Skalieren_Click);
            // 
            // button_Verschieben
            // 
            this.button_Verschieben.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Verschieben.Image = global::PlotterCom.Properties.Resources.forward;
            this.button_Verschieben.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Verschieben.Location = new System.Drawing.Point(196, 113);
            this.button_Verschieben.Name = "button_Verschieben";
            this.button_Verschieben.Size = new System.Drawing.Size(127, 49);
            this.button_Verschieben.TabIndex = 7;
            this.button_Verschieben.Text = "Verschieben";
            this.button_Verschieben.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Verschieben.UseVisualStyleBackColor = true;
            this.button_Verschieben.Click += new System.EventHandler(this.button_Verschieben_Click);
            // 
            // button_Speichern
            // 
            this.button_Speichern.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Speichern.Image = global::PlotterCom.Properties.Resources.filesave;
            this.button_Speichern.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Speichern.Location = new System.Drawing.Point(3, 3);
            this.button_Speichern.Name = "button_Speichern";
            this.button_Speichern.Size = new System.Drawing.Size(127, 49);
            this.button_Speichern.TabIndex = 1;
            this.button_Speichern.Text = "Datei\r\nspeichern...\r\n";
            this.button_Speichern.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Speichern.UseVisualStyleBackColor = true;
            this.button_Speichern.Click += new System.EventHandler(this.button_Speichern_Click);
            // 
            // button_SpeichernAls
            // 
            this.button_SpeichernAls.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_SpeichernAls.Image = global::PlotterCom.Properties.Resources.filesaveas;
            this.button_SpeichernAls.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_SpeichernAls.Location = new System.Drawing.Point(196, 3);
            this.button_SpeichernAls.Name = "button_SpeichernAls";
            this.button_SpeichernAls.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_SpeichernAls.Size = new System.Drawing.Size(128, 49);
            this.button_SpeichernAls.TabIndex = 0;
            this.button_SpeichernAls.Tag = "";
            this.button_SpeichernAls.Text = "Datei\r\nspeichern als...\r\n";
            this.button_SpeichernAls.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_SpeichernAls.UseVisualStyleBackColor = true;
            this.button_SpeichernAls.Click += new System.EventHandler(this.button_SpeichernAls_Click);
            // 
            // button_Optimieren
            // 
            this.button_Optimieren.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Optimieren.Image = global::PlotterCom.Properties.Resources.configure;
            this.button_Optimieren.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Optimieren.Location = new System.Drawing.Point(3, 3);
            this.button_Optimieren.Name = "button_Optimieren";
            this.button_Optimieren.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Optimieren.Size = new System.Drawing.Size(127, 49);
            this.button_Optimieren.TabIndex = 0;
            this.button_Optimieren.Text = "Optimieren";
            this.button_Optimieren.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Optimieren.UseVisualStyleBackColor = true;
            this.button_Optimieren.Click += new System.EventHandler(this.button_Optimieren_Click);
            // 
            // button_OptimierenAbbrechen
            // 
            this.button_OptimierenAbbrechen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_OptimierenAbbrechen.Enabled = false;
            this.button_OptimierenAbbrechen.Image = global::PlotterCom.Properties.Resources.button_cance;
            this.button_OptimierenAbbrechen.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_OptimierenAbbrechen.Location = new System.Drawing.Point(196, 3);
            this.button_OptimierenAbbrechen.Name = "button_OptimierenAbbrechen";
            this.button_OptimierenAbbrechen.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_OptimierenAbbrechen.Size = new System.Drawing.Size(127, 49);
            this.button_OptimierenAbbrechen.TabIndex = 5;
            this.button_OptimierenAbbrechen.Text = "Abbruch";
            this.button_OptimierenAbbrechen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_OptimierenAbbrechen.UseVisualStyleBackColor = true;
            this.button_OptimierenAbbrechen.Click += new System.EventHandler(this.button_OptimierenAbbrechen_Click);
            // 
            // button_Dubletten
            // 
            this.button_Dubletten.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Dubletten.Image = global::PlotterCom.Properties.Resources.editcut;
            this.button_Dubletten.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Dubletten.Location = new System.Drawing.Point(3, 58);
            this.button_Dubletten.Name = "button_Dubletten";
            this.button_Dubletten.Size = new System.Drawing.Size(127, 49);
            this.button_Dubletten.TabIndex = 8;
            this.button_Dubletten.Text = "Verdeckte \r\nLinien entf.";
            this.button_Dubletten.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Dubletten.UseVisualStyleBackColor = true;
            this.button_Dubletten.Click += new System.EventHandler(this.button_Dubletten_Click);
            // 
            // button_Verbinden
            // 
            this.button_Verbinden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Verbinden.Image = global::PlotterCom.Properties.Resources.start_here;
            this.button_Verbinden.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Verbinden.Location = new System.Drawing.Point(3, 113);
            this.button_Verbinden.Name = "button_Verbinden";
            this.button_Verbinden.Size = new System.Drawing.Size(127, 49);
            this.button_Verbinden.TabIndex = 9;
            this.button_Verbinden.Text = "Linien\r\nverbinden";
            this.button_Verbinden.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Verbinden.UseVisualStyleBackColor = true;
            this.button_Verbinden.Click += new System.EventHandler(this.button_Verbinden_Click);
            // 
            // button_Analysieren
            // 
            this.button_Analysieren.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Analysieren.Image = global::PlotterCom.Properties.Resources.find;
            this.button_Analysieren.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Analysieren.Location = new System.Drawing.Point(196, 58);
            this.button_Analysieren.Name = "button_Analysieren";
            this.button_Analysieren.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Analysieren.Size = new System.Drawing.Size(127, 49);
            this.button_Analysieren.TabIndex = 1;
            this.button_Analysieren.Text = "Analyse";
            this.button_Analysieren.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Analysieren.UseVisualStyleBackColor = true;
            this.button_Analysieren.Click += new System.EventHandler(this.button_Analysieren_Click);
            // 
            // button_Senden
            // 
            this.button_Senden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Senden.Image = global::PlotterCom.Properties.Resources.agt_print;
            this.button_Senden.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Senden.Location = new System.Drawing.Point(3, 3);
            this.button_Senden.Name = "button_Senden";
            this.button_Senden.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_Senden.Size = new System.Drawing.Size(127, 49);
            this.button_Senden.TabIndex = 0;
            this.button_Senden.Text = "HPGL\r\nSenden";
            this.button_Senden.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Senden.UseVisualStyleBackColor = true;
            this.button_Senden.Click += new System.EventHandler(this.button_Senden_Click);
            // 
            // button_SendenAbbrechen
            // 
            this.button_SendenAbbrechen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_SendenAbbrechen.Enabled = false;
            this.button_SendenAbbrechen.Image = global::PlotterCom.Properties.Resources.exit;
            this.button_SendenAbbrechen.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_SendenAbbrechen.Location = new System.Drawing.Point(3, 58);
            this.button_SendenAbbrechen.Name = "button_SendenAbbrechen";
            this.button_SendenAbbrechen.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_SendenAbbrechen.Size = new System.Drawing.Size(127, 49);
            this.button_SendenAbbrechen.TabIndex = 1;
            this.button_SendenAbbrechen.Text = "Abbrechen";
            this.button_SendenAbbrechen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_SendenAbbrechen.UseVisualStyleBackColor = true;
            this.button_SendenAbbrechen.Click += new System.EventHandler(this.button_SendenAbbrechen_Click);
            // 
            // button_DirektSenden
            // 
            this.button_DirektSenden.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_DirektSenden.Image = global::PlotterCom.Properties.Resources.next_sail;
            this.button_DirektSenden.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_DirektSenden.Location = new System.Drawing.Point(196, 3);
            this.button_DirektSenden.Name = "button_DirektSenden";
            this.button_DirektSenden.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_DirektSenden.Size = new System.Drawing.Size(128, 49);
            this.button_DirektSenden.TabIndex = 3;
            this.button_DirektSenden.Text = "Datei direkt\r\nsenden";
            this.button_DirektSenden.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_DirektSenden.UseVisualStyleBackColor = true;
            this.button_DirektSenden.Click += new System.EventHandler(this.button_DirektSenden_Click);
            // 
            // button_UmschrRechteck
            // 
            this.button_UmschrRechteck.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_UmschrRechteck.Image = global::PlotterCom.Properties.Resources.frameprint;
            this.button_UmschrRechteck.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_UmschrRechteck.Location = new System.Drawing.Point(196, 58);
            this.button_UmschrRechteck.Name = "button_UmschrRechteck";
            this.button_UmschrRechteck.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.button_UmschrRechteck.Size = new System.Drawing.Size(128, 49);
            this.button_UmschrRechteck.TabIndex = 4;
            this.button_UmschrRechteck.Text = "Umschreib.\r\nRechteck";
            this.button_UmschrRechteck.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_UmschrRechteck.UseVisualStyleBackColor = true;
            this.button_UmschrRechteck.Click += new System.EventHandler(this.button_UmschrRechteck_Click);
            // 
            // button_Netzwerk
            // 
            this.button_Netzwerk.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_Netzwerk.Image = global::PlotterCom.Properties.Resources.connect_to_network;
            this.button_Netzwerk.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.button_Netzwerk.Location = new System.Drawing.Point(3, 133);
            this.button_Netzwerk.Name = "button_Netzwerk";
            this.button_Netzwerk.Size = new System.Drawing.Size(127, 49);
            this.button_Netzwerk.TabIndex = 6;
            this.button_Netzwerk.Text = "Netzwerk test";
            this.button_Netzwerk.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Netzwerk.UseVisualStyleBackColor = true;
            this.button_Netzwerk.Click += new System.EventHandler(this.button_Netzwerk_Click);
            // 
            // Form_Hauptfenster
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(677, 473);
            this.Controls.Add(this.statusStrip_unten);
            this.Controls.Add(this.tableLayoutPanel_Hauptlayout);
            this.Controls.Add(this.menuStrip_HauptMenü);
            this.MainMenuStrip = this.menuStrip_HauptMenü;
            this.MinimumSize = new System.Drawing.Size(685, 500);
            this.Name = "Form_Hauptfenster";
            this.Text = "PlotterCom";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form_Hauptfenster_FormClosing);
            this.Load += new System.EventHandler(this.Form_Hauptfenster_Load);
            this.menuStrip_HauptMenü.ResumeLayout(false);
            this.menuStrip_HauptMenü.PerformLayout();
            this.tableLayoutPanel_Hauptlayout.ResumeLayout(false);
            this.groupBox_Öffnen.ResumeLayout(false);
            this.tableLayoutPanel_Öffnen.ResumeLayout(false);
            this.groupBox_Speichern.ResumeLayout(false);
            this.tableLayoutPanel_Speichern.ResumeLayout(false);
            this.groupBox_Optimieren.ResumeLayout(false);
            this.tableLayoutPanel_Optimieren.ResumeLayout(false);
            this.groupBox_Senden.ResumeLayout(false);
            this.tableLayoutPanel_Senden.ResumeLayout(false);
            this.tableLayoutPanel_Senden.PerformLayout();
            this.statusStrip_unten.ResumeLayout(false);
            this.statusStrip_unten.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip_HauptMenü;
        private System.Windows.Forms.ToolStripMenuItem dateiToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem beendenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem infoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem überPlotterComToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem einstellungenToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem_Strich1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_Hauptlayout;
        private System.Windows.Forms.StatusStrip statusStrip_unten;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar_Unten;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.GroupBox groupBox_Öffnen;
        private System.Windows.Forms.GroupBox groupBox_Speichern;
        private System.Windows.Forms.GroupBox groupBox_Optimieren;
        private System.Windows.Forms.GroupBox groupBox_Senden;
        private System.Windows.Forms.Label label_DateiGeladen;
        private System.Windows.Forms.Button button_Öffnen;
        private System.Windows.Forms.Button button_SpeichernAls;
        private System.Windows.Forms.Button button_Analysieren;
        private System.Windows.Forms.Button button_Optimieren;
        private System.Windows.Forms.Button button_Senden;
        private System.Windows.Forms.ProgressBar progressBar_Senden;
        private System.Windows.Forms.Label label_Senden;
        private System.Windows.Forms.Button button_Viewer;
        private System.Windows.Forms.Button button_Schließen;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_Öffnen;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_Optimieren;
        private System.Windows.Forms.Button button_SendenAbbrechen;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_Speichern;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_Senden;
        private System.Windows.Forms.OpenFileDialog openFileDialog_HPGL;
        private System.Windows.Forms.Button button_Abbruch;
        private System.Windows.Forms.SaveFileDialog saveFileDialog_HPGL;
        private System.Windows.Forms.Button button_DirektSenden;
        private System.Windows.Forms.Button button_UmschrRechteck;
        private System.Windows.Forms.Label label_ETA;
        private System.Windows.Forms.Button button_OptimierenAbbrechen;
        private System.Windows.Forms.Button button_Skalieren;
        private System.Windows.Forms.Button button_Verschieben;
        private System.Windows.Forms.Button button_Speichern;
        private System.Windows.Forms.Button button_Dubletten;
        private System.Windows.Forms.Button button_Verbinden;
        private System.Windows.Forms.Button button_Netzwerk;
    }
}

