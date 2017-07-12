require_relative './lib/led_cube'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

def corner?(pos)
  pos.zip([WIDTH, HEIGHT, DEPTH]).all? { |a, b| a.zero? || a == b - 1 }
end

def gem(a)
  return a if a < 1
  return 1 if a < 3
  return 4 - a if a < 4
  0
end

def new_color()
  ran = rand() * 6
  r = (255 * gem((0 + ran) % 6)).to_i
  g = (255 * gem((2 + ran) % 6)).to_i
  b = (255 * gem((4 + ran) % 6)).to_i
  (r << 16) + (g << 8) + b
end

pos = [0, 0, 0]
ppos = pos.clone
ppos[0] -= 1
color = new_color

loop do
  if corner?(pos)
    ix = Array.new(3) { |i| pos[i] - ppos[i] }.index { |a| !a.zero? }
    ix = (ix + [1, 2].shuffle[0]) % 3
    ppos = pos.clone
    ppos[ix] += pos[ix].zero? ? -1 : 1
    color = new_color
  end
  npos = Array.new(3) { |i| 2 * pos[i] - ppos[i] }
  LED.SetLed(npos[0], npos[1], npos[2], color)
  LED.Show()
  LED.Wait(100)
  ppos = pos
  pos = npos
end
