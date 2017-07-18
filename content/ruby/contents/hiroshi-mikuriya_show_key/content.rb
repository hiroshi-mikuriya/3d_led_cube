require 'io/console'

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

def execute(led)
  key = nil
  color = nil
  Thread.new do
    loop { key = STDIN.getch.ord; color = new_color }
  end
  loop do
    led.Show
    led.Wait(10)
    exit 0 if [0x03, 0x1A].any? { |a| a == key }
    led.Clear
    x, y, z = Array.new(3) { rand(5) }
    led.SetChar(x, y, z, key, color) unless key.nil?
  end
end
