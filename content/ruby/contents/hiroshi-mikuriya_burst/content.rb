def execute(led)
    [0,0,0,   8,16,4].each_slice(3) do |x, y, z|
      led.ShowFirework(x, y, z)
    end
end
