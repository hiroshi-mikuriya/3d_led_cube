require_relative './lib/led_cube'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

LED.SetUrl(ARGV[0]) if 0 < ARGV.size

loop do
  LED.ShowMotioningText1("HELLOWORLD!")
end
