require 'io/console'

##
# Flappy Bird
class FlappyBird
  LED_WIDTH = 16
  LED_HEIGHT = 32
  LED_DEPTH = 8
  PIPE_WIDTH = 3
  G = 40
  JUMP = -20
  ANA = 8
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
    @blockx = LED_WIDTH * 2
    @pos = { x: 0, y: (LED_HEIGHT / 2).to_f }
    @ana = new_ana
    @mutex = Mutex.new
    @v = -10
    @time = Time.now
    th = []
    th.push Thread.new { key_thread until @game_over }
    th.push Thread.new { block_thread until @game_over }
    main_thread until @game_over
    th.each(&:join)
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
      @led.ShowFirework(@pos[:x] + BIRD.first.size / 2, y, 0)
    else
      set_block
      set_bird
      @led.Show
    end
  end

  def crash?
    return true if @pos[:y].negative? || LED_HEIGHT <= @pos[:y]
    return false if BIRD.first.size <= @blockx
    kabe = (0...LED_HEIGHT).to_a - @ana.to_a
    bird = (@pos[:y].to_i...(@pos[:y].to_i + BIRD.size)).to_a
    (kabe & bird).size.positive?
  end

  def key_thread
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    @mutex.synchronize { @v = JUMP }
  end

  def block_thread
    if (@blockx + PIPE_WIDTH).zero?
      @ana = new_ana
      @blockx = LED_WIDTH + PIPE_WIDTH
    else
      @blockx -= 1
    end
    sleep(0.2)
  end

  def new_ana
    ana = BIRD.size + ANA
    r = rand(LED_HEIGHT - ana)
    (r...(r + ana))
  end

  def set_block
    (0...LED_HEIGHT).each do |y|
      next if @ana.any? { |a| a == y }
      (0..PIPE_WIDTH).each { |x| @led.SetLed(@blockx + x, y, 0, 0x00FF00) }
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
