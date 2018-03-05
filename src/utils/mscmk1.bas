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

Function pValL (s As String) As Integer
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

Function pValR (s As String) As Integer
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
	pValR = res
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
			AddAlias lP (0), pValR (lP (1))
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
	ElseIf Left (word, 1) = "$" Or Left (word, 1) = "#" Then
		res = -1
	Else
		res = -1
		For i = 1 To Len (word)
			m = Mid (word, i, 1)
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
		ElseIf lP (i) = "SET" Or lP (i) = "INC" Or lP (i) = "DEC" Then
			If Not strictMscNumeric (lP (i + 1)) Then
				lP (i) = lP (i) & lP (i + 1)
				lPshiftLeft i + 2
			End If
		End If
		i = i + 1
	Wend
End Sub

Sub fixEquals (position As Integer)
	If lP (position) = "=" Then lPshiftLeft (position + 1)
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
	If debug Then Print

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
							lvalue = pValL (lP (1))
							If lvalue = -1 Then Print "ERROR - Wrong lvalue": End

							Select Case lP (2)
								Case "=": opCode = &H10
								Case "<": opCode = &H11
								Case ">": opCode = &H12
								Case "<>", "!=": opCode = &H13
								Case Else:
									Print "ERROR - Wrong operand": End
							End Select

							clausula = clausula + Chr (opCode) + Chr (lvalue) + Chr (pValR (lP (3)))
							clausulasUsed (opCode) = -1							
						Else
							Select Case Ucase (lP (1))

								' Conditions about position
								Case "PLAYER_TOUCHES":
									clausula = clausula + chr (&H20) + chr (pValR (lP (2))) + chr (pValR (lP (3)))
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
									clausula = clausula + chr (opCode) + chr (pValR (lP (3)))
									clausulasUsed (opCode) = -1
								Case "NPANT_NOT"
									clausula = clausula + chr (&H51) + chr (pValR (lP (2)))
									clausulasUsed (&H51) = -1
								Case "LEVEL"
									Select Case lP (2)
										Case "=": opCode = &H80
										Case "<>", "!=": OpCode = &H81
										Case Else
											Print "ERROR - Wrong operand": End
									End Select
									clausula = clausula + chr (opCode) + chr (pValR (lP (3)))
									clausulasUsed (opCode) = -1

								' Conditions about enemies killed
								Case "ALL_ENEMIES_DEAD":
									clausula = clausula + chr (&H30)
									clausulasUsed (&H30) = -1
								Case "ENEMIES_KILLED_EQUALS":
									clausula = clausula + chr (&H31) + chr (pValR (lP (2)))
									clausulasUsed (&H31) = -1
								Case "ENEMIES_KILLED", "ENEMS_KILLED":
									clausula = clausula + chr (&H31) + chr (pValR (lP (3)))
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
											value = pValR (lP (3))
											opCode = &H62
										Case "~":
											value = pValL (lP (3))
											If value = -1 Then Print "ERROR - Wrong lvalue in ARG ~": End
											value = 128 + value
											opCode = &H63
										Case Else
											Print "ERROR - Wrong operand": End
									End Select
									clausula = clausula + chr (opCode) + chr (value)
									clausulasUsed (opCode) = -1

								' Conditions based upon # of collectable objects
								Case "PLAYER_HAS_OBJECTS"
									clausula = clausula + chr (&H40)
									clausulasUsed (&H61) = -1
								Case "OBJECT_COUNT"
									If lP(2) <> "=" Then Print "ERROR - Wrong operand": End
									clausula = clausula + chr (&H41) + chr (pValR (lP (3)))
									clausulasUsed (&H41) = -1

								' Conditions based on the timer
								Case "TIMER"
									Select Case lP (2)
										Case ">=": opCode = &H70
										Case "<=": opCode = &H71
										Case "=": opCode = &H72
										Case "<>", "!=": opCode = &H73
										Case Else
											Print "ERROR - Wrong operand": End
									End Select
									clausula = clausula + Chr (opCode) + Chr (pValR (lP (3)))
									clausulasUsed (opCode) = -1

								' T
								Case "TRUE"
									clausula = clausula + chr (&HF0)
									clausulasUsed (&HF0) = -1
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
			Select Case Ucase (lP (0))

				' Setters
				Case "SET":
					' 0   1      2 3
					' SET lvalue = rvalue
					lvalue = pValL (lP (1))
					If lvalue = -1 Then Print "ERROR - Wrong lvalue": End
					rValue = pValR (lP (3))
					If lP (2) <> "=" Then Print "ERROR - Syntax error in SET": End
					clausula = clausula + Chr (&H1) + Chr (lvalue) + Chr (rvalue)
					actionsUsed (&H1) = -1
				Case "SET_TILE"
					' 0        1 2 3 4'
					' SET_TILE x y = t'
					If lP (3) <> "=" Then Print "ERROR - Syntax error in SET_TILE": End
					clausula = clausula + Chr (&H20) + Chr (pValR (lP (1))) + Chr (pValR (lP (2))) + Chr (pValR (lP (4)))
					actionsUsed (&H20) = -1
				Case "SET_SAFE"
				    ' 0        1    2 3
					' SET_SAFE HERE
					' SET_SAFE np   x y
					If lP (1) = "HERE" Then
						clausula = clausula + Chr (&HEA)
						actionsUsed (&HEA) = -1
					Else
						clausula = clausula + Chr (&HE9) + Chr (pValR (lP (1))) + Chr (pValR (lP (2))) + Chr (pValR (lP (3)))
						actionsUsed (&HE9) = -1
					End If
				Case "SET_FIRE_ZONE":
					clausula = clausula + Chr (&H51) + Chr (Val (lP (1))) + Chr (Val (lP (2))) + Chr (Val (lP (3))) + Chr (Val (lP (4)))
					actionsUsed (&H51) = -1
				Case "SET_FIRE_ZONE_TILES":
					fzx1 = pValR (lP (1)) * 16 - 7: If fzx1 < 0 Then fzx1 = 0
					fzy1 = (pValR (lP (2)) + 1) * 16 - 15: If fzy1 < 0 Then fzy1 = 0
					fzx2 = pValR (lP (3)) * 16 + 15: If fzx2 > 255 Then fzx2 = 255
					fzy2 = (pValR (lP (4)) + 1) * 16 + 15: If fzx2 > 207 Then fzx2 = 207
					clausula = clausula + Chr (&H51) + Chr (fzx1) + Chr (fzy1) + Chr (fzx2) + Chr (fzy2)
					actionsUsed (&H51) = -1
				Case "SETX", "SET_X":
					fixEquals 1
					clausula = clausula + Chr (&H6B) + Chr (pValR (lP (1)))
					actionsUsed (&H6B) = -1
				Case "SETX", "SET_Y":
					fixEquals 1
					clausula = clausula + Chr (&H6A) + Chr (pValR (lP (1)))
					actionsUsed (&H6A) = -1
				Case "SETXY", "SET_XY":
					fixEquals 1
					clausula = clausula + Chr (&H68) + Chr (pValR (lP (1))) + Chr (pValR (lP (2)))
					actionsUsed (&H68) = -1
				Case "SET_TIMER"
					fixEquals 1
					clausula = clausula + Chr (&H70) + Chr (pValR (lP (1)))
					actionsUsed (&H70) = -1

				' Maths
				Case "INC", "DEC", "ADD", "SUB"
					' 0  1      2
					' OP lvalue rvalue
					lValue = pValL (lP (1))
					If lValue = -1 Then Print "ERROR - Wrong lvalue": End
					rValue = pValR (lP (2))
					Select Case Ucase (lP (0))
						Case "INC", "ADD": opCode = &H10
						Case "DEC", "SUB": opCode = &H11
					End Select
					clausula = clausula + Chr (opCode) + Chr (lValue) +  Chr (rValue)
					actionsUsed (opCode) = -1
				Case "SWAP"
					clausula = clausula + Chr (&H14) + Chr (pVal (lP (1))) + Chr (pVal (lP (2)))
					actionsUsed (&H14) = -1
				Case "FLIPFLOP"
					clausula = clausula + Chr (&H15) + Chr (pVal (lP (1)))
					actionsUsed (&H15) = -1

				Case "INC_LIFE"
				Case "DEC_LIFE"
				Case "RECHARGE"
				Case "INC_OBJECTS"
				Case "DEC_OBJECTS"

				Case "FLICKER"
				
				Case "PRINT_TILE_AT"
				Case "SHOW"
				Case "TEXT"
				Case "TEXTBOX"

				Case "REDRAW"
				Case "REENTER"
				Case "WARP_TO"
				Case "REPOSTN"

				Case "TIMER_START"
				Case "TIMER_STOP"

				Case "ADD_CONTAINER"
				Case "ADD_SPRITE"

				Case "EXTERN"

				Case "PAUSE"

				Case "MUSIC"
				Case "SOUND"

				Case "GAME", "GAME_OVER"
				Case "WIN", "WIN_GAME"

				Case "BREAK"

				Case "DECORATIONS"

				' Finally
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

Print #fOut, "#ifdef CLEAR_FLAGS"
Print #fOut, "void msc_clear_flags (void) {"
Print #fOut, "    memfill (flags, 0, MAX_FLAGS);"
Print #fOut, "}"
Print #fOut, "#endif"
Print #fOut, ""
Print #fOut, "unsigned char read_byte (void) {"
Print #fOut, "    return *script ++;"
Print #fOut, "}"
Print #fOut, ""
Print #fOut, "unsigned char read_vbyte (void) {"
Print #fOut, "    sc_c = *script ++;"
Print #fOut, "    if (sc_c & 128) return flags [sc_c & 127];"
Print #fOut, "    return sc_c;"
Print #fOut, "}"
Print #fOut, ""
Print #fOut, "void readxy (void) {"
Print #fOut, "    sc_x = read_vbyte ();"
Print #fOut, "    sc_y = read_vbyte ();"
Print #fOut, "}"
If actionsUsed (&H68) Or actionsUsed (&H6A) Or actionsUsed (&H6B) Or actionsUsed (&H6C) Or actionsUsed (&H6D) Then
	Print #fOut, ""
	Print #fOut, "void reloc_player (void) {"
	Print #fOut, "    prx = read_vbyte () << 4;        px = prx << FIXBITS;"
	Print #fOut, "    pry = (read_vbyte () << 4) + 16; py = pry << FIXBITS;"
	Print #fOut, "    player_stop ();"
	Print #fOut, "}"
End If
Print #fOut, ""
Print #fOut, "void run_script (unsigned char whichs) {"
Print #fOut, "    // read address offset from index"
Print #fOut, "    gp_gen = (unsigned char *) script_pool + whichs + whichs;"
Print #fOut, "    rda = *gp_gen ++; rdb = *gp_gen;"
Print #fOut, "    script_result = 0;"
Print #fOut, "    sc_continuar = 0;"
Print #fOut, "    if (!(rda | rdb)) return;"
Print #fOut, "    script = (unsigned char *) script_pool + rda + (rdb << 8);"
Print #fOut, ""
Print #fOut, "    while ((sc_c = read_byte ()) != 0xff) {"
Print #fOut, "        next_script = script + sc_c;"
Print #fOut, "        sc_terminado = sc_continuar = 0;"
Print #fOut, "        while (!sc_terminado) {"
Print #fOut, "            switch (read_byte ()) {"

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
	' IF NPANT = n
	' &H50 n
	Print #fOut, "                case 0x50: sc_terminado = (n_pant != read_vbyte ()); break;"
End If
If clausulasUsed (&H51) Then
	' IF NPANT <> n
	' &H51 n
	Print #fOut, "                case 0x51: sc_terminado = (n_pant == read_vbyte ()); break;"
End If

If clausulasUsed (&H80) Then
	' IF LEVEL = n
	' &H80 n
	Print #fOut, "                case 0x80: sc_terminado = (level != read_vbyte ()); break;"
End If
If clausulasUsed (&H80) Then
	' IF LEVEL <> n
	' &H81 n
	Print #fOut, "                case 0x81: sc_terminado = (level == read_vbyte ()); break;"
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

' Conditions based on collectable objects count
If clausulasUsed (&H40) Then
	' IF PLAYER_HAS_OBJECTS
	' &H40
	Print #fOut, "                case 0x40: sc_terminado = (pobjs == 0); break;"
End If
If clausulasUsed (&H41) Then
	' IF OBJECT_COUNT = n
	' &H41 n
	Print #fOut, "                case 0x41: sc_terminado = (pobjs != read_vbyte ()); break;"
End If

' Conditions based on TIMER
If clausulasUsed (&H70) Then
	' IF TIMER >= n
	' &H70 n
	Print #fOut, "                case 0x70: sc_terminado = (timer < read_vbyte ()); break;"
End If
If clausulasUsed (&H71) Then
	' IF TIMER <= n
	' &H71 n
	Print #fOut, "                case 0x71: sc_terminado = (timer > read_vbyte ()); break;"
End If
If clausulasUsed (&H72) Then
	' IF TIMER = n
	' &H72 n
	Print #fOut, "                case 0x72: sc_terminado = (timer != read_vbyte ()); break;"
End If
If clausulasUsed (&H73) Then
	' IF TIMER <> n
	' &H73 n
	Print #fOut, "                case 0x73: sc_terminado = (timer == read_vbyte ()); break;"
End If

' T
If clausulasUsed (&HF0) Then
	' IF TRUE
	' &HF0
	Print #fOut, "                case 0xf0: break;"
End If

' End conditions

Print #fOut, "                case 0xff: sc_terminado = sc_continuar = 1; break;"
Print #fOut, "            }"
Print #fOut, "        }"
Print #fOut, ""
Print #fOut, "        if (sc_continuar) {"
Print #fOut, "            fire_script_success |= sc_continuar;"
Print #fOut, "            sc_terminado = 0;"
Print #fOut, "            while (!sc_terminado) {"
Print #fOut, "                switch (read_byte ()) {"

' Actions

' Setter actions

If actionsUsed (&H1) Then
	' SET #x = y
	' &H11 x y
	Print #fOut, "                    case 0x01: readxy (); flags [sc_x] = sc_y; break;"
End If

If actionsUsed (&H20) Then
	' SET_TILE x y t
	' &H20 x y t
	Print #fOut, "                    case 0x20: readxy (); _x = sc_x; _y = sc_y; _t = read_vbyte (); map_set (); break;"
End If

If actionsUsed (&H51) Then
	' SET_FIRE_ZONE x1 y1 x2 y2
	' &H51 'x1' 'y1' 'x2' 'y2'
	Print #fOut, "                    case 0x51: fzx1 = read_byte (); fzy1 = read_byte (); fzx2 = read_byte (); fzy2 = read_byte (); break;"
End If

If actionsUsed (&H68) Then
	' SET_XY x y
	' &H68 x y
	Print #fOut, "                    case 0x68: reloc_player (); break;"
End If
If actionsUsed (&H6A) Then
	' SET_Y x
	' &H6A x
	Print #fOut, "                    case 0x6a: py = read_vbyte () << 10; player_stop (); break;"
End If
If actionsUsed (&H6B) Then
	' SET_X x
	' &H6B x
	Print #fOut, "                    case 0x6b: px = read_vbyte () << 10; player_stop (); break;"
End If

If actionsUsed (&H70) Then
	' SET_TIMER n
	' &H70 n
	Print #fOut, "                    case 0x70: timer = read_vbyte (); break;"
End If

If actionsUsed (&HE9) Then
	' SET SAFE n, x, y
	' &HE9 n x y
	Print #fOut, "                    case 0xe9: n_pant_safe = read_vbyte (); px_safe = read_vbyte () << 10; py_safe = read_vbyte () << 10; break;"
End If
If actionsUsed (&HEA) Then
	' SET SAFE HERE
	' &HEA
	Print #fOut, "                    case 0xea: player_register_safe_spot (); break;"
End If

' Maths

If actionsUsed (&H10) Then
	' INC #n x
	' &H10 n x
	Print #fOut, "                    case 0x10: readxy (); flags [sc_x] += sc_y; break;"
End If
If actionsUsed (&H11) Then
	' DEC #n x
	' &H11 n x
	Print #fOut, "                    case 0x11: readxy (); flags [sc_x] -= sc_y; break;"
End If

If actionsUsed (&H14) Then
	' SWAP #x #y
	' &H14 x y
	Print #fOut, "                    case 0x14: readxy (); rda = flags [sc_x]; flags [sc_x] = flags [sc_y]; flags [sc_y] = rda; break;"
End If

If actionsUsed (&H15) Then
	' FLIPFLOP #n
	' &H15 n
	Print #fOut, "                    case 0x15: sc_x = read_vbyte (); flags [sc_x] = 1 - flags [sc_x]; break;"
End If

' End actions

Print #fOut, "                    case 0xff: sc_terminado = 1; break;"
Print #fOut, "                }"
Print #fOut, "            }"
Print #fOut, "        }"
Print #fOut, "        script = next_script;"
Print #fOut, "    }"
Print #fOut, "}"

Close

Print "DONE!"
