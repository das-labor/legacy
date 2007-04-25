class Timer
  def initialize(init, name)
    @time = init
    @name = name
    Thread.new{ ticker }
  end
  def ticker
    loop do
      sleep @time
      puts "#{@name} ticking all #{@time} seconds."
    end
  end
  def time(time)
    puts "#{@name} gets new interval with #{time}!"
    @time = time
  end
end

timerb = Timer.new(60*60, "timerb")
#timera.time 5
sleep 20
puts "end!"


#########################################
####### smaller, less...
#########################################
class Timer
  def initialize(init)
    Thread.new{ ticker(init) }
  end
  def ticker(seconds)
    loop do
      sleep seconds
      puts "ticking all #{seconds} seconds."
    end
  end
end

timera = Timer.new(1)
sleep 5