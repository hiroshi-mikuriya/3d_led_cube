require 'timers'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

class RGB
    def self.white
        RGB.new(0xff, 0xff, 0xff)
    end

    def self.rand(base=0)
        add = 255 - base
        RGB.new(base + Kernel::rand(0..add), base + Kernel::rand(0..add), base + Kernel::rand(0..add))
    end

    attr_accessor :r, :g, :b
    def initialize(r, g, b)
        @r = r.to_i
        @g = g.to_i
        @b = b.to_i
        @r = 0 if @r < 0
        @g = 0 if @g < 0
        @b = 0 if @b < 0
        @r = 255 if @r > 255
        @g = 255 if @g > 255
        @b = 255 if @b > 255
    end

    def -(color)
        return RGB.new(@r-color.r, @g-color.g, @b-color.b) if color.kind_of?(RGB)
        RGB.new(@r-color, @g-color, @b-color)
    end

    def *(color) 
        RGB.new(@r*color, @g*color, @b*color)
    end

    def rgb
        to_i
    end

    def to_i
        (@r << 16) + (@g << 8) + @b
    end

    def to_s
        "R:"+@r.to_s + " G:"+@g.to_s + " B:"+@b.to_s
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

class DrawUtility

    DIRECTION_FRONT = 1
    DIRECTION_UP = 2
    DIRECTION_SIDE = 3
    
    def self.circle(canvas, x, y, z, color, r, direction=DIRECTION_FRONT)
        dth = 0.02*Math::PI
        0.step(2.0*Math::PI, dth) do |th|
            c1=r*Math.cos(th)
            c2=r*Math.sin(th)

            ambigous1 = (c1 - c1.round).abs
            ambigous2 = (c2 - c2.round).abs
            ambigous = (ambigous1 + ambigous2)/2

            xadd = yadd = zadd = 0
            case direction
            when DIRECTION_FRONT then
                xadd = c1
                yadd = c2
            when DIRECTION_UP then
                xadd = c1
                zadd = c2
            when DIRECTION_SIDE then
                yadd = c1
                zadd = c2
            end
            canvas.set_led(x+xadd, y+yadd, z+zadd, color-(color*ambigous))
        end
    end

end
class Circle < Dot

    def initialize(x, y, z, r, color)
        super(x, y, z, color, -1)
        @r = r
    end

    def draw(canvas)
        DrawUtility.circle(canvas, @x, @y, @z, @color, @r)
    end
end

class IncrementalCircle < Dot

    DIRECTION_FRONT = 1
    DIRECTION_UP = 2
    DIRECTION_SIDE = 3

    def initialize(x, y, z, r, color, direction)
        super(x, y, z, color, -1)
        @r = r
        @direction = direction
    end

    def expired?
        @r > 16
    end

    def draw(canvas)
        @r += 1
        DrawUtility.circle(canvas, @x, @y, @z, @color-@r*20, @r, @direction)
    end
end

class TrappedMatter < Dot

    def initialize(x, y, z, size, color)
        super(x, y, z, color, -1)
        @size = size
        @buffer = []
        @xadd=2
        @yadd=1
        @zadd=1
    end

    def draw(canvas)
 
        @x += @xadd
        @y += @yadd
        @z += @zadd

        if @x > WIDTH or @x < 0 then
            @xadd = -@xadd
            @x += @xadd
            canvas.add_object(IncrementalCircle.new( 
                            @x, @y, @z, 0, RGB.rand(0xaa), 
                            IncrementalCircle::DIRECTION_SIDE))
        end
        if @y > HEIGHT or @y < 0 then
            @yadd = -@yadd
            @y += @yadd
            canvas.add_object(IncrementalCircle.new(
                            @x, @y, @z, 0, RGB.rand(0xaa), 
                            IncrementalCircle::DIRECTION_UP))
        end
        if @z > DEPTH or @z < 0 then
            @zadd = -@zadd
            @z += @zadd
            canvas.add_object(IncrementalCircle.new(
                            @x, @y, @z, 0, RGB.rand(0xaa), 
                            IncrementalCircle::DIRECTION_FRONT))
        end

       @buffer << [@x, @y, @z, @color]
        @buffer.shift if @buffer.size > @size

        @buffer.each do |b|
            canvas.set_led(b[0], b[1], b[2], b[3])
        end
        
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
        canvas.set_led(@x, @y, @z, @color-decay)
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
        @led.SetLed(x.round, y.round, z.round, color.to_i)
    end
end


def execute(led)
    canvas = Canvas.new(led)
#    canvas.add_object(Dot.new(0, 0, 0, RGB.white, 10000))
#    canvas.add_object(EphemeralDot.new(2, 2, 2, RGB.white, 10000))
#    canvas.add_object(EphemeralDot.new(5, 5, 5, RGB.new(0xff, 0xaa, 0x88), 10000))
#    canvas.add_object(DroppingMatter.new(0, 0, 0, RGB.white))
    canvas.add_object(TrappedMatter.new(3,3,3,10,RGB.white))

    loop do

        if canvas.object_count < 13 then
            colors = [RGB.new(rand(0x00..0x88), rand(0x00..0x88), rand(0xcc..0xff)),
                      RGB.new(rand(0x00..0x88), rand(0xcc..0xff), rand(0xcc..0xff)),
                      RGB.new(rand(0x00..0x88), rand(0xcc..0xff), rand(0x00..0x88))]
            
#            canvas.add_object(DroppingMatter.new(rand(0..WIDTH), 0, rand(0..DEPTH), colors[rand(0..2)], rand(1..3)/10.0 ))
#            canvas.add_object(IncrementalCircle.new(rand(0..WIDTH), 
#                            rand(0..HEIGHT), 0, 0, RGB.white, 
#                            IncrementalCircle::DIRECTION_SIDE))
        end
 
        canvas.show
        led.Wait(20)

    end

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



