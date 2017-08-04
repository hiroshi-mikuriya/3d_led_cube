require 'io/console'
require 'Colorable'
include Colorable

class Wave
  WIDTH = 16
  HEIGHT = 32
  DEPTH = 8

  def initialize(led)
    @led = led
    @mutex = Mutex.new
    @wave = 10
    Thread.new { key_thread }
    Thread.new { wave_thread }
    main_thread
  end

  def main_thread
    h = 0
    loop do
      now = Time.now
      d = (now - now.to_i).to_f * WIDTH
      y = HEIGHT * 3 / 5
      @led.Clear
      dd = 0.2
      rgb = HSB.new(h, 100, 100).to_rgb
      @mutex.synchronize do
        (WIDTH / dd).to_i.times do |i|
          x = i * dd
          transport = Math.sin((x + d) * 2 / WIDTH * Math::PI)
          (0...DEPTH).each do |z|
            color = rgb.map { |v| [v - z * 20, 0].max }.inject { |a, e| a * 0x100 + e }
            @led.SetLed(x, y + transport * @wave - z, z, color)
          end
        end
      end
      @led.Show
      h = (h + 5) % 360
    end
  end

  def key_thread
    loop do
      key = STDIN.getch.ord
      exit 0 if [0x03, 0x1A].any? { |a| a == key }
      @mutex.synchronize { @wave = 15 }
    end
  end

  def wave_thread
    loop do
      @mutex.synchronize { @wave = [0, @wave - 1].max }
      sleep(0.5)
    end
  end
end

def execute(led)
  Wave.new(led)
end
