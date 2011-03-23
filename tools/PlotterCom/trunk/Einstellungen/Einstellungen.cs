using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Einstellungen.cs - Config-GUI for PlotterCom.
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
    public partial class Einstellungen : Form {

        private System.IO.Ports.SerialPort _serialPort = null;

        private bool _LogOrdnerGeändert = false; // Wenn sich der Log-Ordner ändert, beim speichern das
                                                 // Log-System neu initialisieren!

        public Einstellungen() {
            InitializeComponent();
        }

        private void button_ConfigFile_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Der Config-File-Auswahlbutton wurde geklickt!", 7);

            SaveFileDialog ConfigFileDialog = new SaveFileDialog();
            ConfigFileDialog.Filter = "XML Dateien (*.xml)|*.xml|All files (*.*)|*.*";
            ConfigFileDialog.CheckPathExists = true;
            ConfigFileDialog.FileName = PlotterCom.ConfigManager.Config.ConfigFile;

            if (ConfigFileDialog.ShowDialog() == DialogResult.OK) {
                PlotterCom.StaticLogger.Log("Benutzer hat OK geklickt. Setze Text ein!", 7);
                textBox_ConfigFile.Text = ConfigFileDialog.FileName;
            } else {
                PlotterCom.StaticLogger.Log("Benuter hat abbrechen geklickt!", 6);
            }
            
        }

        private void button_SpeichernBeenden_Click(object sender, EventArgs e) {
            AlleEinstellungenInConfigManager();
            AlleEinstellungenSpeichern();
            this.Close();
        }

        private void button_Abbruch_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void button_Speichern_Click(object sender, EventArgs e) {
            AlleEinstellungenInConfigManager();
            AlleEinstellungenSpeichern();
        }

        private void button_NeuLaden_Click(object sender, EventArgs e) {
            AlleEinstellungenAusConfigManager();
        }

        private void button_LogOrdner_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Auswahldialog für den LogOrdner wird angezeigt.", 6);

            FolderBrowserDialog LogOrdnerWahl = new FolderBrowserDialog();
            LogOrdnerWahl.SelectedPath = System.Environment.CurrentDirectory;

            if (LogOrdnerWahl.ShowDialog(this) == DialogResult.OK) {
                if (System.IO.Directory.Exists(LogOrdnerWahl.SelectedPath)) {
                    textBox_LogOrdner.Text = LogOrdnerWahl.SelectedPath;
                } else {
                    PlotterCom.StaticLogger.Log("Ordner nicht gefunden!", 5);
                    MessageBox.Show(
                        "Order nicht Gefunden!",
                        "Fehler",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                }
                _LogOrdnerGeändert = true;
            }
        }

        private void AlleEinstellungenSpeichern() {
            PlotterCom.StaticLogger.Log("Speichere alle Einstellungen ab.");

            if (!String.IsNullOrEmpty(textBox_ConfigFile.Text)) {
                PlotterCom.StaticLogger.Log("Setze ConfigFile im ConfigManager neu", 8);
                PlotterCom.ConfigManager.Config.ConfigFile = textBox_ConfigFile.Text;
            } else {
                PlotterCom.StaticLogger.Log("Das Textfeld 'ConfigFile' ist leer!", 6);
                MessageBox.Show("Bitte eine Datei zum speichern der Einstellungen angeben!",
                    "Fehler beim Speichern!",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }


            if (!PlotterCom.ConfigManager.Config.EinstellungenSpeichernInDatei(
                    PlotterCom.ConfigManager.Config.ConfigFile)) {

                PlotterCom.StaticLogger.Log("EinstellungenSpeichernInDatei hat false zurück gegeben. Informiere den Benutzer!", 4);

                MessageBox.Show("Die Einstellungen konnte nicht abgespeichert werden!",
                    "Fehler beim speichern!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        private void AlleEinstellungenLaden() {
            PlotterCom.StaticLogger.Log("Lade alle Einstellungen in den ConfigManager.", 6);

            if (!String.IsNullOrEmpty(textBox_ConfigFile.Text)) {

                PlotterCom.ConfigManager.Config.EinstellungenLadenAusDatei(textBox_ConfigFile.Text);
                AlleEinstellungenAusConfigManager();

            } else {
                PlotterCom.StaticLogger.Log("Das Textfeld 'ConfigFile' ist leer!", 6);
                MessageBox.Show("Bitte eine Datei zum Laden der Einstellungen angeben!",
                    "Fehler beim Laden!",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }

        }

        private void AlleEinstellungenInConfigManager() {
            PlotterCom.StaticLogger.Log("Schreibe alle Einstellungen in den ConfigManager!", 6);

            // --- TabPage: Allgemein ---
            PlotterCom.ConfigManager.Config.BeimBeendenBestätigen = radioButton_BeendenJa.Checked;

            if (textBox_LogOrdner.Text.Length > 0) {
                PlotterCom.ConfigManager.Config.LogOrdner = textBox_LogOrdner.Text;
            }

            if (textBox_ConfigFile.Text.Length > 0) {
                PlotterCom.ConfigManager.Config.ConfigFile = textBox_ConfigFile.Text;
            }

            PlotterCom.ConfigManager.Config.LogLevel = comboBox_LogLevel.SelectedIndex;

            // --- TabPage: ComPort ---
            PlotterCom.ConfigManager.Config.ComPort = comboBox_ComPort.SelectedItem.ToString();
            PlotterCom.ConfigManager.Config.ComBaudRate = Int32.Parse(comboBox_Baud.SelectedItem.ToString());

            PlotterCom.ConfigManager.Config.ComDataBits = Int32.Parse(comboBox_DataBits.SelectedItem.ToString());
            PlotterCom.ConfigManager.Config.ComHandshake = comboBox_HandShake.SelectedItem.ToString();
            PlotterCom.ConfigManager.Config.ComParity = comboBox_Parity.SelectedItem.ToString();
            PlotterCom.ConfigManager.Config.ComStopBits = comboBox_StopBits.SelectedItem.ToString();

            // --- TabPage: Plotter ---
            PlotterCom.ConfigManager.Config.PlotterName = textBoxModell.Text;
            PlotterCom.ConfigManager.Config.PlotterGröße = comboBox_Größe.SelectedItem.ToString();

            try {
                PlotterCom.ConfigManager.Config.PlotterMinX = System.Int32.Parse(textBox_P1X.Text);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte den Wert der Textbox P1-X nich interpretieren!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
            }
            try {
                PlotterCom.ConfigManager.Config.PlotterMinY = System.Int32.Parse(textBox_P1Y.Text);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte den Wert der Textbox P1-Y nich interpretieren!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
            }
            try {
                PlotterCom.ConfigManager.Config.PlotterMaxX = System.Int32.Parse(textBox_P2X.Text);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte den Wert der Textbox P2-X nich interpretieren!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
            }
            try {
                PlotterCom.ConfigManager.Config.PlotterMaxY = System.Int32.Parse(textBox_P2Y.Text);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte den Wert der Textbox P2-Y nich interpretieren!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
            }

            // --- TabPage: HPGL ---
            PlotterCom.ConfigManager.Config.HPGLUrsprung = checkBox_Ursprung.Checked;
            PlotterCom.ConfigManager.Config.HPGLWarnungBereich = checkBox_Warnung.Checked;
            PlotterCom.ConfigManager.Config.HPGLNurStift1 = checkBox_Stift1.Checked;
            PlotterCom.ConfigManager.Config.HPGLPlotGeschwindigkeit = trackBar_Geschwindigkeit.Value;

            if (radioButton_JoinNie.Checked) {
                PlotterCom.ConfigManager.Config.HPGLVerbinden = false;
                PlotterCom.ConfigManager.Config.HPGLVerbindenParallel = false;
            }

            if (radioButton_JoinImmer.Checked) {
                PlotterCom.ConfigManager.Config.HPGLVerbinden = true;
                PlotterCom.ConfigManager.Config.HPGLVerbindenParallel = false;
            }

            if (radioButton_JoinParallel.Checked) {
                PlotterCom.ConfigManager.Config.HPGLVerbinden = false;
                PlotterCom.ConfigManager.Config.HPGLVerbindenParallel = true;
            }

            try {
                PlotterCom.ConfigManager.Config.HPGLVerbindenSchritte = System.Int32.Parse(textBox_JoinTicks.Text);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte Wert für Schrittweite zum Verbinden nicht lesen.", 4);
                PlotterCom.StaticLogger.Log("Message: " + ex.Message, 4);
            }

            // --- TabPage: Viewer ---
            PlotterCom.ConfigManager.Config.ViewerHQ = radioButton_HQ.Checked;
            PlotterCom.ConfigManager.Config.ViewerSkalieren = checkBox_Skalieren.Checked;


            // --- TabPage: Neztwerk ---

            // Änderungen am Log-System sofort ausführen!
            PlotterCom.StaticLogger.LogLevel = PlotterCom.ConfigManager.Config.LogLevel;

            if (_LogOrdnerGeändert) {
                PlotterCom.StaticLogger.Log("Der LogOrdner wurde geändert. Öffne Logdatei neu!", 1);

                _LogOrdnerGeändert = false;
                PlotterCom.StaticLogger.LogFileFolder = PlotterCom.ConfigManager.Config.LogOrdner;
                PlotterCom.StaticLogger.LogDateiNeuÖffnen();
            }


        }

        private void AlleEinstellungenAusConfigManager() {
            PlotterCom.StaticLogger.Log("Lese alle Einstellungen aus dem ConfigManager!", 6);

            // --- TabPage: Allgemein ---
            if (PlotterCom.ConfigManager.Config.BeimBeendenBestätigen) {
                radioButton_BeendenJa.Checked = true;
            } else {
                radioButton_BeendenNein.Checked = true;
            }

            if (PlotterCom.ConfigManager.Config.LogOrdner.Length > 0) {
                textBox_LogOrdner.Text = PlotterCom.ConfigManager.Config.LogOrdner;
            }

            if (PlotterCom.ConfigManager.Config.ConfigFile.Length > 0) {
                textBox_ConfigFile.Text = PlotterCom.ConfigManager.Config.ConfigFile;
            }

            comboBox_LogLevel.SelectedIndex = PlotterCom.ConfigManager.Config.LogLevel;

            // --- TabPage: ComPort ---
            comboBox_ComPort.SelectedItem = PlotterCom.ConfigManager.Config.ComPort;
            comboBox_Baud.SelectedItem = PlotterCom.ConfigManager.Config.ComBaudRate.ToString();
            comboBox_DataBits.SelectedItem = PlotterCom.ConfigManager.Config.ComDataBits.ToString();
            comboBox_HandShake.SelectedItem = PlotterCom.ConfigManager.Config.ComHandshake;
            comboBox_Parity.SelectedItem = PlotterCom.ConfigManager.Config.ComParity;
            comboBox_StopBits.SelectedItem = PlotterCom.ConfigManager.Config.ComStopBits;

            // --- TabPage: Plotter ---
            textBoxModell.Text = PlotterCom.ConfigManager.Config.PlotterName;
            comboBox_Größe.SelectedItem = PlotterCom.ConfigManager.Config.PlotterGröße;
            textBox_P1X.Text = PlotterCom.ConfigManager.Config.PlotterMinX.ToString();
            textBox_P1Y.Text = PlotterCom.ConfigManager.Config.PlotterMinY.ToString();
            textBox_P2X.Text = PlotterCom.ConfigManager.Config.PlotterMaxX.ToString();
            textBox_P2Y.Text = PlotterCom.ConfigManager.Config.PlotterMaxY.ToString();

            // --- TabPage: HPGL ---
            checkBox_Ursprung.Checked = PlotterCom.ConfigManager.Config.HPGLUrsprung;
            checkBox_Warnung.Checked = PlotterCom.ConfigManager.Config.HPGLWarnungBereich;
            checkBox_Stift1.Checked = PlotterCom.ConfigManager.Config.HPGLNurStift1;
            trackBar_Geschwindigkeit.Value = PlotterCom.ConfigManager.Config.HPGLPlotGeschwindigkeit;
            trackBar_Geschwindigkeit_Scroll(this, EventArgs.Empty);

            if (PlotterCom.ConfigManager.Config.HPGLVerbinden) {
                radioButton_JoinImmer.Checked = true;
            }

            if (PlotterCom.ConfigManager.Config.HPGLVerbindenParallel) {
                radioButton_JoinParallel.Checked = true;
            }

            if (!PlotterCom.ConfigManager.Config.HPGLVerbinden && !PlotterCom.ConfigManager.Config.HPGLVerbinden) {
                radioButton_JoinNie.Checked = true;
            }

            textBox_JoinTicks.Text = PlotterCom.ConfigManager.Config.HPGLVerbindenSchritte.ToString();

            // --- TabPage: Viewer ---
            checkBox_Skalieren.Checked = PlotterCom.ConfigManager.Config.ViewerSkalieren;
            radioButton_HQ.Checked = PlotterCom.ConfigManager.Config.ViewerHQ;
            radioButton_LQ.Checked = !PlotterCom.ConfigManager.Config.ViewerHQ;
        }
        private void Einstellungen_Load(object sender, EventArgs e) {
            comboBox_LogLevel.SelectedIndex = 8;
            ComPortEinstellungenInitialisieren();
            AlleEinstellungenAusConfigManager();
        }

        private void textBox_LogOrdner_TextChanged(object sender, EventArgs e) {
            _LogOrdnerGeändert = true;
        }

        private void ComPortEinstellungenInitialisieren() {
            PlotterCom.StaticLogger.Log("ComPort-Einstellungen werden initialisiert.", 8);

            PlotterCom.StaticLogger.Log("Leere ComboBoxen.", 8);
            comboBox_ComPort.Items.Clear();
            comboBox_Parity.Items.Clear();
            comboBox_StopBits.Items.Clear();
            comboBox_HandShake.Items.Clear();

            comboBox_ComPort.Items.AddRange(PortNamenSortiert());
            comboBox_Parity.Items.AddRange(Enum.GetNames(typeof(System.IO.Ports.Parity)));
            comboBox_StopBits.Items.AddRange(Enum.GetNames(typeof(System.IO.Ports.StopBits)));
            comboBox_HandShake.Items.AddRange(Enum.GetNames(typeof(System.IO.Ports.Handshake)));

            if (comboBox_ComPort.Items.Count > 0) {
                comboBox_ComPort.SelectedIndex = 0;
            } else {
                PlotterCom.StaticLogger.Log("Es gibt keine COM-Ports auf diesem PC. Programm kann nicht arbeiten!", 1);
                MessageBox.Show("Es wurden keine COM-Ports auf diesem PC gefunden.\n" +
                    "Bitte richten Sie einen COM-Port ein und starten Sie das Programm neu.\n" +
                    "Ohne COM-Port kann das Programm nicht genutzt werden!",
                    "Kein COM-Port gefunden!",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Enabled = false;
            }

            comboBox_Baud.SelectedIndex = 5;
            comboBox_HandShake.SelectedIndex = 0;
            comboBox_DataBits.SelectedIndex = 1;
            comboBox_Parity.SelectedIndex = 0;
            comboBox_StopBits.SelectedIndex = 1;
        }

        private string[] PortNamenSortiert() {
            PlotterCom.StaticLogger.Log("Erstelle neue sortierte Portliste.", 7);
            string[] PortListe = System.IO.Ports.SerialPort.GetPortNames();

            PlotterCom.StaticLogger.Log("Liste wird sortiert...", 8);
            Array.Sort(PortListe);

            PlotterCom.StaticLogger.Log("Fertig, Liste hat " + PortListe.Length.ToString() + " Items!", 7);
            return PortListe;
        }


        private void button_PortÖffnen_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("ComPort soll geöffnet werden.", 7);
            bool flgOpenError = false;

            if (_serialPort != null && _serialPort.IsOpen) {
                PlotterCom.StaticLogger.Log("Es existiert noch ein aktiver ComPort. Er wird nun geschlossen.", 6);
                _serialPort.Close();
                _serialPort.Dispose();
                _serialPort = null;
            }


            PlotterCom.StaticLogger.Log("Erstelle neues SerialPort-Objekt.", 6);
            _serialPort = new System.IO.Ports.SerialPort();

            try {
                PlotterCom.StaticLogger.Log("Weise dem ComPort die vom Benutzer gewählten Eigenschaften zu.", 7);
                _serialPort.BaudRate  = System.Int32.Parse(comboBox_Baud.Text);
                _serialPort.DataBits  = System.Int32.Parse(comboBox_DataBits.Text);
                _serialPort.StopBits  = (System.IO.Ports.StopBits) Enum.Parse(typeof(System.IO.Ports.StopBits), comboBox_StopBits.Text);
                _serialPort.Parity    = (System.IO.Ports.Parity) Enum.Parse(typeof(System.IO.Ports.Parity), comboBox_Parity.Text);
                _serialPort.PortName  = comboBox_ComPort.Text;
                _serialPort.Handshake = System.IO.Ports.Handshake.RequestToSend;
            } catch (System.ArgumentException aex) {
                flgOpenError = true;
                PlotterCom.StaticLogger.Log("Fehler beim Parsen der Einstellungen (ArgumentException)! ComPort kann nicht geöffnet werden.", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + aex.Message, 4);
            } catch (System.FormatException fex) {
                flgOpenError = true;
                PlotterCom.StaticLogger.Log("Fehler beim Parsen der Einstellungen (FormatException)! ComPort kann nicht geöffnet werden.", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + fex.Message, 4);
            }

            if (flgOpenError) {
                PlotterCom.StaticLogger.Log("Comport öffnen wird mit einem Fehler abgebrochen!", 4);
                return;
            }

            try {
                PlotterCom.StaticLogger.Log("Einstellungen wurde zugewiesen. Öffne Port!", 6);

                _serialPort.Open();

                PlotterCom.StaticLogger.Log("Port ist offen!", 5);
            } catch (UnauthorizedAccessException uaex) {
                flgOpenError = true;
                PlotterCom.StaticLogger.Log("Zugriff auf den ComPort wurde verweigert!", 3);
                PlotterCom.StaticLogger.Log("Fehlermeldung: " + uaex.Message, 3);
            } catch (System.IO.IOException ioex) {
                flgOpenError = true;
                PlotterCom.StaticLogger.Log("Zugriff auf den ComPort fehlgeschlagen! IO-Fehler.", 3);
                PlotterCom.StaticLogger.Log("Fehlermeldung: " + ioex.Message, 3);
            } catch (ArgumentException aex) {
                flgOpenError = true;
                PlotterCom.StaticLogger.Log("Zugriff auf den ComPort fehlgeschlagen! Parameterfehler.", 3);
                PlotterCom.StaticLogger.Log("Fehlermeldung: " + aex.Message, 3);
            }

            if (flgOpenError) {
                PlotterCom.StaticLogger.Log("Öffnen des ComPorts fehlgeschlagen! Breche ab.", 3);
                return;
            }

            _serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(SerialPort_DataReceived);


            comboBox_Baud.Enabled = false;
            comboBox_ComPort.Enabled = false;
            comboBox_DataBits.Enabled = false;
            comboBox_HandShake.Enabled = false;
            comboBox_Parity.Enabled = false;
            comboBox_StopBits.Enabled = false;

            button_PortÖffnen.Enabled = false;
            button_PortSchliessen.Enabled = true;
            button_Senden.Enabled = true;


            textBox_HPGL.Focus();
        }

        private void SerialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            //this.Invoke(new EventHandler(delegate {
            //    // Hier was tun!
            //}));
        }

        private void button_PortSchliessen_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Der ComPort soll geschlossen werden!", 6);

            if (_serialPort != null && _serialPort.IsOpen) {

                _serialPort.Close();
                _serialPort.Dispose();
                _serialPort = null;

                PlotterCom.StaticLogger.Log("Der ComPort wurde geschlossen!", 5);

                button_PortSchliessen.Enabled = false;
                button_PortÖffnen.Enabled = true;
                button_Senden.Enabled = false;

                comboBox_Baud.Enabled = true;
                comboBox_ComPort.Enabled = true;
                comboBox_DataBits.Enabled = true;
                comboBox_HandShake.Enabled = true;
                comboBox_Parity.Enabled = true;
                comboBox_StopBits.Enabled = true;

            } else {
                PlotterCom.StaticLogger.Log("Der Comport ist bereits geschlossen oder nicht vorhanden!", 4);
                _serialPort = null;
            }
        }

        private void button_Senden_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Der Benutzer hat 'Senden' geklickt.", 8);
            Senden();
        }

        private void Senden() {
            if (_serialPort != null && _serialPort.IsOpen) {

                if (String.IsNullOrEmpty(textBox_HPGL.Text)) {
                    PlotterCom.StaticLogger.Log("Der Benutzer will Senden, die Textbox ist aber leer. Sende Zeilenumbruch!", 6);
                    _serialPort.WriteLine(String.Empty);
                } else {
                    _serialPort.WriteLine(textBox_HPGL.Text);
                    textBox_HPGL.SelectAll();
                }

            } else {
                PlotterCom.StaticLogger.Log("Der Serielle Port ist nicht offen!", 4);
            }
        }

        private void Einstellungen_FormClosing(object sender, FormClosingEventArgs e) {
            if (_serialPort != null && _serialPort.IsOpen) {
                PlotterCom.StaticLogger.Log("Der ComPort ist noch offen. Er wird geschlossen!", 4);
                _serialPort.Close();
            }
        }

        private void textBox_HPGL_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                e.Handled = true;
                Senden();
            }
        }

        private void beendenToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void überEinstellungenToolStripMenuItem_Click(object sender, EventArgs e) {
            MessageBox.Show(
                "PlotterCom Einstellungen  Copyright (C) 2011  André Matuschek\n" +
                "This program comes with ABSOLUTELY NO WARRANTY.\n" +
                "This is free software, and you are welcome to redistribute it " +
                "under certain conditions; see http://www.gnu.org/licenses/ for details.",
                "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void speichernToolStripMenuItem_Click(object sender, EventArgs e) {
            button_Speichern_Click(this, EventArgs.Empty);
        }

        private void neuLadenToolStripMenuItem_Click(object sender, EventArgs e) {
            button_NeuLaden_Click(this, EventArgs.Empty);
        }

        private void trackBar_Geschwindigkeit_Scroll(object sender, EventArgs e) {
            label_AktuelleGeschw.Text = trackBar_Geschwindigkeit.Value.ToString() + " cm/s";
        }

    }
}