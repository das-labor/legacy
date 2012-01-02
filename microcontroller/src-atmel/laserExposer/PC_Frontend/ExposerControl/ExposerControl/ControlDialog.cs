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
    public partial class ControlDialog : Form
    {

        ComHandler comHandler;
        
        public ControlDialog(ComHandler c)
        {
            InitializeComponent();
            
            comHandler = c;

            string[] theSerialPortNames = System.IO.Ports.SerialPort.GetPortNames();
            port_comboBox.Items.AddRange(theSerialPortNames);
            //port_comboBox.SelectedItem = theSerialPortNames[0];
            port_comboBox.SelectedItem = Properties.Settings.Default.ComPort;

        }


        private void connect_button_Click(object sender, EventArgs e)
        {
            if (!comHandler.connected)
            {
                //Console.WriteLine(port_comboBox.SelectedItem);

                /*
                comHandler.onInfoReceived = infoReceived;
                comHandler.onParametersReceived = parametersReceived;
                comHandler.onPrametersSaved = parametersSaved;
                comHandler.onPatchtableUpdated = patchtableUpdated;
                comHandler.onProgressChanged = progressChanged;
                comHandler.onCommunicationError = onCommunicationError;
                */                

                if (comHandler.GuiOpen(port_comboBox.SelectedItem.ToString()))
                {
                    Properties.Settings.Default.ComPort = port_comboBox.SelectedItem.ToString();
                    Properties.Settings.Default.Save();
                    port_status_textBox.Text = "connected";
                    connect_button.Text = "disconnect";
                }
  
                return;

            }
            else
            {
                comHandler.Close();
                port_status_textBox.Text = "not connected";
                connect_button.Text = "connect";
            }

        }

        private void laserOnButton_Click(object sender, EventArgs e)
        {
            comHandler.sendLaser(true);

        }

        private void laserOffButton_Click(object sender, EventArgs e)
        {
            comHandler.sendLaser(false);
        }

        private void scannerOnButton_Click(object sender, EventArgs e)
        {
            comHandler.sendScanner(true);
        }

        private void scannerOffButton_Click(object sender, EventArgs e)
        {
            comHandler.sendScanner(false);
        }

        private void stepperOffButton_Click(object sender, EventArgs e)
        {
            comHandler.sendStepper(-3);
        }

        private void stepperCommandButton_Click(object sender, EventArgs e)
        {
            comHandler.sendStepper((int)stepperNumericUpdown.Value);
        }

    }
}
