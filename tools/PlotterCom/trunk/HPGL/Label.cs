using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Label.cs - Object for HPGL label management.
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
    public class Label {

        public Label(string LabelText, double PosX, double PosY, UInt16 Stift) {

            if (LabelText == null) {
                this.Text = String.Empty;
            } else {
                this.Text = LabelText;
            }

            this.StartX = PosX;
            this.StartY = PosY;
            this.Pen = Stift;
        }

        public string Text = String.Empty;

        public double Winkel = 0D;
        public double Größe = 1D;

        public double StartX = 0;
        public double StartY = 0;

        public UInt16 Pen = 0;

        public bool Optimiert = false;


    }
}
