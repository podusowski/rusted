xrandr_out=`xrandr --current | head -n1`
width=`echo $xrandr_out | awk '{print $8}'`
height=`echo $xrandr_out | awk '{print $10}' | sed 's/,//'`

width=$(($width-100))
height=$(($height-100))

echo $width x $height

_build/linux-release/runTestClient --graphics.width $width --graphics.height $height 
