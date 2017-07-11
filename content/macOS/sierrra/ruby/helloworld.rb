require 'fiddle/import'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

module LED
  extend Fiddle::Importer
  dlload './libledLib.dylib'
  extern 'void SetUrl(char *)'
  extern 'void SetLed(int, int, int, int)'
  extern 'void Clear()'
  extern 'void Show()'
  extern 'void Wait(int)'
  extern 'void ShowMotioningText1(char *)'
end

LED.SetUrl(ARGV[0]) if 0 < ARGV.size

loop do
  LED.ShowMotioningText1("HELLOWORLD!")
end
