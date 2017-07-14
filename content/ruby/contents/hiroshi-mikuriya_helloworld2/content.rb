WIDTH = 16
HEIGHT = 32
DEPTH = 8

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
    msg = 'Hello World!!(^_^;)'
    colors = Array.new(msg.size) { new_color }
    (0...((msg.size + 1) * WIDTH)).each do |x|
        led.Clear
        msg.chars.each.with_index do |c, i|
            y = rand(2)
            z = rand(4)
            led.SetChar((i + 1) * WIDTH - x, y, z, c.ord, colors[i])
        end
        led.Show
        led.Wait(1)
    end
end
