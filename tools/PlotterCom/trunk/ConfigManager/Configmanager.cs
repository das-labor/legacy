using System;
using System.Collections.Generic;
using System.Text;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Configmanager.cs - The PlotterCom ConfigManager.
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
    public sealed class ConfigManager {

        #region Membervariablen

        private static ConfigManager _cfgManager = new ConfigManager();

        [System.Xml.Serialization.XmlIgnoreAttribute()]
        private System.Xml.Serialization.XmlSerializer _xmlSerialisierer = null;

        public bool BeimBeendenBestätigen = false;
        public string LogOrdner = Environment.CurrentDirectory;
        public int LogLevel = 9;
        public string ConfigFile = System.IO.Path.Combine(Environment.CurrentDirectory, "PlotterCom.xml");

        public int    ComLogBoxMaxZeilen = 1000;
        public string ComPort = "COM1";
        public string ComHandshake = "RequestToSend";
        public int    ComBaudRate = 9600;
        public string ComParity = "Even";
        public int    ComDataBits = 8;
        public string ComStopBits = "One";

        public string PlotterName = String.Empty;
        public string PlotterGröße = "DIN A 4";
        public int    PlotterMaxX = 15000;
        public int    PlotterMaxY = 10000;
        public int    PlotterMinX = 0;
        public int    PlotterMinY = 0;

        public bool HPGLUrsprung = true;
        public bool HPGLWarnungBereich = true;
        public bool HPGLNurStift1 = false;
        public bool HPGLVerbinden = false;
        public bool HPGLVerbindenParallel = false;
        public int  HPGLVerbindenSchritte = 4;
        public int  HPGLPlotGeschwindigkeit = 40;

        public UInt16 NetProto = 0; 
        public string NetIPAdresse = "10.0.1.30";
        public int    NetPortNr = 85;

        public bool ViewerHQ = true;
        public bool ViewerSkalieren = false;

        #endregion

        public static ConfigManager Config {
            get {
                return _cfgManager;
            }
            set {
                if (value != null) {
                    PlotterCom.StaticLogger.Log("Zentrale Config-Klasse wird ersetzt!", 6);
                    _cfgManager = value;
                } else {
                    PlotterCom.StaticLogger.Log("Zentrale Config-Klasse soll null gesetzt werden. Erstelle neue Config!", 4);
                    _cfgManager = new ConfigManager();
                }
            }
        }

        private System.Xml.Serialization.XmlSerializer ErzeugeSerialisierer() {
            PlotterCom.StaticLogger.Log("Ein Serialisierer wurde angefordert.", 8);

            if (_xmlSerialisierer == null) {
                PlotterCom.StaticLogger.Log("Ein neuer Serialisierer wird erstellt.", 7);
                _xmlSerialisierer = new System.Xml.Serialization.XmlSerializer(typeof(ConfigManager));
            }

            return _xmlSerialisierer;
        }

        private string ConfigStringErstellen() {
            PlotterCom.StaticLogger.Log("Einstellungen werden serialisiert.", 6);

            // TODO: Try/Catch-Block.
            System.IO.StringWriter strWriter = new System.IO.StringWriter();
            ErzeugeSerialisierer().Serialize(strWriter, this);

            return strWriter.ToString();
        }

        public bool EinstellungenSpeichernInDatei(string DateiName) {
            System.IO.FileStream outStream = null;
            System.IO.StreamWriter configStreamWriter = null;

            PlotterCom.StaticLogger.Log("Speichere Einstellungen in Datei ab.", 7);

            try {
                outStream = System.IO.File.Open(DateiName, System.IO.FileMode.Create, System.IO.FileAccess.Write, System.IO.FileShare.Read);
                configStreamWriter = new System.IO.StreamWriter(outStream);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Kann Einstellungsdatei nicht zum schreiben öffnen!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
                return false;
            }

            configStreamWriter.Write(ConfigStringErstellen());

            configStreamWriter.Flush();
            outStream.Flush();

            configStreamWriter.Close();
            outStream.Close();

            return true;
        }

        public bool EinstellungenLadenAusDatei(string DateiName) {
            System.IO.FileStream inStream = null;
            System.IO.StreamReader configStreamReader = null;

            PlotterCom.StaticLogger.Log("Lade Einstellungen aus Datei!", 6);

            try {
                inStream = System.IO.File.Open(DateiName, System.IO.FileMode.Open, System.IO.FileAccess.Read, System.IO.FileShare.Read);
                configStreamReader = new System.IO.StreamReader(inStream);
            }catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Kann Einstellungsdatei nicht zum lesen öffnen!", 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
                return false;
            }

            EinstellungenLaden(configStreamReader.ReadToEnd());

            configStreamReader.Close();
            inStream.Close();

            PlotterCom.StaticLogger.LogLevel = PlotterCom.ConfigManager.Config.LogLevel;
            PlotterCom.StaticLogger.LogFileFolder = PlotterCom.ConfigManager.Config.LogOrdner;
            PlotterCom.StaticLogger.LogDateiNeuÖffnen();

            
            return true;
        }

        public void EinstellungenLaden(string ConfigString) {
            PlotterCom.StaticLogger.Log("Einstellungen werden aus String geladen!", 6);
            System.IO.StringReader strReader = new System.IO.StringReader(ConfigString);

            try {
                // Wenn der ConfigManager nicht neu erstellt wird, gibt er bei QueryStringValue
                // einfach den alten Wert zurück statt den Wert neu aus der DB zu laden.
                // Daher den ConfigManager neu erstellen. Dann muss er den aktuellen Wert aus der DB laden.
  
                PlotterCom.ConfigManager.Config = (ConfigManager) (ErzeugeSerialisierer().Deserialize(strReader));

            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Einstellungen konnten nicht geladen werden!", 3);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 3);
            }
        }
    }
}
