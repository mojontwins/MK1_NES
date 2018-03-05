' Nuevo compilador MojonTwins Script Compiler
' MT Engine MK1 NES (0.7+)
' Copyleft 2016 The Mojon Twins

' Compilar con freeBasic (http://www.freebasic.net).

Const LIST_WORDS_SIZE = 128
Const LIST_ALIAS_SIZE = 255
Const MACRO_SIZE = 32
Const MAX_MACROS = 31

Redim Shared As String script (0)
Redim Shared As Integer addresses (0)
Dim Shared As Integer clausulasUsed (255)
Dim Shared As Integer actionsUsed (255)
Dim Shared As uByte myBin (16383)
Dim As uInteger maxClausuleSize
Dim Shared lP (LIST_WORDS_SIZE) As String
Dim Shared listaAlias (LIST_ALIAS_SIZE) As String
Dim Shared decoInclude As String
Dim Shared As Integer doIncludeDecos, decoFetch
Dim Shared As Byte decorated (255)
Dim Shared As Integer maxScr
Dim Shared As String macros (MAX_MACROS, MACRO_SIZE)
Dim Shared As Integer macrosPtr

' Items
Dim Shared As Integer maxItem
Dim Shared As Integer itemSetx, itemSety, itemSetStep, itemSetOr, itemFlag, slotFlag
Dim Shared As Integer itemSelectClr, itemSelectC1, itemSelectC2, itemEmpty

maxClausuleSize = 0
itemEmpty = -1

Sub resetScript (n as Integer)
	Dim as integer i
	For i = 0 to n
		script (i) = ""
	next i
	For i = 0 to 16383
		myBin (i) = 0
	Next i
End Sub

Sub stringToArray (in As String)
	Dim As Integer m, index, i1, i2, found, quotes
	Dim As String character, curWord
	
	For m = 0 To LIST_WORDS_SIZE: lP (m) = "": Next m
	in = Trim (in): If in = "" Then Exit Sub
		
	index = 0: curWord = "": in = in + " "
	quotes = 0
	
	For m = 1 To Len (in)
		character = Ucase (Mid (in, m, 1))
		If quotes Then
			If character = Chr (34) Then 
				quotes = 0
			Else
				curWord = curWord + character
			End If
		Else 
			If character = Chr (34) Then
				quotes = -1
			ElseIf Instr (" " & Chr (9) & ",<=>;()", character) Then
				If curWord <> "" Then
					' Macro insertion
					If Left (curWord, 1) = "%" Then
						' Attempt to find macro
						found = 0
						For i1 = 0 TO MAX_MACROS
							If curWord = macros (i1, 0) Then
								For i2 = 1 TO MACRO_SIZE
									If macros (i1, i2) <> "" Then
										lP (index) = macros (i1, i2)			
										index = index + 1
										IF index >= LIST_WORDS_SIZE Then Exit For
									End If
								Next i2
								found = -1
								Exit For
							End If
						Next i1
						If Not found Then
							lP (index) = curWord
							index = index + 1
							If index >= LIST_WORDS_SIZE Then Exit For
						End If
					Else
						lP (index) = curWord
						index = index + 1
						If index >= LIST_WORDS_SIZE Then Exit For
					End If
					curWord = ""
				End If
				If Instr (" " & Chr (9), character) = 0 Then
					lP (index) = character
					index = index + 1
					If index >= LIST_WORDS_SIZE Then Exit For
				End If
			Else
				curWord = curWord & character
			End If
		End If
	Next m
	'for m = 0 to index:Print lP (m); " ";:next m: Print
End Sub

Function pval (s as string) as integer
	Dim res as integer
	Dim i as integer
	' Patch
	if s = "SLOT_SELECTED" then
		pval = 128 + slotFlag
	elseif s = "ITEM_SELECTED" then
		pval = 128 + itemFlag
	else
		if (left(s, 1) = "#") Then
			'? "CALL TO GET VALUE OF " & right (s, len(s) - 1)
			res = 128 + pval (right (s, len(s) - 1))
			'? "WHICH WAS " & (res - 128)
		ElseIf (left (s, 1) = "$") Then
			res = 0
			For i = 0 To LIST_ALIAS_SIZE
				if s = listaAlias (i) Then res = i: Exit For
			Next i
			'? "ALIAS: " & s & " VALUE:" & res
		Else
			res = val (s)
		End If
		pval = res
	end if
end function

Sub ProcesaItems (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		stringToArray (linea)
		Select Case Ucase (lP (0))
			Case "SLOT_FLAG":
				slotFlag = val (lP (1))
			Case "ITEM_FLAG":
				itemFlag = val (lP (1))
			Case "LOCATION":
				itemSetX = val (lP (1))
				itemSetY = val (lP (3))
			Case "DISPOSITION":
				itemSetStep = val (lP (3))
				If lP (1) = "HORZ" then itemSetOr = 0 Else ItemSetOr = 1
			Case "SELECTOR":
				itemSelectClr = val (lP (1))
				itemSelectC1 = val (lP (3))
				itemSelectC2 = val (lP (5))
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
	listaAlias (flag) = nameA
	'? "Adding " & flag & " = " & nameA
End Sub

Sub AddMacro ()
	' Macro name is lP (0)
	' Macro is from lP (1) onwards (MACRO_SIZE words)
	Dim As Integer i
	For i = 0 To MACRO_SIZE
		macros (macrosPtr, i) = lP (i)
	Next i
	If macrosPtr < MAX_MACROS Then macrosPtr = macrosPtr + 1 Else Print "** WARNING ** MAX_MACROS reached! Next will overwrite last."
End Sub

Sub ProcesaAlias (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		stringToArray (linea)
		If lP (0) = "END" Then terminado = Not 0
		If Left (lP (0), 1) = "$" Then
			AddAlias lP (0), pval (lP (1))
		ElseIf Left (lP (0), 1) = "%" Then
			AddMacro
		End If
		If eof (f) Then terminado = Not 0
	Wend
End Sub

Sub autoExpandFlags ()
	Dim As Integer irw
	' Esta función auto-expande "#X" por "FLAG X" en el primer parámetro
	' vale para no tener que poner "SET FLAG $HOLA = 1" y poder poner "SET $HOLA = 1"
	' que me parece un poco menos coñazo y, endehe luego, más legible.
	
	' En cualquier caso está en LP 1, y al detectarla hay que echar lo demás una casilla "pátrás" 
	' y meter flag en LP 1 y el valor en LP 2.
	
	' ¿Cuántos echar p'atrás? Pongamos que 10, cubrirá esto mucho rato hasta que casque y
	' yo me tire 2 horas preguntándome por qué.
	
	' Pêro para no variar la CALIDAZZ de este códigow.
	
	If Len (lP (1)) > 1 Then
		If Left (lP (1), 1) = "$" Then
			For irw = 10 to 2 Step -1
				lP (irw) = lP (irw - 1)
			Next irw
			lP (1) = "FLAG"
		End If
	End If	
End Sub

Function procesaClausulas (f As integer) As String
	' Lee cláusulas de la pantalla nPant del archivo abierto f
	Dim linea As String
	Dim sc_terminado As Integer
	Dim estado As integer
	Dim clausulas As String
	Dim clausula As String
	Dim numclausulas As Integer
	Dim longitud As Integer
	Dim ai As Integer
	Dim itt As Integer
	
	Dim As Integer fzx1, fzx2, fzy1, fzy2

	Dim As Integer dF

	sc_terminado = 0
	estado = 0
	numclausulas = 0
	longitud = 0
	clausulas = ""

	'' ===========================================================================
	' Decorations?
	If doIncludeDecos Then
		' Abrimos el archivo
		dF = FreeFile
		Open decoInclude For Input as #dF
			' Buscamos la pantalla en cuestión

			While Not sc_terminado And Not Eof (dF)
				Line input #dF, linea
				linea = Trim (linea, Any chr (32) + chr (9))
				If linea <> "" Then
					stringToArray (linea)
					If lP (0) = "ENTERING" and val (lP (2)) = decoFetch Then
						estado = 1
						sc_terminado = Not 0
					End If
				End If
			Wend

			If estado = 1 Then
				? "Including custom decorations for screen " & decoFetch

				' Creamos una nueva clausula IF TRUE
				clausula = chr (&HF0)
				clausulasUsed (&HF0) = -1

				' Then
				clausula = clausula + Chr (255)

				' Ahora buscamos la sección DECORATIONS
				sc_terminado = 0
				While Not sc_terminado And Not Eof (dF)
					Line input #dF, linea
					linea = Trim (linea, Any chr (32) + chr (9))
					stringToArray (linea)

					if lP(0) = "DECORATIONS" Then
						' parsear e incluir una lista de prints.
						clausula = clausula + Chr (&HF4)
						actionsUsed (&HF4) = -1

						do
							Line input #dF, linea
							linea = Trim (linea, Any chr (32) + chr (9))
							if ucase (linea)="END" then sc_terminado = -1: exit do
							stringToArray (linea)
							' X,Y,T -> XY T
							clausula = clausula + Chr ((Val(lP(0)) Shl 4) + (Val(lP(2)) And 15)) + Chr (Val(lP(4)))
						loop

						' END del decorations
						clausula = clausula + Chr (&HFF)
					End If
				Wend

				' End de la cláusula
				clausula = clausula + Chr (&HFF)

				' Añadimos la longitud de la cláusula a la cláusula
				clausula = Chr (len (clausula)) + clausula

				' Y la cláusula a la lista de cláusulas.
				clausulas = clausulas + clausula
			End If
		Close dF
	End If
	'' ===========================================================================

	clausula = ""
	sc_terminado = 0
	estado = 0

	If f Then
		While not sc_terminado And Not eof (f)
			Line input #f, linea
			linea = Trim (linea, Any chr (32) + chr (9))
			'?estado & " " & linea
			'?linea ;"-";:displayMe clausula
			stringToArray (linea)

			if estado <> 1 then
				' Leyendo cláusulas
				Select Case lP (0)
					case "IF":
						autoExpandFlags ()
						Select Case lP (1)
							Case "PLAYER_HAS_ITEM":
								clausula = clausula + chr (&H1) + chr (pval (lP (2)))
								numClausulas = numClausulas + 1
								clausulasUsed (&H1) = -1
							Case "PLAYER_HASN'T_ITEM":
								clausula = clausula + chr (&H2) + chr (pval (lP (2)))
								numClausulas = numClausulas + 1
								clausulasUsed (&H2) = -1
							Case "SEL_ITEM":
								Select case lP (2)
									Case "=":
										clausula = clausula + chr (&H10) + chr (itemFlag) + chr (pval (lP (3)))
										numClausulas = numClausulas + 1
										clausulasUsed (&H10) = -1
									Case "<>", "!=":
										clausula = clausula + chr (&H13) + chr (itemFlag) + chr (pval (lP (3)))
										numClausulas = numClausulas + 1
										clausulasUsed (&H13) = -1
								End Select
							Case "ITEM":
								Select case lP (3)
									Case "=":
										clausula = clausula + chr (&H3) + chr (pval (lP (2))) + chr (pval(lP (4)))
										numClausulas = numClausulas + 1
										clausulasUsed (&H3) = -1
									Case "<>", "!=":
										clausula = clausula + chr (&H4) + chr (pval (lP (2))) + chr (pval(lP (4)))
										numClausulas = numClausulas + 1
										clausulasUsed (&H4) = -1
								End Select
							Case "FLAG":
								Select Case lP (3)
									Case "=":
										if lP (4) = "FLAG" Then
											clausula = clausula + chr (&H14) + chr (pval (lP (2))) + chr (pval(lP (5)))
											clausulasUsed (&H14) = -1
										Else
											clausula = clausula + chr (&H10) + chr (pval (lP (2))) + chr (pval(lP (4)))
											clausulasUsed (&H10) = -1
										End If
									Case "<":
										If lP (4) = "FLAG" Then
											clausula = clausula + chr (&H15) + chr (pval (lP (2))) + chr (pval(lP (5)))
											clausulasUsed (&H15) = -1
										Else
											clausula = clausula + chr (&H11) + chr (pval (lP (2))) + chr (pval(lP (4)))
											clausulasUsed (&H11) = -1
										End If
									Case ">":
										If lP (4) = "FLAG" Then
											clausula = clausula + chr (&H16) + chr (pval (lP (2))) + chr (pval(lP (5)))
											clausulasUsed (&H16) = -1
										Else
											clausula = clausula + chr (&H12) + chr (pval (lP (2))) + chr (pval(lP (4)))
											clausulasUsed (&H12) = -1
										End If
									Case "<>", "!=":
										If lP (4) = "FLAG" Then
											clausula = clausula + chr (&H17) + chr (pval (lP (2))) + chr (pval(lP (5)))
											clausulasUsed (&H17) = -1
										Else
											clausula = clausula + chr (&H13) + chr (pval (lP (2))) + chr (pval(lP (4)))
											clausulasUsed (&H13) = -1
										End If
								End Select
								numClausulas = numClausulas + 1
							Case "PLAYER_TOUCHES":
								clausula = clausula + chr (&H20) + chr (pval (lP (2))) + chr (pval (lP (4)))
								clausulasUsed (&H20) = -1
								numClausulas = numClausulas + 1
							Case "PLAYER_IN_X":
								clausula = clausula + chr (&H21) + chr (val (lP (2))) + chr (val (lP (4)))
								clausulasUsed (&H21) = -1
								numClausulas = numClausulas + 1
							Case "PLAYER_IN_X_TILES":
								fzx1 = val (lP (2)) * 16 - 15
								If fzx1 < 0 Then fzx1 = 0
								fzx2 = val (lP (4)) * 16 + 15
								If fzx2 > 255 Then fzx2 = 255
								clausula = clausula + chr (&H21) + chr (fzx1) + chr (fzx2)
								clausulasUsed (&H21) = -1
								numClausulas = numClausulas + 1
							Case "PLAYER_IN_Y_TILES":
								fzx1 = val (lP (2)) * 16 - 15
								If fzx1 < 0 Then fzx1 = 0
								fzx2 = (val (lP (4)) + 1) * 16 + 15
								If fzx2 > 191 Then fzx2 = 191
								clausula = clausula + chr (&H22) + chr (fzx1) + chr (fzx2)
								clausulasUsed (&H22) = -1
								numClausulas = numClausulas + 1
							Case "PLAYER_IN_Y":
								clausula = clausula + chr (&H22) + chr (val (lP (2))) + chr (val (lP (4)))
								clausulasUsed (&H22) = -1
								numClausulas = numClausulas + 1
							Case "ALL_ENEMIES_DEAD"
								clausula = clausula + chr (&H30)
								clausulasUsed (&H30) = -1
								numClausulas = numClausulas + 1
							Case "ENEMIES_KILLED_EQUALS"
								clausula = clausula + chr (&H31) + chr (pval (lP (2)))
								clausulasUsed (&H31) = -1
								numClausulas = numClausulas + 1
							Case "PLAYER_HAS_OBJECTS"
								clausula = clausula + chr (&H40)
								clausulasUsed (&H40) = -1
								numClausulas = numClausulas + 1
							Case "OBJECT_COUNT"
								clausula = clausula + chr (&H41) + chr (pval (lP (3)))
								clausulasUsed (&H41) = -1
								numClausulas = numClausulas + 1
							Case "NPANT"
								clausula = clausula + chr (&H50) + chr (pval (lP (2)))
								clausulasUsed (&H50) = -1
								numClausulas = numClausulas + 1
							Case "NPANT_NOT"
								clausula = clausula + chr (&H51) + chr (pval (lP (2)))
								clausulasUsed (&H51) = -1
								numClausulas = numClausulas + 1
							Case "JUST_PUSHED"
								clausula = clausula + chr (&H60)
								clausulasUsed (&H60) = -1
								numClausulas = numClausulas + 1
							Case "JUST_INTERACTED"
								clausula = clausula + chr (&H61)
								clausulasUsed (&H61) = -1
								numClausulas = numClausulas + 1
							Case "ARG"
								clausula = clausula + chr (&H62) + chr (pval (lP (3)))
								clausulasUsed (&H62) = -1
								numClausulas = numClausulas + 1
							Case "TIMER"
								If lP (2) = ">=" Then
									clausula = clausula + chr (&H70) + chr (pval (lP (3)))
									clausulasUsed (&H70) = -1
									numClausulas = numClausulas + 1
								ElseIf lP (2) = "<=" Then
									clausula = clausula + chr (&H71) + chr (pval (lP (3)))
									clausulasUsed (&H71) = -1
									numClausulas = numClausulas + 1
								End If
							Case "LEVEL"
								If lP (2) = "=" Then
									clausula = clausula + Chr (&H80) + chr (pval (lP (3)))
									clausulasUsed (&H80) = -1
									numClausulas = numClausulas + 1
								End If
							Case "TRUE"
								clausula = clausula + chr (&HF0)
								clausulasUsed (&HF0) = -1
								numClausulas = numClausulas + 1
							Case Else
								' Intento recuperar para construcciones tipo IF #3 = 4!
								'											 0	1  2 3
								If lP (1) <> "" Then
									If lP (1) = "SLOT_SELECTED" Then lP (1) = "#" & slotFlag
									If lP (1) = "ITEM_SELECTED" Then lP (1) = "#" & itemFlag
									If Left (lP (1), 1) = "#" Then
										' Generar un IF FLAG n = ...
										' Shift:
										For itt = 5 To 3 Step -1
											lP (itt) = lP (itt - 1)
										Next itt
										
										lP (2) = Right (lP (1), len (lP (1)) - 1)
														
										Select Case lP (3)
											Case "=":
												if lP (4) = "FLAG" Then
													clausula = clausula + chr (&H14) + chr (pval (lP (2))) + chr (pval(lP (5)))
													clausulasUsed (&H14) = -1
												Else
													clausula = clausula + chr (&H10) + chr (pval (lP (2))) + chr (pval(lP (4)))
													clausulasUsed (&H10) = -1
												End If
											Case "<":
												If lP (4) = "FLAG" Then
													clausula = clausula + chr (&H15) + chr (pval (lP (2))) + chr (pval(lP (5)))
													clausulasUsed (&H15) = -1
												Else
													clausula = clausula + chr (&H11) + chr (pval (lP (2))) + chr (pval(lP (4)))
													clausulasUsed (&H11) = -1
												End If
											Case ">":
												If lP (4) = "FLAG" Then
													clausula = clausula + chr (&H16) + chr (pval (lP (2))) + chr (pval(lP (5)))
													clausulasUsed (&H16) = -1
												Else
													clausula = clausula + chr (&H12) + chr (pval (lP (2))) + chr (pval(lP (4)))
													clausulasUsed (&H12) = -1
												End If
											Case "<>", "!=":
												If lP (4) = "FLAG" Then
													clausula = clausula + chr (&H17) + chr (pval (lP (2))) + chr (pval(lP (5)))
													clausulasUsed (&H17) = -1
												Else
													clausula = clausula + chr (&H13) + chr (pval (lP (2))) + chr (pval(lP (4)))
													clausulasUsed (&H13) = -1
												End If
										End Select
										numClausulas = numClausulas + 1
									End If
								End If
						End Select
					case "THEN":
						clausula = clausula + Chr (255)
						if numclausulas = 0 Then Print "ERROR: THEN sin cláusulas": sc_terminado = -1
						estado = 1
					case "END":
						if estado = 0 then
						sc_terminado = -1
						end if
				end select
			else
				' Leyendo acciones
				Select Case lP (0)
					Case "SET":
						autoExpandFlags ()
						Select Case lP (1)
							Case "ITEM":
								clausula = clausula + Chr (&H0) + Chr (pval (lP (2))) + chr (pval (lP (4)))
								actionsUsed (&H0) = -1
							Case "FLAG":
								clausula = clausula + Chr (&H1) + Chr (pval (lP (2))) + chr (pval (lP (4)))
								actionsUsed (&H1) = -1
							Case "TILE":
								clausula = clausula + Chr (&H20) + Chr (pval (lP (3))) + Chr (pval (lP (5))) + Chr (pval (lP (8)))
								actionsUsed (&H20) = -1
							Case "SAFE":
								If lp (2) = "HERE" Then
									clausula = clausula + Chr (&HEA)
									actionsUsed (&HEA) = -1
								Else
									clausula = clausula + Chr (&HE9) + Chr (pval (lP (2))) + Chr (pval (lp (4))) + Chr (pval (lp (6)))
									actionsUsed (&HE9) = -1
								End If
						End Select
					Case "INC":
						autoExpandFlags ()
						Select Case lP (1)
							Case "FLAG":
								clausula = clausula + Chr (&H10) + Chr (pval (lP (2))) + chr (pval (lP (4)))
								actionsUsed (&H10) = -1
							Case "LIFE":
								clausula = clausula + Chr (&H30) + Chr (pval (lP (2)))
								actionsUsed (&H30) = -1
							Case "OBJECTS":
								clausula = clausula + Chr (&H40) + Chr (pval (lP (2)))
								actionsUsed (&H40) = -1
						End Select
					Case "DEC":
						autoExpandFlags ()
						Select Case lP (1)
							Case "FLAG":
								clausula = clausula + Chr (&H11) + Chr (pval (lP (2))) + chr (pval (lP (4)))
								actionsUsed (&H11) = -1
							Case "LIFE":
								clausula = clausula + Chr (&H31) + Chr (pval (lP (2)))
								actionsUsed (&H31) = -1
							Case "OBJECTS":
								clausula = clausula + Chr (&H41) + Chr (pval (lP (2)))
								actionsUsed (&H41) = -1
						End Select
					Case "ADD":
						clausula = clausula + Chr (&H12) + Chr (pval (lP (2))) + chr (pval (lP (4)))
						actionsUsed (&H12) = -1
					Case "SUB":
						clausula = clausula + Chr (&H13) + Chr (pval (lP (2))) + chr (pval (lP (4)))
						actionsUsed (&H13) = -1
					Case "SWAP":
						clausula = clausula + Chr (&H14) + Chr (pval (lP (1))) + chr (pval (lP (3)))
						actionsUsed (&H14) = -1
					Case "FLIPFLOP":
						clausula = clausula + Chr (&H15) + Chr (pval (lP (1)))
						actionsUsed (&H15) = -1
					Case "FLICKER":
						clausula = clausula + Chr (&H32)
						actionsUsed (&H32) = -1
					Case "DIZZY":
						clausula = clausula + Chr (&H33)
						actionsUsed (&H33) = -1
					Case "PRINT_TILE_AT":
						clausula = clausula + Chr (&H50) + Chr (pval (lP (2))) + Chr (pval (lP (4))) + Chr (pval (lP (7)))
						actionsUsed (&H50) = -1
					Case "SET_FIRE_ZONE":
						clausula = clausula + Chr (&H51) + Chr (pval (lP (1))) + Chr (pval (lP (3))) + Chr (pval (lP (5))) + Chr (pval (lP (7)))
						actionsUsed (&H51) = -1
					Case "SET_FIRE_ZONE_TILES":
						fzx1 = pval (lP (1)) * 16 - 7
						If fzx1 < 0 Then fzx1 = 0
						fzy1 = (pval (lP (3)) + 1) * 16 - 15
						If fzy1 < 0 Then fzy1 = 0
						fzx2 = pval (lP (5)) * 16 + 15
						If fzx2 > 255 Then fzx2 = 255
						fzy2 = (pval (lP (7)) + 1) * 16 + 15
						If fzy2 > 191 Then fzy2 = 191
						
						clausula = clausula + Chr (&H51) + Chr (fzx1) + Chr (fzy1) + Chr (fzx2) + Chr (fzy2)
						actionsUsed (&H51) = -1
					Case "SHOW_COINS":
						clausula = clausula + Chr (&H60)
						actionsUsed (&H60) = -1
					Case "HIDE_COINS":
						clausula = clausula + Chr (&H61)
						actionsUsed (&H61) = -1
					Case "ENABLE_KILL_SLOWLY"
						clausula = clausula + Chr (&H62)
						actionsUsed (&H62) = -1
					Case "DISABLE_KILL_SLOWLY"
						clausula = clausula + Chr (&H63)
						actionsUsed (&H63) = -1
					Case "ENABLE_TYPE_6"
						clausula = clausula + Chr (&H64)
						actionsUsed (&H64) = -1
					Case "DISABLE_TYPE_6"
						clausula = clausula + Chr (&H65)
						actionsUsed (&H65) = -1
					Case "ENABLE_MAKE_TYPE_6"
						clausula = clausula + Chr (&H66)
						actionsUsed (&H66) = -1
					Case "DISABLE_MAKE_TYPE_6"
						clausula = clausula + Chr (&H67)
						actionsUsed (&H67) = -1
					Case "REDRAW"
						clausula = clausula + Chr (&H6E)
						actionsUsed (&H6E) = -1
					Case "REENTER"
						clausula = clausula + Chr (&H6F)
						actionsUsed (&H6F) = -1
					Case "REHASH"
						clausula = clausula + Chr (&H73)
						actionsUsed (&H73) = -1
					Case "WARP_TO"
						clausula = clausula + Chr (&H6D) + Chr (pval (lP (1))) + Chr (pval (lP (3))) + Chr (pval (lP (5)))
						actionsUsed (&H6D) = -1
					Case "REPOSTN"
						clausula = clausula + Chr (&H6C) + Chr (pval (lP (1))) + Chr (pval (lP (3)))
						actionsUsed (&H6C) = -1
					Case "SETX"
						clausula = clausula + Chr (&H6B) + Chr (pval (lP (1)))
						actionsUsed (&H6B) = -1
					Case "SETY"
						clausula = clausula + Chr (&H6A) + Chr (pval (lP (1)))
						actionsUsed (&H6A) = -1
					Case "SETXY"
						clausula = clausula + Chr (&H68) + Chr (pval (lP (1))) + Chr (pval (lP (3)))
						actionsUsed (&H68) = -1
					Case "WARP_TO_LEVEL"
						clausula = clausula + Chr (&H69) + Chr (pval (lP (1))) + Chr (pval (lP (3))) + Chr (pval (lP (5))) + Chr (pval (lP (7))) + Chr (pval (lP (9)))
						actionsUsed (&H69) = -1
					Case "SET_TIMER"
						clausula = clausula + Chr (&H70) + Chr (pval (lP (1))) 
						actionsUsed (&H70) = -1
					Case "TIMER_START"
						clausula = clausula + Chr (&H71)
						actionsUsed (&H71) = -1
					Case "TIMER_STOP"
						clausula = clausula + Chr (&H72)
						actionsUsed (&H72) = -1
					Case "ENEMY"
						If lP (2) = "ON" Then
							clausula = clausula + Chr (&H80) + Chr (pval (lP (1)))
							actionsUsed (&H80) = -1
						ElseIf lP (2) = "OFF" Then
							clausula = clausula + Chr (&H81) + Chr (pval (lP (1)))
							actionsUsed (&H81) = -1
						ElseIf lp (2) = "TYPE" Then
							clausula = clausula + Chr (&H83) + Chr (pval (lP (1))) + Chr (pval (lP (3)))
							actionsUsed (&H83) = -1
						End If
					Case "ENEMIES"
						If lp (1) = "RESTORE" Then
							If lp (2) = "ALL" Then
								clausula = clausula + Chr (&H85)
								actionsUsed (&H85) = -1
							Else
								clausula = clausula + Chr (&H84)
								actionsUsed (&H84) = -1
							End If
						End If
					Case "ADD_FLOATING_OBJECT"
						clausula = clausula + Chr (&H82) + Chr (pval (lP (1))) + Chr (pval (lP (3))) + Chr (pval (lP (5)))
						actionsUsed (&H82) = -1
					Case "ADD_CONTAINER"
						clausula = clausula + Chr (&H86) + Chr (val (lP (3)) + 16 * (1+val (lP (5)))) + Chr (128 + pval (lP (1)))
						actionsUsed (&H86) = -1
					Case "ADD_SPRITE"
						clausula = clausula + Chr (&H86) + Chr (val (lP (3)) + 16 * (1+val (lP (5)))) + Chr (pval (lP (1)))
						actionsUsed (&H86) = -1
					Case "SHOW_CONTAINERS"
						clausula = clausula + Chr (&H87)
						actionsUsed (&H87) = -1
					Case "NEXT_LEVEL":
						clausula = clausula + Chr (&HD0)
						actionsUsed (&HD0) = -1
					Case "SOUND":
						clausula = clausula + Chr (&HE0) + Chr (pval (lP (1)))
						actionsUsed (&HE0) = -1
					Case "SHOW":
						clausula = clausula + Chr (&HE1)
						actionsUsed (&HE1) = -1
					Case "RECHARGE":
						clausula = clausula + Chr (&HE2)
						actionsUsed (&HE2) = -1
					Case "TEXT":
						clausula = clausula + Chr (&HE3) + Chr (Len (lP (1)))
						For ai = 1 To Len (lP (1))
							If Mid (lP (1), ai, 1) = "_" Then
								clausula = clausula + Chr (32)
							Else
								clausula = clausula + Mid (lP (1), ai, 1)
							End If
						Next ai
						clausula = clausula + Chr (0)
						actionsUsed (&HE3) = -1
					Case "OPENTEXT":
						clausula = clausula + Chr (&HEB)
						actionsUsed (&HEB) = -1
					Case "CLOSETEXT":
						clausula = clausula + Chr (&HEC)
						actionsUsed (&HEC) = -1
					case "TEXTBOX":
						clausula = clausula + Chr (&HED) + Chr (Val (lP (1)))
						actionsUsed (&HED) = -1
					Case "EXTERN":
						clausula = clausula + Chr (&HE4) + Chr (Val (lP (1)))
						actionsUsed (&HE4) = -1
					Case "EXTERN_E":
						clausula = clausula + Chr (&HE8) + Chr (Val (lP (1))) + Chr (Val (lP (3)))
						actionsUsed (&HE8) = -1
					Case "PAUSE":
						clausula = clausula + Chr (&HE5) + Chr (Pval (lP (1)))
						actionsUsed (&HE5) = -1
					Case "MUSIC"
						clausula = clausula + Chr (&HE6) + Chr (Pval (lP (1)))
						actionsUsed (&HE6) = -1
					Case "REDRAW_ITEMS"
						clausula = clausula + Chr (&HE7)
						actionsUsed (&HE7) = -1
					Case "GAME":
						clausula = clausula + Chr (&HF0)
						actionsUsed (240) = -1
					Case "WIN":
						clausula = clausula + Chr (&HF1)
						actionsUsed (241) = -1
					Case "BREAK":
						clausula = clausula + Chr (&HF2)
						actionsUsed (&HF2) = -1
					Case "GAME_ENDING":
						clausula = clausula + Chr (&HF3)
						actionsUsed (&HF3) = -1
					case "DECORATIONS"
						' parsear e incluir una lista de prints.
						clausula = clausula + Chr (&HF4)
						actionsUsed (&HF4) = -1
						do
							Line input #f, linea
							linea = Trim (linea, Any chr (32) + chr (9))
							if ucase (linea)="END" then exit do
							stringToArray (linea)
							' X,Y,T -> XY T
							clausula = clausula + Chr ((Val(lP(0)) Shl 4) + (Val(lP(2)) And 15)) + Chr (Val(lP(4)))
						loop
						clausula = clausula + Chr (&HFF)
					Case "END":
						clausula = clausula + Chr (&HFF)
						clausula = Chr (len (clausula)) + clausula
						clausulas = clausulas + clausula
						numclausulas = 0
						estado = 0
						clausula = ""
				End Select
			End if
		Wend
	End If
	If Clausulas <> "" Then procesaClausulas = Clausulas + Chr (255)
End Function

Sub displayMe (clausula As String)
	Dim i As Integer
	Dim p As String

	For i = 1 To Len (clausula)
		p = Str (asc (mid (clausula, i, 1)))
		if Len (p) = 1 Then p = "00" + p
		If Len (p) = 2 Then p = "0" + p
		? p;
		If i < Len (clausula) Then ? ", ";
	Next i
	Print
End Sub

Dim As Integer f, f2, f3, i, j, clin, nPant, maxidx, scriptCount, binPt, offsPt
Dim As String linea, clausulas, o, inFileName, outFileName
Dim As Integer keepGoing, sDone, nSections, killing
Dim As String thisLevelConstantName

' TODO: Comprobación de la entrada
'inFileName = "test.spt"
'outFileName = "msc.h"
inFileName = command (1)
maxidx = 2 * pval (command (2))
maxScr = val (Command (2)) - 1

if command (1) = "" or	maxidx = 0 then
	print "MojonTwins Scripts Compiler 3.95 [NES]"
	print "MT Engine NES MK1 1.0+"
	Print
	print "usage: msc3nes input n_pants"
	print ""
	print "input   - script file"
	print "n_pants - # of screens"
	print
	system
end if

redim script (maxidx + 5)
redim addresses (maxidx + 5)
resetScript (maxidx + 5)

Print "MK1 v1.0 msc3nes 3.95 ~ ";

'Kill "scripts.bin"

macrosPtr = 0
For i = 0 To MAX_MACROS
	For j = 0 TO MACRO_SIZE
		macros (i, j) = ""
	Next j
Next i

f = Freefile
Open inFileName for input as #f
f2 = Freefile
Open "mscnes.h" For Output as #f2
f3 = Freefile
'Open "msc-config.h" For Output as #f3
'
'Print #f3, "// msc-config.h"
'Print #f3, "// Generado por Mojon Script Compiler 3 de la MT Engine NES MK1"
'Print #f3, "// Copyleft 2016 The Mojon Twins"
'Print #f3, ""
'Print #f3, "unsigned char sc_x, sc_y, sc_n, sc_m, sc_c;"
'Print #f3, "unsigned char script_result = 0;"
'Print #f3, "unsigned char sc_terminado = 0;"
'Print #f3, "unsigned char sc_continuar = 0;"
'Print #f3, ""
'Print #f3, "extern unsigned char main_script [0];"
'Print #f3, "#asm"
'Print #f3, "	 ._main_script"
'Print #f3, "		 BINARY ""scripts.bin"""
'Print #f3, "#endasm"
'Print #f3, "#endif"
'Print #f3, "unsigned char warp_to_level;"
'Print #f3, "extern unsigned char *script;"
'Print #f3, "#asm"
'Print #f3, "	 ._script defw 0"
'Print #f3, "#endasm"
'Print #f3, ""

Print #f2, "// script data and interpreter"
Print #f2, "// generated by msc3nes by the mojon twins"
Print #f2, ""
Print #f2, "const unsigned char script_pool [] = {"

maxClausuleSize = 0
scriptCount = 0
offsPt = 0
nSections = 0
killing = 0

Print "Parsing " & inFileName & " ~ ";

keepGoing = -1
sDone = 0
decoInclude = ""
thisLevelConstantName = ""
While keepGoing
	if (eof (f)) then
		keepGoing = 0
		linea = ""
		'if sDone then
			lP (0) = "END_OF_LEVEL"
			killing = -1
		'end if
	else
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		stringToArray (linea)
	end if

	doIncludeDecos = 0

	Select Case lP (0)
		Case "LEVELID":
			thisLevelConstantName = lP (1)
			? "L" & scriptcount & " = " & thisLevelConstantName & " ~ ";
			' ? "Level " & scriptcount & "'s name is " & thisLevelConstantName
		Case "INC_DECORATIONS":
			decoInclude = lP (1)
			? "L" & scriptcount & " decos @ " & decoInclude & " ~ ";
			' ? "Extra decorations file for current level is " & decoInclude
			For i = 0 To maxScr: decorated (i) = 0: Next i
		Case "ITEMSET":
			procesaItems f
		Case "DEFALIAS"
			procesaAlias f
		Case "ENTERING":
			sDone = -1
			if lP (1) <> "GAME" And lp (1) <> "ANY" And decoInclude <> "" Then
				doIncludeDecos = -1
				decoFetch = Val (lP (2))
				decorated (decoFetch) = -1
			End If
			clausulas = procesaClausulas (f)
			stringToArray (linea)
			if lP (1) = "GAME" then
				script (maxidx) = clausulas
			elseif lP (1) = "ANY" then
				script (maxidx + 1) = clausulas
			else
				For i = 2 to LIST_WORDS_SIZE
					If lP (i) <> "" And lP (i) <> "," Then
						script (2 * val(lP(i))) = clausulas
					End If
				Next i
			endif
			nSections = nSections + 1
		Case "PRESS_FIRE":
			sDone = -1
			clausulas = procesaClausulas (f)
			stringToArray (linea)
			if lP (2) = "ANY" then
				script (maxidx + 2) = clausulas
			else
				For i = 3 to LIST_WORDS_SIZE
					If lP (i) <> "" And lP (i) <> "," Then
						script (1 + 2 * val(lP(i))) = clausulas
					End If
				Next i
			end if
			nSections = nSections + 1
		Case "ON_TIMER_OFF":
			sDone = -1
			clausulas = procesaClausulas (f)
			stringToArray (linea)
			script (maxidx + 3) = clausulas
			nSections = nSections + 1
		Case "PLAYER_GETS_COIN":
			sDone = -1
			clausulas = procesaClausulas (f)
			stringToArray (linea)
			script (maxidx + 4) = clausulas
			nSections = nSections + 1
		Case "PLAYER_KILLS_ENEMY":
			sDone = -1
			clausulas = procesaClausulas (f)
			stringToArray (linea)
			script (maxidx + 5) = clausulas
			nSections = nSections + 1
		Case "END_OF_LEVEL":
			' Decorations: check if screens containing to ENTERING script
			' Should be decorated...
			If decoInclude <> "" Then
				For i = 0 To maxScr
					If Not decorated (i) Then
						doIncludeDecos = -1
						decoFetch = i
						clausulas = procesaClausulas (0)
						script (2 * i) = clausulas
						nSections = nSections + 1
					End If
				Next i
			End If

			If killing Then
				'Print "End Of Script reached."
				'If nSections Then Print "Sections for the game: " & nSections
				? "Finished. Total sections: " & nSections & " ~ ";
			Else
				'Print "End Of Level reached."
				'Print "Sections in this level: " & nSections
				? "Level finished. Sections: " & nSections & " ~ ";
			End If

			If sDone Then
				' write and reset
				binPt = (maxidx + 6) * 2
				for i = 0 to maxidx + 5
					If Len (script (i)) > maxClausuleSize Then maxClausuleSize = Len (script (i))
					If Len (script (i)) = 0 Then
						addresses (i) = 0
					Else
						addresses (i) = binPt
					End If
					For j = 1 To Len (script (i))
						myBin (binPt) = Asc (Mid (script (i), j, 1))
						binPt = binPt + 1
					Next j
					myBin (i * 2) = addresses (i) And 255
					myBin (i * 2 + 1) = addresses (i) Shr 8
				next i
				
				'For i = 0 To binPt - 1
				'	Put #f2, , myBin (i)
				'Next i
				j = 0: clin = 0
				Print #f2, "/*            0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F */"
				Print #f2, "/* " & hex (clin, 4) & " */ ";
				For i = 0 To binPt - 1
					Print #f2, "0x" & Lcase (Hex (myBin (i), 2));
					j = j + 1: If j = 16 Then j = 0
					If i < binPt - 1 Then 
						Print #f2, ", ";
						If j = 0 Then clin = clin + 1: Print #f2, "": Print #f2, "/* " & hex (clin, 4) & " */ ";
					End If
				Next i

				If thisLevelConstantName = "" Then
					'Print #f3, "#define SCRIPT_" & scriptcount & " 0x" & Hex (offsPt, 4)
				Else
					'Print #f3, "#define " & thisLevelConstantName & " 0x" & Hex (offsPt, 4)
				End If

				offsPt = offsPt + binPt

				scriptCount = scriptCount + 1
				resetScript (maxidx + 5)
			EndIf				
			sDone = 0
			decoInclude = ""
			thisLevelConstantName = ""
			nSections = 0
	End Select
Wend

Print #f2, "": Print #f2, "};": Print #f2, ""

If maxItem > 0 then

	'print #f3, ""
	'print #f3, "// This game uses items..."
	'print #f3, ""
	'print #f3, "#define MSC_MAXITEMS " & str (maxitem)
	'print #f3, "#define FLAG_SLOT_SELECTED " & slotFlag
	'print #f3, "#define FLAG_ITEM_SELECTED " & itemFlag
	'print #f3, "#define ITEM_EMPTY " & itemEmpty
	'print #f3, " "
	'print #f3, "unsigned char items [MSC_MAXITEMS];"
	'print #f3, "unsigned char its_it, its_p;"
	'print #f3, " "
	'print #f3, "int key_z;"
	'print #f3, "unsigned char key_z_pressed = 0;"
	'print #f3, " "
'	'
	'print #f3, "void draw_cursor (unsigned char a, unsigned char b, unsigned char x, unsigned char y) {"
	'print #f3, "	if (a != b) {"
	'print #f3, "		sp_PrintAtInv (y, x, 0, 0);"
	'print #f3, "		sp_PrintAtInv (y, x + 1, 0, 0);"
	'print #f3, "	} else {"
	'print #f3, "		sp_PrintAtInv (y, x, " & itemSelectClr & ", " & itemSelectC1 & ");"
	'print #f3, "		sp_PrintAtInv (y, x + 1, " & itemSelectClr & ", " & itemSelectC2 & ");"
	'print #f3, "	}"
	'print #f3, "}"
	'print #f3, " "
	
	' Generate display_items
	'If itemSetOr = 0 Then
		' Horizontal
		'print #f3, "void display_items (void) {"
		'print #f3, "	 its_p = " & itemSetX & ";"
		'print #f3, "	 for (its_it = 0; its_it < MSC_MAXITEMS; its_it ++) {"
		'if itemEmpty <> -1 then
			'print #f3, "		 if (items [its_it]) {"
			'print #f3, "			 draw_coloured_tile (its_p, " & itemSetY & ", items [its_it]);"
			'print #f3, "		 } else {"
			'print #f3, "			 draw_coloured_tile (its_p, " & itemSetY & ", " & itemEmpty & ");"
			'print #f3, "		 }"
		'else
			'print #f3, "		 draw_coloured_tile (its_p, " & itemSetY & ", items [its_it]);"
		'end if
		'print #f3, "		 draw_cursor (its_it, flags [FLAG_SLOT_SELECTED], its_p, " & (itemSetY + 2) & ");"
		'print #f3, "		 its_p += " & itemSetStep & ";"
		'print #f3, "	 }"
		'print #f3, "}"
		'print #f3, " "
	'Else
		' Vertical
		'print #f3, "void display_items (void) {"
		'print #f3, "	 its_p = " & itemSetY & ";"
		'print #f3, "	 for (its_it = 0; its_it < MSC_MAXITEMS; its_it ++) {"
		'if itemEmpty <> -1 then
			'print #f3, "		 if (items [its_it]) {"
			'print #f3, "			 draw_coloured_tile (" & itemSetX & ", its_p, items [its_it]);"
			'print #f3, "		 } else {"
			'print #f3, "			 draw_coloured_tile (" & itemSetX & ", its_p, " & itemEmpty & ");"
			'print #f3, "		 }"
		'else
			'print #f3, "		 draw_coloured_tile (" & itemSetX & ", its_p, items [its_it]);"
		'end if
		'print #f3, "		 draw_cursor (its_it, flags [FLAG_SLOT_SELECTED], " & itemSetX & ", its_p + 2);"
		'print #f3, "		 its_p += " & itemSetStep & ";"
		'print #f3, "	 }"
		'print #f3, "}"
		'print #f3, " "
	'End If
	
	'print #f3, "void do_inventory_fiddling (void) {"
	'print #f3, "	if (sp_KeyPressed (key_z)) {"
	'print #f3, "		if (!key_z_pressed) {"
	'print #f3, "#ifdef MODE_128K"
	'print #f3, "			_AY_PL_SND (2);"
	'print #f3, "#else"
	'print #f3, "			peta_el_beeper (2);"
	'print #f3, "#endif"
	'print #f3, "#ifdef MSC_MAXITEMS"
	'print #f3, "			flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % MSC_MAXITEMS;"
	'print #f3, "#else"
	'print #f3, "			flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % SIM_DISPLAY_MAXITEMS;"
	'print #f3, "#endif"
	'print #f3, "			display_items ();"
	'print #f3, "		}"
	'print #f3, "		key_z_pressed = 1;"
	'print #f3, "	} else {"
	'print #f3, "		key_z_pressed = 0;"
	'print #f3, "	}"
	'print #f3, "}"
	'print #f3, " "

	'If clausulasUsed (&H1) Or clausulasUsed (&H2) Then
		' Search item
		'print #f3, "unsigned char search_item (unsigned char t) {"
		'print #f3, "	 for (its_it = 0; its_it < MSC_MAXITEMS; its_it ++) {"
		'print #f3, "		 if (items [its_it] == t) return 1;"
		'print #f3, "	 }"
		'print #f3, "	 return 0;"
		'print #f3, "}"
	'End If

	'print #f3, ""
end if

Close #f

Print #f2, "#ifdef CLEAR_FLAGS"
Print #f2, "void msc_clear_flags (void) {"
Print #f2, "    memfill (flags, 0, MAX_FLAGS);"
Print #f2, "}"
Print #f2, "#endif"
Print #f2, ""
Print #f2, "unsigned char read_byte (void) {"
Print #f2, "	return *script ++;"
Print #f2, "}"
Print #f2, ""
Print #f2, "unsigned char read_vbyte (void) {"
Print #f2, "	sc_c = *script ++;"
Print #f2, "	if (sc_c & 128) return flags [sc_c & 127];"
Print #f2, "	return sc_c;"
Print #f2, "}"
Print #f2, ""
Print #f2, "void readxy (void) {"
Print #f2, "	sc_x = read_vbyte ();"
Print #f2, "	sc_y = read_vbyte ();"
Print #f2, "}"
If actionsUsed (&H68) Or actionsUsed (&H6A) Or actionsUsed (&H6B) Or actionsUsed (&H6C) Or actionsUsed (&H6D) Then
	Print #f2, ""
	Print #f2, "void reloc_player (void) {"
	Print #f2, "	prx = read_vbyte () << 4;        px = prx << FIXBITS;"
	Print #f2, "	pry = (read_vbyte () << 4) + 16; py = pry << FIXBITS;"
	Print #f2, "	player_stop ();"
	Print #f2, "}"
End If
Print #f2, ""
Print #f2, "void run_script (unsigned char whichs) {"
Print #f2, "	// read address offset from index"
Print #f2, "	gp_gen = (unsigned char *) script_pool + whichs + whichs;"
Print #f2, "	rda = *gp_gen ++; rdb = *gp_gen;"
Print #f2, "	script_result = 0;"
Print #f2, "	sc_continuar = 0;"
Print #f2, "	if (!(rda | rdb)) return;"
Print #f2, "	script = (unsigned char *) script_pool + rda + (rdb << 8);"
Print #f2, ""
Print #f2, "	// todo : update selected item flag"
Print #f2, ""
Print #f2, "	while ((sc_c = read_byte ()) != 0xff) {"
Print #f2, "		next_script = script + sc_c;"
Print #f2, "		sc_terminado = sc_continuar = 0;"
Print #f2, "		while (!sc_terminado) {"
Print #f2, "			switch (read_byte ()) {"

'' TODO
if clausulasUsed (&H1) Then
	print #f2, "				case 0x01:"
	print #f2, "					// IF PLAYER_HAS_ITEM sc_x"
	print #f2, "					// Opcode: 01 sc_x"
	print #f2, "					sc_terminado = (!search_item (read_vbyte ()));"
	print #f2, "					break;"
end if

if clausulasUsed (&H2) Then
	print #f2, "				case 0x02:"
	print #f2, "					// IF PLAYER_HASN'T_ITEM sc_x"
	print #f2, "					// Opcode: 02 sc_x"
	print #f2, "					sc_terminado = (search_item (read_vbyte ()));"
	print #f2, "					break;"
end if

If clausulasUsed (&H3) Then
	print #f2, "				case 0x03:"
	print #f2, "					// IF ITEM x = n"
	print #f2, "					// Opcode: 03 x n"
	print #f2, "					sc_terminado = items [read_vbyte ()] != read_vbyte ();"
	print #f2, "					break;"
End If

If clausulasUsed (&H4) Then
	print #f2, "				case 0x04:"
	print #f2, "					// IF ITEM x <> n"
	print #f2, "					// Opcode: 04 x n"
	print #f2, "					sc_terminado = items [read_vbyte ()] == read_vbyte ();"
	print #f2, "					break;"
End If
'' EOTODO

if clausulasUsed (&H10) Then
	print #f2, "				case 0x10:"
	print #f2, "					// IF FLAG sc_x = sc_n"
	print #f2, "					// Opcode: 10 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] != sc_y);"
	print #f2, "					break;"
end if

if clausulasUsed (&H11) Then
	print #f2, "				case 0x11:"
	print #f2, "					// IF FLAG sc_x < sc_n"
	print #f2, "					// Opcode: 11 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] >= sc_y);"
	print #f2, "					break;"
end if

if clausulasUsed (&H12) Then
	print #f2, "				case 0x12:"
	print #f2, "					// IF FLAG sc_x > sc_n"
	print #f2, "					// Opcode: 12 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] <= sc_y);"
	print #f2, "					break;"
end if

if clausulasUsed (&H13) Then
	print #f2, "				case 0x13:"
	print #f2, "					// IF FLAG sc_x <> sc_n"
	print #f2, "					// Opcode: 13 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] == sc_y);"
	print #f2, "					break;"
end if

if clausulasUsed (&H14) Then
	print #f2, "				case 0x14:"
	print #f2, "					// IF FLAG sc_x = FLAG sc_y"
	print #f2, "					// Opcode: 14 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] != flags [sc_y]);"
	print #f2, "					break;"
end if

if clausulasUsed (&H15) Then
	print #f2, "				case 0x15:"
	print #f2, "					// IF FLAG sc_x < FLAG sc_y"
	print #f2, "					// Opcode: 15 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] >= flags [sc_y]);"
	print #f2, "					break;"
end if

if clausulasUsed (&H16) Then
	print #f2, "				case 0x16:"
	print #f2, "					// IF FLAG sc_x > FLAG sc_y"
	print #f2, "					// Opcode: 16 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] <= flags [sc_y]);"
	print #f2, "					break;"
end if

if clausulasUsed (&H17) Then
	print #f2, "				case 0x17:"
	print #f2, "					// IF FLAG sc_x <> FLAG sc_y"
	print #f2, "					// Opcode: 17 sc_x sc_n"
	print #f2, "					readxy ();"
	print #f2, "					sc_terminado = (flags [sc_x] == flags [sc_y]);"
	print #f2, "					break;"
end if

if clausulasUsed (&H20) Then
	print #f2, "				case 0x20:"
	print #f2, "					// IF PLAYER_TOUCHES x, y"
	print #f2, "					// Opcode: 20 sc_x sc_y"
	print #f2, "					readxy ();"
	print #f2, "					sc_x <<= 4; sc_y = 16 + (sc_y << 4);"
	print #f2, "					sc_terminado = (!(prx + 7 >= sc_x && prx <= sc_x + 15 && pry + 15 >= sc_y && pry <= sc_y + 15));"
	print #f2, "					break;"
end if

if clausulasUsed (&H21) Then
	print #f2, "				case 0x21:"
	print #f2, "					// IF PLAYER_IN_X x1, x2"
	print #f2, "					// Opcode: 21 x1 x2"
	print #f2, "					sc_x = read_byte ();"
	print #f2, "					sc_y = read_byte ();"
	print #f2, "					sc_terminado = (!(prx >= sc_x && prx <= sc_y));"
	print #f2, "					break;"
end if

if clausulasUsed (&H22) Then
	print #f2, "				case 0x22:"
	print #f2, "					// IF PLAYER_IN_Y y1, y2"
	print #f2, "					// Opcode: 22 y1 y2"
	print #f2, "					sc_x = read_byte ();"
	print #f2, "					sc_y = read_byte ();"
	print #f2, "					sc_terminado = (!(pry >= sc_x && pry <= sc_y));"
	print #f2, "					break;"
end if

if clausulasUsed (&H30) Then
	print #f2, "				case 0x30:"
	print #f2, "					// IF ALL_ENEMIES_DEAD"
	print #f2, "					// Opcode: 30"
	print #f2, "					sc_terminado = (pkilled != BADDIES_COUNT);"
	print #f2, "					break;"
end if

if clausulasUsed (&H31) Then
	print #f2, "				case 0x31:"
	print #f2, "					// IF ENEMIES_KILLED_EQUALS sc_n"
	print #f2, "					// Opcode: 31 sc_n"
	print #f2, "					sc_terminado = (pkilled != read_vbyte ());"
	print #f2, "					break;"
End If

if clausulasUsed (&H40) Then
	print #f2, "				case 0x40:"
	print #f2, "					 // IF PLAYER_HAS_OBJECTS"
	print #f2, "					 // Opcode: 40"
	print #f2, "					 sc_terminado = (pobjs == 0);"
	print #f2, "					 break;"
End If

If clausulasUsed (&H41) Then
	print #f2, "				case 0x41:"
	print #f2, "					 // IF OBJECT_COUNT = sc_n"
	print #f2, "					 // Opcode: 41 sc_n"
	print #f2, "					 sc_terminado = (pobjs != read_vbyte ());"
	print #f2, "					 break;"
End If

If clausulasUsed (&H50) Then
	print #f2, "				case 0x50:"
	print #f2, "					 // IF NPANT sc_n"
	print #f2, "					 // Opcode: 50 sc_n"
	print #f2, "					 sc_terminado = (n_pant != read_vbyte ());"
	print #f2, "					 break;"
End If

If clausulasUsed (&H51) Then
	print #f2, "				case 0x51:"
	print #f2, "					 // IF NPANT_NOT sc_n"
	print #f2, "					 // Opcode: 51 sc_n"
	print #f2, "					 sc_terminado = (n_pant == read_vbyte ());"
	print #f2, "					 break;"
End If

If clausulasUsed (&H60) Then
	print #f2, "				case 0x60:"
	print #f2, "					 // IF JUST_PUSHED"
	print #f2, "					 // Opcode: 60"
	print #f2, "					 sc_terminado = (!just_pushed);"
	print #f2, "					 break;"
End If

If clausulasUsed (&H61) Then
	Print #f2, "                case 0x61:"
	Print #f2, "                    // IF JUST_INTERACTED"
	Print #f2, "                    // Opcode: 61"
	Print #f2, "                    sc_terminado = (!just_interacted);"
	Print #f2, "                    break;"
End If

If clausulasUsed (&H62) Then
	Print #f2, "                case 0x62:"
	Print #f2, "                    // IF ARG = sc_n"
	Print #f2, "                    // Opciode: 62 sc_n"
	Print #f2, "                    sc_terminado = (script_arg != read_vbyte ());"
	Print #f2, "                    break;"
End If

If clausulasUsed (&H70) Then
	print #f2, "				case 0x70:"
	print #f2, "					// IF TIMER >= sc_x"
	print #f2, "					sc_terminado = (timer < read_vbyte ());"
	print #f2, "					break;"
End If

If clausulasUsed (&H71) Then
	print #f2, "				case 0x71:"
	print #f2, "					// IF TIMER <= sc_x"
	print #f2, "					sc_terminado = (timer > read_vbyte ());"
	print #f2, "					break;"
End If

If clausulasUsed (&H80) Then
	print #f2, "				case 0x80:"
	print #f2, "					// IF LEVEL = sc_n"
	print #f2, "					// Opcode: 80 sc_n"
	print #f2, "					sc_terminado = (read_vbyte () != level);"
	print #f2, "					break;"
End If
'' EOTODO

if clausulasUsed (&HF0) Then
	print #f2, "				case 0xF0:"
	print #f2, "					// IF TRUE"
	print #f2, "					// Opcode: F0"
	print #f2, "					break;"
End If

Print #f2, "				case 0xff:"
Print #f2, "					// then"
Print #f2, "					// opcode: ff"
Print #f2, "					sc_terminado = sc_continuar = 1;"
Print #f2, "					break;"
Print #f2, "			}"
Print #f2, "		}"
Print #f2, ""
Print #f2, "		if (sc_continuar) {"
Print #f2, "            fire_script_success |= sc_continuar;"
Print #f2, "			sc_terminado = 0;"
Print #f2, "			while (!sc_terminado) {"
Print #f2, "				switch (read_byte ()) {"

'' TODO
if actionsUsed (&H0) Then
	print #f2, "					case 0x00:"
	print #f2, "						// SET ITEM sc_x sc_n"
	print #f2, "						// Opcode: 00 sc_x sc_n"
	print #f2, "						readxy ();"
	print #f2, "						items [sc_x] = sc_y;"
	print #f2, "						display_items ();"
	print #f2, "						break;"
End If
'' EOTODO

if actionsUsed (&H1) Then
	print #f2, "					case 0x01:"
	print #f2, "						// SET FLAG sc_x = sc_n"
	print #f2, "						// Opcode: 01 sc_x sc_n"
	print #f2, "						readxy ();"
	print #f2, "						flags [sc_x] = sc_y;"
	print #f2, "						break;"
End If

if actionsUsed (&H10) Then
	print #f2, "					case 0x10:"
	print #f2, "						// INC FLAG sc_x, sc_n"
	print #f2, "						// Opcode: 10 sc_x sc_n"
	print #f2, "						readxy ();"
	print #f2, "						flags [sc_x] += sc_y;"
	print #f2, "						break;"
End If

if actionsUsed (&H11) Then
	print #f2, "					case 0x11:"
	print #f2, "						// DEC FLAG sc_x, sc_n"
	print #f2, "						// Opcode: 11 sc_x sc_n"
	print #f2, "						readxy ();"
	print #f2, "						flags [sc_x] -= sc_y;"
	print #f2, "						break;"
End If

if actionsUsed (&H12) Then
	print #f2, "					case 0x12:"
	print #f2, "						// ADD FLAGS sc_x, sc_y"
	print #f2, "						// Opcode: 12 sc_x sc_y"
	print #f2, "						readxy ();"
	print #f2, "						flags [sc_x] = flags [sc_x] + flags [sc_y];"
	print #f2, "						break;"
End If

if actionsUsed (&H13) Then
	print #f2, "					case 0x13:"
	print #f2, "						// SUB FLAGS sc_x, sc_y"
	print #f2, "						// Opcode: 13 sc_x sc_y"
	print #f2, "						readxy ();"
	print #f2, "						flags [sc_x] = flags [sc_x] - flags [sc_y];"
	print #f2, "						break;"
End If

if actionsUsed (&H14) Then
	print #f2, "					case 0x14:"
	print #f2, "						// SWAP FLAGS sc_x, sc_y"
	print #f2, "						// Opcode: 14 sc_x sc_y"
	print #f2, "						readxy ();"
	print #f2, "						sc_n = flags [sc_x];"
	print #f2, "						flags [sc_x] = flags [sc_y];"
	print #f2, "						flags [sc_y] = sc_n;"
	print #f2, "						break;"
End If

If actionsUsed (&H15) Then
	print #f2, "					case 0x15:"
	print #f2, "						// FLIPFLOP sc_x"
	print #f2, "						// Opcode: 15 sc_x"
	print #f2, "						sc_x = read_vbyte ();"
	print #f2, "						flags [sc_x] = 1 - flags [sc_x];"
	print #f2, "						break;"
End If

if actionsUsed (&H20) Then
	print #f2, "					case 0x20:"
	print #f2, "						// SET TILE (sc_x, sc_y) = sc_n"
	print #f2, "						// Opcode: 20 sc_x sc_y sc_n"
	print #f2, "						readxy ();"
	print #f2, "						_x = sc_x; _y = sc_y; _t = read_vbyte (); map_set ();"
	print #f2, "						break;"
End If

if actionsUsed (&H30) Then
	print #f2, "					case 0x30:"
	print #f2, "						// INC LIFE sc_n"
	print #f2, "						// Opcode: 30 sc_n"
	print #f2, "						plife += read_vbyte ();"
	print #f2, "						break;"
End If

if actionsUsed (&H31) Then
	print #f2, "					case 0x31:"
	print #f2, "						// DEC LIFE sc_n"
	print #f2, "						// Opcode: 31 sc_n"
	print #f2, "						plife -= read_vbyte ();"
	print #f2, "						break;"
End If

if actionsUsed (&H32) Then
	print #f2, "					case 0x32:"
	print #f2, "						// FLICKER"
	print #f2, "						// Opcode: 32"
	print #f2, "						pstate = EST_PARP; pctstate = 100;"
	print #f2, "						break;"
End If


if actionsUsed (&H40) Then
	print #f2, "					case 0x40:"
	print #f2, "						// INC OBJECTS sc_n"
	print #f2, "						// Opcode: 40 sc_n"
	print #f2, "						pobjs += read_vbyte ();"
	print #f2, "						break;"
End If

if actionsUsed (&H41) Then
	print #f2, "					case 0x41:"
	print #f2, "						// DEC OBJECTS sc_n"
	print #f2, "						// Opcode: 41 sc_n"
	print #f2, "						pobjs -= read_vbyte ();"
	print #f2, "						break;"
End If

if actionsUsed (&H50) then
	print #f2, "					case 0x50:"
	print #f2, "						// PRINT_TILE_AT (sc_x, sc_y) = sc_n"
	print #f2, "						// Opcode: 50 sc_x sc_y sc_n"
	print #f2, "						readxy ();"
	print #f2, "						_x = sc_x; _y = sc_y; _t = read_vbyte (); update_list_tile ();"
	print #f2, "						break;"
end if

if actionsUsed (&H51) Then
	print #f2, "					case 0x51:"
	print #f2, "						// SET_FIRE_ZONE x1, y1, x2, y2"
	print #f2, "						// Opcode: 51 x1 y1 x2 y2"
	print #f2, "						fzx1 = read_byte ();"
	print #f2, "						fzy1 = read_byte ();"
	print #f2, "						fzx2 = read_byte ();"
	print #f2, "						fzy2 = read_byte ();"
	print #f2, "						f_zone_ac = 1;"
	print #f2, "						break;"
End If

If actionsUsed (&H68) Then
	print #f2, "					case 0x68:"
	print #f2, "						// SETXY sc_x sc_y"
	print #f2, "						reloc_player ();"	
	print #f2, "						break;"
End If

If actionsUsed (&H6A) Then
	print #f2, "					case 0x6A:"
	print #f2, "						// SETY sc_y"
	print #f2, "						// Opcode: 6B sc_y"
	print #f2, "						py = read_vbyte () << 10;"
	Print #f2, "						player_stop ();"
	print #f2, "						break;"
End If

If actionsUsed (&H6B) Then
	print #f2, "					case 0x6B:"
	print #f2, "						// SETX sc_x"
	print #f2, "						// Opcode: 6B sc_x"
	print #f2, "						px = read_vbyte () << 10;"
	Print #f2, "						player_stop ();"
	print #f2, "						break;"
End If

'' TODO
If actionsUsed (&H6C) Then
	print #f2, "					case 0x6C:"
	print #f2, "						// REPOSTN sc_x sc_y"
	print #f2, "						// Opcode: 6C sc_x sc_y"
	print #f2, "						do_respawn = 0;"
	print #f2, "						reloc_player ();"
	print #f2, "						on_pant = 0xfe;"
	print #f2, "						sc_terminado = 1;"
	print #f2, "						break;"
End If
'' EOTODO

If actionsUsed (&H6D) Then
	print #f2, "					case 0x6D:"
	print #f2, "						// WARP_TO sc_n sc_x sc_y"
	print #f2, "						// Opcode: 6D sc_n"
	print #f2, "						n_pant = read_vbyte ();"
	print #f2, "						on_pant = 0xfe;"
	print #f2, "						reloc_player ();"
	print #f2, "						return;"
End If

'if actionsUsed (&H6E) Then
'	print #f2, "					case 0x6E:"
'	print #f2, "						// REDRAW"
'	print #f2, "						// Opcode: 6E"
'	print #f2, "						draw_scr_background ();"
'	print #f2, "#ifdef ENABLE_FLOATING_OBJECTS"
'	print #f2, "						FO_paint_all ();"
'	print #f2, "#endif"
'	print #f2, "						break;"
'End If

' New implementation (testing ...)
'If actionsUsed (&H6E) Then
'	print #f2, "					case 0x6E:"
'	print #f2, "						// REDRAW"
'	print #f2, "						// Opcode: 6E"
'	print #f2, "						sc_x = sc_y = 0;"
'	print #f2, "						for (sc_c = 0; sc_c < 192; sc_c ++) {"
'	print #f2, "							update_tile (sc_x, sc_y, map_attr [sc_c], map_buff [sc_c]);"
'	print #f2, "							sc_x ++; if (sc_x == 15) { sc_x = 0; sc_y ++; }"
'	print #f2, "						}"
'	print #f2, "#ifdef ENABLE_FLOATING_OBJECTS"
'	print #f2, "						FO_paint_all ();"
'	print #f2, "#endif"
'	print #f2, "						break;"
'End If

' URM this will look ugly but...
If actionsUsed (&H6E) Then
	print #f2, "					case 0x6e:"
	print #f2, "						// redraw"
	print #f2, "						// opcode: 6e"
	print #f2, "						gpit = 192; gp_gen = map_buff;"
	print #f2, "						rdx = rdy = 0;"
	print #f2, "						ppu_waitnmi (); clear_update_list ();"
	print #f2, "						ppu_off ();"
	print #f2, "						while (gpit --) draw_map_tile (*gp_gen ++);"
	print #f2, "						ppu_on_all ();"
	print #f2, "						break;"
End If

if actionsUsed (&H6F) Then
	print #f2, "					case 0x6F:"
	print #f2, "						// REENTER"
	print #f2, "						// Opcode: 6F"
	print #f2, "						//do_respawn = 0;"
	print #f2, "						on_pant = 0xfe; "
	print #f2, "						return;"
End If

'' TODO
if actionsUsed (&H70) Then
	print #f2, "					case 0x70:"
	print #f2, "						// SET_TIMER a, b"
	print #f2, "						// Opcode: 0x70 a b"
	print #f2, "						timer = read_vbyte ();"
	print #f2, "						timer_frames = 50;"
	print #f2, "						timer_zero = 0;"
	print #f2, "						break;"
End If

If actionsUsed (&H71) Then
	print #f2, "					case 0x71:"
	print #f2, "						// TIMER_START"
	print #f2, "						// Opcode: 0x71"
	print #f2, "						timer_on = 1;"
	print #f2, "						break;"
End If

If actionsUsed (&H72) Then
	print #f2, "					case 0x72:"
	print #f2, "						// TIMER_START"
	print #f2, "						// Opcode: 0x72"
	print #f2, "						timer_on = 0;"
	print #f2, "						break;"
End If

If actionsUsed (&H73) Then
	print #f2, "					case 0x73:"
	print #f2, "						// REHASH"
	print #f2, "						// Opcode: 73"
	print #f2, "						do_respawn = 0;"
	print #f2, "						on_pant = 0xfe; "
	print #f2, "						sc_terminado = 1;"
	print #f2, "						no_draw = 1;"
	print #f2, "						return;"
End If

If actionsUsed (&H80) Then
	print #f2, "					case 0x80:"
	print #f2, "						// ENEM n ON"
	print #f2, "						// Opcode: 0x80 n"
	print #f2, "						baddies [enoffs + read_vbyte ()].t &= 0x7F;"
	print #f2, "						break;"
End If

If actionsUsed (&H81) Then
	print #f2, "					case 0x81:"
	print #f2, "						// ENEM n OFF"
	print #f2, "						// Opcode: 0x81 n"
	print #f2, "						baddies [enoffs + read_vbyte ()].t |= 0x80;"
	print #f2, "						break;"
End If

If actionsUsed (&H82) Then
	Print #f2, "					case 0x82:"
	Print #f2, "						// ADD_FLOATING_OBJECT n, x, y"
	Print #f2, "						sc_n = read_byte ();"
	Print #f2, "						readxy ();"
	Print #f2, "						sc_m = FO_add (sc_x, sc_y, sc_n);"
	Print #f2, "						FO_paint (sc_m);"
	Print #f2, "						break;"
End If

If actionsUsed (&H86) Then
	Print #f2, "					case 0x86:"
	Print #f2, "						// ADD_CONTAINER/ADD_SPRITE f, x, y"
	Print #f2, "						sc_x = read_byte (); sc_y = read_byte ();"
	Print #f2, "						containers_add ();"
	Print #f2, "						break;"
End If

If actionsUsed (&H87) Then
	Print #f2, "					case 0x87:"
	Print #f2, "						// SHOW_CONTAINERS"
	Print #f2, "						containers_draw ();"
	Print #f2, "						break;"
End If

If actionsUsed (&H83) Then
	Print #f2, "					case 0x83:"
	Print #f2, "						// ENEMY n TYPE t"
	Print #f2, "						sc_n = enoffs + read_vbyte ();"
	Print #f2, "						if (baddies [sc_n]) {"
	Print #f2, "							baddies [sc_n].t &= 0x80;"
	Print #f2, "							baddies [sc_n].t |= read_vbyte ();"
	Print #f2, "						}"
	Print #f2, "						break;"
End If

If actionsUsed (&H84) Then
	Print #f2, "					case 0x84:"
	Print #f2, "						// ENEMIES RESTORE"
	Print #f2, "						for (sc_n = 0; sc_n < 3; sc_n ++) {"
	Print #f2, "							enoffsmasi = enoffs + sc_n;"
	Print #f2, "							baddies [enoffsmasi].t = enemy_backup [enoffsmasi];"
	Print #f2, "							en_an_base_frame [sc_n] = (baddies [enoffsmasi].t & 3) << 1;"
	Print #f2, "						}"
	Print #f2, "						break;" 
End If

If actionsUsed (&H85) Then
	Print #f2, "					case 0x84:"
	Print #f2, "						// ENEMIES RESTORE ALL"
	Print #f2, "						for (sc_n = 0; sc_n < 3 * MAP_W * MAP_H; sc_n ++) {";
	Print #f2, "							baddies [sc_n].t = enemy_backup [sc_n];"
	Print #f2, "						}"
	Print #f2, "						break;" 
End If

if actionsUsed (&HD0)  Then
	print #f2, "					case 0xD0:"
	print #f2, "						// NEXT_LEVEL"
	print #f2, "						// Opcode: D0"
	print #f2, "						n_pant ++;"
	print #f2, "						init_player_values ();"
	print #f2, "						draw_scr ();"
	print #f2, "						break;"
End If
'' EOTODO

if actionsUsed (&HE0) Then
	print #f2, "					case 0xE0:"
	print #f2, "						// SOUND sc_n"
	print #f2, "						// Opcode: E0 sc_n"
	print #f2, "						sfx_play (read_vbyte (), 1);"
	print #f2, "						break;"
End If

If actionsUsed (&HE1) Then
	print #f2, "					case 0xE1:"
	print #f2, "						// SHOW"
	print #f2, "						// Opcode: E1"
	print #f2, "						ppu_waitnmi (); clear_update_list ();"
	print #f2, "						break;"
End If

if actionsUsed (&HE2) Then
	print #f2, "					case 0xE2:"
	print #f2, "						// RECHARGE"
	print #f2, "						plife = PLAYER_LIFE;"
	print #f2, "						break;"
End If

if actionsUsed (&HE3) Then
	print #f2, "					case 0xE3:"
	print #f2, "                        // TEXT"
	print #f2, "                        // Opcode: E3 length characters* 0"
	print #f2, "                        _x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT; gp_gen = script;"
	print #f2, "						sc_n = read_byte (); script += (sc_n + 1);"
	print #f2, "                        pr_ul_str ();"
	print #f2, "						break;"
End If

if actionsUsed (&HEB) Then
	Print #f2, "					case 0xEB:"
	Print #f2, "						// OPENTEXT"
	Print #f2, "						open_box (0); break;"
End If

If actionsUsed (&HEC) Then
	Print #f2, "					case 0xEC:"
	Print #f2, "						// CLOSETEXT"
	Print #f2, "						open_box (1); break;"
End If

If actionsUsed (&HED) Then
	Print #f2, "                    case 0xED:"
	Print #f2, "                        // TEXTBOX n"
	Print #f2, "                        // Opcode: 0xED sc_n"
	Print #f2, "                        gp_gen = custom_texts [read_vbyte ()];"
	Print #f2, "                        textbox_do ();"
	print #f2, "						break;"
End If

'' TODO
if actionsUsed (&HE4) Then
	print #f2, "					case 0xE4:"
	print #f2, "						// EXTERN sc_n"
	print #f2, "						// Opcode: 0xE4 sc_n"
	print #f2, "						do_extern_action (read_byte ());"
	print #f2, "						break;"
End IF

if actionsUsed (&HE8) Then
	print #f2, "					case 0xE8:"
	print #f2, "						// EXTERN_E n, m"
	print #f2, "						// Opcode: 0xE8 n, m"
	print #f2, "						do_extern_action (read_byte (), read_byte ());"
	print #f2, "						break;"
End IF

if actionsUsed (&HE5) Then
	print #f2, "					case 0xE5:"
	print #f2, "						// PAUSE sc_n"
	print #f2, "						sc_n = read_vbyte ();"
	print #f2, "						while (sc_n --) ppu_waitnmi ();"
	print #f2, "						break;"
End If

If actionsUsed (&HE6) Then
	print #f2, "					case 0xE6:"
	print #f2, "						// MUSIC n"
	print #f2, "                        music_play (game_music [read_vbyte]);"
	print #f2, "						break;"
End If

If actionsUsed (&HE7) Then
	print #f2, "					case 0xE7:"
	print #f2, "						// REDRAW_ITEMS"
	print #f2, "						// Opcode: 0xE7"
	print #f2, "						display_items ();"
	print #f2, "						break;"
End If
'' EOTODO

If actionsUsed (&HE9) Then
	print #f2, "					case 0xE9:"
	print #f2, "						// SET SAFE n, x, y"
	print #f2, "						// Opcode: 0xE9 n x y"
	print #f2, "						n_pant_safe = read_byte ();"
	print #f2, "						px_safe = read_byte () << 10;"
	print #f2, "						py_safe = read_byte () << 10;"
	print #f2, "						break;" 
End If

If actionsUsed (&HEA) Then
	print #f2, "					case 0xEA:"
	print #f2, "						// SET SAFE HERE"
	print #f2, "						// Opcode: 0xEA"
	print #f2, "						n_pant_safe = n_pant;"
	print #f2, "						px_safe = px;"
	print #f2, "						py_safe = py;"
	print #f2, "						break;" 
End If

if actionsUsed (&HF0) Then
	print #f2, "					case 0xF0:"
	print #f2, "						// GAME OVER"
	print #f2, "						script_result = 2;"
	print #f2, "						return;"
End If

if actionsUsed (&HF1) Then
	print #f2, "					case 0xF1:"
	print #f2, "						// WIN"
	print #f2, "						script_result = 1;"
	print #f2, "						return;"
End If

If actionsUsed (&HF2) Then
	print #f2, "					case 0xF2:"
	print #f2, "						// BREAK"
	print #f2, "						return;"
end if

If actionsUsed (&HF3) Then
	print #f2, "					case 0xF3:"
	print #f2, "						// Final del todo"
	print #f2, "						script_result = 4;"
	print #f2, "						return;"
	print #f2, "						break;"
End If

if actionsUsed (&HF4) Then
	print #f2, "					case 0xF4:"
	print #f2, "						// DECORATIONS"
	print #f2, "						while (0xff != (sc_x = read_byte ())) {"
	print #f2, "							_x = sc_x >> 4; _y = sc_x & 15; _t = read_byte ();"
	print #f2, "							map_set ();"
	print #f2, "							ppu_waitnmi (); clear_update_list ();"
	print #f2, "						}"
	print #f2, "						break;"
End If

Print #f2, "					case 0xff:"
Print #f2, "						sc_terminado = 1;"
Print #f2, "						break;"
Print #f2, "				}"
Print #f2, "			}"
Print #f2, "		}"

Print #f2, "		script = next_script;"
Print #f2, "	}"

Print #f2, "}"

close #f3

print "DONE!"
