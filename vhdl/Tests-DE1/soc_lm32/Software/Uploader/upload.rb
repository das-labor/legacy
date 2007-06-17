#!/usr/bin/env ruby
$0 = __FILE__

require "serialport.so"

def wait (msec)
    t = Time.now
    while (Time.now - t)*1000 < msec do
    end    
end

sp = SerialPort.new(ARGV[0], 57600, 8, 2, SerialPort::NONE)
checksum = 0
upFileSize =  File.size(ARGV[1])
uploadFile = File.open(ARGV[1], "r")
puts "File loaded." if uploadFile
puts upFileSize
begin
    sramSize = 0x00040000
    waitTime = 5    
    
    puts "Spike Uploade Tool"
    ch = 0;
    # warte auf Eingabe
    begin
        putc  (ch = sp.getc)
    end while ch != 62
        
    puts "Spike bootloader found"
    
    STDIN.getc
    error = 0
    puts "tansmitting"
    uploadFile.each_line do |line|
       b = 1
       if line[0..1] == "S3"
            begin 
                sp.putc 'u' 
                sz = line[2..-5]
                sz.each_byte do |b|
                    wait(waitTime/4)
                    putc '.'
                    sp.putc b
                end   
                c1   = sp.getc
                c2   = sp.getc  
                c    = sprintf("%c%c", c1, c2)
                if error > 3
                    puts "checksum error"
                    break
                end          
                error += 1    
            end while c.hex != line[-4..-3].hex
            wait(waitTime)
            begin
                putc (ch = sp.getc)
            end while ch != 62
            puts " "
            error = 0
        end
    end	
    		
=begin	
	sp.printf("uB0000000")
	#tty.puts "(B0000000)"
	wait(waitTime)
	sp.printf("%08X", upFileSize)
	#tty.printf("(%08X)", upFileSize)
	wait(waitTime)
	i = 0
	uploadFile.each_byte do |b|
		checksum += b
		sp.printf("%02X", b)
		#tty.printf("(%02X)", b)
		wait(waitTime/4)
		i += 1
	end
=end
    	
    STDIN.getc
        	
    sp.printf("vB0000000");
    wait(waitTime)
    sp.printf("B0000400")
    wait(waitTime)
    begin
        putc (ch = sp.getc)
    end while ch != 62    #tty.puts "Checksum"
    STDIN.getc
    
    #tty.puts((checksum & 0xff).to_s)
    wait(waitTime)
    puts "Run"
    sp.printf("gB0000000")
    puts "Complete"
   begin
        putc (ch = sp.getc)
    end while ch != 62   
    
=begin
tty.getc
sp.printf("uB0000000")
tty.puts "(B0000000)"
wait(waitTime)
sp.printf("%08X", sramSize)
tty.printf("(%08X)", sramSize)
wait(waitTime)
sramSize.times do
    sp.printf("00")
    wait(waitTime/4) 
end
=end
	
=begin
open("/dev/tty", "r+") { |tty|
    tty.sync = true
    Thread.new {
    while true do
        tty.printf("%c", sp.getc)
    end
    }
    while (l = tty.gets) do
    sp.write(l.sub("\n", "\r"))
    end
}
=end

ensure 
    uploadFile.close unless uploadFile.nil?
    sp.close unless sp.nil?
     
    puts "Serialport closed"
end
