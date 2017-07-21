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
  BLOCK_SIZE = 4

  def initialize(led)
    @led = led
    @field = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT) { 0 } }
    @block = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT + BLOCK_SIZE) { 0 } }
    @x = LED_WIDTH / 2
    @mutex = Mutex.new
    Thread.abort_on_exception = true
    Thread.new { loop { key_proc } }
    Thread.new { loop { block_proc } }
    color = new_color
    @block[4][0] = color
    @block[5][0] = color
    @block[5][1] = color
    @block[6][0] = color
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
    shift_block
    set_field_and_block
    sleep(0.2)
  end

  def shift_block
    (1...@block[0].size).reverse_each do |y|
      (0...@block.size).each do |x|
        @block[x][y] = @block[x][y - 1]
        @block[x][y - 1] = 0
      end
    end
  end

  def set_field_and_block
    @led.Clear
    @mutex.synchronize do
      xfr, yfr = [FIELD_WIDTH, FIELD_HEIGHT].map { |xy| (0...xy).to_a }
      xfr.product(yfr).each do |xf, yf|
        xr, yr = [xf, yf].map { |xy| ((xy * CELL)...((xy + 1) * CELL)).to_a }
        xr.product(yr).each do |xx, yy|
          @led.SetLed(xx, yy, 0, @field[xf][yf] + @block[xf][yf + BLOCK_SIZE])
        end
      end
    end
  end

  def show
    loop do
      @mutex.synchronize { @led.Show }
      @led.Wait(50)
    end
  end
end

def execute(led)
  Tetris.new(led).show
end
