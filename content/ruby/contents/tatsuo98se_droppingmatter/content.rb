require 'timers'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

class RGB
    def self.white
        RGB.new(0xff, 0xff, 0xff)
    end

    attr_accessor :r, :g, :b
    def initialize(r, g, b)
        @r = r
        @g = g
        @b = b
        @r = 0 if @r < 0
        @g = 0 if @g < 0
        @b = 0 if @b < 0
    end

    def -(color) 
        RGB.new(@r-color, @g-color, @b-color)
    end
    
    def rgb
        to_i
    end

    def to_i
        (@r << 16) + (@g << 8) + @b
    end

    def black?
        @r == 0 && @g==0 &&@b==0
    end
end

class Dot
    attr_accessor :x, :y, :z, :color
    attr_reader :born_at, :lifetime

    def initialize(x, y, z, color, lifetime = -1)
        @x = x
        @y = y
        @z = z
        @color = color
        @born_at = time_to_msec(Time.now)
        @lifetime = lifetime
    end

    def time_to_msec(time)
        (time.to_f * 1000).to_i
    end

    def expired?
        return false if @lifetime == -1
        @lifetime < elasped
    end

    def elasped
        return time_to_msec(Time.now) - @born_at
    end

    def draw(canvas)
        canvas.set_led(@x, @y, @z, @color)
    end
end

class DroppingMatter < Dot

    MAX_MATTER=10

    def initialize(x, y, z, color, gravity=0.1)
        super(x, y, z, color, -1)
        @matter = []
        @gravity = gravity
        
    end

    def expired?
        @y > (HEIGHT*2)
    end

    def draw(canvas)
        @y=(@gravity * (elasped/100)**2).round

        @matter << @y
        @matter.shift if @matter.size > MAX_MATTER

        (@matter.min..@matter.max).each do | y |
            canvas.set_led(@x, y, @z, @color-((@matter.max-y)*20))
        end

    end
end

class EphemeralDot < Dot
    def initialize(x, y, z, color, lifetime = -1)
        super(x, y, z, color, lifetime)
    end

    def decay
        elasped / 5
    end

    def expired?
        super && (@color-decay).black?
    end

    def draw(canvas)
        canvas.set_led(@x, @y, @z, @color-decay) # 50は適当
    end

end

class Canvas
    attr_accessor :layers

    def initialize(led)
        @led = led
        @canvas = {}
        @object = []
    end

    def add_object(object)
        @object << object
    end

    def object_count
        @object.size
    end

    def show
        @led.Clear
        @canvas = {}
        @object.each do |object|
            if object.expired? then
                @object.delete(object)
            else
                object.draw(self)
            end
        end
        @led.Show()
    end

    def set_led(x, y, z, color)
        @led.SetLed(x, y, z, color.to_i)
    end
end


def execute(led)
    canvas = Canvas.new(led)
#    canvas.add_object(Dot.new(0, 0, 0, RGB.white, 10000))
#    canvas.add_object(EphemeralDot.new(2, 2, 2, RGB.white, 10000))
#    canvas.add_object(EphemeralDot.new(5, 5, 5, RGB.new(0xff, 0xaa, 0x88), 10000))
    canvas.add_object(DroppingMatter.new(0, 0, 0, RGB.white))

    loop do

        if canvas.object_count < 13 then
            colors = [RGB.new(rand(0x00..0x88), rand(0x00..0x88), rand(0xcc..0xff)),
                      RGB.new(rand(0x00..0x88), rand(0xcc..0xff), rand(0xcc..0xff)),
                      RGB.new(rand(0x00..0x88), rand(0xcc..0xff), rand(0x00..0x88))]
            
            canvas.add_object(DroppingMatter.new(rand(0..WIDTH), 0, rand(0..DEPTH), colors[rand(0..2)], rand(1..3)/10.0 ))
        end
        canvas.show
        led.Wait(20)

    end

    t.join

    # write your led operations.
    #
    # you can use methods below.

    # led.SetLed(int, int, int, int)
    # led.Clear()
    # led.Show()
    # led.Wait(int)
    # led.ShowMotioningText1(char *)
    # led.ShowMotioningText1("abc")
end



