using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

/*  PlotterCom - Tools for HPGL and plotters.
 *  StatiticLogger.cs - The logging-system for PlotterCom.
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
    public class StaticLogger {

        /* LogLevel:
         * 0 = Logging abgeschaltet, Meldung wird nicht angezeigt
         * 1 = Kritische Fehler und Programmabbrüche.
         * 2 = Schwere Fehler.
         * 3 = Normale Programmfehler.
         * 4 = Weniger schwerwiegende Fehler.
         * 5 = Wichtige Hinweise.
         * 6 = Alle Hinweise.
         * 7 = Hinweise zum Programmablauf und alle SQL-Strings
         * 8 = Fast alle Meldungen, ohne sehr Testlastige Ausgaben.
         * 9 = Wirklich alle Debug-Ausgaben.
         */

        private static string _logfileFolder = String.Empty;
        private static string _logfileName = "Log_{0:d}.log";
        private static Stream _outStream = null;
        private static StreamWriter _writer = null;
        private static int _logFileDay = -1;
        private static bool _offline = true;

        public static int LogLevel = 9;
        public static bool ZeigeCallerName = true;

        public static string LogFileName {
            get {
                return _logfileName;
            }
            set {
                _logfileName = value;
            }
        }

        public static string LogFileFolder {
            get {
                return _logfileFolder;
            }
            set {
                if (System.IO.Directory.Exists(value)) {
                    _logfileFolder = value;
                } else {
                    System.Windows.Forms.MessageBox.Show("Der Log-Ordner " + value + " existiert nicht!\n" + 
                        "Das Log-System wird deaktiviert.",
                        "Fehler im Log-System!",
                        System.Windows.Forms.MessageBoxButtons.OK,
                        System.Windows.Forms.MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// Schreibt eine Logmessage in das Logfile.
        /// </summary>
        /// <param name="message">Die Nachricht, die in das Logfile geschrieben werden soll.</param>
        /// <param name="MessageLogLevel">Das Loglevel:
        /// * 0 = Logging abgeschaltet, Meldung wird nicht angezeigt
        /// * 1 = Kritische Fehler und Programmabbrüche.
        /// * 2 = Schwere Fehler.
        /// * 3 = Normale Programmfehler.
        /// * 4 = Weniger schwerwiegende Fehler.
        /// * 5 = Wichtige Hinweise.
        /// * 6 = Alle Hinweise.
        /// * 7 = Hinweise zum Programmablauf und alle SQL-Strings
        /// * 8 = Fast alle Meldungen, ohne sehr Testlastige Ausgaben.
        /// * 9 = Wirklich alle Debug-Ausgaben.
        /// </param>
        public static void Log(string message, int MessageLogLevel) {
            // Die Kultur sichern und auf DE umstellen, damit die Zeitangaben einheitlich sind!
            System.Globalization.CultureInfo aktuelleKultur = System.Windows.Forms.Application.CurrentCulture;
            System.Windows.Forms.Application.CurrentCulture = new System.Globalization.CultureInfo("de-DE");

            if (!_offline) {
                if (_outStream == null) {
                    _logFileDay = DateTime.Now.Day;
                    try {
                        _outStream = File.Open(System.IO.Path.Combine(
                            _logfileFolder, String.Format(_logfileName, DateTime.Now)),
                            FileMode.Append,
                            FileAccess.Write,
                            FileShare.Read);
                    } catch (Exception e) {
                        _offline = true;
                        System.Windows.Forms.MessageBox.Show("Logger kann seine Ausgabedatei nicht öffnen!\n" +
                            e.Message + "\nLog-System wird deaktiviert!", 
                        "Fehler im Log-System!",
                        System.Windows.Forms.MessageBoxButtons.OK,
                        System.Windows.Forms.MessageBoxIcon.Error);

                        return;
                    }
                    if (_writer == null) {
                        _writer = new StreamWriter(_outStream);
                        _writer.Write(
                                DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                                System.Environment.UserName + " " +
                                System.Environment.MachineName + " " +
                                "PlotterCom.StaticLogger.Log(): ");
                        _writer.WriteLine("Logger wird initialisiert!");
                        _writer.Write(
                                DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                                System.Environment.UserName + " " +
                                System.Environment.MachineName + " " +
                                "PlotterCom.StaticLogger.Log(): ");
                        _writer.WriteLine("LogLevel: " + LogLevel.ToString());
#if DEBUG
                        _writer.Write(
                                DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                                System.Environment.UserName + " " +
                                System.Environment.MachineName + " " +
                                "PlotterCom.StaticLogger.Log(): ");
                        _writer.WriteLine("Logger befidet sich im DEBUG-Mode!");
#endif
                        _writer.Flush();
                    }
                }

                if (_logFileDay != DateTime.Now.Day) {
                    _writer.Close();
                    _outStream.Close();
                    _logFileDay = DateTime.Now.Day;
                    _outStream = File.Open(System.IO.Path.Combine(
                        _logfileFolder, String.Format(_logfileName, DateTime.Now)),
                        FileMode.Append,
                        FileAccess.Write,
                        FileShare.Read);
                    _writer = new StreamWriter(_outStream);
                    _writer.Write(
                            DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                            System.Environment.UserName + " " +
                            System.Environment.MachineName + " " +
                            "PlotterCom.StaticLogger.Log(): ");
                    _writer.WriteLine("Logger wechselt das Logfile!");
                    _writer.Flush();
                }

                if ((MessageLogLevel <= LogLevel) && (MessageLogLevel > 0)) {
                    System.Diagnostics.StackTrace st;

                    if (ZeigeCallerName) {
                        st = new System.Diagnostics.StackTrace(1);
                    } else {
                        st = null;
                    }

                    if (st != null) {
                        System.Diagnostics.StackFrame sf = st.GetFrame(0);

                        if (sf != null) {
                            _writer.Write(
                                DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                                System.Environment.UserName + " " +
                                System.Environment.MachineName + " " +
                                sf.GetMethod().DeclaringType.FullName + "." + sf.GetMethod().Name + "(): ");
                        }
                    } else {
                        _writer.Write(
                            DateTime.Now.ToLongTimeString() + "." + DateTime.Now.Millisecond.ToString("000") + " " +
                            System.Environment.UserName + " " +
                            System.Environment.MachineName + " ");
                    }
                    _writer.WriteLine(message);
                    _writer.Flush();
                } // logLevel
            } // if (offline)

            // Aktuelle KulturInformationen wiederherstellen.
            System.Windows.Forms.Application.CurrentCulture = aktuelleKultur;
        }

        /// <summary>
        /// Schreibt eine LogMessage in die Logdatei. Diese Funktion sollte nicht benutzt werden, da kein LogLevel übergeben
        /// wird. Es wird das LogLevel 1 angenommen.
        /// </summary>
        /// <param name="message">Die Nachricht, die in die LogDatei geschrieben werden soll.</param>
        public static void Log(string message) {
            // Wird kein Loglevel angegeben, wir Loglevel 1 angenommen.
            Log(message, 1);
        }


        public static void LogDateiNeuÖffnen() {
            Log("Logger-Konfiguration geändert, Neuinitialisierung wird angefordert!", 5);
            //_offline = false;
            _logFileDay = -1;
            Log("Logger-Konfiguration geändert, Neuinitialisierung wurde angefordert!", 5);
        }

        public static void Initialisiere() {
            _offline = false;
            Log("StaticLogger wurde erfolgreich initialisiert!");
        }

        /// <summary>
        /// Initialisiert den StaticLogger.
        /// </summary>
        /// <param name="LogOrdner">Gibt den Ordner an, in dem die Logdatei angelegt werden soll.</param>
        public static void Initialisiere(string LogOrdner) {
            _offline = false;
            LogFileFolder = LogOrdner;
            Log("StaticLogger wurde erfolgreich initialisiert!");
        }


    }
    
}
