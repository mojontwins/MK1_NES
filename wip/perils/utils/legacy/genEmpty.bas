Dim As Integer f
Dim As Integer i
Dim As uByte d
d = 0
If Command (2) = "" Then Print "genEmpty.exe file.bin size":Print:End
f = Freefile
Open Command (1) For Binary as #f
For i = 1 To Val (Command (2))
Put #f,, d
Next i
Close #f
Print "DONE"
