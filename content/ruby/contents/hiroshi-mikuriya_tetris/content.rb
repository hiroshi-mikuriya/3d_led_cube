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
    @mutex = Mutex.new
    Thread.abort_on_exception = true
    Thread.new { loop { key_proc } }
    Thread.new { loop { block_proc } }
    add_new_block
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
    when 68
      puts 'left'
    end
  end

  def block_proc
    shift_block
    set_field_and_block
    if hit?
      copy_block_to_field
      add_new_block
    end
    sleep(0.3)
  end

  def copy_block_to_field
    (0...FIELD_WIDTH).each do |x|
      (0...FIELD_HEIGHT).each do |y|
        b = @block[x][y + BLOCK_SIZE]
        @field[x][y] = b unless b.zero?
      end
    end
  end

  def hit?
    (0...FIELD_HEIGHT).each do |y|
      (0...FIELD_WIDTH).each do |x|
        b = @block[x][y + BLOCK_SIZE]
        return true if 0 < b && y == FIELD_HEIGHT - 1
      end
    end
    false
  end

  def add_new_block
    (0...@block.size).each do |x|
      (0...@block[0].size).each do |y|
        @block[x][y] = 0
      end
    end
    color = new_color
    xb = (FIELD_WIDTH - BLOCK_SIZE) / 2
    blk = BLOCKS[rand(BLOCKS.size)]
    blk.chars.map { |a| format('%04b', a) }.each.with_index do |bin, y|
      (0...bin.size).each do |x|
        @block[xb + x][y] = bin[x].to_i * color
      end
    end
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
