require 'io/console'

class Wave
  WIDTH = 16
  HEIGHT = 32
  DEPTH = 8

  def initialize(led)
    @led = led
    @mutex = Mutex.new
    Thread.new { key_thread }
    main_thread
  end

  def main_thread
    loop do
      now = Time.now
      d = (now - now.to_i).to_f * WIDTH
      y = HEIGHT / 2
      @led.Clear
      dd = 0.2
      @wave = 10
      @mutex.synchronize do
        (WIDTH / dd).to_i.times do |i|
          x = i * dd
          a = Math.sin((x + d) * 2 / WIDTH * Math::PI)
          (0...DEPTH).each do |z|
            color = 0xFF - z * 16
            @led.SetLed(x, y + a * @wave - z, z, color << 8)
          end
        end
      end
      @led.Show
      sleep(0.01)
    end
  end

  def key_thread
    loop do
      key = STDIN.getch.ord
      puts key
      exit 0 if [0x03, 0x1A].any? { |a| a == key }
      @mutex.synchronize { @wave += 1 }
    end
  end
end

def execute(led)
  Wave.new(led)
end
