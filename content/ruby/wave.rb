require_relative './lib/led_cube'

WIDTH = 16
HEIGHT = 32
DEPTH = 8

LED.SetUrl(ARGV[0]) if 0 < ARGV.size

def can_show(x, y, z)
  (0...WIDTH).include?(x) && (0...HEIGHT).include?(y) && (0...DEPTH).include?(z)
end

def wave
  n0 = 300
  (0...n0).each do |i|
    LED.Clear()
    it = 3.14 * 10 / n0
    xt = 3.14 * 2
    yt = 3.14 * 2 * 1.41
    rt= 3.14 * 1 * 1.41 / n0
    gt = 3.14 * 1 * 1.26 / n0
    bt = 3.14 * 1 * 1.59 / n0
    lt = 20.0
    light = i < lt ? i / lt : n0 - i<lt ? (n0 - i) / lt : 1
    (0...HEIGHT).each do |iy|
      (0...WIDTH).each do |ix|
        x = xt*(ix-WIDTH/2)/WIDTH*2
        y = yt*(iy - HEIGHT / 2) / HEIGHT*2
        iz = (Math.sin(x)*Math.sin(y)*Math.sin(i*it)*DEPTH/2+ DEPTH/2).to_i
        t = 3.141592 * 2 / 3
        col = Proc.new do |t|
          dx = (ix - WIDTH / 2)
          dy = (iy - HEIGHT / 2)
          d = (dx*dx + dy*dy)**0.7 / 10.0 + t + i*it
          w = Math.sin(3.1416 * 7 / 6)
          v = Math.sin(d) < w ? 0 : (Math.sin(d)-w) / (1-w)
          255 & (v * 255*light).to_i
        end
        c = col.call(t) * 1 + col.call(t*2) * 0x100 + col.call(t*3) * 0x10000
        [-1, 0, 1].each do |dz|
          LED.SetLed(ix, iy, iz+dz, c) if can_show(ix, iy, iz+dz)
        end
      end
    end
    LED.Show()
    LED.Wait(20)
  end
end

LED.ShowMotioningText1("Demo")
wave()
