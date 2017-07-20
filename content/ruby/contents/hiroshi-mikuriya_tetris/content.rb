require 'io/console'

HEIGHT = 32
WIDTH = 16
DEPTH = 8

def gem(a)
  return a if a < 1
  return 1 if a < 3
  return 4 - a if a < 4
  0
end

def new_color
  ran = rand * 6
  r, g, b = Array.new(3) { |a| (255 * gem((a * 2 + ran) % 6)).to_i }
  r * 0x10000 + g * 0x100 + b
end

def show_msg(led, msg, color)
  (0...((msg.size + 1) * WIDTH)).each do |x|
    led.Clear
    msg.chars.each.with_index do |c, i|
      y = rand(2)
      z = rand(4)
      led.SetChar((i + 1) * WIDTH - x, y, z, c.ord, color)
    end
  end
end

Blocks = %w(0660 4444 0470 0270).freeze

def execute(led)
  sleep(10)
  
  x = WIDTH / 2

  Thread.new do
    loop do
      key = STDIN.getch.ord
      exit 0 if [0x03, 0x1A].any? { |a| a == key }
      case key
      when 65
        puts 'up'
      when 66
        puts 'down'
      when 67
        puts 'right'
        x += 1
      when 68
        puts 'left'
        x -= 1
      end
      # show_msg(led, 'GAMEOVER', new_color) if key == 'g'
    end
  end

  Thread.new do
    loop do
      blk = Blocks[rand(Blocks.size)]
      color = new_color
      x = WIDTH / 2 - 2
      (0...HEIGHT).each do |y|
        led.Clear
        xx = x
        blk.chars.each.with_index(y) do |c, yi|
          format('%04b', c).chars.each.with_index(xx) do |b, xi|
            led.SetLed(xi, yi, 0, b.to_i * color)
          end
        end
        sleep(0.2)
      end
    end
  end

  loop do
    led.Show
    led.Wait(100)
  end
end
