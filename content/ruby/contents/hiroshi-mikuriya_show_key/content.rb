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
    r * 0x10000 + g * 0x100 + b
end

def execute(led)
  loop do
    led.Show
    key = led.Wait(100)
    next if key < 0
    led.Clear
    x = rand(3)
    y = rand(3)
    z = rand(3)
    led.SetChar(x, y, z, key, new_color)
  end
end
