using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  ViwerForm.cs - Form for the HPGL viewer.
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
    public partial class ViewerForm : Form {
        public ViewerForm() {
            InitializeComponent();
        }

        public HPGL Code {
            set {
                hpglViewer.ZeigeHPGL(value);

                if (value != null) {
                    toolStripStatusLabel_Unten.Text = value.Linien.Count.ToString() + " Linien werden angezeigt!";
                }
            }
        }

        private void toolStripButton_Beenden_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void toolStripButton_Scale_Click(object sender, EventArgs e) {
            toolStripButton_Scale.Checked = !toolStripButton_Scale.Checked;

            if (toolStripButton_Scale.Checked) {
                hpglViewer.AnsichtSkalieren = true;
            } else {
                hpglViewer.AnsichtSkalieren = false;
            }
        }

        private void toolStripButton_HQ_Click(object sender, EventArgs e) {
            toolStripButton_HQ.Checked = !toolStripButton_HQ.Checked;

            if (toolStripButton_HQ.Checked) {
                hpglViewer.ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            } else {
                hpglViewer.ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;
            }

            
        }

        private void ViewerForm_Load(object sender, EventArgs e) {

        }

        private void ViewerForm_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            if (e.KeyCode == Keys.Escape) {
                this.Close();
            }
        }

        private void ViewerForm_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Escape) {
                this.Close();
            }
            
        }

        private void ViewerForm_Shown(object sender, EventArgs e) {
            if (PlotterCom.ConfigManager.Config.ViewerHQ) {
                toolStripButton_HQ.Checked = true;
                hpglViewer.ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            } else {
                toolStripButton_HQ.Checked = false;
                hpglViewer.ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;
            }

            if (PlotterCom.ConfigManager.Config.ViewerSkalieren) {
                toolStripButton_Scale.Checked = true;
                hpglViewer.AnsichtSkalieren = true;
            } else {
                toolStripButton_Scale.Checked = false;
                hpglViewer.AnsichtSkalieren = false;
            }
        }

    }
}
