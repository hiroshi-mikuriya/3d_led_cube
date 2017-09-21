require 'io/console'

##
# テトリスクラス
class Tetris
  LED_HEIGHT = 32 # 3D LED CUBEのY方向のLED数
  LED_WIDTH = 16 # 3D LED CUBEのX方向のLED数
  LED_DEPTH = 8 # 3D LED CUBEのZ方向のLED数
  CELL = 2 # セルあたりのLED数
  FIELD_WIDTH = LED_WIDTH / CELL # X方向のLED数をセルサイズで割った値
  FIELD_HEIGHT = LED_HEIGHT / CELL # Y方向のLED数をセルサイズで割った値
  BLOCKS = %w(0660 4444 0470 0170 0270 0630 0360).freeze # ブロックの種類（定義したものがランダムで出現する）
  BLOCK_SIZE = 4 # ブロックの縦横セル数（空白含む）
  DELAY = 200 # ブロックの落下速度（小さい方がはやい）

  ##
  # 彩度の高い色をだすための計算
  def gem(a)
    return a if a < 1
    return 1 if a < 3
    return 4 - a if a < 4
    0
  end

  ##
  # 新しい彩度の高い色を取得する
  def new_color
    ran = rand * 6
    r, g, b = Array.new(3) { |a| (255 * gem((a * 2 + ran) % 6)).to_i }
    r * 0x10000 + g * 0x100 + b
  end

  ##
  # スクロールメッセージを表示する
  def show_msg(msg, color)
    (0...((msg.size + 1) * LED_WIDTH)).each do |x|
      @led.Clear
      msg.chars.each.with_index do |c, i|
        y = rand(2)
        z = rand(4)
        @led.SetChar((i + 1) * LED_WIDTH - x, y, z, c.ord, color)
      end
      @led.Show
      @led.Wait(2)
    end
  end

  ##
  # コンストラクタ内でテトリスの全処理を行う（開始からゲームオーバーまで）
  def initialize(led)
    die if BLOCKS.any? { |b| b.size != BLOCK_SIZE }
    @led = led
    @led.ShowMotioningText1('321')
    @field = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT) { 0 } }
    @mutex = Mutex.new
    @game_over = false
    Thread.abort_on_exception = true
    add_new_block
    th = Thread.new { key_thread until @game_over }
    until @game_over
      @mutex.synchronize do
        block_thread
        set_field_and_block
      end
      @led.Show
      @led.Wait(DELAY)
    end
    puts 'GAME OVER'
    show_msg('GAMEOVER', new_color)
    th.join
  end

  ##
  # キー入力を扱うスレッド
  def key_thread
    key = STDIN.getch.ord
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    @mutex.synchronize do
      case key
      when 65, 97, 98, 120, 121
        rotate_block_if_fit
      when 66
        @pos[:y] += 1 unless hit_down?
      when 67
        @pos[:x] += 1 unless hit_right?
      when 68
        @pos[:x] -= 1 unless hit_left?
      end
    end
  end

  ##
  # ブロックの落下や列の消去などを行うスレッド
  def block_thread
    if hit_down?
      copy_block_to_field
      erase_completed_rows
      add_new_block
      @game_over = hit_down?
    else
      @pos[:y] += 1
    end
  end

  ##
  # ブロックをフィールドにコピーする
  def copy_block_to_field
    @block.each.with_index(@pos[:y]) do |bin, y|
      next if y.negative?
      bin.each.with_index(@pos[:x]) do |b, x|
        @field[x][y] = @color unless b.zero?
      end
    end
  end

  ##
  # 列を削除するエフェクト
  def erase_effect(y)
    colors = Array.new(FIELD_WIDTH) { |x| @field[x][y] }
    zr = (0..LED_DEPTH)
    zr.each do |z|
      zr.each do |zz|
        (0...FIELD_WIDTH).each { |x| set_cell_led(x, y, zz, 0) }
      end
      colors.each.with_index { |color, x| set_cell_led(x, y, z, color) }
      @led.Show
      @led.Wait(50)
    end
  end

  ##
  # そろった列を消す
  def erase_completed_rows
    new_field = Array.new(FIELD_WIDTH) { Array.new(FIELD_HEIGHT) { 0 } }
    ny = FIELD_HEIGHT - 1
    (0...FIELD_HEIGHT).reverse_each do |fy|
      if (0...FIELD_WIDTH).any? { |x| @field[x][fy].zero? }
        (0...FIELD_WIDTH).each { |x| new_field[x][ny] = @field[x][fy] }
        ny -= 1
      else
        erase_effect(fy)
      end
    end
    @field = new_field
  end

  ##
  # ブロックが底もしくは積みブロックにぶつかった判定
  def hit_down?
    @block.reverse.each.with_index do |bin, i|
      y = @pos[:y] + BLOCK_SIZE - i - 1
      bin.each.with_index(@pos[:x]) do |b, x|
        next if b.zero? || y < -1
        return true if (FIELD_HEIGHT - 1) <= y || @field[x][y + 1].positive?
      end
    end
    false
  end

  ##
  # ブロックが左にぶつかった判定
  def hit_left?
    @block.each.with_index(@pos[:y]) do |bin, y|
      bin.each.with_index(@pos[:x]) do |b, x|
        next if b.zero?
        return true if x <= 0 || @field[x - 1][y].positive?
      end
    end
    false
  end

  ##
  # ブロックが右にぶつかった判定
  def hit_right?
    @block.each.with_index(@pos[:y]) do |bin, y|
      bin.each.with_index(@pos[:x]) do |b, x|
        next if b.zero?
        return true if (FIELD_WIDTH - 1) <= x || @field[x + 1][y].positive?
      end
    end
    false
  end

  ##
  # 新しいブロックを追加する
  def add_new_block
    @color = new_color
    @pos = { x: (FIELD_WIDTH - BLOCK_SIZE) / 2, y: -BLOCK_SIZE }
    @block = BLOCKS[rand(BLOCKS.size)].chars.map { |a| format('%04b', a).chars.map(&:to_i) }
  end

  ##
  # 回転可能ならばブロックを回転する
  def rotate_block_if_fit
    rot = Array.new(BLOCK_SIZE) { |i| Array.new(BLOCK_SIZE) { |j| @block[j].reverse[i] } }
    @block = rot if fit?(rot, @pos)
  end

  ##
  # フィールドにブロックが配置可能か調査する
  def fit?(block, pos)
    block.each.with_index(pos[:y]) do |bin, y|
      bin.each.with_index(pos[:x]) do |b, x|
        next if b.zero?
        return false unless (0...FIELD_HEIGHT).cover?(y) && (0...FIELD_WIDTH).cover?(x)
        return false if @field[x][y].positive?
      end
    end
    true
  end

  ##
  # フィールド座標からLEDの座標へ変換し、セル単位でLEDの色を設定する
  def set_cell_led(cx, cy, z, rgb)
    xr, yr = [cx, cy].map { |xy| ((xy * CELL)...((xy + 1) * CELL)).to_a }.freeze
    xr.product(yr).each do |xx, yy|
      (z...(z + CELL)).each { |zz| @led.SetLed(xx, yy, zz, rgb) }
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
      bin.each.with_index(@pos[:x]) do |b, x|
        set_cell_led(x, y, 0, @color) unless b.zero?
      end
    end
  end
end

def execute(led)
  loop { Tetris.new(led) }
end
