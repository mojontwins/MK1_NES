' Map Extruder.
' Converts 15x10 screens into 16x12 screens filling with zeroes EACH screen
' Inputs and outputs mappy's raw map files
' Copyleft 2013 by The Mojon Twins

Sub usage
	Print "$ mapxtruder mapa.map w h"
	Print
	Print "w = width in screens."
	Print "h = height in screens."
	Print "outputs output.map"
End Sub

Dim As integer i, j, map_w, map_h, y_pant, x_pant, n_pant, x, y, f
Dim As integer mapLastByte, last_pant
Redim As uByte in_map (0, 0, 0)
Redim As uByte out_map (0, 0, 0)

If Len (command (3)) = 0 or val (Command (2)) = 0 or val (Command (3)) = 0 then usage: End

map_w = Val (Command (2))
map_h = Val (Command (3))
last_pant = map_w * map_h - 1

Redim in_map (last_pant, 14, 9)
Redim out_map (last_pant, 15, 11)
 
f = Freefile
Open Command (1) For Binary as #f
mapLastByte = map_w * map_h * 150 - 1
For i = 0 To mapLastByte
	y_pant = int(i / (150 * map_w))
	x_pant = int (i / 15) mod map_w
	y = int (i / (15 * map_w)) Mod 10
	x = i Mod 15
	n_pant = y_pant * map_w + x_pant
	Get #f, , in_map (n_pant, x, y)
Next i
Close #f

For n_pant = 0 to last_pant
	For y = 0 To 9
		For x = 0 To 14
			out_map (n_pant, x, 1 + y) = in_map (n_pant, x, y)
	Next x, y
Next n_pant

' Write

f = Freefile
Kill "output.map"
Open "output.map" For Binary as #f
mapLastByte = map_w * map_h * 192 - 1
For i = 0 To mapLastByte
	y_pant = int(i / (192 * map_w))
	x_pant = int (i / 16) mod map_w
	y = int (i / (16 * map_w)) Mod 12
	x = i Mod 16
	n_pant = y_pant * map_w + x_pant
	Put #f, , out_map (n_pant, x, y)
Next i
Close #f

? "Done!"
