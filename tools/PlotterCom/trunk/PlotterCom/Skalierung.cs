using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Skalierung.cs - Scale HPGL-files.
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

namespace PlotterCom
{
    public partial class Skalierung : Form {

        public double Skalierungsfaktor = 100D;
        public HPGL HPGLCode = null;

        // Todo: Absolute größe (Schrittweite 0,025mm)
        // Schrittweite in Config
        public Skalierung() {
            InitializeComponent();
        }

        private void trackBarSkalierung_Scroll(object sender, EventArgs e) {
            double Breite, Höhe;

            if (trackBar_Skalierung.Value < 50) {
                Skalierungsfaktor = ((double)trackBar_Skalierung.Value * (2D - (1D / 5D)) + 10D);
            } else {
                Skalierungsfaktor = (((double)trackBar_Skalierung.Value - 50D) * (18D) + 100D);
            }

            textBox_Faktor.Text = Skalierungsfaktor.ToString("0.#") + "%";
            Skalierungsfaktor /= 100D;

            if (HPGLCode != null) {
                Breite = (HPGLCode.MaximaleXKoordinate - HPGLCode.MinimaleXKoordinate) * 0.025D * Skalierungsfaktor;
                Höhe = (HPGLCode.MaximaleYKoordinate - HPGLCode.MinimaleYKoordinate) * 0.025D * Skalierungsfaktor;
                textBox_AbsolutX.Text = Breite.ToString("#") + "mm";
                textBox_AbsolutY.Text = Höhe.ToString("#") + "mm";
            }


        }


        private void checkBox_Auto_CheckedChanged(object sender, EventArgs e) {
            trackBar_Skalierung.Enabled = !checkBox_Auto.Checked;
            label_MinSkalierung.Enabled = !checkBox_Auto.Checked;
            label_MaxSkalierung.Enabled = !checkBox_Auto.Checked;
            label_MitteSkalierung.Enabled = !checkBox_Auto.Checked;
            textBox_Faktor.ReadOnly = checkBox_Auto.Checked;

            if (HPGLCode == null) {
                return;
            }

            if (checkBox_Auto.Checked) {
                Skalierungsfaktor = Math.Min(
                    (PlotterCom.ConfigManager.Config.PlotterMaxX / (HPGLCode.MaximaleXKoordinate - HPGLCode.MinimaleXKoordinate)),
                    (PlotterCom.ConfigManager.Config.PlotterMaxY / (HPGLCode.MaximaleYKoordinate - HPGLCode.MinimaleYKoordinate)));

                if (Skalierungsfaktor < 0.1D) {
                    Skalierungsfaktor = 0.1D;
                }

                if (Skalierungsfaktor > 10D) {
                    Skalierungsfaktor = 10D;
                }


                textBox_Faktor.Text = (Skalierungsfaktor * 100D).ToString("0.##");
                TryParseFaktor();
            } else {
                textBox_Faktor.Text = "100%";
                TryParseFaktor();
            }
        }

        private void button_OK_Click(object sender, EventArgs e) {
            double VerschiebungX, VerschiebungY;


            if (HPGLCode != null) {

                VerschiebungX = HPGLCode.MinimaleXKoordinate;
                VerschiebungY = HPGLCode.MinimaleYKoordinate;

                if (checkBox_Auto.Checked) {
                    HPGLCode.VerschiebeAlleLinien(-VerschiebungX, -VerschiebungY);
                }

                HPGLCode.Skaliere(Skalierungsfaktor);

                HPGLCode.BerechnetMinMaxNeu();

                this.DialogResult = System.Windows.Forms.DialogResult.OK;
            } else {
                this.DialogResult = System.Windows.Forms.DialogResult.None;
            }

            this.Close();
        }

        private void Skalierung_Load(object sender, EventArgs e) {
            textBox_PlotterX.Text = ((PlotterCom.ConfigManager.Config.PlotterMaxX - ConfigManager.Config.PlotterMinX) * 0.025D).ToString("0mm");
            textBox_PlotterY.Text = ((PlotterCom.ConfigManager.Config.PlotterMaxY - ConfigManager.Config.PlotterMinY) * 0.025D).ToString("0mm");
        }

        private void textBox_Faktor_Leave(object sender, EventArgs e) {
            TryParseFaktor();
        }

        private void TryParseFaktor() {
            double Breite, Höhe;

            try {
                Skalierungsfaktor = System.Double.Parse(textBox_Faktor.Text.Replace("%", String.Empty));
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Konnte Skalierungsfaktor-Eingabe nicht lesen!", 5);
                PlotterCom.StaticLogger.Log("Fehlermeldung: " + ex.Message, 5);
                Skalierungsfaktor = 100D;
            } 

            if (Skalierungsfaktor < 10D) {
                Skalierungsfaktor = 10D;
            }

            if (Skalierungsfaktor > 1000D) {
                Skalierungsfaktor = 1000D;
            }

            textBox_Faktor.Text = Skalierungsfaktor.ToString("0.##") + "%";
            

            Skalierungsfaktor /= 100D;

            UpdateTrackBarValue();


            if (HPGLCode != null) {
                Breite = (HPGLCode.MaximaleXKoordinate - HPGLCode.MinimaleXKoordinate) * 0.025D * Skalierungsfaktor;
                Höhe = (HPGLCode.MaximaleYKoordinate - HPGLCode.MinimaleYKoordinate) * 0.025D * Skalierungsfaktor;
                textBox_AbsolutX.Text = Breite.ToString("#") + "mm";
                textBox_AbsolutY.Text = Höhe.ToString("#") + "mm";
            }
        }

        private void UpdateTrackBarValue() {
            double MeinSkalierungsFaktor;
            int TrackBarValue;

            if (Skalierungsfaktor < 0.1D) {
                 Skalierungsfaktor = 0.1D;
            } 

            if (Skalierungsfaktor > 10) {
                Skalierungsfaktor = 10;
            }
            
            MeinSkalierungsFaktor = Skalierungsfaktor * 100D;

            if (MeinSkalierungsFaktor < 100D) {
                TrackBarValue = (int)((MeinSkalierungsFaktor - 10D) / (2D - (1D / 5D)));
            } else {
                TrackBarValue = (int)((MeinSkalierungsFaktor - 100D) / 18D + 50D);
            }

            trackBar_Skalierung.Value = TrackBarValue;

        }

        private void button_Abbruch_Click(object sender, EventArgs e) {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }



        private void textBox_Faktor_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                TryParseFaktor();
            }
        }






    }
}
