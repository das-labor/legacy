using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

/*  PlotterCom - Tools for HPGL and plotters.
 *  HPGLViewer.cs - A HPGL viewer.
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
    public partial class HPGLViewer : UserControl {

        // TODO: 
        // - Skalieren implementieren.
        // - Button skalieren mit an und aus schalten.
        // - Optimieren nach Stift, nicht global.

        private System.Collections.Generic.List<PlotterCom.HPGLElemente.Linie> _Linien = null;
        private HPGL _HPGL = null;

        private Color _Hintergrundfarbe = Color.FromArgb(250, 250, 230);
        private Color _FarbeStift1 = Color.Black;
        private Color _FarbeStift2 = Color.Red;
        private Color _FarbeStift3 = Color.Green;
        private Color _FarbeStift4 = Color.Blue;
        private Color _FarbeStift5 = Color.Yellow;
        private Color _FarbeStift6 = Color.Violet;
        private Color _FarbeStift7 = Color.Brown;
        private Color _FarbeStift8 = Color.White;

        private System.Drawing.Drawing2D.SmoothingMode _ZeichenQualität = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
        private bool _AnsichtSkalieren = false;

        private float PlotterMinX = 0F;
        private float PlotterMinY = 0F;
        private float PlotterMaxX = 0F;
        private float PlotterMaxY = 0F;

        public Color Hintergrundfarbe {
            get {
                return _Hintergrundfarbe;
            }
            set {
                _Hintergrundfarbe = value;
            }
        }

        public System.Drawing.Drawing2D.SmoothingMode ZeichenQualität {
            get {
                return _ZeichenQualität;
            }
            set {
                _ZeichenQualität = value;
                PlotterCom.StaticLogger.Log("Setze ZeichenQualität auf " + _ZeichenQualität.ToString() + ".", 6);
                this.Invalidate();
            }
        }

        public bool AnsichtSkalieren {
            get { return _AnsichtSkalieren; }
            set { 
                _AnsichtSkalieren = value;
                Invalidate();
            }
        }
        
        
        
        // TODO: Get- / Set-Methoden für die Stiftfarben.

        public HPGLViewer() {
            PlotterCom.StaticLogger.Log("Viewer wird initialisiert!", 8);
            // Den Style setzen, damit die Anzeige nicht flackert.

            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.DoubleBuffer |
                ControlStyles.OptimizedDoubleBuffer,
                true);

            InitializeComponent();
            UpdateConfigManagerWerte();
        }

        public void ZeigeHPGL(PlotterCom.HPGL GrafikCode) {
            PlotterCom.StaticLogger.Log("Erhalte HPGL-Objekt!", 6);
            UpdateConfigManagerWerte();
            _Linien = GrafikCode.Linien;
            _HPGL = GrafikCode;

            _HPGL.BerechnetMinMaxNeu(); // Zur Sicherheit!
            this.Invalidate();

        }

        
        private void UpdateConfigManagerWerte() {
            PlotterCom.StaticLogger.Log("Lese Werte des ConfigManagers neu ein!", 8);

            // Lese die Werte neu ein, so dass die float-Umwandlung hier statt findet!
            PlotterMinX = (float) PlotterCom.ConfigManager.Config.PlotterMinX;
            PlotterMinY = (float) PlotterCom.ConfigManager.Config.PlotterMinY;
            PlotterMaxX = (float) PlotterCom.ConfigManager.Config.PlotterMaxX;
            PlotterMaxY = (float) PlotterCom.ConfigManager.Config.PlotterMaxY;
            
        }

        protected override void OnPaint(PaintEventArgs e) {
            Graphics grfx = e.Graphics;
            SolidBrush HintergrundBrush = new SolidBrush(_Hintergrundfarbe);
            SolidBrush ControlHintergrundBrush = new SolidBrush(this.BackColor);

            Pen Stift1 = new Pen(_FarbeStift1, 0F);
            Pen Stift2 = new Pen(_FarbeStift2, 0F);
            Pen Stift3 = new Pen(_FarbeStift3, 0F);
            Pen Stift4 = new Pen(_FarbeStift4, 0F);
            Pen Stift5 = new Pen(_FarbeStift5, 0F);
            Pen Stift6 = new Pen(_FarbeStift6, 0F);
            Pen Stift7 = new Pen(_FarbeStift7, 0F);
            Pen Stift8 = new Pen(_FarbeStift8, 0F);

            Pen ZeichenStift = Stift1;

            System.Drawing.Drawing2D.Matrix ZeichenMatrix = null;
            float Skalierung = 0F;

            grfx.SmoothingMode = _ZeichenQualität;

            grfx.FillRectangle(ControlHintergrundBrush, this.ClientRectangle);

            ZeichenMatrix = new System.Drawing.Drawing2D.Matrix();


            if (_AnsichtSkalieren) {

                Skalierung = (float)Math.Min(
                    ((float)this.Width - 6F) / (_HPGL.MaximaleXKoordinate - _HPGL.MinimaleXKoordinate),
                    ((float)this.Height - 6F) / (_HPGL.MaximaleYKoordinate - _HPGL.MinimaleYKoordinate)
                    );


                ZeichenMatrix.Translate(
                    (((float)_HPGL.MinimaleXKoordinate * Skalierung * -1F) + 3F), 
                    ( (float)this.Height - 3F) + ((float)_HPGL.MinimaleYKoordinate * Skalierung)
                );

            } else {

                Skalierung = Math.Min(
                    ((float)this.Width - 6F) / (PlotterMaxX - PlotterMinX),
                    ((float)this.Height - 6F) / (PlotterMaxY - PlotterMinY)
                    );

                ZeichenMatrix.Translate(3, this.Height - 3);
            }


            ZeichenMatrix.Scale(Skalierung, -Skalierung);
            grfx.Transform = ZeichenMatrix;

                grfx.FillRectangle(HintergrundBrush,
                    PlotterMinX,
                    PlotterMinY,
                    PlotterMaxX,
                    PlotterMaxY);

                grfx.DrawRectangle(Stift1,
                    PlotterMinX,
                    PlotterMinY,
                    PlotterMaxX,
                    PlotterMaxY);

            if (_Linien != null) {
                foreach (HPGLElemente.Linie ZeichenLinie in _Linien) {

                    switch (ZeichenLinie.Pen) {
                        case 1:
                            ZeichenStift = Stift1;
                            break;
                        case 2:
                            ZeichenStift = Stift2;
                            break;
                        case 3:
                            ZeichenStift = Stift3;
                            break;
                        case 4:
                            ZeichenStift = Stift4;
                            break;
                        case 5:
                            ZeichenStift = Stift5;
                            break;
                        case 6:
                            ZeichenStift = Stift6;
                            break;
                        case 7:
                            ZeichenStift = Stift7;
                            break;
                        case 8:
                            ZeichenStift = Stift8;
                            break;
                    }

                    grfx.DrawLine(
                        ZeichenStift,
                        (float)ZeichenLinie.StartX,
                        (float)ZeichenLinie.StartY,
                        (float)ZeichenLinie.EndX,
                        (float)ZeichenLinie.EndY
                        );
                }
            }

        }

        private void HPGLViewer_SizeChanged(object sender, EventArgs e) {
            this.Invalidate();
        }

    }
}
