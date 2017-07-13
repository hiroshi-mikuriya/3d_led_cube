require_relative './lib/led_cube'
require 'optparse'

option={}
OptionParser.new do |opt|
  opt.banner = "Usage: run_contents.rb [options]"
  
  opt.on('-c', '--content-dir CONTENT_DIR', "specify directory name where content.py is in you'd like to execute") do |v|
    option[:content_dir] = v
  end
  
  opt.on('-d', '--dest [DEST]',   '(optional) ip address of destination device which connect to real 3d cube.') do |v| 
    option[:dest] = v
  end

  opt.parse!(ARGV)

  LED.SetUrl(option[:dest]) unless option[:dest].nil?
  unless option[:content_dir].nil?
    require_relative(File.join(option[:content_dir], 'content'))
    execute(LED)
  else
    puts opt.help
  end
end

