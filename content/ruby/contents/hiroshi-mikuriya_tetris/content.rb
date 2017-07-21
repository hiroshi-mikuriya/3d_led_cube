require 'io/console'

LED_HEIGHT = 32
LED_WIDTH = 16
LED_DEPTH = 8

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
  (0...((msg.size + 1) * LED_WIDTH)).each do |x|
    led.Clear
    msg.chars.each.with_index do |c, i|
      y = rand(2)
      z = rand(4)
      led.SetChar((i + 1) * LED_WIDTH - x, y, z, c.ord, color)
    end
  end
end

##
# Tetris class
class Tetris
  CELL = 2
  FIELD_WIDTH = LED_WIDTH / CELL
  FIELD_HEIGHT = LED_HEIGHT / CELL
  BLOCKS = %w(0660 4444 0470 0170 0270 0630 0360).freeze

  def initialize(led)
    @led = led
    @field = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT) { new_color } }
    @x = LED_WIDTH / 2
    @mutex = Mutex.new
    Thread.new { loop { key_proc } }
    Thread.new { loop { block_proc } }
    set_blocks
  end

  def key_proc
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    case key
    when 65
      puts 'up'
    when 66
      puts 'down'
    when 67
      puts 'right'
      @mutex.synchronize { @x += 1 }
    when 68
      puts 'left'
      @mutex.synchronize { @x -= 1 }
    end
  end

  def block_proc
    blk = BLOCKS[rand(BLOCKS.size)]
    color = new_color
    @x = LED_WIDTH / 2 - 2
    (0...LED_HEIGHT).each do |y|
      #@led.Clear
      xx = @x
      blk.chars.each.with_index(y) do |c, yi|
        format('%04b', c).chars.each.with_index(xx) do |b, xi|
          @led.SetLed(xi, yi, 0, b.to_i * color)
        end
      end
      sleep(0.2)
    end
  end

  def set_blocks
    @led.Clear
    @mutex.synchronize do
      (0...FIELD_WIDTH).to_a.product((0...FIELD_HEIGHT).to_a).each do |x, y|
        range = [x, y].map { |xy| ((xy * CELL)...((xy + 1) * CELL)).to_a }
        range[0].product(range[1]).each do |xx, yy|
          @led.SetLed(xx, yy, 7, @field[x][y])
        end
      end
    end
  end

  def show
    loop do
      @led.Show
      @led.Wait(100)
    end
  end
end

def execute(led)
  Tetris.new(led).show
end
