using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Verschieben.cs - Move all HPGL-coordinates.
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
    public partial class Verschieben : Form {

        public HPGL HPGLCode = null;

        public Verschieben() {
            InitializeComponent();
        }

        private void button_Abbruch_Click(object sender, EventArgs e) {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void button_OK_Click(object sender, EventArgs e) {
            PlotterCom.StaticLogger.Log("Benutzer hat OK geklickt!", 6);
            double DeltaXmm = 0D;
            double DeltaYmm = 0D;
            bool WerteOK = false;

            try {
                DeltaXmm = System.Double.Parse(textBox_VerschX.Text.Replace("m", String.Empty));
                DeltaYmm = System.Double.Parse(textBox_VerschY.Text.Replace("m", String.Empty));
                WerteOK = true;
            } catch (Exception ex) {
                PlotterCom.StaticLogger.Log("Kann Werte in den Textboxen nicht lesen!", 4);
                PlotterCom.StaticLogger.Log("Meldung: " + ex.Message, 4);
                WerteOK = false;
            }


            if (WerteOK) {
                PlotterCom.StaticLogger.Log("Verschiebe um " + DeltaXmm + "mm x " + DeltaYmm + "mm.", 6);

                HPGLCode.VerschiebeAlleLinien(DeltaXmm / 0.025D, DeltaYmm / 0.025D);
                HPGLCode.BerechnetMinMaxNeu();

                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            } else {
                MessageBox.Show("Kann die eingegebenen Werte nicht interpretieren!",
                    "Fehler!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void checkBox_Ursprung_CheckedChanged(object sender, EventArgs e) {
            if (checkBox_Ursprung.Checked) {
                textBox_VerschX.ReadOnly = true;
                textBox_VerschY.ReadOnly = true;

                if (HPGLCode != null) {
                    double DeltaX = 0D;
                    double DeltaY = 0D;

                    DeltaX = (PlotterCom.ConfigManager.Config.PlotterMinX - HPGLCode.MinimaleXKoordinate);
                    DeltaY = (PlotterCom.ConfigManager.Config.PlotterMinY - HPGLCode.MinimaleYKoordinate);

                    DeltaX *= 0.025D;
                    DeltaY *= 0.025D;

                    // Damit die Zeichnung nicht ganz in der Ecke hängt...
                    DeltaX += 1D;
                    DeltaY += 1D;

                    textBox_VerschX.Text = DeltaX.ToString("0.0000") + "mm";
                    textBox_VerschY.Text = DeltaY.ToString("0.0000") + "mm";

                }

            } else {
                textBox_VerschX.ReadOnly = false;
                textBox_VerschY.ReadOnly = false;
            }
        }

        
    }
}
