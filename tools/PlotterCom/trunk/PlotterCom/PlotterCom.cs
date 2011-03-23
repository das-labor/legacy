using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;


 /*  PlotterCom - Tools for HPGL and plotters.
  *  PlotterCom.cs - Main Programm.
  * 
  *  Copyright (C) 2011  André Matuschek
  *  
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  * 
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  * 
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  * 
  */


namespace PlotterCom {
    public partial class Form_Hauptfenster : Form {

        // TODO:
        // - HPGL-Befehle wie:
        //   - Label
        //   - Circle
        //   - Komplette Liste.
        // - DateiName in Liste.

        private System.IO.Ports.SerialPort _Port = null;
        private PlotterCom.HPGL _HPGL = null;
        private System.Net.Sockets.UdpClient udpClient = null;

        private bool _SendenAbbrechen = false;

        public Form_Hauptfenster() {
            PlotterCom.StaticLogger.LogFileName = "PlotterCom_{0:d}.log";
            PlotterCom.StaticLogger.LogFileFolder = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            PlotterCom.StaticLogger.Initialisiere();
            PlotterCom.StaticLogger.Log("Initialisieren GUI-Komponenten.", 8);

            InitializeComponent();

            PlotterCom.StaticLogger.Log("Konstruktor beendet.", 8);
        }

        private void Form_Hauptfenster_Load(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Load-Event feuert!", 8);
            ButtonsAnAus();
            // TODO: Config-Manager alle Einst. aus Config laden. Config suchen!

            ConfigFileSuchenUndLaden();

            Status("Bereit...");
        }



        private void Form_Hauptfenster_FormClosing(object sender, FormClosingEventArgs e) {
            if (PlotterCom.ConfigManager.Config.BeimBeendenBestätigen) {
                DialogResult Antwort = MessageBox.Show("Programm beeden?", "Beenden", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                if (Antwort == DialogResult.Yes) {
                    if (_Port != null && _Port.IsOpen) {
                        _Port.Close();
                    }
                } else {
                    e.Cancel = true;
                }
            } else {
                if (_Port != null && _Port.IsOpen) {
                    _Port.Close();
                }
            }
        }


        private void beendenToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void überPlotterComToolStripMenuItem_Click(object sender, EventArgs e) {
            MessageBox.Show(
                "PlotterCom  Copyright (C) 2011  André Matuschek\n" + 
                "This program comes with ABSOLUTELY NO WARRANTY.\n" + 
                "This is free software, and you are welcome to redistribute it " +
                "under certain conditions; see http://www.gnu.org/licenses/ for details.",
                "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void ConfigFileSuchenUndLaden() {
            PlotterCom.StaticLogger.Log("Suche ConfigFile und lade es!", 6);
            
            string ConfigFile = System.IO.Path.Combine(Environment.CurrentDirectory, "PlotterCom.xml");
            
            if (System.IO.File.Exists(ConfigFile)) {
                PlotterCom.StaticLogger.Log("Habe das ConfigFile gefunden: " + ConfigFile, 6);
                PlotterCom.ConfigManager.Config.EinstellungenLadenAusDatei(ConfigFile);
                return;
            }

            ConfigFile = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "PlotterCom.xml");

            if (System.IO.File.Exists(ConfigFile)) {
                PlotterCom.StaticLogger.Log("Habe das ConfigFile gefunden: " + ConfigFile, 6);
                PlotterCom.ConfigManager.Config.EinstellungenLadenAusDatei(ConfigFile);
                return;
            }

            PlotterCom.StaticLogger.Log("Konnte keine Config-Datei finden. Lade defaults!", 4);

        }

        private void ausDateiLadenToolStripMenuItem_Click(object sender, EventArgs e) {
            HPGL hpglFile = new HPGL();
            hpglFile.LeseHPGLFile("C:\\Users\\a.matuschek\\Documents\\Desktop\\GL-C-F.plt");
            hpglFile.SchreibeHPGLInDatei("C:\\Users\\a.matuschek\\Documents\\Desktop\\GL-C-F_2.plt");
        }

        private void einstellungenToolStripMenuItem_Click(object sender, EventArgs e) {
            PlotterCom.Einstellungen frmEinstellungen = new Einstellungen();
            frmEinstellungen.ShowDialog();
        }

        private void Status(string StatusText) {

            if (!String.IsNullOrEmpty(StatusText)) {
                toolStripStatusLabel.Text = DateTime.Now.ToLongTimeString() + " - " + StatusText;
            } else {
                PlotterCom.StaticLogger.Log("Kann Status nicht anzeigen. StatusText ist leer!", 5);
            }

        }

        private void SendeStreamAnPlotter(System.IO.StreamReader datenquelle) {
            SendeStreamAnPlotter(datenquelle, datenquelle.BaseStream.Length);
        }

        private void SendeStreamAnPlotter(System.IO.TextReader datenquelle, long länge) {
            DateTime StartZeitpunkt = DateTime.Now;
            TimeSpan SendeZeit = TimeSpan.Zero;
            DateTime LetztesETAUpdate = DateTime.Now;
            PlotterCom.StaticLogger.Log("Sende Stream an Plotter!", 6);

            long geschriebeneBytes = 0;
            int geleseneBytes = 0;
            double Fortschritt = 0;
            char[] buffer = null;
            int blockGröße = 8;

            if (datenquelle == null) {
                PlotterCom.StaticLogger.Log("Kann keine Daten senden. Datenquelle ist null!", 4);
                return;
            }

            _SendenAbbrechen = false;

            button_Senden.Enabled = false;
            button_SpeichernAls.Enabled = false;
            button_UmschrRechteck.Enabled = false;
            button_SendenAbbrechen.Enabled = true;
            button_DirektSenden.Enabled = false;
            button_Abbruch.Enabled = false;
            button_Optimieren.Enabled = false;
            button_Analysieren.Enabled = false;
            button_Öffnen.Enabled = false;
            button_Schließen.Enabled = false;
            button_Viewer.Enabled = false;
            button_Skalieren.Enabled = false;
            button_Speichern.Enabled = false;
            button_Verschieben.Enabled = false;
            button_UmschrRechteck.Enabled = false;
            button_Verbinden.Enabled = false;
            button_Dubletten.Enabled = false;


            if (_Port == null || !_Port.IsOpen) {

                PlotterCom.StaticLogger.Log("Der Port exisitiert noch nicht. Erestelle neuen SerialPort!", 6);

                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();

                _Port = new System.IO.Ports.SerialPort();
                _Port.PortName = PlotterCom.ConfigManager.Config.ComPort;
                _Port.Parity = (System.IO.Ports.Parity) Enum.Parse(typeof(System.IO.Ports.Parity), PlotterCom.ConfigManager.Config.ComParity);
                _Port.BaudRate = PlotterCom.ConfigManager.Config.ComBaudRate;
                _Port.DataBits = PlotterCom.ConfigManager.Config.ComDataBits;
                _Port.Handshake = (System.IO.Ports.Handshake) (Enum.Parse(typeof(System.IO.Ports.Handshake), PlotterCom.ConfigManager.Config.ComHandshake));
                _Port.StopBits = (System.IO.Ports.StopBits) (Enum.Parse(typeof(System.IO.Ports.StopBits), PlotterCom.ConfigManager.Config.ComStopBits));


                if (Array.IndexOf(System.IO.Ports.SerialPort.GetPortNames(), _Port.PortName) == -1) {
                    MessageBox.Show("Der in den Einstellungen hinterlegte Port \"" + _Port.PortName +
                        "\" ist nicht verfügbar. Bitte die Port-Einstellungen anpassen!",
                        "Fehler!", MessageBoxButtons.OK, MessageBoxIcon.Warning);

                    button_Senden.Enabled = true;
                    button_SpeichernAls.Enabled = true;
                    button_UmschrRechteck.Enabled = true;
                    button_SendenAbbrechen.Enabled = true;
                    button_DirektSenden.Enabled = true;
                    button_Abbruch.Enabled = true;
                    button_Optimieren.Enabled = true;
                    button_Analysieren.Enabled = true;
                    button_Öffnen.Enabled = true;
                    button_Schließen.Enabled = true;
                    button_Viewer.Enabled = true;

                    ButtonsAnAus();

                    label_Senden.Text = "Bereit...";
                    progressBar_Senden.Value = 0;

                    button_SendenAbbrechen.Enabled = false;

                    return;

                }

                try {
                    PlotterCom.StaticLogger.Log("Versuche den Port zu öffnen!", 6);
                    _Port.Open();
                } catch (Exception ex) {
                    PlotterCom.StaticLogger.Log("Konnte den Port nicht öffnen!", 4);
                    PlotterCom.StaticLogger.Log("Fehlermeldung: " + ex.Message, 4);
                    return;
                }

                PlotterCom.StaticLogger.Log("Port wurde neu geöffnet. Warte 100ms auf Bereitschaft!", 7);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();

            } else {
                _Port.DiscardOutBuffer();
            }

            buffer = new char[blockGröße];

            while (!_SendenAbbrechen) {

                System.Windows.Forms.Application.DoEvents();

                while (!_Port.CtsHolding) {
                    toolStripStatusLabel.Text = "Warte...";

                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();

                    if (_SendenAbbrechen) {
                        break;
                    }

                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();

                    if (_SendenAbbrechen) {
                        break;
                    }

                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();
                    System.Threading.Thread.Sleep(5);
                    System.Windows.Forms.Application.DoEvents();

                    if (_SendenAbbrechen) {
                        break;
                    }
                } // Warten.

                toolStripStatusLabel.Text = "Sende...";

                // Lese einen Datenblock.
                geleseneBytes = datenquelle.Read(buffer, 0, blockGröße);

                if (geleseneBytes == 0) {
                    break;
                }

                // Sende einen Datenblock
                try {
                    _Port.Write(buffer, 0, blockGröße);
                } catch (Exception ex) {
                    PlotterCom.StaticLogger.Log("Konnte nicht senden!", 4);
                    PlotterCom.StaticLogger.Log("Fehlermeldung: " + ex.Message, 4);
                }

                geschriebeneBytes += blockGröße;

                if ((länge > 0) && (länge > geleseneBytes)) {

                    if (geschriebeneBytes < länge) {
                        Fortschritt = ((double)geschriebeneBytes / (double)(länge + 4D));
                    } else {
                        Fortschritt = 1;
                    }

                    SendeZeit = DateTime.Now - StartZeitpunkt;

                    progressBar_Senden.Value = (int) (Fortschritt * 100D);
                    label_Senden.Text = "Fortschritt: " + (Fortschritt * 100D).ToString("00.0") +"%";

                    if ((DateTime.Now - LetztesETAUpdate) > TimeSpan.FromSeconds(1))
                    {
                        LetztesETAUpdate = DateTime.Now;

                        if (SendeZeit > TimeSpan.FromSeconds(4) && Fortschritt > 0.001D)
                        {
                            double Sekunden = SendeZeit.TotalSeconds;
                            TimeSpan RestZeit;
                            Sekunden = Sekunden * (1D / Fortschritt);
                            RestZeit = TimeSpan.FromSeconds(Math.Abs(Sekunden - SendeZeit.TotalSeconds));
                            label_ETA.Text = "ETA: " + RestZeit.ToString("hh\\:mm\\:ss") + " ";
                        }
                        else
                        {
                            label_ETA.Text = "ETA: berechne...  ";
                        }
                    }

                }

                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(12); // 10 reicht auch. 12 als Sicherheit...
                System.Windows.Forms.Application.DoEvents();

            } // while !EndOfStream

            button_Senden.Enabled = true;
            button_SpeichernAls.Enabled = true;
            button_UmschrRechteck.Enabled = true;
            button_SendenAbbrechen.Enabled = true;
            button_DirektSenden.Enabled = true;
            button_Abbruch.Enabled = true;
            button_Optimieren.Enabled = true;
            button_Analysieren.Enabled = true;
            button_Öffnen.Enabled = true;
            button_Schließen.Enabled = true;
            button_Viewer.Enabled = true;

            ButtonsAnAus();

            label_Senden.Text = "Bereit...";
            progressBar_Senden.Value = 0;

            button_SendenAbbrechen.Enabled = false;

        }

        private void button_Senden_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Der Benutzer hat Senden geklickt!", 4);
            string HPGLCode = null;
            System.IO.StringReader stringStreamer = null;
            

            if (_HPGL == null) {
                PlotterCom.StaticLogger.Log("Kein HPGL-Objekt vorhanden. Ende.", 4);
                return;
            }

            HPGLCode = _HPGL.ToString();
            stringStreamer = new System.IO.StringReader(HPGLCode);

            SendeStreamAnNetzwetrk(stringStreamer, HPGLCode.Length);
            //SendeStreamAnPlotter(stringStreamer, HPGLCode.Length);
            
            

        }


        private void button_Öffnen_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Benutzer hat 'Öffnen' geklickt!", 8);
            DialogResult Antwort;

            Antwort = openFileDialog_HPGL.ShowDialog();

            if (Antwort != DialogResult.OK) {
                PlotterCom.StaticLogger.Log("Benutzer hat nicht 'OK' geklickt, Abbruch!", 8);
                return;
            }

            if (System.IO.File.Exists(openFileDialog_HPGL.FileName)) {
                toolStripProgressBar_Unten.Visible = true;
                button_Abbruch.Enabled = true;
                button_Öffnen.Enabled = false;

                button_Senden.Image = global::PlotterCom.Properties.Resources.agt_print;

                _HPGL = new HPGL();
                _HPGL.LeseHPGLFile(openFileDialog_HPGL.FileName, toolStripProgressBar_Unten, toolStripStatusLabel);

                this.Text = "PlotterCom  - " + System.IO.Path.GetFileName(openFileDialog_HPGL.FileName);

                ButtonsAnAus();

                toolStripProgressBar_Unten.Visible = false;

                if (_HPGL == null) {
                    Status("Laden wurde abgebrochen!");
                    PlotterCom.StaticLogger.Log("Datei Laden wurde abgebrochen!", 6);
                } else {
                    Status("Datei wurde gelesen! " + _HPGL.AnzahlLinien + " Linien.");
                    PlotterCom.StaticLogger.Log("Datei wurde gelesen! " + _HPGL.AnzahlLinien + " Linien.", 6);
                }

            } else {
                PlotterCom.StaticLogger.Log("Benutzer hat eine Datei ausgewählt, die nicht exisitiert!", 4);
                MessageBox.Show(
                    "Die Datei kann nicht geöffnet werden!", "Fehler beim Lesen!",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button_Schließen_Click(object sender, EventArgs e) {
            _HPGL = null;
            button_Senden.Image = global::PlotterCom.Properties.Resources.agt_print;
            ButtonsAnAus();
        }

        private void button_Abrruch_Click(object sender, EventArgs e) {
            if (_HPGL != null) {
                _HPGL.LadenAbbrechen = true;
                _HPGL = null;
            }
            
            ButtonsAnAus();
            button_Senden.Image = global::PlotterCom.Properties.Resources.agt_print;
        }

        private void button_SpeichernAls_Click(object sender, EventArgs e) {
            DialogResult Antwort;

            Antwort = saveFileDialog_HPGL.ShowDialog();
            if (Antwort == System.Windows.Forms.DialogResult.OK) {
                PlotterCom.StaticLogger.Log("Speichere HPGL unter: " + saveFileDialog_HPGL.FileName + ".", 6);
                _HPGL.SchreibeHPGLInDatei(saveFileDialog_HPGL.FileName);
            } else {
                PlotterCom.StaticLogger.Log("Benutzer hat das Speichern abgebrochen!", 4);
            }
        }

        private void button_Analysieren_Click(object sender, EventArgs e) {
            TimeSpan tsPlotDauer;
            PlotterCom.StaticLogger.Log("Benutzer hat Analyse angeklickt!", 6);

            if (_HPGL== null) {
                return;
            }

            _HPGL.Analyse();
            tsPlotDauer = TimeSpan.FromMilliseconds(_HPGL.PlotDauer);

            MessageBox.Show("Simulierte Plotdauer: " + tsPlotDauer.ToString("hh\\:mm\\:ss"),
                "Analyse", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void button_DirektSenden_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Benutzer hat 'Direkt senden' geklickt. Frage nach Datei.", 8);
            System.IO.StreamReader streamReaderHPGL = null;

            DialogResult Antwort = openFileDialog_HPGL.ShowDialog();

            if (Antwort != DialogResult.OK) {
                PlotterCom.StaticLogger.Log("Benutzer hat nicht OK geklickt. Breche ab!", 5);
                return;
            }

            if (System.IO.File.Exists(openFileDialog_HPGL.FileName)) {
                streamReaderHPGL = new System.IO.StreamReader(openFileDialog_HPGL.FileName);
                SendeStreamAnPlotter(streamReaderHPGL, streamReaderHPGL.BaseStream.Length);
            } else {
                PlotterCom.StaticLogger.Log("Die Datei existiert nicht!", 4);
            }

        }

        private void button_SendenAbbrechen_Click(object sender, EventArgs e) {
            _SendenAbbrechen = true;
            label_Senden.Text = "Bereit...";
        }


        private void button_Optimieren_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Benutzer hat Optimieren geklickt!", 6);
            // TODO: Optimieren abbrechen.
            // Datei schließen darf nicht möglich sein während der optim.
            if (_HPGL != null)
            {
                this.toolStripProgressBar_Unten.Visible = true;

                button_Optimieren.Enabled = false;
                button_Abbruch.Focus();
                

                button_OptimierenAbbrechen.Enabled = true;
                button_Viewer.Enabled = false;
                button_SpeichernAls.Enabled = false;
                button_Abbruch.Enabled = false;
                button_Schließen.Enabled = false;
                button_Senden.Enabled = false;
                button_DirektSenden.Enabled = false;
                button_Analysieren.Enabled = false;
                button_Verschieben.Enabled = false;
                button_Speichern.Enabled = false;
                button_Skalieren.Enabled = false;
                button_UmschrRechteck.Enabled = false;
                button_Verbinden.Enabled = false;
                button_Dubletten.Enabled = false;

                _HPGL.Optimiere(this.toolStripProgressBar_Unten, this.toolStripStatusLabel);

                this.toolStripProgressBar_Unten.Visible = false;
                button_Senden.Image = global::PlotterCom.Properties.Resources.filequickprint;
                toolStripStatusLabel.Text = "Optimierung beendet!";
                PlotterCom.StaticLogger.Log("Optimierung beendet!", 6);
            }
            ButtonsAnAus();
        }

        private void ButtonsAnAus() {
            PlotterCom.StaticLogger.Log("Untersuche, welche Buttons enabled werden und welche nicht!", 8);

            if (_HPGL == null) {
                button_Öffnen.Enabled = true;
                button_Abbruch.Enabled = false;
                button_Schließen.Enabled = false;
                button_Viewer.Enabled = false;
                button_Analysieren.Enabled = false;
                button_Optimieren.Enabled = false;
                button_OptimierenAbbrechen.Enabled = false;
                button_SpeichernAls.Enabled = false;
                button_Speichern.Enabled = false;
                button_Senden.Enabled = false;
                button_UmschrRechteck.Enabled = false;
                button_DirektSenden.Enabled = true;
                button_Verbinden.Enabled = false;
                button_Skalieren.Enabled = false;
                button_Verschieben.Enabled = false;
                button_Dubletten.Enabled = false;

                label_DateiGeladen.Image = global::PlotterCom.Properties.Resources.button_cance;
                this.Text = "PlotterCom";

            } else {
                button_Öffnen.Enabled = false;
                button_Abbruch.Enabled = false;
                button_Schließen.Enabled = true;
                button_Viewer.Enabled = true;
                button_Analysieren.Enabled = true;
                button_Optimieren.Enabled = true;
                button_OptimierenAbbrechen.Enabled = false;
                button_SpeichernAls.Enabled = true;
                button_Speichern.Enabled = true;
                button_Senden.Enabled = true;
                button_UmschrRechteck.Enabled = true;
                button_DirektSenden.Enabled = false;
                button_Verbinden.Enabled = true;
                button_Skalieren.Enabled = true;
                button_Verschieben.Enabled = true;
                button_Dubletten.Enabled = true;

                label_DateiGeladen.Image = global::PlotterCom.Properties.Resources.button_ok;

            }
        }

        private void button_Viewer_Click(object sender, EventArgs e) {
            ViewerForm vt = new ViewerForm();
            vt.Code = _HPGL;
            vt.ShowDialog();
        }

        private void button_UmschrRechteck_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Zeichne umschreibendes Rechteck!", 6);
            string UmschrRechteckCode = string.Empty;
            System.IO.StringReader HPGLCodeReader = null;

            if (_Port == null || !_Port.IsOpen) {

                PlotterCom.StaticLogger.Log("Der Port exisitiert noch nicht. Erestelle neuen SerialPort!", 6);

                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();

                _Port = new System.IO.Ports.SerialPort();
                _Port.PortName = PlotterCom.ConfigManager.Config.ComPort;
                _Port.Parity = (System.IO.Ports.Parity)Enum.Parse(typeof(System.IO.Ports.Parity), PlotterCom.ConfigManager.Config.ComParity);
                _Port.BaudRate = PlotterCom.ConfigManager.Config.ComBaudRate;
                _Port.DataBits = PlotterCom.ConfigManager.Config.ComDataBits;
                _Port.Handshake = (System.IO.Ports.Handshake)(Enum.Parse(typeof(System.IO.Ports.Handshake), PlotterCom.ConfigManager.Config.ComHandshake));
                _Port.StopBits = (System.IO.Ports.StopBits)(Enum.Parse(typeof(System.IO.Ports.StopBits), PlotterCom.ConfigManager.Config.ComStopBits));

                try {
                    _Port.Open();
                } catch (Exception ex) {
                    PlotterCom.StaticLogger.Log("Fehler beim Öffnen des Ports!", 4);
                    PlotterCom.StaticLogger.Log("Meldung: " + ex.Message, 4);
                    MessageBox.Show("Konnte den Port nicht öffnen, bitte die Einstellungen prüfen!",
                        "Fehler!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                PlotterCom.StaticLogger.Log("Port wurde neu geöffnet. Warte 100ms auf Bereitschaft!", 7);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();

            } else {
                _Port.DiscardOutBuffer();
            }

            System.Threading.Thread.Sleep(50);

            UmschrRechteckCode = _HPGL.UmschreibendesRechteckPlotten();
            HPGLCodeReader = new System.IO.StringReader(UmschrRechteckCode);

            SendeStreamAnPlotter(HPGLCodeReader, UmschrRechteckCode.Length);
 

            //_Port.Close();
            //_Port = null;
        }

        private void button_OptimierenAbbrechen_Click(object sender, EventArgs e) {
            button_Senden.Image = global::PlotterCom.Properties.Resources.agt_print;
            if (_HPGL != null) {
                _HPGL.OptimierenAbbrechen = true;
            }
        }

        private void button_Skalieren_Click(object sender, EventArgs e) {
            DialogResult Antwort;
            Skalierung frmSkalierung = new Skalierung();

            frmSkalierung.HPGLCode = _HPGL;

            Antwort = frmSkalierung.ShowDialog();

            if (Antwort == System.Windows.Forms.DialogResult.OK) {
                MessageBox.Show("Skalierung wurde angewendet. Verwenden Sie den Viewer zum Anzeigen des Ergebnisses!",
                    "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            } else {
                
            }

        }



        private void button_Speichern_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Benutzer hat 'Speichern' geklickt!", 6);

            if (String.IsNullOrEmpty(openFileDialog_HPGL.FileName)) {
                PlotterCom.StaticLogger.Log("Kein Dateiname vorhanden!", 4);
                button_SpeichernAls_Click(this, EventArgs.Empty);
            } else {
                _HPGL.SchreibeHPGLInDatei(openFileDialog_HPGL.FileName);
            }
        }

        private void button_Verschieben_Click(object sender, EventArgs e) {
            DialogResult Antwort;
            Verschieben frmVerschieben = new Verschieben();
            frmVerschieben.HPGLCode = _HPGL;
            Antwort = frmVerschieben.ShowDialog();

            if (Antwort == System.Windows.Forms.DialogResult.OK) {
                MessageBox.Show("Die Verschiebung wurde ausgeführt. Bitte nutzen Sie den Viewer zum Anzeigen des Ergebnisses!",
                    "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void button_Dubletten_Click(object sender, EventArgs e) {
            int GefundeneDubletten = 0;

            if (_HPGL != null) {

                button_Dubletten.Enabled = false;
                button_Abbruch.Focus();

                button_Optimieren.Enabled = false;
                button_OptimierenAbbrechen.Enabled = true;
                button_Viewer.Enabled = false;
                button_SpeichernAls.Enabled = false;
                button_Abbruch.Enabled = false;
                button_Schließen.Enabled = false;
                button_Senden.Enabled = false;
                button_DirektSenden.Enabled = false;
                button_Analysieren.Enabled = false;
                button_Verschieben.Enabled = false;
                button_Speichern.Enabled = false;
                button_Skalieren.Enabled = false;
                button_UmschrRechteck.Enabled = false;
                button_Verbinden.Enabled = false;
                button_Dubletten.Enabled = false;

                toolStripProgressBar_Unten.Visible = true;

                GefundeneDubletten = _HPGL.EntferneDubletten(this.toolStripProgressBar_Unten, this.toolStripStatusLabel);

                toolStripProgressBar_Unten.Visible = false;
                toolStripStatusLabel.Text = "Bereit...";

                if (GefundeneDubletten > 0) {
                    MessageBox.Show("Es wurden " + GefundeneDubletten + " unnötige Linien entfernt!",
                        "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                } else {
                    MessageBox.Show("Es wurden keine unnötige Linien gefunden!",
                        "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

            ButtonsAnAus();
        }

        private void button_Verbinden_Click(object sender, EventArgs e) {
            int AnzahlHilfsLinien;
            PlotterCom.StaticLogger.Log("Der Benutzer hat 'Linien verbinden' geklickt!", 6);


            if (_HPGL != null) {
                AnzahlHilfsLinien = _HPGL.LinienVerbinden();

                if (AnzahlHilfsLinien > 0) {
                    MessageBox.Show("Es wurden " + AnzahlHilfsLinien + " Hilfslinien generiert und eingefügt!",
                        "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                } else {
                    MessageBox.Show("Es wurden keine Hilfslinien generiert!",
                        "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private void button_Netzwerk_Click(object sender, EventArgs e) {
            TCPTest();
        }

        private void SendeStreamAnNetzwetrk(System.IO.TextReader datenquelle, long länge) {
            DateTime StartZeitpunkt = DateTime.Now;
            TimeSpan SendeZeit = TimeSpan.Zero;
            DateTime LetztesETAUpdate = DateTime.Now;
            PlotterCom.StaticLogger.Log("Sende Stream an Plotter!", 6);

            long geschriebeneBytes = 0;
            int geleseneBytes = 0;
            double Fortschritt = 0;
            char[] buffer = null;
            int blockGröße = 16;

            if (datenquelle == null) {
                PlotterCom.StaticLogger.Log("Kann keine Daten senden. Datenquelle ist null!", 4);
                return;
            }

            _SendenAbbrechen = false;

            button_Senden.Enabled = false;
            button_SpeichernAls.Enabled = false;
            button_UmschrRechteck.Enabled = false;
            button_SendenAbbrechen.Enabled = true;
            button_DirektSenden.Enabled = false;
            button_Abbruch.Enabled = false;
            button_Optimieren.Enabled = false;
            button_Analysieren.Enabled = false;
            button_Öffnen.Enabled = false;
            button_Schließen.Enabled = false;
            button_Viewer.Enabled = false;
            button_Skalieren.Enabled = false;
            button_Speichern.Enabled = false;
            button_Verschieben.Enabled = false;
            button_UmschrRechteck.Enabled = false;
            button_Verbinden.Enabled = false;
            button_Dubletten.Enabled = false;



                try {
                    PlotterCom.StaticLogger.Log("Versuche den Port zu öffnen!", 6);
                    // tcpopen
                } catch (Exception ex) {
                    PlotterCom.StaticLogger.Log("Konnte den Port nicht öffnen!", 4);
                    PlotterCom.StaticLogger.Log("Fehlermeldung: " + ex.Message, 4);
                    return;
                }

                PlotterCom.StaticLogger.Log("Port wurde neu geöffnet. Warte 100ms auf Bereitschaft!", 7);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(50);
                System.Windows.Forms.Application.DoEvents();

                IPAddress ipAddress = IPAddress.Parse("10.0.1.134");
                IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, 8888);
                System.Net.Sockets.NetworkStream nwStream;
            

                System.Net.Sockets.TcpClient tcpClient = new System.Net.Sockets.TcpClient();
                tcpClient.NoDelay = true;
                tcpClient.SendBufferSize = 12;

                tcpClient.Connect(ipEndPoint);

                nwStream = tcpClient.GetStream();
 
            buffer = new char[blockGröße];

            while (!_SendenAbbrechen) {
                
                System.Windows.Forms.Application.DoEvents();




                toolStripStatusLabel.Text = "Sende...";

                // Lese einen Datenblock.
                geleseneBytes = datenquelle.Read(buffer, 0, blockGröße);

                if (geleseneBytes == 0) {
                    break;
                }

                // Sende einen Datenblock
                try {
                    string str = new String(buffer);
                    
                    nwStream.Write(Encoding.ASCII.GetBytes(buffer, 0, blockGröße), 0, blockGröße);
                    nwStream.Flush();
                } catch (Exception ex) {
                    PlotterCom.StaticLogger.Log("Konnte nicht senden!", 4);
                    PlotterCom.StaticLogger.Log("Fehlermeldung: " + ex.Message, 4);
                }

                geschriebeneBytes += blockGröße;

                if ((länge > 0) && (länge > geleseneBytes)) {

                    if (geschriebeneBytes < länge) {
                        Fortschritt = ((double)geschriebeneBytes / (double)(länge + 4D));
                    } else {
                        Fortschritt = 1;
                    }

                    SendeZeit = DateTime.Now - StartZeitpunkt;

                    progressBar_Senden.Value = (int)(Fortschritt * 100D);
                    label_Senden.Text = "Fortschritt: " + (Fortschritt * 100D).ToString("00.0") + "%";

                    if ((DateTime.Now - LetztesETAUpdate) > TimeSpan.FromSeconds(1)) {
                        LetztesETAUpdate = DateTime.Now;

                        if (SendeZeit > TimeSpan.FromSeconds(4) && Fortschritt > 0.001D) {
                            double Sekunden = SendeZeit.TotalSeconds;
                            TimeSpan RestZeit;
                            Sekunden = Sekunden * (1D / Fortschritt);
                            RestZeit = TimeSpan.FromSeconds(Math.Abs(Sekunden - SendeZeit.TotalSeconds));
                            label_ETA.Text = "ETA: " + RestZeit.ToString("hh\\:mm\\:ss") + " ";
                        } else {
                            label_ETA.Text = "ETA: berechne...  ";
                        }
                    }

                }

                System.Windows.Forms.Application.DoEvents();
                System.Threading.Thread.Sleep(12); // 10 reicht auch. 12 als Sicherheit...
                System.Windows.Forms.Application.DoEvents();

            } // while !EndOfStream

            button_Senden.Enabled = true;
            button_SpeichernAls.Enabled = true;
            button_UmschrRechteck.Enabled = true;
            button_SendenAbbrechen.Enabled = true;
            button_DirektSenden.Enabled = true;
            button_Abbruch.Enabled = true;
            button_Optimieren.Enabled = true;
            button_Analysieren.Enabled = true;
            button_Öffnen.Enabled = true;
            button_Schließen.Enabled = true;
            button_Viewer.Enabled = true;

            ButtonsAnAus();

            label_Senden.Text = "Bereit...";
            progressBar_Senden.Value = 0;

            button_SendenAbbrechen.Enabled = false;

        }


        private void TCPTest() {
            IPAddress ipAddress = IPAddress.Parse("10.0.1.199");
            IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, 7970);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("IN;PU0,0;SP1;PA1000,1000;SP0;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;PA10000,10000;");
            System.Net.Sockets.NetworkStream nwStream;

            System.Net.Sockets.TcpClient tcpClient = new System.Net.Sockets.TcpClient();

            tcpClient.Connect(ipEndPoint);

            nwStream = tcpClient.GetStream();

            nwStream.Write(sendBytes, 0, 32);
            nwStream.Flush();
            nwStream.Write(sendBytes, 32, 32);
            nwStream.Flush();
            nwStream.Write(sendBytes, 64, 32);
            nwStream.Flush();

            nwStream.Close();
            tcpClient.Close();

            
        }

        private void UPDTest() {
            udpClient = new System.Net.Sockets.UdpClient();
            Byte[] sendBytes = Encoding.ASCII.GetBytes("IN;PU0,0;SP1;PA1000,1000;SP0;NR");
            IPAddress targetIP = IPAddress.Parse("10.0.1.27");
            IPEndPoint targetEP = new IPEndPoint(targetIP, 85);

            try {
                udpClient.Connect(targetEP);
                udpClient.Send(sendBytes, sendBytes.Length);
            } catch (Exception ex) {
                MessageBox.Show(ex.Message, "Fehler!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }





   }
}
