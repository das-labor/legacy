using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ExposerControl
{
    public class MachineData
    {

        public int startDelay;

        public int[] delaySettings = new int[40];

        public int stepperStartPos;

        public MachineData() {
            readCalibratiomnFromFile();
            generateTimingData();
        }

        public void readCalibratiomnFromFile() {
            int idx = 0;
            try
            {
                string[] cnf = System.IO.File.ReadAllLines("calib.txt");
                startDelay = int.Parse(cnf[idx++]);

                for (int x = 0; x < 40; x++)
                {
                    delaySettings[x] = int.Parse(cnf[idx++]);
                }

                stepperStartPos = int.Parse(cnf[idx++]);
            
            }
            catch (System.IO.FileNotFoundException) {
                MessageBox.Show("No calibration file found - using defaults.");
                startDelay = 19500;
                for (int x = 0; x < 40; x++)
                {
                    delaySettings[x] = 160;
                }
                stepperStartPos = 295;
            }
            

        }

        public void saveCalibrationToFile() {
            int idx = 0;
            string[] cnf = new string[42];

            cnf[idx++] = string.Format("{0:d}", startDelay );

            for (int x = 0; x < 40; x++)
            {
                cnf[idx++] = string.Format("{0:d}", delaySettings[x] );
            }

            cnf[idx++] = string.Format("{0:d}", stepperStartPos);

            System.IO.File.WriteAllLines("calib.txt", cnf);
        
        }

        public byte[] generatePattern()
        {
            byte[] p = new byte[480];

            //put 4 pixels wide lines every 100mil
            for (int x = 0; x < 20; x++)
            {
                p[x * 15    ] = 0xf0;
                p[x * 15 + 7] = 0x0f;
            }

            return translateLine (p);
        }

        byte[] timingData;

        public void generateTimingData()
        {
            timingData = new byte[480];
            byte[] tbl = timingData;
            /*
            int begin_delay = startDelay / 8;
            byte tmp = (byte)(begin_delay / 32);
            for (x = 0; x < 31; x++)
            {
                tbl[x] = tmp;
            }
            begin_delay -= ((int)tmp) * 31;
            tbl[31] = (byte)begin_delay;
            */

            int x;
  
            int pos = 0;
            int finepos = 0; // pos = finepos / 8
            int tbl_cnt = 0;

            for (x = 0; x < 40; x++)
            {
                int count;
                if ((x & 0x01) == 0)
                {
                    count = 8;
                }
                else
                {
                    count = 7;
                }

                for (int y = 0; y < count; y++)
                {
                    finepos += delaySettings[x];
                    int oldpos = pos;
                    pos = finepos / 8;
                    tbl[tbl_cnt++] = (byte)(pos - oldpos);
                }
            }

            for (; tbl_cnt < 480; tbl_cnt++)
            {
                tbl[tbl_cnt] = 0;
            }
        }

        public byte[] translateLine(byte[] line_600dpi) {
            //array has 480 bytes, each represents 8 pixels, msb left
            //translates 600dpi lines to 1/16MHz delays between changes

            int min_delay = 30;

            int[] delays = new int[512];

            int delay_akk = 0;
            int last_change = 0;

            bool aktval = false;
            int dp = 0;

            int x;
            for (x = 0; x < 480; x++) {
                int bit;
                for (bit = 7; bit >= 0; bit--) {
                    bool val = (line_600dpi[x] & (1 << bit)) != 0;
                    if(val != aktval) {
                        aktval = val;
                        delays[dp++] = (delay_akk / 2) - last_change;
                        last_change = (delay_akk / 2);
                    }
                    delay_akk += timingData[x];
                }
            }

            int subfromnext = 0;
            byte[] ret = new byte[512];
            int rp = 0;

            for (x = 0; x < dp; x++) {
                int tmp = delays[x];
                tmp -= subfromnext;
                if (tmp < min_delay)
                {
                    subfromnext = min_delay - tmp;
                    tmp = min_delay;
                }
                else {
                    subfromnext = 0;
                }

                if (tmp < 256)
                {
                    ret[rp++] = (byte)tmp;
                }
                else {
                    ret[rp++] = 0;
                    ret[rp++] = (byte)(tmp & 0xff);
                    ret[rp++] = (byte)(tmp >> 8);
                }
            }

            ret[rp++] = 0; //end mark
            ret[rp++] = 0;
            ret[rp++] = 0;



            return ( ret.Take(rp).ToArray() );
        }
    }
}
