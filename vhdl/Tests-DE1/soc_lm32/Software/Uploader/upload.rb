#!/usr/bin/env ruby
$0 = __FILE__

require "serialport.so"

def wait (msec)
    t = Time.now    
    while (Time.now - t)*1000 < msec do
    end    
end

sp = SerialPort.new(ARGV[0], 57600, 8, 1, SerialPort::NONE)
checksum = 0
upFileSize =  File.size(ARGV[1])
uploadFile = File.open(ARGV[1], "r")
puts "File loaded." if uploadFile
puts upFileSize
begin
    waitTime = 4    
    
    puts "Spike Upload Tool"
    ch = 0;
    # warte auf Eingabe
    sp.putc 'r'
    begin
        putc(ch = sp.getc)
    end while ch != 62
        
    puts "Spike bootloader found"
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
                c1 = sp.getc
                c2 = sp.getc  
                c  = sprintf("%c%c", c1, c2)
                if error > 3
                    puts "checksum error"
                    break
                end          
                error += 1    
            end while c.hex != line[-4..-3].hex
            wait(waitTime)
            begin
                putc(ch = sp.getc)
            end while ch != 62
            puts " "
            error = 0
        end
    end	
    		
    sp.printf("vB0000000");
    wait(waitTime)
    sp.printf("B0000700")
    wait(waitTime)
    begin
        putc(ch = sp.getc)
    end while ch != 62    #tty.puts "Checksum"
    #STDIN.getc
    
    wait(waitTime)
    puts "Run"
    sp.printf("gB0000000")
    puts "Complete"
    begin
        putc(ch = sp.getc)
    end while ch != 62   
   
ensure 
    uploadFile.close unless uploadFile.nil?
    sp.close unless sp.nil?
    puts "\n\rSerialport closed"
end
