require 'io/console'

##
# Flappy Bird
class FlappyBird
  LED_WIDTH = 16
  LED_HEIGHT = 32
  LED_DEPTH = 8
  ANA_SIZE = 8

  def initialize(led)
    @led = led
    @game_over = false
    @blockz = LED_DEPTH - 1
    @ana = new_ana
    th = Thread.new { key_thread until @game_over }
    main_thread until @game_over
    th.join
  end

  def main_thread
    @led.Clear
    set_block
    @led.Show
    @led.Wait(200)
    @ana = new_ana if @blockz.zero?
    @blockz = @blockz.zero? ? LED_DEPTH - 1 : @blockz - 1
  end

  def new_ana
    r = rand(LED_HEIGHT - ANA_SIZE)
    (r...(r + ANA_SIZE))
  end

  def set_block
    (0...LED_HEIGHT).each do |y|
      next if @ana.any? { |a| a == y }
      (0...LED_WIDTH).each do |x|
        @led.SetLed(x, y, @blockz, 0x00FF00)
      end
    end
  end

  def key_thread
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
  end
end

def execute(led)
  loop { FlappyBird.new(led) }
end
