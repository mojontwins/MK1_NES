Dim As Integer i, fIn, fOut
Dim As uByte d

If Command (2) = "" Then Print "$ fillto.exe file.bin size": Print: End
Name Command (1) As "temp"

fIn = FreeFile
Open "temp" For Binary As #fIn

fOut = Freefile
Open Command (1) For Binary as #fOut

For i = 1 To Val (Command (2))
	If Eof (fIn) Then d = 0 Else Get #fIn, , d
	Put #fOut, , d
Next i

Close
Kill "temp"
? "Now " & Command (1) & " is " & Command (2) & " bytes."
