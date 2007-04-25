require "serialport.so" 

sp = SerialPort.new("/dev/tty.usbserial-A50019dG", 9600, 8, 1, SerialPort::NONE)

input = "" 
while input != "q" 
  puts "Light a light:" 
  input = gets.chomp
    sp.putc input.to_s
end

sp.close               


int ledPin = 9;
int analogPin = 3;
int val = 0;
int incomingByte = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  //val = analogRead(analogPin);
  //analogWrite(ledPin, val / 4);
  analogWrite(ledPin, 100);
  
  //if (Serial.available() > 0) {
	//	incomingByte = Serial.read();
  //              //analogWrite(ledPin, incomingByte);
  //              if (incomingByte > 0) {
  //                analogWrite(ledPin, incomingByte);
  //                Serial.print(incomingByte);
  //                incomingByte = 0;
  //                Serial.print(incomingByte);
  //              }
	//}
  
}