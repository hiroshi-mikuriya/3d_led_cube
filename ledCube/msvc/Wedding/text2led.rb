require "RMagick"
require "pry"
letters="♥WELCOME♥"

W=H=16

File.open( "welcome.hxx", "w" ) do |f|
  letters.each_char.with_index do |c, ix|
    img = Magick::Image.new( W*10, H*10 )
    draw = Magick::Draw.new
    draw.font = "Bookman-Old-Style"
    draw.pointsize = 16
    draw.gravity = Magick::CenterGravity
    draw.text( 0, 0, c );
    draw.draw( img )
    metrics = draw.get_type_metrics(c)
    img.trim!
    w0,h0 = img.columns, img.rows
    w=((H-2.0)*w0/h0).round
    img.resize!( w, H-2 )
    img.border!( 1, 1, "white")
    pixels= img.get_pixels( 0, 0, w+2, H ).map{ |px| (px.to_HSL.last*-15+'P'.ord ).floor.chr }.join
    f.puts "\n//#{c.gsub( "♥", "heart" )}"
    f.puts %Q!{#{w+2}, #{H}, {"#{pixels}"}},!
    img.write( "#{ix}.png" )
  end
end

W=H=16

File.open( "heart.hxx", "w" ) do |f|
  c="♥"
  img = Magick::Image.new( W*10, H*10 )
  draw = Magick::Draw.new
  draw.font = "Arial"
  draw.pointsize = 20
  draw.gravity = Magick::CenterGravity
  draw.text( 0, 0, c );
  draw.draw( img )
  metrics = draw.get_type_metrics(c)
  img.trim!
  w0,h0 = img.columns, img.rows
  w=((H-2.0)*w0/h0).round
  img.resize!( w, H-2 )
  img.border!( 1, 1, "white")
  pixels= img.get_pixels( 0, 0, w+2, H ).map{ |px| (px.to_HSL.last*-15+'P'.ord ).floor.chr }.join
  f.puts "\n//#{c.gsub( "♥", "heart" )}"
  f.puts %Q!{#{w+2}, #{H}, {"#{pixels}"}}!
  img.write( "heart.png" )
end
