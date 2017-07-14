require 'rbconfig'
require 'fiddle/import'

def os
  @os ||= (
    host_os = RbConfig::CONFIG['host_os']
    case host_os
    when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
      :windows
    when /darwin|mac os/
      :macosx
    when /linux/
      :linux
    when /solaris|bsd/
      :unix
    else
      :unknown
    end
  )
end

module LED
  extend Fiddle::Importer

  dirname = File.dirname(__FILE__)
  case os
  when :macosx
    dlload dirname + '/libledLib.dylib'
  when :windows
    if RbConfig::CONFIG['target_cpu'] == 'i386' then
      dlload dirname + '/ledLib32.dll'
    else
      dlload dirname + '/ledLib64.dll'
    end
  else
    raise RuntimeError
  end

  extern 'void SetUrl(char *)'
  extern 'void SetLed(int, int, int, int)'
  extern 'void Clear()'
  extern 'void Show()'
  extern 'void Wait(int)'
  extern 'void ShowMotioningText1(char *)'
  extern 'void SetChar(int, int, int, char, int)'
end
