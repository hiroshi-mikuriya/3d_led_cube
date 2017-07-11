require 'fiddle/import'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

module LED
  extend Fiddle::Importer
  dlload './ledLib.dll'
  extern 'void SetUrl(char *)'
  extern 'void SetLed(int, int, int, int)'
  extern 'void Clear()'
  extern 'void Show()'
  extern 'void Wait(int)'
  extern 'void ShowMotioningText1(char*)'
end

LED.SetUrl(ARGV[0]) unless ARGV.empty?

def can_show(x, y, z)
  (0...WIDTH).cover?(x) && (0...HEIGHT).cover?(y) && (0...DEPTH).cover?(z)
end

def corner?(pos)
  pos.zip([WIDTH, HEIGHT, DEPTH]).all? { |a, b| a.zero? || a == b - 1 }
end

pos = [0, 0, 0]
ppos = pos.clone
ppos[0] -= 1

loop do
  if corner?(pos)
    ix = Array.new(3) { |i| pos[i] - ppos[i] }.index { |a| !a.zero? }
    ix = (ix + 1) % 3
    ppos = pos.clone
    ppos[ix] += pos[ix].zero? ? -1 : 1
  end
  npos = Array.new(3) { |i| 2 * pos[i] - ppos[i] }
  LED.Clear()
  LED.SetLed(npos[0], npos[1], npos[2], 0x00FFFF)
  LED.Show()
  LED.Wait(100)
  ppos = pos
  pos = npos
end
