using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace ExposerControl
{
    public partial class MainWindow : Form
    {
        Bitmap bmp;
        Bitmap scaledBmp;

        ComHandler comHandler;

        MachineData machineData;

        Timer statusTimer   = new Timer();
        Timer exposureTimer = new Timer();

        public MainWindow()
        {
            InitializeComponent();

            comHandler = new ComHandler();
            machineData = new MachineData();

            comHandler.onStatusRead += new IntDelegate(comHandler_onStatusRead);
            comHandler.onCommunicationError += new SimpleDelegate(comHandler_onCommunicationError);

            statusTimer.Tick += new EventHandler(statusTimer_Tick);
            statusTimer.Interval = 100;
            statusTimer.Start();

            exposureTimer.Tick += new EventHandler(exposureTimer_Tick);
        }

        
        #region COMMUNICATION_ERROR

        void onCommunicationError() {
            MessageBox.Show("Communication Error, Port closed.", "Communication Error");
            connectButton.Text = "connect";
        }

        void comHandler_onCommunicationError()
        {
            Invoke(new SimpleDelegate(onCommunicationError  ));
        }

        #endregion

        #region MACHINE_STATUS_READ_AND_EVENT_CALLING

        void statusTimer_Tick(object sender, EventArgs e)
        {
            if(comHandler.connected)
                comHandler.getStatus();
        }

        bool doorClosed;
        bool servoLocked;
        bool stepperReady;

        SimpleDelegate onMachineReady;
        SimpleDelegate onMachineNotReady;
        SimpleDelegate onStepperReady;

        void onStatusRead(int status) {
            doorClosed              = (status & 0x04) != 0;
            servoLocked             = (status & 0x02) != 0;
            stepperReady            = (status & 0x01) != 0;
            bool stepperCommandDone = (status & 0x08) != 0;

            doorCheckBox.Checked         = doorClosed;
            servoLockedCheckBox.Checked  = servoLocked;
            stepperReadyCheckBox.Checked = stepperReady;

            if (doorClosed && servoLocked && stepperReady)
            {
                if (onMachineReady != null)
                {
                    onMachineReady();
                }
            }
            else {
                if (onMachineNotReady != null)
                {
                    onMachineNotReady();
                }
            }
            
            if (onStepperReady != null) {
                if (stepperCommandDone) {
                    onStepperReady();
                }
            }

        }

        void comHandler_onStatusRead(int theint)
        {
            Invoke(new IntDelegate(onStatusRead), theint);
        }

        #endregion

        #region FILE_OPEN_AND_READ

        private void openImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openImageFileDialog.ShowDialog();
        }


        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openPdfDialog.ShowDialog();

        }

        private void openImageFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            bmp = new Bitmap(openImageFileDialog.FileName);
            resetThumbnail();

            updateTimeEstimate();
        }


        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            Process p = new Process();
            // Redirect the output stream of the child process.
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = "pdftoppm";
            p.StartInfo.Arguments = "-mono -f 1 -l 1 -r 600 " + openPdfDialog.FileName + " C:/Windows/Temp/";
            p.Start();
            // Do not wait for the child process to exit before
            // reading to the end of its redirected stream.
            // p.WaitForExit();
            // Read the output stream first and then wait.
            string output = p.StandardOutput.ReadToEnd();
            p.WaitForExit();


            FileInfo f = new FileInfo("c:\\Windows\\Temp\\-000001.pbm");
            StreamReader s = f.OpenText();

            
            if (s.ReadLine() != "P4") {
                return;
            }

            string wh;
            wh = s.ReadLine();
            s.Close();

            int file_offset = wh.Length + 4;

            string [] wha = wh.Split(' ');

            int width = int.Parse(wha[0]);
            int height = int.Parse(wha[1]);

            int linebytes = (width + 7)/ 8 ;
            int numbytes = height * linebytes;


            

            byte[] rawpbm = new byte[numbytes];
            FileStream fs = f.OpenRead();
            fs.Seek(file_offset, SeekOrigin.Begin);
            fs.Read(rawpbm, 0, numbytes);
            fs.Close();

            int start_line = 0;
            int end_line = 0;

            bool find_start = true ;

            int x,y;
            for (y = 0; y < height; y++)
            {
                bool line_used = false;
                for (x = 0; x < linebytes; x++){
                    if(rawpbm[linebytes * y + x] != 0){
                        line_used = true;
                    }
                }

                if (find_start)
                {
                    if (line_used)
                    {
                        find_start = false;
                        start_line = y;
                    }
                }
                else {
                    if (!line_used) {
                        end_line = y;
                        break;
                    }
                }
            }


            int start_linebyte = 0;
            int end_linebyte = 0;


            for (x = 0; x < linebytes; x++)
            {
                if (rawpbm[linebytes * start_line + x] != 0)
                {
                    start_linebyte = x;
                    break;
                }
            }

            for (x = linebytes - 1; x > 0; x--)
            {
                if (rawpbm[linebytes * start_line + x] != 0)
                {
                    end_linebyte = x+1;
                    break;
                }
            }

            int bmp_height = end_line - start_line;
            int bmp_width = (end_linebyte - start_linebyte) * 8;

            double height_mm = (double)bmp_height / (600.0 / 25.4);
            double width_mm = (double)bmp_width / (600.0 / 25.4);

            sizeLabel.Text = string.Format("{0:0.0}mm x {1:0.0}mm", width_mm, height_mm);
            
            bmp = new Bitmap(bmp_width, bmp_height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            
            for (y = 0; y < bmp_height; y++)
            {



                for (x = 0; x < bmp_width; x++)
                {
                    int offset = (start_line + y) * linebytes * 8 + (start_linebyte * 8) + x;
                    int foo = rawpbm[offset / 8];
                    int modu = offset % 8;

                    if ((rawpbm[offset / 8] & (0x80 >> (offset%8))) == 0  )
                    {
                        bmp.SetPixel(x, y, Color.White);
                    }
                    else {
                        bmp.SetPixel(x, y, Color.Black);
                    }
                }
            }

            resetThumbnail();
            
            updateTimeEstimate();
        }

        void resetThumbnail() {
            scaledBmp = new Bitmap(bmp, bmp.Width / 4, bmp.Height / 4);

            pictureBox1.Image = scaledBmp;
            pictureBox1.Update();
        }

        bool oriantationVertical = false;

        byte[] getLineFromBmp(int line) {
            int b;
            byte val;

            byte[] data = new byte[480];

            if (oriantationVertical)
            {
                for (int x = 0; x < (bmp.Width - (bmp.Width % 8)); x += 8)
                {
                    val = 0;
                    for (b = 0; b < 8; b++)
                    {
                        val <<= 1;
                        if (bmp.GetPixel(x + b, line).G > 128)
                        {

                            scaledBmp.SetPixel((x + b)/4, line/4, Color.Blue);
                            val |= (byte)0x01;
                        }
                    }
                    data[(x / 8)] = val;
                }

            }
            else {
                for (int y = 0; y < (bmp.Height - (bmp.Height % 8)); y += 8)
                {
                    val = 0;
                    for (b = 0; b < 8; b++)
                    {
                        val <<= 1;
                        if (bmp.GetPixel(line, y + b).G > 128)
                        {
                            try
                            {
                                //bmp.SetPixel(line, y + b,  Color.Blue);
                                scaledBmp.SetPixel(line / 4, (y + b) / 4, Color.Blue);
                            }
                            catch (System.ArgumentOutOfRangeException) { 
                            }
                            
                            val |= (byte)0x01;
                        }
                    }
                    data[(y / 8)] = val;
                }
            }

            pictureBox1.Refresh();

            return data;
        }

        #endregion

        #region EXPOSURE


        int currentLine;
        int elapsed_time;

        enum ExposureState {idle, starting, running};
        ExposureState exposureState = ExposureState.idle;
        
        void exposureTimer_Tick(object sender, EventArgs e)
        {
            if (currentLine >= bmp.Width)
            {
                exposureReset();
            }
            else
            {
                byte[] data = getLineFromBmp(currentLine);

                byte[] tdata = machineData.translateLine(data);

                comHandler.sendData(tdata);

                comHandler.sendStepper(-1);

                lineNumericUpDown.Value = currentLine;
                currentLine++;
                elapsed_time += exposureTimer.Interval;
                setTimerLabel(elapsed_time / 1000);
            }
        }


        bool checkConnectionAndBmp()
        {
            if (comHandler.connected == false)
            {
                MessageBox.Show("Please connect first!");
                return false;
            }
            if (bmp == null) {
                MessageBox.Show("Please load a Bitmap!");
                return false;
            }

            return true;
        }


        void exposureReset()
        {
            if (comHandler.connected == false)
            {
                MessageBox.Show("Please connect first!");
                return;
            }
            exposureState = ExposureState.idle;
            exposureTimer.Stop();
            comHandler.sendLaser(false);
            comHandler.sendScanner(false);
            comHandler.sendStepper(0);//go home
            onStepperReady = null;
            onMachineReady = null;
            statusTimer.Start();
            continueButton.Text = "Continue";
        }

        void exposurePause() {
            if (exposureState == ExposureState.running)
            {
                exposureState = ExposureState.idle;
                exposureTimer.Stop();
                comHandler.sendLaser(false);
                comHandler.sendScanner(false);
                statusTimer.Start();
                continueButton.Text = "Continue";
                onMachineNotReady = null;
            }
        }

        void exposureContinue() {
            if (checkConnectionAndBmp())
            {
                if (exposureState == ExposureState.idle)
                {
                    exposureTimer.Interval = (int)delayNumeric.Value;
                    exposureState = ExposureState.starting;
                    currentLine = (int)lineNumericUpDown.Value;
                    comHandler.sendScanner(true);
                    onMachineReady = goMachineReady;
                    continueButton.Text = "Pause";
                }
            }
        }

        void exposureGo()
        {
            if (checkConnectionAndBmp())
            {
                if (exposureState == ExposureState.idle)
                {
                    exposureTimer.Interval = (int)delayNumeric.Value;
                    resetThumbnail();
                    exposureState = ExposureState.starting;
                    currentLine = 0;
                    comHandler.sendScanner(true);
                    comHandler.sendStepper(0);//go home
                    onStepperReady = stepperIsHome;
                    continueButton.Text = "Pause";
                }
            }
        }

        void stepperIsHome() {
            comHandler.sendStepper(machineData.stepperStartPos );//go home
            onStepperReady = stepperIsStart;
        }

        void stepperIsStart() {
            onStepperReady = null;
            onMachineReady = goMachineReady;
        }

        void goMachineReady()
        {
            exposureState = ExposureState.running;
            comHandler.sendLaser(true);
            onMachineReady = null;
            onMachineNotReady = exposurePause;
            elapsed_time = 0;
            statusTimer.Stop();
            exposureTimer.Start();
        }

        private void goButton_Click(object sender, EventArgs e)
        {
            exposureGo();
        }

       
        private void continueButton_Click(object sender, EventArgs e)
        {
            if (continueButton.Text == "Continue")
            {
                exposureContinue ();
            }
            else {
                exposurePause();
            }
        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            exposureReset();
        }

        #endregion

        private void delayNumeric_ValueChanged(object sender, EventArgs e)
        {
            exposureTimer.Interval = (int)delayNumeric.Value;
            updateTimeEstimate();
        }

        void updateTimeEstimate() {
            if (bmp != null)
            {
                int est = (bmp.Width * ((int)delayNumeric.Value)) / 1000;

                timeEstimateLabel.Text = string.Format("{0:00}:{1:00}", est / 60, est % 60);
            }
        }

        void setTimerLabel(int s) {
            timerLabel.Text = string.Format("{0:00}:{1:00}", s / 60, s % 60);
        }


        private void connectButton_Click(object sender, EventArgs e)
        {
            if (!comHandler.connected)
            {
                if (comHandler.GuiOpen(Properties.Settings.Default.ComPort))
                {
                    connectButton.Text  = "disconnect";
                }
            }
            else
            {
                comHandler.Close();
                connectButton.Text  = "connect";
            }
        }

        private void controlButton_Click(object sender, EventArgs e)
        {
            ControlDialog d = new ControlDialog(comHandler);
            d.Show();
        }

        private void editCalibrationButton_Click(object sender, EventArgs e)
        {
            Calibrator_Form cf = new Calibrator_Form(comHandler, machineData);
            cf.Show();
        }

        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            comHandler.Close();
        }




        
      
    }
}
