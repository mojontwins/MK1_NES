' mscmk1.bas
' Mojontwins Script Compiler for MK1_NES / AGNES
' Copyleft 2018 by The Mojon Twins

' $ fbc mscmk1.bas mtparser.bas

#include "mtparser.bi"

Const LIST_WORDS_SIZE = 128
Const LIST_ALIAS_SIZE = 255
Const MACRO_SIZE = 32
Const MAX_MACROS = 31

Dim Shared As String script (0)
Dim Shared As Integer addresses (0)
Dim Shared As Integer clausulasUsed (255)
Dim Shared As Integer actionsUsed (255)
Dim Shared As uByte myBin (16383)
Dim As uInteger maxClausuleSize
Dim Shared lP (LIST_WORDS_SIZE) As String
Dim Shared As String listAlias (LIST_ALIAS_SIZE) 
Dim Shared As String decoInclude
Dim Shared As Integer doIncludeDecos, decoFetch
Dim Shared As Byte decorated (255)
Dim Shared As Integer maxScr
Dim Shared As String macros (MAX_MACROS, MACRO_SIZE)
Dim Shared As Integer macrosPtr
Dim Shared As Integer maxItem
Dim Shared As Integer itemSetx, itemSety, itemSetStep, itemSetOr, itemFlag, slotFlag
Dim Shared As Integer itemSelectClr, itemSelectC1, itemSelectC2, itemEmpty
Dim Shared As Integer debug

Sub usage
	Print "Usage:"
	Print
	Print "$ mscmk1.exe input.spt output.h maxpants [debug]"
End Sub

Function inCommand (a As String) As Integer
	Dim As Integer i
	i = 1
	While (Command (i) <> "")
		If Command (i) = a Then Return -1
		i = i + 1
	Wend
	Return 0
End Function

Sub resetScript (n As Integer)
	Dim As Integer i
	For i = 0 To n: script (i) = "": Next i
	For i = 0 To 16383: myBin (i) = 0: Next i
End Sub

Function pVal (s As String) As Integer
	Dim As Integer res, i
	If Left (s, 1) = "#" Then
		res = 128 + Val (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = i: Exit For
		Next i
	Else
		res = Val (s)
	End If

	Return res
End Function

Function pvalL (s As String) As Integer
	Dim As Integer res, i

	If s = "SLOT_SELECTED" Then 
		res = slotFlag
	ElseIf s = "ITEM_SELECTED" Then
		res = itemFlag
	ElseIf Left (s, 1) = "#" Then
		res = pVal (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = i: Exit For
		Next i
	Else
		res = -1
	End If

	Return res
End Function

Function pvalR (s As String) As Integer
	Dim As Integer res, i
	If s = "SLOT_SELECTED" Then 
		res = 128 + slotFlag
	ElseIf s = "ITEM_SELECTED" Then
		res = 128 + itemFlag
	ElseIf Left (s, 1) = "#" Then
		res = 128 + Val (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = 128 + i: Exit For
		Next i
	Else
		res = Val (s)
	End If
	pvalR = res
End Function

Sub ProcessItems (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
		Select Case Ucase (lP (0))
			Case "SLOT_FLAG":
				slotFlag = val (lP (1))
			Case "ITEM_FLAG":
				itemFlag = val (lP (1))
			Case "LOCATION":
				itemSetX = val (lP (1))
				itemSetY = val (lP (2))
			Case "DISPOSITION":
				itemSetStep = val (lP (2))
				If lP (1) = "HORZ" then itemSetOr = 0 Else ItemSetOr = 1
			Case "SELECTOR":
				itemSelectClr = val (lP (1))
				itemSelectC1 = val (lP (2))
				itemSelectC2 = val (lP (3))
			Case "EMPTY":
				itemEmpty = val (lP (1))
			Case "SIZE":
				maxItem = val (lP (1))
			Case "END":
				terminado = Not 0
		End Select
		If eof (f) Then terminado = Not 0
	Wend
End Sub

Sub addAlias (nameA As String, flag As Integer)
	listAlias (flag) = nameA
End Sub

Sub addMacro ()
	Dim As Integer i
	For i = 0 To MACRO_SIZE
		macros (macrosPtr, i) = lP (i)
	Next i
	If macrosPtr < MAX_MACROS Then macrosPtr = macrosPtr + 1 Else Print "** WARNING ** MAX_MACROS reached! Next will overwrite last."
End Sub

Sub ProcessAlias (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
		If lP (0) = "END" Then terminado = Not 0
		If Left (lP (0), 1) = "$" Then
			AddAlias lP (0), pvalR (lP (1))
		ElseIf Left (lP (0), 1) = "%" Then
			AddMacro
		End If
		If eof (f) Then terminado = Not 0
	Wend
End Sub

Sub lPshiftLeft (first As Integer)
	Dim As Integer i
	For i = first To LIST_WORDS_SIZE
		lP (i - 1) = lP (i)
	Next i
End Sub

Function strictMscNumeric (word As String) As Integer
	Dim As Integer res, i
	Dim As String m
	If word = "" Then
		res = 0
	ElseIf Left (word, 1) = "$" Or Left (word, 1) = "'" Then
		res = -1
	Else
		res = -1
		For i = 1 To Len (word)
			If m < "0" Or m > "9" Then
				res = 0: Exit For
			End If
		Next i
	End If

	Return res
End Function

Sub fixTokens
	Dim As Integer i 

	' Fixes:
	i = 0: While i < LIST_WORDS_SIZE
		If lP (i) = "" Then 
			Exit While
		ElseIf lP (i) = "FLAG" Then
			If Len (lP (i + 1)) Then
				If Left (lP (i + 1), 1) <> "$" Then
					lP (i + 1) = "#" & lP (i + 1)
				End If
				lPshiftLeft i + 1
			End If
		Else 
			i = i + 1
		End If
	Wend

	i = 0: While i < LIST_WORDS_SIZE
		If lP (i) = "" Then 
			Exit While
		ElseIf lP (i) = "SET" Then
			If Not strictMscNumeric (lP (i + 1)) Then
				lP (i) = "SET_" & lP (i + 1)
				lPshiftLeft i + 2
			End If
		End If
		i = i + 1
	Wend
End Sub

Function procesaClausulas (f As Integer) As String
	' Read clausules from 'f' and outputs a string

	Dim As String linea
	Dim As String clausulas, clausula

	Dim As Integer sc_terminado, estado
	Dim As Integer longitud
	Dim As Integer ai, itt, dF, opCode, lvalue, rvalue, i, value
	Dim As Integer fzx1, fzy1, fzx2, fzy2

	longitud = 0
	clausulas = ""

	' Add decorations
	If doIncludeDecos Then

		' Open file
		dF = FreeFile
		Open decoInclude For Input as #dF
			
			' Fetch screen
			sc_terminado = 0
			estado = 0
			While Not sc_terminado And Not Eof (dF)
				Line input #dF, linea
				linea = Trim (linea, Any chr (32) + chr (9))
				If linea <> "" Then
					parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
					If lP (0) = "ENTERING" and val (lP (2)) = decoFetch Then
						estado = 1
						sc_terminado = Not 0
					End If
				End If
			Wend

			If estado = 1 Then
				? "Decos " & decoFetch

				' Add a new clausule IF TRUE
				clausula = chr (&HF0)
				clausulasUsed (&HF0) = -1

				' Then
				clausula = clausula + Chr (255)

				' fetch section DECORATIONS
				sc_terminado = 0
				While Not sc_terminado And Not Eof (dF)
					Line input #dF, linea
					linea = Trim (linea, Any chr (32) + chr (9))
					parseTokenizeString linea, lP (), ",;()" & chr (9), "'"

					if lP(0) = "DECORATIONS" Then
						' Parse
						clausula = clausula + Chr (&HF4)
						actionsUsed (&HF4) = -1

						do
							Line input #dF, linea
							linea = Trim (linea, Any chr (32) + chr (9))
							if ucase (linea)="END" then sc_terminado = -1: exit do
							parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
							' X,Y,T -> XY T
							clausula = clausula + Chr ((Val(lP(0)) Shl 4) + (Val(lP(1)) And 15)) + Chr (Val(lP(2)))
						loop

						' End
						clausula = clausula + Chr (&HFF)
					End If
				Wend

				' End 
				clausula = clausula + Chr (&HFF)

				' Add length
				clausula = Chr (len (clausula)) + clausula

				' Add clausule
				clausulas = clausulas + clausula
			End If
		Close dF		
	End If

	sc_terminado = 0
	estado = 0
	clausula = ""

	While Not sc_terminado And Not Eof (f)
		Line Input #f, linea
		linea = Trim (linea, Any Chr (32) + Chr (9))

		parseCleanTokens lP ()
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
		If lP (0) = "" Then Continue While
		
		fixTokens

		If debug Then 
			Print parseGlueTokens (lP ())
		End If

		If estado <> 1 Then
			' Conditions

			Select Case Ucase (lP (0))
				Case "IF":
					' Which condition?

					If Len (lP (1)) Then

						If Left (lP (1), 1) = "$" Or Left (lP (1), 1) = "#" Then
							' 0  1    2  3
							' IF FLAG OP RVALUE
							lvalue = pvalL (lP (1))
							If lvalue = -1 Then Print "ERROR - Wrong lvalue": End

							Select Case lP (2)
								Case "=": opCode = &H10
								Case "<": opCode = &H11
								Case ">": opCode = &H12
								Case "<>", "!=": opCode = &H13
								Case Else:
									Print "ERROR - Wrong operand": End
							End Select

							clausula = clausula + Chr (opCode) + Chr (lvalue) + Chr (pvalR (lP (3)))
							clausulasUsed (opCode) = -1							
						Else
							Select Case Ucase (lP (1))
								' Conditions about position
								Case "PLAYER_TOUCHES":
									clausula = clausula + chr (&H20) + chr (pvalR (lP (2))) + chr (pvalR (lP (3)))
									clausulasUsed (&H20) = -1
								Case "PLAYER_IN_X":
									clausula = clausula + chr (&H21) + chr (val (lP (2))) + chr (val (lP (3)))
									clausulasUsed (&H21) = -1
								Case "PLAYER_IN_X_TILES":
									fzx1 = val (lP (2)) * 16 - 15
									If fzx1 < 0 Then fzx1 = 0
									fzx2 = val (lP (3)) * 16 + 15
									If fzx2 > 255 Then fzx2 = 255
									clausula = clausula + chr (&H21) + chr (fzx1) + chr (fzx2)
									clausulasUsed (&H21) = -1
								Case "PLAYER_IN_Y":
									clausula = clausula + chr (&H22) + chr (val (lP (2))) + chr (val (lP (3)))
									clausulasUsed (&H22) = -1
								Case "PLAYER_IN_Y_TILES":
									fzx1 = val (lP (2)) * 16 - 15
									If fzx1 < 0 Then fzx1 = 0
									fzx2 = (val (lP (3)) + 1) * 16 + 15
									If fzx2 > 191 Then fzx2 = 191
									clausula = clausula + chr (&H22) + chr (fzx1) + chr (fzx2)
									clausulasUsed (&H22) = -1
								Case "NPANT":
									' 0  1     2    3
									' IF NPANT [OP] 10'
									If lP (2) <> "=" And lP (2) <> "!=" And lP (3) <> "<>" Then
										lP (3) = lP (2): lP (2) = "="
									End If
									Select Case lP (2)
										Case "=": opCode = &H50
										Case "<>", "!=": opCode = &H51
										Case Else
											Print "ERROR - Wrong operand": End
									End Select
									clausula = clausula + chr (opCode) + chr (pvalR (lP (3)))
									clausulasUsed (opCode) = -1
								Case "NPANT_NOT"
									clausula = clausula + chr (&H51) + chr (pvalR (lP (2)))
									clausulasUsed (&H51) = -1

								' Conditions about enemies killed
								Case "ALL_ENEMIES_DEAD":
									clausula = clausula + chr (&H30)
									clausulasUsed (&H30) = -1
								Case "ENEMIES_KILLED_EQUALS":
									clausula = clausula + chr (&H31) + chr (pvalR (lP (2)))
									clausulasUsed (&H31) = -1
								Case "ENEMIES_KILLED", "ENEMS_KILLED":
									clausula = clausula + chr (&H31) + chr (pvalR (lP (3)))
									clausulasUsed (&H31) = -1

								' Conditions based on recent actions
								Case "JUST_PUSHED":
									clausula = clausula + chr (&H60)
									clausulasUsed (&H60) = -1
								Case "JUST_INTERACTED"
									clausula = clausula + chr (&H61)
									clausulasUsed (&H61) = -1
								Case "ARG"
									Select Case lP (2)
										Case "=":
											value = pvalR (lP (3))
											opCode = &H62
										Case "~":
											value = pvalL (lP (3))
											If value = -1 Then Print "ERROR - Wrong lvalue in ARG ~": End
											value = 128 + value
											opCode = &H63
										Case Else
											Print "ERROR - Wrong operand": End
									End Select
									clausula = clausula + chr (opCode) + chr (value)
									clausulasUsed (opCode) = -1
							End Select
						End If

					End If
				Case "THEN":
					' Close conditions; changes state
					If clausula = "" Then 
						? "ERROR - THEN without IFs": End
					End If
					clausula = clausula + Chr (255)					
					estado = 1

				Case "END":
					' And end in state 0 closes this section
					If estado = 0 Then sc_terminado = -1

			End Select

		Else
			' Actions
			Select Case lP (0)
				Case "END":
					clausula = clausula + Chr (&HFF)
					clausula = Chr (len (clausula)) + clausula
					
					If debug Then
						For i = 1 To Len (clausula) 
							Print Hex (Asc (Mid (clausula, i, 1)), 2); " ";
						Next i
						Print 
					End If

					clausulas = clausulas + clausula
					estado = 0
					clausula = ""
			End Select
		End If
	Wend

	procesaClausulas = clausulas
End Function

maxClausuleSize = 0
itemEmpty = -1

Dim As Integer fIn, fOut

Print "MK1 v1.0 mscmk1 1.0 ~ ";
If Len (Command (3)) = 0 Then usage: End
debug = inCommand ("debug")

fIn = FreeFile
Open Command (1) For Input As #fIn
fOut = FreeFile
Open Command (2) For Output As #fOut

' Test
procesaClausulas fIn

' Write interpreter

' Conditions with IF FLAG

If clausulasUsed (&H10) Then
	' IF FLAG x = y
	' &H10 x y
	Print #fOut, "                case 0x10: readxy (); sc_terminado = (flags [sc_x] != sc_y); break;"
End If
If clausulasUsed (&H11) Then
	' IF FLAG x < y
	' &H11 x y
	Print #fOut, "                case 0x11: readxy (); sc_terminado = (flags [sc_x] >= sc_y); break;"
End If
If clausulasUsed (&H12) Then
	' IF FLAG x > y
	' &H12 x y
	Print #fOut, "                case 0x12: readxy (); sc_terminado = (flags [sc_x] <= sc_y); break;"
End If
If clausulasUsed (&H13) Then
	' IF FLAG x != y
	' &H13 x y
	Print #fOut, "                case 0x13: readxy (); sc_terminado = (flags [sc_x] == sc_y); break;"
End If

' Conditions about POSITION

If clausulasUsed (&H20) Then
	' IF PLAYER_TOUCHES (x, y)
	' &H20 x y
	Print #fOut, "                case 0x20: readxy (); sc_x << = 4; sc_y = 16 + (sc_y << 4); sc_terminado = (!(prx + 7 >= sc_x && prx <= sc_x + 15 && pry + 15 >= sc_y && pry <= sc_y + 15)); break;"
End If
If clausulasUsed (&H21) Then
	' IF PLAYER_IN_X ('x1', 'x2')
	' &H21 'x' 'y'
	Print #fOut, "                case 0x21: sc_terminado = (!(prx >= read_byte () && prx <= read_byte ())); break;"
End If
If clausulasUsed (&H22) Then
	' IF PLAYER_IN_Y ('y1', 'y2')
	' &H22 'x' 'y'
	Print #fOut, "                case 0x22: sc_terminado = (!(pry >= read_byte () && pry <= read_byte ())); break;"
End If

If clausulasUsed (&H50) Then
	' IF NPANT = n'
	' &H50 n
	Print #fOut, "                case 0x50: sc_terminado = (n_pant != read_vbyte ()); break;"
End If
If clausulasUsed (&H51) Then
	' IF NPANT <> n'
	' &H51 n
	Print #fOut, "                case 0x51: sc_terminado = (n_pant == read_vbyte ()); break;"
End If

' Conditions about enemies DEAD

If clausulasUsed (&H30) Then
	' IF ALL_ENEMIES_DEAD
	' &H30
	Print #fOut, "                case 0x30: sc_terminado = (pkilled != c_max_enems); break;"	
End If
If clausulasUsed (&H31) Then
	' IF ENEMIES_KILLED_EQUALS n
	' IF ENEMIES_KILLED = n
	' IF ENEMS_KILLED = n
	' &H31 n
	Print #fOut, "                case 0x31: sc_terminado = (pkilled != read_vbyte ()); break;"
End If

' Conditions based on recent actions

If clausulasUsed (&H60) Then
	' IF JUST_PUSHED
	' &60
	Print #fOut, "                case 0x60: sc_terminado = (!just_pushed); break;"
End If
If clausulasUsed (&H61) Then
	' IF JUST_INTERACTED
	' &61
	Print #fOut, "                case 0x61: sc_terminado = (!just_interacted); break;"
End If
If clausulasUsed (&H62) Then
	' IF ARG = n
	' &62 n
	Print #fOut, "                case 0x62: sc_terminado = (script_arg != read_vbyte ()); break;"
End If
If clausulasUsed (&H63) Then
	' IF ARG ~ 'n'
	' &63 'n'
	Print #fOut, "                case 0x63: sc_terminado = (script_arg != read_byte ()); break;"
End If
