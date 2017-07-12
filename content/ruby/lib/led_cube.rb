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

  case os
  when :macosx
    dlload './lib/libledLib.dylib'
  when :windows
    if RbConfig::CONFIG['target_cpu'] == 'i386' then
      dlload './lib/ledLib32.dll'
    else
      dlload './lib/ledLib64.dll'
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
end
