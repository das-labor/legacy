using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  HPGL.cs - HPGL tools.
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

    /// <summary>
    /// Diese Klasse kapselt eine HPGL-Datei bzw. einen Druckjob.
    /// </summary>
    public class HPGL {

        private System.Collections.Generic.List<PlotterCom.HPGLElemente.Linie> _Linien = null;
        public double MaximaleXKoordinate = 0;
        public double MaximaleYKoordinate = 0;
        public double MinimaleXKoordinate = 0;
        public double MinimaleYKoordinate = 0;

        public bool LadenAbbrechen = false;
        public bool OptimierenAbbrechen = false;

        private bool   HPGLLadenStiftUnten = false;
        private UInt16 HPGLLadenAktuellerStift = 1;
        private double HPGLLadenPosX = 0D;
        private double HPGLLadenPosY = 0D;
        private char[] HPGLLadenKomma     = new char[] { ',' };
        private char[] HPGLLadenSemikolon = new char[] { ';', '\n', '\r'};

        private double _AnalyseSteckenLänge    = 0D;
        private double _AnalyseFahrweg         = 0D;
        private int    _AnalyseStiftbewegungen = 0;
        private int    _AnalyseStiftwechsel    = 0;
        private double _AnalysePlotDauer       = 0D;

        public int AnzahlLinien {
            get {
                if (_Linien != null) {
                    PlotterCom.StaticLogger.Log("Aktuelle Anzahl an Linien: " + _Linien.Count, 8);
                    return _Linien.Count;
                } else {
                    PlotterCom.StaticLogger.Log("Derzeit ist _Linien = null. Gebe 0 zurück!", 8);
                    return 0;
                }
            }
        }

        #region Analyseergebnisse
        public void LeseHPGLFile(string DateiName) {
            LeseHPGLFile(DateiName, null, null);
        }

        public double SummeStreckenLänge {
            get { return _AnalyseSteckenLänge; }
        }

        public double GesamtFahrweg {
            get { return _AnalyseFahrweg; }
        }

        public int AnzahlStiftbewegungen {
            get { return _AnalyseStiftbewegungen; }
        }

        public int Sitftwechsel {
            get { return _AnalyseStiftwechsel; }
        }

        public double PlotDauer {
            get { return _AnalysePlotDauer; }
        }
        #endregion

        public System.Collections.Generic.List<PlotterCom.HPGLElemente.Linie> Linien {
            get { return _Linien; }
        }

        public HPGL() {
        }

        /// <summary>
        /// Erzeugt aus den Daten dieser HPGL-Klasse einen HPGL-String der an einen Plotter
        /// gesendet werden kann.
        /// </summary>
        /// <returns>HPGL-Daten als String.</returns>
        public override string ToString() {
            StringBuilder HPGLCode = new StringBuilder("IN;");
            double AktuelleStiftPositionX = 0;
            double AktuelleStiftPositionY = 0;
            UInt16 AktuellerStift = 0;
            bool NurStift1 = PlotterCom.ConfigManager.Config.HPGLNurStift1;
            bool LinienVerbinden = PlotterCom.ConfigManager.Config.HPGLVerbinden;
            double LinienVerbindenAb = PlotterCom.ConfigManager.Config.HPGLVerbindenSchritte;

            // Kultur auf US stellen, damit Fließkommazahlen korrekt formatiert werden.
            Application.CurrentCulture = new System.Globalization.CultureInfo("EN-us", false);

            HPGLCode.Append("VS");
            HPGLCode.Append(PlotterCom.ConfigManager.Config.HPGLPlotGeschwindigkeit.ToString());
            HPGLCode.Append(";");
            HPGLCode.Append("SP8;PU0,0;\n");

            foreach (PlotterCom.HPGLElemente.Linie AktuelleLinie in _Linien) {

                // Stiftwechsel nötig?
                if ((!NurStift1) && (AktuellerStift != AktuelleLinie.Pen)) {
                    HPGLCode.Append("SP");
                    HPGLCode.Append(AktuelleLinie.Pen);
                    HPGLCode.Append(";");
                    HPGLCode.Append(Environment.NewLine);
                    AktuellerStift = AktuelleLinie.Pen;
                }

                // Wenn Linien nicht genau zusammen passen: Stift heben und zur StartPos fahren.
                if ((AktuelleLinie.StartX != AktuelleStiftPositionX) ||
                    (AktuelleLinie.StartY != AktuelleStiftPositionY)) {
                    HPGLCode.Append("PU");
                    HPGLCode.Append(AktuelleLinie.StartX);
                    HPGLCode.Append(",");
                    HPGLCode.Append(AktuelleLinie.StartY);
                    HPGLCode.Append(";");
                    HPGLCode.Append(Environment.NewLine);
                    HPGLCode.Append("PD");
                    HPGLCode.Append(AktuelleLinie.EndX);
                    HPGLCode.Append(",");
                    HPGLCode.Append(AktuelleLinie.EndY);
                    HPGLCode.Append(";");
                    HPGLCode.Append(Environment.NewLine);
                } else { // Sonst: einfach weiter zeichnen.
                    HPGLCode.Append("PA");
                    HPGLCode.Append(AktuelleLinie.EndX);
                    HPGLCode.Append(",");
                    HPGLCode.Append(AktuelleLinie.EndY);
                    HPGLCode.Append(";");
                    HPGLCode.Append(Environment.NewLine);
                }

                // Letzte Stiftposition merken.
                AktuelleStiftPositionX = AktuelleLinie.EndX;
                AktuelleStiftPositionY = AktuelleLinie.EndY;
            }


            // Kultur wiederherstellen.
            Application.CurrentCulture = new System.Globalization.CultureInfo("DE-de", false);

            HPGLCode.Append("PU0,0;SP0;\n");

            return HPGLCode.ToString();
        }

        public void SchreibeHPGLInDatei(string DateiName) {
            System.IO.FileStream FilOutStream = System.IO.File.Open(DateiName, System.IO.FileMode.Create, System.IO.FileAccess.Write, System.IO.FileShare.None);
            System.IO.StreamWriter sw = new System.IO.StreamWriter(FilOutStream);
            sw.Write(this.ToString());
            sw.Flush();
            FilOutStream.Flush();
            sw.Close();
            FilOutStream.Close();
        }

        /// <summary>
        /// Ließt eine HPGL-Datei ein.
        /// </summary>
        /// <param name="DateiName"></param>
        /// <param name="progressBar"></param>
        /// <param name="progressLabel"></param>
        public void LeseHPGLFile(string DateiName, ToolStripProgressBar progressBar, ToolStripLabel progressLabel) {
            System.IO.StreamReader HPGLReader = null;
            string HPGLSequenz = String.Empty;
            string[] HPGLBefehle = null;

            _Linien = new List<PlotterCom.HPGLElemente.Linie>(1048576);

            Application.CurrentCulture = new System.Globalization.CultureInfo("EN-US", false);

            if (String.IsNullOrEmpty(DateiName)) {
                PlotterCom.StaticLogger.Log("Es wurde keine Datei angebenen. Ende.", 4);
                return;
            }

            PlotterCom.StaticLogger.Log("Prüfe, ob Datei existiert!", 8);
            if (!System.IO.File.Exists(DateiName)) {
                PlotterCom.StaticLogger.Log("Die Angegebene Datei existiert nicht. Ende.", 4);
                return;
            } else {
                PlotterCom.StaticLogger.Log("Die Angegebene Datei existiert.", 8);
            }


            PlotterCom.StaticLogger.Log("Öffne die Angegebene Datei!", 8);
            try {
                HPGLReader = new System.IO.StreamReader(DateiName);
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte die angegebene Datei nicht zum lesen öffnen!", 4);
                PlotterCom.StaticLogger.Log("Datei: " + DateiName, 4);
                PlotterCom.StaticLogger.Log("Fehler: " + ex.Message, 4);
            }

            HPGLLadenStiftUnten = false;
            HPGLLadenAktuellerStift = 1;
            HPGLLadenPosX = 0D;
            HPGLLadenPosY = 0D;

            PlotterCom.StaticLogger.Log("Lese die angegebene Datei komplett ein.", 8);
            HPGLSequenz = HPGLReader.ReadToEnd();

            PlotterCom.StaticLogger.Log("Entferne alle Zeilenumbrüche aus der Datei.", 8);
            //HPGLSequenz = HPGLSequenz.Replace("\n", String.Empty);
            //HPGLSequenz = HPGLSequenz.Replace("\r", String.Empty);
            HPGLSequenz = HPGLSequenz.Replace("\t", String.Empty);

            PlotterCom.StaticLogger.Log("Trenne HPGL-Code in einzelne Befehle!", 8);
            HPGLBefehle = HPGLSequenz.Split(HPGLLadenSemikolon, StringSplitOptions.RemoveEmptyEntries);

            if (HPGLSequenz.Length > 1) {
                PlotterCom.StaticLogger.Log("Verarbeite " + HPGLBefehle.Length.ToString() + " Befehle!", 6);
            } else {
                PlotterCom.StaticLogger.Log("Keine Befehle gefunden! Abbruch!", 4);
            }

            // In allen ab hier aufgerufenen Funktionen wird nicht mehr geloggt, um die
            // Geschwindigkeit zu erhöhen!

            foreach (string HPGLBefehl in HPGLBefehle) {
                ParseHPGLBefehl(HPGLBefehl);
            }

            HPGLReader.Close();


            PlotterCom.StaticLogger.Log("Anzahl Linien: " + _Linien.Count + ".", 6);
            BerechnetMinMaxNeu();

            if (PlotterCom.ConfigManager.Config.HPGLUrsprung) {
                PlotterCom.StaticLogger.Log("Schiebe Grafik in den Ursprung!", 6);
                VerschiebeAlleLinien(
                    (PlotterCom.ConfigManager.Config.PlotterMinX - MinimaleXKoordinate + 2),
                    (PlotterCom.ConfigManager.Config.PlotterMinY - MinimaleYKoordinate + 2));
                BerechnetMinMaxNeu();
            }

            if (PlotterCom.ConfigManager.Config.HPGLWarnungBereich) {
                if ((MinimaleXKoordinate < PlotterCom.ConfigManager.Config.PlotterMinX) ||
                    (MinimaleYKoordinate < PlotterCom.ConfigManager.Config.PlotterMinY) ||
                    (MaximaleXKoordinate > PlotterCom.ConfigManager.Config.PlotterMaxX) ||
                    (MaximaleYKoordinate > PlotterCom.ConfigManager.Config.PlotterMaxY)) {

                    PlotterCom.StaticLogger.Log(
                        "Die aktuelle HPGL-Datei enthält Koordinaten, die nicht im Plotbaren Bereich des Plotters " +
                        "liegen. Möglicherweise wird die Datei nicht ordnungsgemäß gedruckt!", 5);

                    MessageBox.Show(
                        "Die aktuelle HPGL-Datei enthält Koordinaten, die nicht im Plotbaren Bereich des Plotters " +
                        "liegen. Möglicherweise wird die Datei nicht ordnungsgemäß gedruckt!",
                        "Warnung!",
                        MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            PlotterCom.StaticLogger.Log("MinX: " + MinimaleXKoordinate + ", MinY: " + MinimaleYKoordinate +
                ", MaxX: " + MaximaleXKoordinate + ", MaxY: " + MaximaleYKoordinate + ".", 6);

            Application.CurrentCulture = new System.Globalization.CultureInfo("DE-de", false);


        }

        /// <summary>
        /// Parsed einen HPGL-Befehl.
        /// </summary>
        /// <param name="HPGLBefehl">Der Befehl mit Parametern.</param>
        private void ParseHPGLBefehl(string HPGLBefehl) {

            if (String.IsNullOrWhiteSpace(HPGLBefehl)) {
                PlotterCom.StaticLogger.Log("Der HPGL-Befehl ist leer. Er wird ignoriert!", 9);
                return;
            }

            HPGLBefehl = HPGLBefehl.Trim();

            if (HPGLBefehl.Length < 2) {
                PlotterCom.StaticLogger.Log("Der HPGL-Befehl ist zu kurz. Er wird ignoriert!", 9);
                return;
            }

            // Hier wird kein switch-Statement verwendet, da sonst die Funktion String.Equals nicht mit 
            // speziellen Vergleichsparametern benutzt werden kann.

            if (HPGLBefehl.Substring(0,2).Equals("PA", StringComparison.OrdinalIgnoreCase)) {
                ParsePA(HPGLBefehl.Substring(2));
                return;
            }

            if (HPGLBefehl.Substring(0, 2).Equals("PR", StringComparison.OrdinalIgnoreCase)) {
                ParsePR(HPGLBefehl.Substring(2));
                return;
            }

            if (HPGLBefehl.Substring(0, 2).Equals("PD", StringComparison.OrdinalIgnoreCase)) {
                ParsePD(HPGLBefehl.Substring(2));
                return;
            }

            if (HPGLBefehl.Substring(0, 2).Equals("PU", StringComparison.OrdinalIgnoreCase)) {
                ParsePU(HPGLBefehl.Substring(2));
                return;
            }

            if (HPGLBefehl.Substring(0, 2).Equals("SP", StringComparison.OrdinalIgnoreCase)) {
                if (HPGLBefehl.Length == 3) {
                    try {
                        if (!PlotterCom.ConfigManager.Config.HPGLNurStift1) {
                            HPGLLadenAktuellerStift = System.UInt16.Parse(HPGLBefehl.Substring(2, 1));
                        }
                    } catch {
                        PlotterCom.StaticLogger.Log("Stiftnummer kann nicht interpretiert werden!", 4);
                    }
                } else {
                    PlotterCom.StaticLogger.Log("Der Select Pen Befehl ist nicht genau 3 Zeichen lang!", 4);
                }
                return;
            }

            if (HPGLBefehl.Substring(0,2).Equals("IN", StringComparison.OrdinalIgnoreCase)) {
                PlotterCom.StaticLogger.Log("Initialisierung gelesen.", 8);
                HPGLLadenPosX = 0D;
                HPGLLadenPosY = 0D;
                return;
            }

            PlotterCom.StaticLogger.Log("Verstehe den Befehl nicht: " + HPGLBefehl, 4);
        }

        /// <summary>
        /// Parsed die Parameter des PenDown-Befehls.
        /// </summary>
        /// <param name="PDParameter">Koordinatenliste oder PR/PA.</param>
        private void ParsePD(string Parameter) {
            string[] ParameterListe = null;
            double PosX, PosY;

            HPGLLadenStiftUnten = true;
            
            if ((Parameter != null) && (Parameter.Length > 4)) {

                // Hier kann direkt ein weiterer Befehl folgen!
                if (Parameter.Substring(0, 2).Equals("PA")) {
                    ParsePA(Parameter.Substring(2));
                    return;
                }

                if (Parameter.Substring(0, 2).Equals("PR")) {
                    ParsePR(Parameter.Substring(2));
                    return;
                }

                ParameterListe = Parameter.Split(HPGLLadenKomma, StringSplitOptions.RemoveEmptyEntries);

                if ((ParameterListe.Length > 0) && (ParameterListe.Length % 2 == 0)) {
                    for (int i = 0; i < ParameterListe.Length - 1; i += 2) {
                        PosX = System.Double.Parse(ParameterListe[i]);
                        PosY = System.Double.Parse(ParameterListe[i + 1]);

                        if (HPGLLadenStiftUnten) {
                            _Linien.Add(new PlotterCom.HPGLElemente.Linie(
                                HPGLLadenPosX, HPGLLadenPosY, PosX, PosY, HPGLLadenAktuellerStift));
                        }

                        HPGLLadenPosX = PosX;
                        HPGLLadenPosY = PosY;
                    }

                } else {
                    PlotterCom.StaticLogger.Log("Parameterliste hat eine ungerate Parameterzahl!", 4);
                }

            }
        }

        /// <summary>
        /// Parsed die Parameter des PenUp-Befehls.
        /// </summary>
        /// <param name="PDParameter">Koordinatenliste oder PR/PA.</param>
        private void ParsePU(string Parameter) {
            string[] ParameterListe = null;
            double PosX, PosY;

            HPGLLadenStiftUnten = false;

            if ((Parameter != null) && (Parameter.Length > 4)) {

                // Hier kann direkt ein weiterer Befehl folgen!
                if (Parameter.Substring(0, 2).Equals("PA")) {

                    // Die Befehlsfolge PUPA sollte nicht vorkommen, da sie den selben Effekt
                    // wie PU hätte.
                    ParsePA(Parameter.Substring(2));
                    return;
                }

                if (Parameter.Substring(0, 2).Equals("PR")) {
                    ParsePR(Parameter.Substring(2));
                    return;
                }

                ParameterListe = Parameter.Split(HPGLLadenKomma, StringSplitOptions.RemoveEmptyEntries);

                if ((ParameterListe.Length > 0) && (ParameterListe.Length % 2 == 0)) {
                    for (int i = 0; i < ParameterListe.Length - 1; i += 2) {
                        PosX = System.Double.Parse(ParameterListe[i]);
                        PosY = System.Double.Parse(ParameterListe[i + 1]);

                        // Es muss nichts dem Linien-Array hinzugefügt werden!

                        HPGLLadenPosX = PosX;
                        HPGLLadenPosY = PosY;
                    }

                } else {
                    PlotterCom.StaticLogger.Log("Parameterliste hat eine ungerate Parameterzahl!", 4);
                }

            }
        }

        /// <summary>
        /// Parsed die Parameter des PlotAbsolut-Befehls.
        /// </summary>
        /// <param name="PDParameter">Koordinatenliste, Kommagetrennt.</param>
        private void ParsePA(string Parameter) {
            string[] ParameterListe = null;
            double PosX, PosY;

            if ((Parameter != null) && (Parameter.Length > 4)) {

                ParameterListe = Parameter.Split(HPGLLadenKomma, StringSplitOptions.RemoveEmptyEntries);

                if ((ParameterListe.Length > 0) && (ParameterListe.Length % 2 == 0)) {
                    for (int i = 0; i < ParameterListe.Length - 1; i += 2) {
                        PosX = System.Double.Parse(ParameterListe[i]);
                        PosY = System.Double.Parse(ParameterListe[i + 1]);

                        // TODO: Die Abfrage ob der Stift unten ist kann früher gemacht werden!
                        // Das spart ev. Laufzeit.
                        if (HPGLLadenStiftUnten) {
                            _Linien.Add(new PlotterCom.HPGLElemente.Linie(
                                HPGLLadenPosX, HPGLLadenPosY, PosX, PosY, HPGLLadenAktuellerStift));
                        }

                        HPGLLadenPosX = PosX;
                        HPGLLadenPosY = PosY;

                    }

                } else {
                    PlotterCom.StaticLogger.Log("Parameterliste hat eine ungerate Parameterzahl!", 4);
                }

            }
        }

        /// <summary>
        /// Parsed die Parameter des PlotRelative-Befehls.
        /// </summary>
        /// <param name="PDParameter">Koordinatenliste, Kommagetrennt.</param>
        private void ParsePR(string Parameter) {
            string[] ParameterListe = null;
            double PosX, PosY;

            if ((Parameter != null) && (Parameter.Length > 4)) {

                ParameterListe = Parameter.Split(HPGLLadenKomma, StringSplitOptions.RemoveEmptyEntries);

                if ((ParameterListe.Length > 0) && (ParameterListe.Length % 2 == 0)) {
                    for (int i = 0; i < ParameterListe.Length - 1; i += 2) {
                        PosX = System.Double.Parse(ParameterListe[i]);
                        PosY = System.Double.Parse(ParameterListe[i + 1]);

                        // TODO: Die Abfrage ob der Stift unten ist kann früher gemacht werden!
                        // Das spart ev. Laufzeit.
                        if (HPGLLadenStiftUnten) {
                            _Linien.Add(new PlotterCom.HPGLElemente.Linie(
                                HPGLLadenPosX, HPGLLadenPosY, HPGLLadenPosX + PosX, HPGLLadenPosY + PosY, HPGLLadenAktuellerStift));
                        }

                        HPGLLadenPosX += PosX;
                        HPGLLadenPosY += PosY;

                    }

                } else {
                    PlotterCom.StaticLogger.Log("Parameterliste hat eine ungerate Parameterzahl!", 4);
                }

            }
        }


        public void VerschiebeAlleLinien(double dX, double dy) {
            PlotterCom.StaticLogger.Log("Verschiebe alle alle Linien um " + dX + " / " + dy + "!", 6);

            if (_Linien != null) {
                foreach (HPGLElemente.Linie deltaLinie in _Linien) {
                    deltaLinie.StartX += dX;
                    deltaLinie.EndX += dX;
                    deltaLinie.StartY += dy;
                    deltaLinie.EndY += dy;
                }
            }
        }

        public void Skaliere(double Skalierungsfaktor) {
            if (_Linien == null || _Linien.Count == 0) {
                PlotterCom.StaticLogger.Log("Kann nicht Skalieren, keine Linien!", 4);
                return;
            }

            if (Skalierungsfaktor < 0.1D) {
                PlotterCom.StaticLogger.Log("Kann nicht Skalieren, Faktor zu klein!", 4);
                return;
            }

            if (Skalierungsfaktor > 10D) {
                PlotterCom.StaticLogger.Log("Kann nicht Skalieren, Faktor zu groß!", 4);
                return;
            }

            PlotterCom.StaticLogger.Log("Skaliere mit Faktor: " + Skalierungsfaktor + ".", 6);

            
            foreach (HPGLElemente.Linie AktuelleLine in _Linien) {
            
                AktuelleLine.StartX *= Skalierungsfaktor;
                AktuelleLine.StartY *= Skalierungsfaktor;
                AktuelleLine.EndX *= Skalierungsfaktor;
                AktuelleLine.EndY *= Skalierungsfaktor;
            }


        }

        public void BerechnetMinMaxNeu() {
            double minX = System.Double.MaxValue, minY = System.Double.MaxValue;
            double maxX = System.Double.MinValue, maxY = System.Double.MinValue;
            
            PlotterCom.StaticLogger.Log("Berechnet Min und Max neu!", 6);

            if (_Linien != null) {
                foreach (HPGLElemente.Linie mmLinie in _Linien) {

                    if (mmLinie.StartX < minX) {
                        minX = mmLinie.StartX;
                    }
                    if (mmLinie.StartX > maxX) {
                        maxX = mmLinie.StartX;
                    }

                    if (mmLinie.StartY < minY) {
                        minY = mmLinie.StartY;
                    }
                    if (mmLinie.StartY > maxY) {
                        maxY = mmLinie.StartY;
                    }

                    if (mmLinie.EndX < minX) {
                        minX = mmLinie.EndX;
                    }
                    if (mmLinie.EndX > maxX) {
                        maxX = mmLinie.EndX;
                    }

                    if (mmLinie.EndY < minY) {
                        minY = mmLinie.EndY;
                    }
                    if (mmLinie.EndY > maxY) {
                        maxY = mmLinie.EndY;
                    }
                }
            } else {
                PlotterCom.StaticLogger.Log("Es gibt keine Linien. Min und Max werden auf 0 gesetzt!", 4);

                minX = 0D;
                minY = 0D;
                maxX = 0D;
                maxY = 0D;
            }

            MinimaleXKoordinate = minX;
            MinimaleYKoordinate = minY;
            MaximaleXKoordinate = maxX;
            MaximaleYKoordinate = maxY;

            if (MaximaleXKoordinate == Double.MinValue) {
                MaximaleXKoordinate = PlotterCom.ConfigManager.Config.PlotterMaxX;
            }

            if (MaximaleYKoordinate == Double.MinValue) {
                MaximaleYKoordinate = PlotterCom.ConfigManager.Config.PlotterMaxY;
            }

            if (MinimaleXKoordinate == Double.MaxValue) {
                MinimaleXKoordinate = PlotterCom.ConfigManager.Config.PlotterMinX;
            }

            if (MinimaleYKoordinate == Double.MaxValue) {
                MinimaleYKoordinate = PlotterCom.ConfigManager.Config.PlotterMinY;
            }

            
            PlotterCom.StaticLogger.Log("MinX: " + MinimaleXKoordinate + ", MinY: " + MinimaleYKoordinate +
                ", MaxX: " + MaximaleXKoordinate + ", MaxY: " + MaximaleYKoordinate + ".", 6);

        }

        public string UmschreibendesRechteckPlotten() {
            string UmschrRechtEck = "IN;SP1;PU0,0;\n";
            System.Globalization.CultureInfo currentCulture = Application.CurrentCulture;

            Application.CurrentCulture = new System.Globalization.CultureInfo("EN-us");

            BerechnetMinMaxNeu();

            UmschrRechtEck += "PU" + MinimaleXKoordinate + "," + MinimaleYKoordinate + ";\n";
            UmschrRechtEck += "PD" + MaximaleXKoordinate + "," + MinimaleYKoordinate + ";\n";
            UmschrRechtEck += "PA" + MaximaleXKoordinate + "," + MaximaleYKoordinate + ";\n";
            UmschrRechtEck += "PA" + MinimaleXKoordinate + "," + MaximaleYKoordinate + ";\n";
            UmschrRechtEck += "PA" + MinimaleXKoordinate + "," + MinimaleYKoordinate + ";\n";
            UmschrRechtEck += "PU0,0;SP0;";

            Application.CurrentCulture = currentCulture;

            return UmschrRechtEck;
        }

        public void Analyse() {
            double AktuelleKoordinateX = 0D;
            double AktuelleKoordinateY = 0D;
            int AktuellerStift = 0;

            _AnalyseFahrweg = 0D;
            _AnalyseSteckenLänge = 0D;
            _AnalyseStiftbewegungen = 0;
            _AnalyseStiftwechsel = 0;
            _AnalysePlotDauer = 0D;

            if (_Linien == null || _Linien.Count == 0) {
                return;
            }

            foreach (HPGLElemente.Linie AktuelleLinie in _Linien) {

                if ((AktuelleKoordinateX != AktuelleLinie.StartX) ||
                    (AktuelleKoordinateY != AktuelleLinie.StartY)) {
                    _AnalyseStiftbewegungen++;


                    _AnalyseFahrweg += Math.Sqrt(
                        ((AktuelleKoordinateX - AktuelleLinie.StartX) *
                         (AktuelleKoordinateX - AktuelleLinie.StartX)) +
                        ((AktuelleKoordinateY - AktuelleLinie.StartY) *
                         (AktuelleKoordinateY - AktuelleLinie.StartY)));
                }

                _AnalyseFahrweg += Math.Sqrt(
                    ((AktuelleLinie.StartX - AktuelleLinie.EndX)  * 
                     (AktuelleLinie.StartX - AktuelleLinie.EndX)) +
                    ((AktuelleLinie.StartY - AktuelleLinie.EndY)  * 
                     (AktuelleLinie.StartY - AktuelleLinie.EndY)) );

                _AnalyseSteckenLänge += Math.Sqrt(
                    ((AktuelleLinie.StartX - AktuelleLinie.EndX) *
                     (AktuelleLinie.StartX - AktuelleLinie.EndX)) +
                    ((AktuelleLinie.StartY - AktuelleLinie.EndY) *
                     (AktuelleLinie.StartY - AktuelleLinie.EndY)));

                if (AktuellerStift != AktuelleLinie.Pen) {
                    AktuellerStift = AktuelleLinie.Pen;
                    _AnalyseStiftwechsel++;
                }

                AktuelleKoordinateX = AktuelleLinie.EndX;
                AktuelleKoordinateY = AktuelleLinie.EndY;
            }

            _AnalysePlotDauer = _AnalyseFahrweg * 1.1D + _AnalyseStiftwechsel * 1000D + _AnalyseStiftbewegungen * 1.2D;
        }

        public void Optimiere(ToolStripProgressBar progressBar, ToolStripLabel label) {
            List<HPGLElemente.Linie> optimierteListe = null;  // Die neue, optimierte Liste.
            List<UInt16> StiftListe = null;                   // Die Liste aller verwendeten Stifte.
            double kleinsterAbstand;                          // Der kleinste Abstand, der bisher gefunden wurde.
            double aktuellerAbstandV = 0D;                    // Temporäre Variable zum halten des aktuellen Abstands vorwärts.
            double aktuellerAbstandR = 0D;                    // Temporäre Variable zum halten des aktuellen Abstands rückwärts.
            bool LinieRückwärts = false;                      // Ist true, wenn die aktuelle Linie umgedreht werden muss.
            bool LinieGefunden = true;                        // Habe ich eine Line gefunden?
            HPGLElemente.Linie besteLinie = null;             // Die aktuell beste Linie.
            HPGLElemente.Linie zuletztOptimierteLinie = null; // Die Linie, ab deren Ende gesucht wird.

            if ((_Linien == null) || (_Linien.Count == 0)) {
                PlotterCom.StaticLogger.Log("Keine Linien zum Optimieren. Ende", 4);
                return;
            }

            // Erstelle Liste mit allen verwendeten Stiften.
            StiftListe = StiftListeErstellen();

            // Fals die letzte Optimierung abgebrochen wurde, setzte OptimierenAbbrechen wieder auf false.
            OptimierenAbbrechen = false;

            // Erstelle eine neue, leere Liste in der die optimierte Liste aufgebaut wird.
            optimierteListe = new List<HPGLElemente.Linie>(_Linien.Count);

            // Zuerst das optimiert-Kennzeichen zurück setzen!
            foreach (HPGLElemente.Linie aktuelleLinie in _Linien) {
                aktuelleLinie.Optimiert = false;
            }

            // Das erste Element suchen und einfügen;
            optimierteListe.Add(ErsteLinie(StiftListe[0]));
            optimierteListe[0].Optimiert = true;
            zuletztOptimierteLinie = optimierteListe[0];

            foreach(UInt16 AktuellerStift in StiftListe) {

                // Zum hereinspringen in die Schleife LinieGefunden erstmal auf true setzen.
                LinieGefunden = true;

                // Solange noch neue Linien gefunden werden...
                while (LinieGefunden) {
                    kleinsterAbstand = System.Double.MaxValue;

                    LinieRückwärts = false;
                    LinieGefunden = false;  // Wird true gesetzt, wenn eine Linie gefunden wurde.
                                            // Es wird keine Linie gefunden wenn alle Linien optimiert wurden, die mit dem 
                                            // aktuellen Stift gezeichnet werden.

                    if (OptimierenAbbrechen) {
                        break;
                    }

                    // ... suche eine Nachfolgerlinie.
                    foreach (HPGLElemente.Linie aktuelleLinie in _Linien) {

                        // Wenn die Linie einen Stiftwechsel erfordern würde, ignoriere sie.
                        if (aktuelleLinie.Pen != AktuellerStift) {
                            continue;
                        }

                        // Bereits optimierte Linien auch ignorieren.
                        if (aktuelleLinie.Optimiert) {
                            // Ev. wäre es sinnvoll, die bereits optimierten aus der Liste zu entfernen, um die
                            // Geschwindigkeit zu erhöhen. Müssste man mal drüber nachdenken was das für Folgen hätte.
                            // --> Abbrechen nicht mehr möglich.
                            // --> Progress-Bar-Wert wird falsch berechnet.
                            continue;
                        }

                        // Berechne den Abstand zum aktuellen Kandidaten.
                        aktuellerAbstandV = (
                            (zuletztOptimierteLinie.EndX - aktuelleLinie.StartX) *
                            (zuletztOptimierteLinie.EndX - aktuelleLinie.StartX)
                            +
                            (zuletztOptimierteLinie.EndY - aktuelleLinie.StartY) *
                            (zuletztOptimierteLinie.EndY - aktuelleLinie.StartY)
                            );

                        // Berechne den Abstand zum Ende des aktuellen Kandidaten.
                        aktuellerAbstandR = (
                            (zuletztOptimierteLinie.EndX - aktuelleLinie.EndX) *
                            (zuletztOptimierteLinie.EndX - aktuelleLinie.EndX)
                            +
                            (zuletztOptimierteLinie.EndY - aktuelleLinie.EndY) *
                            (zuletztOptimierteLinie.EndY - aktuelleLinie.EndY)
                            );

                        if (aktuellerAbstandV < kleinsterAbstand) {
                            kleinsterAbstand = aktuellerAbstandV;
                            besteLinie = aktuelleLinie;
                            LinieRückwärts = false;
                            LinieGefunden = true;
                        }

                        if (aktuellerAbstandR < kleinsterAbstand) {
                            kleinsterAbstand = aktuellerAbstandV;
                            besteLinie = aktuelleLinie;
                            LinieRückwärts = true;
                            LinieGefunden = true;
                        }

                        // Wenn ich eine direkt anschließende Linie gefunden habe, kann ich die Suche stoppen.
                        if (aktuellerAbstandV == 0D) {
                            break;
                        }
                    }

                    // Habe ich etwas gefunden, nimm es in die optimierte Liste auf.
                    if (LinieGefunden) {

                        zuletztOptimierteLinie = besteLinie;

                        // Umdrehen, wenn das Ende näher war.
                        if (LinieRückwärts) {
                            besteLinie.DreheUm();
                        }

                        besteLinie.Optimiert = true;

                        optimierteListe.Add(besteLinie);
                        
                    }

                    if (optimierteListe.Count % 128 == 0) {
                        if ((progressBar != null) && (label != null)) {
                            int Prozent = (int)(((float)optimierteListe.Count / (float)_Linien.Count) * 100F);
                            progressBar.Value = Prozent;
                            label.Text = Prozent.ToString() + "%";
                            System.Windows.Forms.Application.DoEvents();
                        }
                    }
                }
            }

            if (!OptimierenAbbrechen) {
                _Linien = optimierteListe;
            }
        }

        private List<UInt16> StiftListeErstellen() {
            PlotterCom.StaticLogger.Log("Erstelle Stiftliste!", 6);
            List<UInt16> NeueStiftListe = new List<UInt16>();

            if (_Linien == null) {
                return NeueStiftListe;
            } 


            foreach (HPGLElemente.Linie AktuelleLine in _Linien) {
                if (!NeueStiftListe.Contains(AktuelleLine.Pen)) {
                    NeueStiftListe.Add(AktuelleLine.Pen);
                }
            }

            return NeueStiftListe;
        }

        private HPGLElemente.Linie ErsteLinie(UInt16 Stift) {
            double KleinsterAbstand = Double.MaxValue;
            double AktuellerAbstand = 0D;
            bool ErsteLinieRückwärts = false;
            HPGLElemente.Linie ersteLinie = null;

            if (_Linien == null || _Linien.Count == 0) {
                PlotterCom.StaticLogger.Log("Kann keine erste Linie ermitteln, Linien sind leer oder null!", 4);
                return null;
            }
            

            // Irgendwas sollte drin stehen, auch wenn aufgrund des Stiftes keine passende Linie gefunden wird.
            ersteLinie = _Linien[0];

            foreach (HPGLElemente.Linie aktuelleLinie in _Linien) {

                if (aktuelleLinie.Pen != Stift) {
                    continue;
                }

                AktuellerAbstand = (aktuelleLinie.StartX * aktuelleLinie.StartX) + 
                    (aktuelleLinie.StartY + aktuelleLinie.StartY);

                if (AktuellerAbstand < KleinsterAbstand) {
                    KleinsterAbstand = AktuellerAbstand;
                    ErsteLinieRückwärts = false;
                    ersteLinie = aktuelleLinie;
                }

                AktuellerAbstand = (aktuelleLinie.EndX * aktuelleLinie.EndX) +
                    (aktuelleLinie.EndY + aktuelleLinie.EndY);

                if (AktuellerAbstand < KleinsterAbstand) {
                    KleinsterAbstand = AktuellerAbstand;
                    ErsteLinieRückwärts = true;
                    ersteLinie = aktuelleLinie;
                }

            }

            if (ErsteLinieRückwärts) {
                ersteLinie.DreheUm();
            }

            return ersteLinie;
        }

        // Findet und entfernt zu jeder Linie alle Dubletten.
        public int EntferneDubletten(ToolStripProgressBar toolStripProgressBar, ToolStripStatusLabel toolStripStatusLabel) {
            int GefundeneDubletten = 0;
            int LinienNummer = 0;

            PlotterCom.StaticLogger.Log("Starte Dublettensuche in der Linien-Liste!", 6);

            OptimierenAbbrechen = false;

            if (_Linien == null) {
                return 0;
            }


            foreach (HPGLElemente.Linie TestLinie in _Linien) {
                ++LinienNummer;

                if (LinienNummer % 64 == 0) {
                    toolStripProgressBar.Value = (int)(((float)LinienNummer / (float)_Linien.Count) * 100F);
                    toolStripStatusLabel.Text = ((int)(((float)LinienNummer / (float)_Linien.Count) * 100F)).ToString() + "%";
                }

                if (!TestLinie.Dublette) {
                    FindeDublette(TestLinie);
                }
                
                Application.DoEvents();

                if (OptimierenAbbrechen) {
                    break;
                }
            }

            if (OptimierenAbbrechen) {
                return 0;
            }

            for (int i = _Linien.Count - 1; i >= 0; --i) {
                if (_Linien[i].Dublette) {
                    _Linien.Remove(_Linien[i]);
                    ++GefundeneDubletten;
                }
            }


            PlotterCom.StaticLogger.Log("Habe " + GefundeneDubletten + " Dubletten gefunden unt entfernt!", 6);

            return GefundeneDubletten;
        }

        // Findet alle Dubletten zu einer Linie.
        private void FindeDublette(HPGLElemente.Linie dublette) {
            foreach (HPGLElemente.Linie TestLinie in _Linien) {
                if (!dublette.Equals(TestLinie)) {
                    if ((dublette.StartX == TestLinie.StartX) &&
                        (dublette.StartY == TestLinie.StartY) &&
                        (dublette.EndX == TestLinie.EndX) &&
                        (dublette.EndY == TestLinie.EndY)) {

                        TestLinie.Dublette = true;
                    }
                }
            }
        }

        // Verbindet zwei Linien, die nacheinander in der Linien-Liste liegen, wenn der Endpunkt der ersten
        // nicht weiter als eine einstellbare obere Schranke an Schritten vom Anfang der zweiten entfernt liegt.
        public int LinienVerbinden() {
            int Index = 0;
            int NeueLinien = 0;
            double MaxAbstand = PlotterCom.ConfigManager.Config.HPGLVerbindenSchritte;
            double AbstandX, AbstandY;

            // TODO: Parallel-Test, wenn parallel in Config eingestellt.

            if (_Linien != null) {
                for (Index = _Linien.Count - 1 ; Index > 0 ; --Index) {
                    AbstandX = (Math.Abs(_Linien[Index].StartX - _Linien[Index - 1].EndX));
                    AbstandY = (Math.Abs(_Linien[Index].StartY - _Linien[Index - 1].EndY));

                    if (((AbstandX > 0D) || (AbstandY > 0D)) &&
                        ((AbstandX < MaxAbstand) && (AbstandY < MaxAbstand))) {


                        _Linien.Insert(Index,
                            new HPGLElemente.Linie(
                                _Linien[Index - 1].EndX,
                                _Linien[Index - 1].EndY,
                                _Linien[Index].StartX,
                                _Linien[Index].StartY,
                                _Linien[Index].Pen)
                                );
                        
                        ++NeueLinien;
                    }

                }


                return NeueLinien;
            } else {
                return 0;
            }
        }
    }
}
