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
  limit = (msg.size + 1) * LED_WIDTH
  (0...limit).each do |x|
    led.Clear
    msg.chars.each.with_index do |c, i|
      y = rand(2)
      z = rand(4)
      led.SetChar((i + 1) * LED_WIDTH - x, y, z, c.ord, color)
      led.Show
    end
  end
end

##
# テトリスクラス
class Tetris
  CELL = 2
  FIELD_WIDTH = LED_WIDTH / CELL
  FIELD_HEIGHT = LED_HEIGHT / CELL
  BLOCKS = %w(0660 4444 0470 0170 0270 0630 0360).freeze
  BLOCK_SIZE = 4

  def initialize(led)
    @led = led
    @led.ShowMotioningText1('321')
    @field = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT) { 0 } }
    @mutex = Mutex.new
    @game_over = false
    Thread.abort_on_exception = true
    add_new_block
    th = []
    th.push Thread.new { block_thread until @game_over }
    sleep(0.1) # 最初のブロック投入前にキー操作したらクラッシュしそうだから
    th.push Thread.new { key_thread until @game_over }
    until @game_over
      @mutex.synchronize { @led.Show }
      @led.Wait(50)
    end
    puts 'GAME OVER'
    show_msg(@led, 'GAMEOVER', new_color)
    th.each(&:join)
  end

  ##
  # キー入力を扱うスレッド
  def key_thread
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    @mutex.synchronize do
      case key
      when 65
        puts 'up'
        rotate_block_if_can
      when 66
        puts 'down'
        @pos[:y] += 1 unless hit_down?(@block)
      when 67
        puts 'right'
        @pos[:x] += 1 unless hit_right?(@block)
      when 68
        puts 'left'
        @pos[:x] -= 1 unless hit_left?(@block)
      end
    end
  end

  ##
  # ブロックの落下や列の消去などを行うスレッド
  def block_thread
    @mutex.synchronize do
      if hit_down?(@block)
        copy_block_to_field
        erase_completed_rows
        add_new_block
        @game_over = hit_down?(@block)
      else
        @pos[:y] += 1
        set_field_and_block
      end
    end
    sleep(0.3)
  end

  ##
  # ブロックをフィールドにコピーする
  def copy_block_to_field
    @block.each.with_index(@pos[:y]) do |bin, y|
      next if y < 0
      bin.chars.each.with_index(@pos[:x]) do |b, x|
        @field[x][y] = @color unless b.to_i.zero?
      end
    end
  end

  ##
  # そろった列があれば1列だけ消して下にシフトしてtrueを返す
  def erase_completed_row
    a = (0...FIELD_WIDTH).freeze
    (0...FIELD_HEIGHT).reverse_each do |y|
      next if a.any? { |x| @field[x][y].zero? }
      (1..y).reverse_each do |yy|
        a.each { |x| @field[x][yy] = @field[x][yy - 1] }
      end
      a.each { |x| @field[x][0] = 0 }
      return true
    end
    false
  end

  ##
  # そろった列を消す
  def erase_completed_rows
    loop do
      return unless erase_completed_row
    end
  end

  ##
  # ブロックが底もしくは積みブロックにぶつかった判定
  def hit_down?(block)
    block.reverse.each.with_index do |bin, i|
      y = @pos[:y] + BLOCK_SIZE - i - 1
      bin.chars.each.with_index(@pos[:x]) do |b, x|
        next if b.to_i.zero? || y < 0
        return true if  (FIELD_HEIGHT - 1) <= y || 0 < @field[x][y + 1]
      end
    end
    false
  end

  ##
  # ブロックが左にぶつかった判定
  def hit_left?(block)
    block.each.with_index(@pos[:y]) do |bin, y|
      bin.chars.each.with_index(@pos[:x]) do |b, x|
        next if b.to_i.zero?
        return true if x <= 0 || 0 < @field[x - 1][y]
      end
    end
    false
  end
  

  ##
  # ブロックが右にぶつかった判定
  def hit_right?(block)
    block.each.with_index(@pos[:y]) do |bin, y|
      bin.chars.each.with_index(@pos[:x]) do |b, x|
        next if b.to_i.zero?
        return true if (FIELD_WIDTH - 1) <= x || 0 < @field[x - 1][y]
      end
    end
    false
  end
  
  ##
  # 新しいブロックを追加する
  # 古いブロックを消す
  def add_new_block
    @color = new_color
    @pos = { x: (FIELD_WIDTH - BLOCK_SIZE) / 2, y: -BLOCK_SIZE }
    @block = BLOCKS[rand(BLOCKS.size)].chars.map { |a| format('%04b', a) }
  end

  ##
  # 回転可能ならばブロックを回転する
  # 回転したらtrueを返す
  def rotate_block_if_can
    cand = Array.new(BLOCK_SIZE) { |i| Array.new(BLOCK_SIZE) { |j| @block[j].reverse[i] }.join }
    unless hit_down?(cand) || hit_left?(cand) || hit_right?(cand) # TODO 条件が間違っている
      @block = cand
      return true
    end
    return false
  end

  ##
  # フィールド座標からLEDの座標へ変換し、セル単位でLEDの色を設定する
  def set_cell_led(cx, cy, z, rgb)
    xr, yr = [cx, cy].map { |xy| ((xy * CELL)...((xy + 1) * CELL)).to_a }.freeze
    xr.product(yr).each do |xx, yy|
      @led.SetLed(xx, yy, z, rgb)
    end
  end

  ##
  # ブロックとフィールドの色をLEDに設定する
  def set_field_and_block
    @led.Clear
    xr, yr = [FIELD_WIDTH, FIELD_HEIGHT].map { |xy| (0...xy).to_a }.freeze
    xr.product(yr).each do |x, y|
      set_cell_led(x, y, 0, @field[x][y])
    end
    @block.each.with_index(@pos[:y]) do |bin, y|
      next if y < 0
      bin.chars.each.with_index(@pos[:x]) do |b, x|
        next if b.to_i.zero?
        set_cell_led(x, y, 0, @color)
      end
    end
  end
end

def execute(led)
  loop { Tetris.new(led) }
end
