using System;
using System.Collections.Generic;
using System.Text;

/*  PlotterCom - Tools for HPGL and plotters.
 *  Linie.cs - Object for HPGL line management.
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

namespace PlotterCom.HPGLElemente {
    public class Linie {

        public Linie() {
        }

        public Linie(double VonX, double VonY, double BisX, double BisY, UInt16 Stift) {
            StartX = VonX;
            StartY = VonY;
            EndX = BisX;
            EndY = BisY;

            Pen = Stift;
        }

        public void DreheUm() {
            double temp;

            temp = StartX;
            StartX = EndX;
            EndX = temp;

            temp = StartY;
            StartY = EndY;
            EndY = temp;

        }

        
        
        public double StartX = 0;
        public double StartY = 0;

        public double EndX = 0;
        public double EndY = 0;

        public UInt16 Pen = 0;

        public bool Optimiert = false;
        public bool Dublette = false;
    }
}
