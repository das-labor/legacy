using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ExposerControl
{
    public partial class Calibrator_Form : Form
    {
        ComHandler comHandler;

        NumericUpDown[] nuds;
        MachineData machineData;


        void copyClick(object o, EventArgs e) { 
            Button b = (Button)o;
            int i = (int)b.Tag; 
            nuds[i].Value = nuds[i-1].Value;
        }

        public Calibrator_Form(ComHandler h, MachineData m)
        {

            InitializeComponent();

            comHandler = h;
            machineData = m;

            nuds = new NumericUpDown[40];

            for (int x = 0; x < 40; x++) {
                
                Button b = new Button();
                b.Size = new Size(50, 20);
                b.Text = "copy";
                b.Click += new EventHandler(copyClick);
                b.Tag = x;
                
                if(x == 0) b.Visible = false;

                NumericUpDown nud = new NumericUpDown();
                nud.Size = new Size(60, 50);
                nud.Minimum = 8;
                nud.Maximum = 500;
                nud.Value = machineData.delaySettings[x];
                
                nud.ValueChanged += new EventHandler(numericUpdown_ValueChanged);

                nuds[x] = nud;
                
                SplitContainer spl = new SplitContainer();
                spl.Size = new Size(150, 20);
                spl.Panel1.Controls.Add(b);

                spl.Panel2.Controls.Add(nud);

                flowLayoutPanel1.Controls.Add(spl);
            }

            startNumericUpdown.Value = machineData.stepperStartPos;
        }

        private void startNumericUpdown_ValueChanged(object sender, EventArgs e)
        {
            machineData.stepperStartPos = (int)startNumericUpdown.Value;
        }

        private void numericUpdown_ValueChanged(object sender, EventArgs e)
        {
            for (int x = 0; x < 40; x++)
            {
                machineData.delaySettings[x] = (int)nuds[x].Value;
            }
            machineData.generateTimingData();
            byte[] tbl = machineData.generatePattern();
            comHandler.sendData(tbl);
        }

        private void saveCalibrationButton_Click(object sender, EventArgs e)
        {
            machineData.saveCalibrationToFile();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            machineData.readCalibratiomnFromFile();

        }

        private void patternButton_Click(object sender, EventArgs e)
        {
            byte[] tbl = machineData.generatePattern();
            comHandler.sendData(tbl);
        }

    }
}
