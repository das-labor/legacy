using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace ExposerControl
{

    public class NoDeviceException : Exception
    {
    }

    public delegate void OnInfoReceivedDelegate(Hashtable info);
    public delegate void SimpleDelegate();
    public delegate void IntDelegate(int theint);

    public class ComHandler
    {
        static SerialPort _serialPort;
        public bool connected;

        Thread handlerThread;


        private EventWaitHandle _getStatusEvent;
        private EventWaitHandle _sendDataEvent;
        private EventWaitHandle _sendLaserEvent;
        private EventWaitHandle _sendScannerEvent;
        private EventWaitHandle _sendStepperEvent;


        private EventWaitHandle _exitThreadEvent;
        private WaitHandle[] _eventArray;

        public OnInfoReceivedDelegate onInfoReceived;
        public OnInfoReceivedDelegate onParametersReceived;

        public SimpleDelegate onPrametersSaved;
        public SimpleDelegate onPatchtableUpdated;
        public event SimpleDelegate onCommunicationError;


        public IntDelegate onProgressChanged;

        public event IntDelegate onStatusRead;

        public ComHandler()
        {
            _exitThreadEvent = new AutoResetEvent(false);
            _getStatusEvent = new AutoResetEvent(false);
            _sendDataEvent = new AutoResetEvent(false);
            _sendLaserEvent = new AutoResetEvent(false);
            _sendScannerEvent = new AutoResetEvent(false);
            _sendStepperEvent = new AutoResetEvent(false);

            _eventArray = new WaitHandle[6];
            _eventArray[0] = _exitThreadEvent;
            _eventArray[1] = _getStatusEvent;
            _eventArray[2] = _sendDataEvent;
            _eventArray[3] = _sendLaserEvent;
            _eventArray[4] = _sendScannerEvent;
            _eventArray[5] = _sendStepperEvent;



        }



        public void Open(string portName)
        {
            // Create a new SerialPort object with default settings.
            _serialPort = new SerialPort();

            // Allow the user to set the appropriate properties.
            _serialPort.PortName = portName;
            //_serialPort.BaudRate = 500000;
            //_serialPort.BaudRate = 115200;
            _serialPort.BaudRate = 128000;

            _serialPort.Parity = System.IO.Ports.Parity.None;
            _serialPort.DataBits = 8;
            _serialPort.StopBits = System.IO.Ports.StopBits.One;
            _serialPort.Handshake = System.IO.Ports.Handshake.None;

            // Set the read/write timeouts
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.WriteBufferSize = 0x10000;
            _serialPort.ReadBufferSize = 0x10000;

            try
            {
                _serialPort.Open();

                while (!connected)
                {
                    _serialPort.Write("*");

                    string r = _serialPort.ReadLine();
                    if (r.StartsWith("Exposer"))
                    {
                        connected = true;
                    }
                    else
                    {
                        _serialPort.Close();
                        throw new NoDeviceException();
                    }
                }
            }
            catch (TimeoutException)
            {
                _serialPort.Close();
                throw new NoDeviceException();
            }


            handlerThread = new Thread(handler);
            handlerThread.Start();

        }

        public void Close()
        {
            if (connected)
            {
                _exitThreadEvent.Set();
                if (handlerThread != null)
                {
                    handlerThread.Join();
                }
                _serialPort.Close();
                connected = false;
            }
        }

        public void getStatus()
        {
            _getStatusEvent.Set();
        }

        byte[] data;
        public void sendData(byte[] data)
        {
            this.data = data; _sendDataEvent.Set();
        }

        bool laserState;
        public void sendLaser(bool m)
        {
            laserState = m;
            _sendLaserEvent.Set();
        }

        bool scannerState;
        public void sendScanner(bool s)
        {
            scannerState = s;
            _sendScannerEvent.Set();
        }

        int stepper_command;
        public void sendStepper(int cmd) {
            stepper_command = cmd;
            _sendStepperEvent.Set();
        }


        void handler()
        {
            int eventNum;
            try
            {
                while ((eventNum = WaitHandle.WaitAny(_eventArray)) != 0)
                {
                    switch (eventNum)
                    {
                        case 1:
                            {
                                _serialPort.Write("S");
                                int c = _serialPort.ReadByte();
                                if (onStatusRead != null) {
                                    onStatusRead(c);
                                }
                                
                            }
                            break;
                        case 2:
                            {
                                _serialPort.Write("d");

                                int c =_serialPort.ReadByte();
                                writeint16(data.Length);
                                _serialPort.Write(data, 0, data.Length);
                                if (onStatusRead != null)
                                {
                                    onStatusRead(c);
                                }
                                
                            }
                            break;
                        case 3:
                            {
                                _serialPort.Write("l");
                                if (laserState == false)
                                {
                                    _serialPort.Write("0");
                                }
                                else
                                {
                                    _serialPort.Write("1");
                                }

                            }
                            break;
                        case 4:
                            {
                                _serialPort.Write("m");
                                if (scannerState == false)
                                {
                                    _serialPort.Write("0");
                                }
                                else
                                {
                                    _serialPort.Write("1");
                                }

                            }
                            break;
                        case 5:
                            {
                                _serialPort.Write("s");
                                //_serialPort.Write(string.Format("{0:c}",stepper_command));
                                writeint16(stepper_command);

                            }
                            break;

                        /*
                        case 2:
                            {
                                _serialPort.Write("r");
                                string message;
                                Hashtable info = new Hashtable();

                                while ((message = _serialPort.ReadLine().Trim(new char[] { '\r', '\n', ' ' })) != "")
                                {
                                    string[] args = message.Split(new char[] { ':' }, 2);

                                    info.Add(args[0], args[1]);


                                }

                                onParametersReceived(info);
                            }
                            break;
                         */
                        
                    }
                }
            }
            catch (System.IO.IOException) { onCommunicationError(); }
            catch (ObjectDisposedException) { onCommunicationError(); }
            catch (InvalidOperationException) { onCommunicationError(); }
            catch (TimeoutException) { onCommunicationError(); }

            connected = false;
            _serialPort.Close();
        }



        private void writeint16(int i)
        {
            byte[] buf = { (byte)i, (byte)(i >> 8) };
            _serialPort.Write(buf, 0, 2);
        }

        private void writeint8(int i)
        {
            byte[] buf = { (byte)i };
            _serialPort.Write(buf, 0, 1);
        }


        public bool GuiOpen(string portname)
        {
            try
            {
                Open(portname);
                return true;
            }
            catch (UnauthorizedAccessException)
            {
                MessageBox.Show("The serial Port could not be opened. Maybe another application is accessing it.", "Couldn't open port");
                return false;
            }
            catch (NoDeviceException)
            {
                MessageBox.Show("There seems to be no Device connected to the specified serial port.", "Device not found");
                return false;
            }
            catch (System.IO.IOException)
            {
                MessageBox.Show("The serial Port could not be opened.", "IO Error");
                return false;
            }
        }
    }
}
