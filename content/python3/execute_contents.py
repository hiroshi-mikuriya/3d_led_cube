from optparse import OptionParser
from lib.led_cube import *
import importlib
import os.path

parser = OptionParser()
parser.add_option("-c", "--content-dir",
                  action="store", type="string", dest="content_dir",
                  help="specify directory name where content.py is in you'd like to execute", metavar="NAME")
parser.add_option("-d", "--dest",
                  action="store", type="string", dest="dest", 
                  help="(optional) ip address of destination device which connect to real 3d cube.")

(options, args) = parser.parse_args()

if options.dest != None:
    led.SetUrl(options.dest.encode('utf-8'))
 
if options.content_dir != None:
    content_path = os.path.join(options.content_dir, "content")
    content_path = content_path.replace(os.sep, '.')
    # On MSYS2, os.sep returns back slash even though slash is correct separator.
    content_path = content_path.replace('/', '.')
    content = importlib.import_module(content_path)
    content.execute(led)
else:
    parser.print_help()
