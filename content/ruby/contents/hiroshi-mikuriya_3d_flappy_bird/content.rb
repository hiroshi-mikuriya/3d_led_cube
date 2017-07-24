require 'io/console'

##
# Flappy Bird
class FlappyBird
  LED_WIDTH = 16
  LED_HEIGHT = 32
  LED_DEPTH = 8
  G = 30
  BIRD = [
    [0xFFFF00, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000],
    [0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFFFF],
    [0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF],
    [0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000]
  ].freeze

  def initialize(led)
    @led = led
    @led.ShowMotioningText1('321')
    @game_over = false
    @blockz = LED_DEPTH - 1
    @pos = { x: (LED_WIDTH - BIRD.first.size) / 2, y: (LED_HEIGHT / 2).to_f }
    @ana = new_ana
    @mutex = Mutex.new
    @v = -10
    @time = Time.now
    th = []
    th.push Thread.new { key_thread until @game_over }
    th.push Thread.new { block_thread until @game_over }
    main_thread until @game_over
    th.join
  end

  def main_thread
    @mutex.synchronize do
      now = Time.now
      t = now - @time
      @time = now
      @v += G * t
      @pos[:y] += @v * t
    end
    @led.Clear
    if crash?
      @game_over = true
      y = @pos[:y] + BIRD.size / 2
      y = 0 if y.negative?
      y = LED_HEIGHT - 1 if LED_HEIGHT <= y
      @led.ShowFirework(@pos[:x] + BIRD.first.size / 2, y, 0)
    else
      set_block
      set_bird
      @led.Show
    end
  end

  def crash?
    return false unless @blockz.zero?
    return true if @pos[:y].negative? || LED_HEIGHT <= @pos[:y]
    kabe = (0...LED_HEIGHT).to_a - @ana.to_a
    bird = (@pos[:y].to_i...(@pos[:y].to_i + BIRD.size)).to_a
    return true unless (kabe & bird).empty?
    false
  end

  def key_thread
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    @mutex.synchronize { @v = -10 }
  end

  def block_thread
    @ana = new_ana if @blockz.zero?
    @blockz = @blockz.zero? ? LED_DEPTH - 1 : @blockz - 1
    sleep(0.3)
  end

  def new_ana
    ana = BIRD.size + 8
    r = rand(LED_HEIGHT - ana)
    (r...(r + ana))
  end

  def set_block
    (0...LED_HEIGHT).each do |y|
      next if @ana.any? { |a| a == y }
      (0...LED_WIDTH).each do |x|
        @led.SetLed(x, y, @blockz, 0x00FF00)
      end
    end
  end

  def set_bird
    (0...BIRD.size).each do |y|
      (0...BIRD.first.size).each do |x|
        @led.SetLed(x + @pos[:x], y + @pos[:y], 0, BIRD[y][x])
      end
    end
  end
end

def execute(led)
  loop { FlappyBird.new(led) }
end
