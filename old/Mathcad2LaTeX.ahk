#SingleInstance FORCE
StringCaseSense, On

debug = 0
SepDot2Comma = 1

LaunchMessage := "Mathcad2LaTeX v1.4 aktiveret."
MsgBox %LaunchMessage%

StartOfScript:

^+V::																					;Genvejstast til konvertering og inds�tning: Ctrl + Shift + V
	MathString = %clipboard%															;Overf�r udklipsholder til MathString
	MathStringStart = %MathString%
	
StartConvert:																			;Konverteringen starter her
	StringLen, MathStringLength, MathString												;L�ngde af MathString
	ScanPos = 1																			;MathString scannes fra f�rste tegn
	ScanMathString:
		StringMid, StrScan, MathString, %ScanPos%, 1									;Tr�k ét tegn ud ved positionen ScanPos og gem til StrScan
		IfInString, StrScan, (															;Indeholder udtrykket en parentes?
			{
				ScanPosBracketOpen := ScanPos + 1										;Placeringen efter den f�rste �bne parentes (nuv�rende position +1, starten af operator) gemmes til ScanPosBracketOpen
				ScanPos := ScanPos + 1
				goto, OpSize															;G� til OpSize (find st�rrelse af operator)
			}
		If ScanPos = %MathStringLength%													;Scanningen skal stoppe, n�r hele MathString er scannet
			{
				goto, CheckIfDone
			}
		ScanPos := ScanPos + 1
		goto, ScanMathString

	OpSize:
		StringMid, StrScan, MathString, %ScanPos%, 1
		IfInString, StrScan, %A_Space%
			{
				ScanPosOpSizeToSpace := ScanPos - ScanPosBracketOpen					;Placeringen f�r det f�rste mellemrum efter f�rste �bne parentes i forhold til f�rste �bne parentes (nuv�rende position -1, slutning af operator) gemmes til ScanPosOpSizeSpace
				ScanPos := ScanPos + 1
				goto, OpDefine
			}
		ScanPos := ScanPos + 1
		If ScanPos > %MathStringLength%
			{
				OpType = undefined
				Error(1, "Ukendt syntaks. Er emnet kopieret korrekt fra Mathcad?", 47, MathString, MathStringStart, Op, OpType)
			}
		goto, OpSize
		
	OpDefine:
		StringMid, Op, MathString, %ScanPosBracketOpen%, %ScanPosOpSizeToSpace%			;Streng med operator gemmes i Op
		StringLen, OpLength, Op															;L�ngden af operatoren Op
		OpCurrent = %A_Space%%Op%%A_Space%												;Nuv�rende operator med mellerum p� hver side
		
		;SINGUL�RE OPERATORER															;Alle singul�re operatorer med mellemrum p� hver side
		OpLibSingular = " @SUB @PARENS @ARGS @NEG @ABS @KW_STACK `% @DEG @PRIME ! @NOT "
		
		;BIN�RE OPERATORER																;Alle bin�re operatorer med mellemrum p� hver side
		OpLibBinary = " @SETUP = := @EQ @IS + - * / ^ @NTHROOT @APPLY @FUNCTION @ID @LABEL @SEP < > @LEQ @GEQ @SCALE @RSCALE @NEQ @GLOBAL_DEF @CROSS @AND @OR @ELEMENT_OF @POLAR @RANGE "
	
		;TRE-ELEMENT OPERATORER															;Alle operatorer med tre elementer med mellemrum p� hver side
		OpLibTriple = " @SYM_EVAL @DERIV @SUM @PRODUCT @STEPRANGE "
		
		;FIRE-ELEMENT OPERATORER														;Alle operatorer med fire elementer med mellemrum p� hver side
		OpLibQuadruple = " @INTEGRAL "
		
		;MATRIX																			;Matrixoperatoren med mellemrum p� hver side
		OpLibMatrix = " @MATRIX "
		
		IfInString, OpLibBinary, %OpCurrent%
			{
				OpType = binary
			}
		Else IfInString, OpLibSingular, %OpCurrent%
			{
				OpType = singular
			}
		Else IfInString, OpLibTriple, %OpCurrent%
			{
				OpType = triple
			}
		Else IfInString, OpLibQuadruple, %OpCurrent%
			{
				OpType = quadruple
			}
		Else IfInString, OpLibMatrix, %OpCurrent%
			{
				OpType = matrix
			}
		Else
			{
				OpType = undefined
				Error(2, "Ukendt operator.", 88, MathString, MathStringStart, Op, OpType)
			}

	If OpType = singular
		{
			goto, Ex1DefineSingular
		}
	Else If OpType = binary
		{
			goto, Ex1DefineBinary
		}
	Else If OpType = triple
		{
			goto, Ex1DefineTriple
		}	
	Else If OpType = quadruple
		{
			goto, Ex1DefineQuadruple
		}
	Else If OpType = matrix
		{
			goto, ExMatrixDefine
		}

	Ex1DefineSingular:
		Ex1PosStart := ScanPosBracketOpen + ScanPosOpSizeToSpace
		Ex1PosStart := Ex1PosStart + 1													;Expression 1 starter efter f�rste �bne parentes + l�ngden af operatoren + 1
		StringMid, StrScan, MathString, %Ex1PosStart%, 1								;F�rste karakter i expression 1 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 1 slutter
				ScanPos := Ex1PosStart
				FindEx1SingularBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					Else IfInString, StrScan, )											;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 1 er f�rdig
						{
							Ex1PosEnd := ScanPos + 1									;Ex1PosEnd s�ttes til placeringen af f�rste karakter efter expression 1
							goto, AssembleSingular
						}
					goto, FindEx1SingularBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 1 er ikke en parentes
			{
				ScanPos := Ex1PosStart + 1
				FindEx1SingularNonbracketEnd:											;Find n�ste slutparentes (dette markerer enden af expression 1)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, )
						{
							Ex1PosEnd := ScanPos
							goto, AssembleSingular
						}
					ScanPos := ScanPos + 1
					goto, FindEx1SingularNonbracketEnd
			}
		
	Ex1DefineBinary:
		Ex1PosStart := ScanPosBracketOpen + ScanPosOpSizeToSpace
		Ex1PosStart := Ex1PosStart + 1													;Expression 1 starter efter f�rste �bne parentes + l�ngden af operatoren + 1
		StringMid, StrScan, MathString, %Ex1PosStart%, 1								;F�rste karakter i expression 1 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 1 slutter
				ScanPos := Ex1PosStart
				FindEx1BinaryBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					Else IfInString, StrScan, )											;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 1 er f�rdig
						{
							Ex1PosEnd := ScanPos + 1									;Ex1PosEnd s�ttes til placeringen af f�rste karakter efter expression 1
							goto, Ex2DefineBinary
						}
					goto, FindEx1BinaryBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 1 er ikke en parentes
			{
				ScanPos := Ex1PosStart + 1
				FindEx1BinaryNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 1)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex1PosEnd := ScanPos
							goto, Ex2DefineBinary
						}
					ScanPos := ScanPos + 1
					goto, FindEx1BinaryNonbracketEnd
			}
		
		
	Ex2DefineBinary:	
		Ex2PosStart := Ex1PosEnd + 1
		StringMid, StrScan, MathString, %Ex2PosStart%, 1								;F�rste karakter i expression 2 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 2 slutter
				ScanPos := Ex2PosStart
				FindEx2BinaryBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 2 er f�rdig
						{
							Ex2PosEnd := ScanPos + 1									;Ex2PosEnd s�ttes til placeringen af f�rste karakter efter expression 2
							goto, AssembleBinary
						}
					goto, FindEx2BinaryBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 2 er ikke en parentes
			{
				ScanPos := Ex2PosStart + 1
				FindEx2BinaryNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 2)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, )
						{
							Ex2PosEnd := ScanPos
							goto, AssembleBinary
						}
					ScanPos := ScanPos + 1
					goto, FindEx2BinaryNonbracketEnd
			}
		
	Ex1DefineTriple:
		Ex1PosStart := ScanPosBracketOpen + ScanPosOpSizeToSpace
		Ex1PosStart := Ex1PosStart + 1													;Expression 1 starter efter f�rste �bne parentes + l�ngden af operatoren + 1
		StringMid, StrScan, MathString, %Ex1PosStart%, 1								;F�rste karakter i expression 1 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 1 slutter
				ScanPos := Ex1PosStart
				FindEx1TripleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					Else IfInString, StrScan, )											;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 1 er f�rdig
						{
							Ex1PosEnd := ScanPos + 1									;Ex1PosEnd s�ttes til placeringen af f�rste karakter efter expression 1
							goto, Ex2DefineTriple
						}
					goto, FindEx1TripleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 1 er ikke en parentes
			{
				ScanPos := Ex1PosStart + 1
				FindEx1TripleNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 1)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex1PosEnd := ScanPos
							goto, Ex2DefineTriple
						}
					ScanPos := ScanPos + 1
					goto, FindEx1TripleNonbracketEnd
			}
		
		
	Ex2DefineTriple:	
		Ex2PosStart := Ex1PosEnd + 1
		StringMid, StrScan, MathString, %Ex2PosStart%, 1								;F�rste karakter i expression 2 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 2 slutter
				ScanPos := Ex2PosStart
				FindEx2TripleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 2 er f�rdig
						{
							Ex2PosEnd := ScanPos + 1									;Ex2PosEnd s�ttes til placeringen af f�rste karakter efter expression 2
							goto, Ex3DefineTriple
						}
					goto, FindEx2TripleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 2 er ikke en parentes
			{
				ScanPos := Ex2PosStart + 1
				FindEx2TripleNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 2)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex2PosEnd := ScanPos
							goto, Ex3DefineTriple
						}
					ScanPos := ScanPos + 1
					goto, FindEx2TripleNonbracketEnd
			}
			
	Ex3DefineTriple:	
		Ex3PosStart := Ex2PosEnd + 1
		StringMid, StrScan, MathString, %Ex3PosStart%, 1								;F�rste karakter i expression 2 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 2 slutter
				ScanPos := Ex3PosStart
				FindEx3TripleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 2 er f�rdig
						{
							Ex3PosEnd := ScanPos + 1									;Ex2PosEnd s�ttes til placeringen af f�rste karakter efter expression 2
							goto, AssembleTriple
						}
					goto, FindEx3TripleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 2 er ikke en parentes
			{
				ScanPos := Ex3PosStart + 1
				FindEx3TripleNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 2)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, )
						{
							Ex3PosEnd := ScanPos
							goto, AssembleTriple
						}
					ScanPos := ScanPos + 1
					goto, FindEx3TripleNonbracketEnd
			}
		
	Ex1DefineQuadruple:
		Ex1PosStart := ScanPosBracketOpen + ScanPosOpSizeToSpace
		Ex1PosStart := Ex1PosStart + 1													;Expression 1 starter efter f�rste �bne parentes + l�ngden af operatoren + 1
		StringMid, StrScan, MathString, %Ex1PosStart%, 1								;F�rste karakter i expression 1 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 1 slutter
				ScanPos := Ex1PosStart
				FindEx1QuadrupleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					Else IfInString, StrScan, )											;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 1 er f�rdig
						{
							Ex1PosEnd := ScanPos + 1									;Ex1PosEnd s�ttes til placeringen af f�rste karakter efter expression 1
							goto, Ex2DefineQuadruple
						}
					goto, FindEx1QuadrupleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 1 er ikke en parentes
			{
				ScanPos := Ex1PosStart + 1
				FindEx1QuadrupleNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 1)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex1PosEnd := ScanPos
							goto, Ex2DefineQuadruple
						}
					ScanPos := ScanPos + 1
					goto, FindEx1QuadrupleNonbracketEnd
			}
		
		
	Ex2DefineQuadruple:	
		Ex2PosStart := Ex1PosEnd + 1
		StringMid, StrScan, MathString, %Ex2PosStart%, 1								;F�rste karakter i expression 2 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 2 slutter
				ScanPos := Ex2PosStart
				FindEx2QuadrupleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 2 er f�rdig
						{
							Ex2PosEnd := ScanPos + 1									;Ex2PosEnd s�ttes til placeringen af f�rste karakter efter expression 2
							goto, Ex3DefineQuadruple
						}
					goto, FindEx2QuadrupleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 2 er ikke en parentes
			{
				ScanPos := Ex2PosStart + 1
				FindEx2QuadrupleNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 2)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex2PosEnd := ScanPos
							goto, Ex3DefineQuadruple
						}
					ScanPos := ScanPos + 1
					goto, FindEx2QuadrupleNonbracketEnd
			}
			
	Ex3DefineQuadruple:	
		Ex3PosStart := Ex2PosEnd + 1
		StringMid, StrScan, MathString, %Ex3PosStart%, 1								;F�rste karakter i expression 3 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 2 slutter
				ScanPos := Ex3PosStart
				FindEx3QuadrupleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 3 er f�rdig
						{
							Ex3PosEnd := ScanPos + 1									;Ex2PosEnd s�ttes til placeringen af f�rste karakter efter expression 3
							goto, Ex4DefineQuadruple
						}
					goto, FindEx3QuadrupleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 3 er ikke en parentes
			{
				ScanPos := Ex3PosStart + 1
				FindEx3QuadrupleNonbracketEnd:											;Find n�ste mellemrum (dette markerer enden af expression 3)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, %A_Space%
						{
							Ex3PosEnd := ScanPos
							goto, Ex4DefineQuadruple
						}
					ScanPos := ScanPos + 1
					goto, FindEx3QuadrupleNonbracketEnd
			}
			
	Ex4DefineQuadruple:	
		Ex4PosStart := Ex3PosEnd + 1
		StringMid, StrScan, MathString, %Ex4PosStart%, 1								;F�rste karakter i expression 4 gemmes i StrScan
		IfInString, StrScan, (															;Er f�rste karakter en �ben parentes?
			{
				BracketLvl = 1															;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 4 slutter
				ScanPos := Ex4PosStart
				FindEx4QuadrupleBracketClose:
					ScanPos := ScanPos + 1
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, (												;Åben parentes. BracketLvl + 1
						{
							BracketLvl := BracketLvl + 1
						}
					IfInString, StrScan, )												;Lukket parentes. BracketLvl - 1
						{
							BracketLvl := BracketLvl - 1
						}
					If BracketLvl = 0													;BracketLvl = 0. Expression 4 er f�rdig
						{
							Ex4PosEnd := ScanPos + 1									;Ex4PosEnd s�ttes til placeringen af f�rste karakter efter expression 4
							goto, AssembleQuadruple
						}
					goto, FindEx4QuadrupleBracketClose
			}
		Else																			;Nej. F�rste karakter i expression 2 er ikke en parentes
			{
				ScanPos := Ex4PosStart + 1
				FindEx4QuadrupleNonbracketEnd:											;Find n�ste mellemrum (dette markerer enden af expression 4)
					StringMid, StrScan, MathString, %ScanPos%, 1
					IfInString, StrScan, )
						{
							Ex4PosEnd := ScanPos
							goto, AssembleQuadruple
						}
					ScanPos := ScanPos + 1
					goto, FindEx4QuadrupleNonbracketEnd
			}
		
	ExMatrixDefine:
		xn = 1
		ExMPosStart := ScanPosBracketOpen + ScanPosOpSizeToSpace
		ExMPosStart := ExMPosStart + 1													;Expression 1 starter efter f�rste �bne parentes + l�ngden af operatoren + 1
		
		ExLeftLength := ScanPosBracketOpen - 2
		ExLeftTrimCount := MathStringLength - ExLeftLength
		StringTrimRight, ExLeft, MathString, %ExLeftTrimCount%							;Udtrykket til venstre for expression 1 defineres
		
		ExMDefine:
			StringMid, StrScan, MathString, %ExMPosStart%, 1							;F�rste karakter i expression 1 gemmes i StrScan
			IfInString, StrScan, (														;Er f�rste karakter en �ben parentes?
				{
					BracketLvl = 1														;Ja. Én �ben parentes passeret. BracketLvl = 1. Skal v�re 0, n�r expression 1 slutter
					ScanPos := ExMPosStart
					FindExMBracketClose:
						ScanPos := ScanPos + 1
						StringMid, StrScan, MathString, %ScanPos%, 1
						IfInString, StrScan, (											;Åben parentes. BracketLvl + 1
							{
								BracketLvl := BracketLvl + 1
							}
						Else IfInString, StrScan, )										;Lukket parentes. BracketLvl - 1
							{
								BracketLvl := BracketLvl - 1
							}
						If BracketLvl = 0												;BracketLvl = 0. Expression 1 er f�rdig
							{
								ExMPosEnd := ScanPos + 1								;Ex1PosEnd s�ttes til placeringen af f�rste karakter efter expression 1
								goto, ExMxnDefine
							}
						goto, FindExMBracketClose
				}
			Else																		;Nej. F�rste karakter i expression 1 er ikke en parentes
				{
					ScanPos := ExMPosStart + 1
					FindExMNonbracketEnd:												;Find n�ste mellemrum (dette markerer enden af expression 1)
						StringMid, StrScan, MathString, %ScanPos%, 1
						IfInString, StrScan, %A_Space%
							{
								ExMPosEnd := ScanPos
								goto, ExMxnDefine
							}
						Else IfInString, StrScan, )
							{
								ExMPosEnd := ScanPos
								goto, ExMxnDefine
							}
						ScanPos := ScanPos + 1
						goto, FindExMNonbracketEnd
			}
		
	ExMxnDefine:
		ExMLength := ExMPosEnd - ExMPosStart
		StringMid, ExM%xn%, MathString, %ExMPosStart%, %ExMLength%						;Expression 1 defineres
		
		ScanPos := ExMPosEnd
		StringMid, StrScan, MathString, %ScanPos%, 1
		IfInString, StrScan, )															;Er alle expressions definerede?
			{
				goto, AssembleMatrix
			}
		Else
			{
				ExMPosStart := ExMPosEnd + 1
				xn := xn + 1
				goto, ExMDefine
			}
		
	AssembleMatrix:
		ExRightTrimCount := ExMPosEnd
		StringTrimLeft, ExRight, MathString, %ExRightTrimCount%							;Udtrykket til h�jre for sidste expression defineres
		
		MathString = %ExLeft%\left[\begin{matrix}
		xm = 0
		xc = 0
		ExMAdd:
			xm := xm + 1
			xc := xc + 1
			ExM := ExM%xm%
			ExM := SepComma(ExM,SepDot2Comma)
			If xm = 1
				{
					goto, ExMAdd
				}
			Else If xm = 2
				{
					ExMCollumns = %ExM%
					xc = 0
					goto, ExMAdd
				}
			Else
				{
					If xm = %xn%
						{
							MathString = %MathString%{%ExM%}\end{matrix}\right]%ExRight%
							goto, CheckIfDone
						}
					Else If xc = %ExMCollumns%
						{
							MathString = %MathString%{%ExM%}\\[0.0em]
							xc = 0
							goto, ExMAdd
						}
					Else
						{
							MathString = %MathString%{%ExM%}&
							goto, ExMAdd
						}
				}
			
			
	AssembleSingular:
		Ex1Length := Ex1PosEnd - Ex1PosStart
		StringMid, Ex1, MathString, %Ex1PosStart%, %Ex1Length%							;Expression 1 defineres
		Ex1 := SepComma(Ex1,SepDot2Comma)
		
		ExLeftLength := ScanPosBracketOpen - 2
		ExLeftTrimCount := MathStringLength - ExLeftLength
		StringTrimRight, ExLeft, MathString, %ExLeftTrimCount%							;Udtrykket til venstre for expression 1 defineres
		
		ExRightTrimCount := Ex1PosEnd
		StringTrimLeft, ExRight, MathString, %ExRightTrimCount%							;Udtrykket til h�jre for expression 1 defineres
		
		AssembleSingularSubscript:
			StringSubscript = " @SUB "
			IfInString, StringSubscript, %OpCurrent%
				{
					MathString = %ExLeft%_{%Ex1%}{}%ExRight%
				}
				
			StringParenstheses = " @PARENS "
			IfInString, StringParenstheses, %OpCurrent%
				{
					MathString = %ExLeft%\lbrace{%Ex1%}\rbrace%ExRight%
				}
				
		AssembleSingularArguments:
			StringArguments = " @ARGS "
			IfInString, StringArguments, %OpCurrent%
				{
					MathString = %ExLeft%\lbrace{%Ex1%}\rbrace%ExRight%
				}
				
		AssembleSingularNegative:
			StringNegative = " @NEG "
			IfInString, StringNegative, %OpCurrent%
				{
					MathString = %ExLeft%-{%Ex1%}%ExRight%
				}
				
		AssembleSingularAbsolute:
			StringAbsolute = " @ABS "
			IfInString, StringAbsolute, %OpCurrent%
				{
					MathString = %ExLeft%|{%Ex1%}|%ExRight%
				}
				
		AssembleSingularKeywordStack:
			StringKeywordStack = " @KW_STACK "
			IfInString, StringKeywordStack, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}%ExRight%
				}
				
		AssembleSingularPercent:
			StringPercent = " `% "
			IfInString, StringPercent, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{\`%}%ExRight%
				}
		
		AssembleSingularDegree:
			StringDegree = " @DEG "
			IfInString, StringDegree, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{^{\circ}}%ExRight%
				}
		
		AssembleSingularPrime:
			StringPrime = " @PRIME "
			IfInString, StringPrime, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{'}%ExRight%
				}
		
		AssembleSingularFactorial:
			StringFactorial = " ! "
			IfInString, StringFactorial, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{!}%ExRight%
				}
		
		AssembleSingularNot:
			StringNot = " @NOT "
			IfInString, StringNot, %OpCurrent%
				{
					MathString = %ExLeft%\neg{%Ex1%}%ExRight%
				}
				
		goto, CheckIfDone
	
	AssembleBinary:
		Ex1Length := Ex1PosEnd - Ex1PosStart
		StringMid, Ex1, MathString, %Ex1PosStart%, %Ex1Length%							;Expression 1 defineres
		Ex2Length := Ex2PosEnd - Ex2PosStart
		StringMid, Ex2, MathString, %Ex2PosStart%, %Ex2Length%							;Expression 2 defineres
		Ex1 := SepComma(Ex1,SepDot2Comma)
		Ex2 := SepComma(Ex2,SepDot2Comma)
		
		ExLeftLength := ScanPosBracketOpen - 2
		ExLeftTrimCount := MathStringLength - ExLeftLength
		StringTrimRight, ExLeft, MathString, %ExLeftTrimCount%							;Udtrykket til venstre for expression 1 defineres
		
		ExRightTrimCount := Ex2PosEnd
		StringTrimLeft, ExRight, MathString, %ExRightTrimCount%							;Udtrykket til h�jre for expression 2 defineres
		
		AssembleBinarySetup:
			StringEqual = " @SETUP "
			IfInString, StringEqual, %OpCurrent%
				{
					goto, Setup
				}
		
		AssembleBinaryEqual:
			StringEqual = " = := @EQ @IS "
			IfInString, StringEqual, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}={%Ex2%}%ExRight%
				}
		
		AssembleBinaryNotEqual:
			StringNotEqual = " @NEQ "
			IfInString, StringNotEqual, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\neq{%Ex2%}%ExRight%
				}
		
		AssembleBinaryPlus:
			StringPlus = " + "
			IfInString, StringPlus, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}+{%Ex2%}%ExRight%
				}
		
		AssembleBinaryMinus:
			StringMinus = " - "
			IfInString, StringMinus, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}-{%Ex2%}%ExRight%
				}
		
		AssembleBinaryMultiplication:
			StringMultiplication = " * "
			IfInString, StringMultiplication, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\cdot{%Ex2%}%ExRight%
				}
		
		AssembleBinaryDivision:
			StringDivision = " / "
			IfInString, StringDivision, %OpCurrent%
				{
					MathString = %ExLeft%\frac{%Ex1%}{%Ex2%}%ExRight%
				}
		
		AssembleBinaryPower:
			StringPower = " ^ "
			IfInString, StringPower, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}^{%Ex2%}%ExRight%
				}
				
		AssembleBinarySquareroot:
			StringSquareroot = " @NTHROOT "
			IfInString, StringSquareroot, %OpCurrent%
				{
					MathString = %ExLeft%\sqrt[%Ex1%]{%Ex2%}%ExRight%
				}
				
		AssembleBinaryApply:
			StringApply = " @APPLY "
			IfInString, StringApply, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{%Ex2%}%ExRight%
				}
				
		AssembleBinaryID:
			StringID = " @ID @FUNCTION "
			IfInString, StringID, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{%Ex2%}%ExRight%
				}
				
		AssembleBinaryLabel:
			StringLabel = " @LABEL "
			IfInString, StringLabel, %OpCurrent%
				{
					If Ex1 = UNIT
						{
							MathString = %ExLeft%\;\mathrm{%Ex2%}%ExRight%
						}
					Else If Ex1 = CONSTANT
						{
							MathString = %ExLeft%\;\mathrm{%Ex2%}%ExRight%
						}
					Else
						{
							MathString = %ExLeft%{%Ex2%}%ExRight%
						}
				}
				
		AssembleBinarySeparator:
			StringSeparator = " @SEP "
			IfInString, StringSeparator, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%},{%Ex2%}%ExRight%
				}
				
		AssembleBinaryLessThan:
			StringLessThan = " < "
			IfInString, StringLessThan, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}<{%Ex2%}%ExRight%
				}
				
		AssembleBinaryGreaterThan:
			StringGreaterThan = " > "
			IfInString, StringGreaterThan, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}>{%Ex2%}%ExRight%
				}
				
		AssembleBinaryLessThanOrEqual:
			StringLessThanOrEqual = " @LEQ "
			IfInString, StringLessThanOrEqual, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\leq{%Ex2%}%ExRight%
				}
				
		AssembleBinaryGreaterThanOrEqual:
			StringGreaterThanOrEqual = " @GEQ "
			IfInString, StringGreaterThanOrEqual, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\geq{%Ex2%}%ExRight%
				}
				
		AssembleBinaryUnit:
			StringUnit = " @SCALE "
			IfInString, StringUnit, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\;\mathrm{%Ex2%}%ExRight%
				}
				
		AssembleBinaryRSCALE:
			StringRSCALE = " @RSCALE "
			IfInString, StringRSCALE, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}{%Ex2%}%ExRight%
				}
		
		AssembleBinaryGlobalDefinition:
			StringGlobalDefinition = " @GLOBAL_DEF "
			IfInString, StringGlobalDefinition, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\equiv{%Ex2%}%ExRight%
				}
		
		AssembleBinaryCross:
			StringCross = " @CROSS "
			IfInString, StringCross, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\times{%Ex2%}%ExRight%
				}
		
		AssembleBinaryAnd:
			StringAnd = " @AND "
			IfInString, StringAnd, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\wedge{%Ex2%}%ExRight%
				}
		
		AssembleBinaryOr:
			StringOr = " @OR "
			IfInString, StringOr, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\vee{%Ex2%}%ExRight%
				}
		
		AssembleBinaryElementOf:
			StringElementOf = " @ELEMENT_OF "
			IfInString, StringElementOf, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\in{%Ex2%}%ExRight%
				}
		
		AssembleBinaryPolar:
			StringPolar = " @POLAR "
			IfInString, StringPolar, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\angle{%Ex2%}%ExRight%
				}
		
		AssembleBinaryRange:
			StringRange = " @RANGE "
			IfInString, StringRange, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}..{%Ex2%}%ExRight%
				}
				
		goto, CheckIfDone

	AssembleTriple:
		Ex1Length := Ex1PosEnd - Ex1PosStart
		StringMid, Ex1, MathString, %Ex1PosStart%, %Ex1Length%							;Expression 1 defineres
		Ex2Length := Ex2PosEnd - Ex2PosStart
		StringMid, Ex2, MathString, %Ex2PosStart%, %Ex2Length%							;Expression 2 defineres
		Ex3Length := Ex3PosEnd - Ex3PosStart
		StringMid, Ex3, MathString, %Ex3PosStart%, %Ex3Length%							;Expression 3 defineres
		Ex1 := SepComma(Ex1,SepDot2Comma)
		Ex2 := SepComma(Ex2,SepDot2Comma)
		Ex3 := SepComma(Ex3,SepDot2Comma)
		
		ExLeftLength := ScanPosBracketOpen - 2
		ExLeftTrimCount := MathStringLength - ExLeftLength
		StringTrimRight, ExLeft, MathString, %ExLeftTrimCount%							;Udtrykket til venstre for expression 1 defineres
		
		ExRightTrimCount := Ex3PosEnd
		StringTrimLeft, ExRight, MathString, %ExRightTrimCount%							;Udtrykket til h�jre for expression 3 defineres
		
		AssembleTripleSymEval:
			StringSymEval = " @SYM_EVAL "
			IfInString, StringSymEval, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%}\xrightarrow{%Ex2%}{%Ex3%}%ExRight%
				}
				
		AssembleTripleDerivative:
			StringDerivative = " @DERIV "
			IfInString, StringDerivative, %OpCurrent%
				{
					MathString = %ExLeft%{\frac{\mathrm{d}^{%Ex2%}}{\mathrm{d}{%Ex1%}^{%Ex2%}}{%Ex3%}}%ExRight%
				}
		
		AssembleTripleSum:
			StringSum = " @SUM "
			IfInString, StringSum, %OpCurrent%
				{
					MathString = %ExLeft%{\sum_{%Ex1%}^{%Ex2%} {%Ex3%}}%ExRight%
				}
		
		AssembleTripleProduct:
			StringProduct = " @PRODUCT "
			IfInString, StringProduct, %OpCurrent%
				{
					MathString = %ExLeft%{\prod_{%Ex1%}^{%Ex2%} {%Ex3%}}%ExRight%
				}
		
		AssembleTripleStepRange:
			StringStepRange = " @STEPRANGE "
			IfInString, StringStepRange, %OpCurrent%
				{
					MathString = %ExLeft%{%Ex1%},{%Ex2%}..{%Ex3%}%ExRight%
				}
		
	AssembleQuadruple:
		Ex1Length := Ex1PosEnd - Ex1PosStart
		StringMid, Ex1, MathString, %Ex1PosStart%, %Ex1Length%							;Expression 1 defineres
		Ex2Length := Ex2PosEnd - Ex2PosStart
		StringMid, Ex2, MathString, %Ex2PosStart%, %Ex2Length%							;Expression 2 defineres
		Ex3Length := Ex3PosEnd - Ex3PosStart
		StringMid, Ex3, MathString, %Ex3PosStart%, %Ex3Length%							;Expression 3 defineres
		Ex4Length := Ex4PosEnd - Ex4PosStart
		StringMid, Ex4, MathString, %Ex4PosStart%, %Ex4Length%							;Expression 4 defineres
		Ex1 := SepComma(Ex1,SepDot2Comma)
		Ex2 := SepComma(Ex2,SepDot2Comma)
		Ex3 := SepComma(Ex3,SepDot2Comma)
		Ex4 := SepComma(Ex4,SepDot2Comma)
		
		ExLeftLength := ScanPosBracketOpen - 2
		ExLeftTrimCount := MathStringLength - ExLeftLength
		StringTrimRight, ExLeft, MathString, %ExLeftTrimCount%							;Udtrykket til venstre for expression 1 defineres
		
		ExRightTrimCount := Ex4PosEnd
		StringTrimLeft, ExRight, MathString, %ExRightTrimCount%							;Udtrykket til h�jre for expression 4 defineres
		
		AssembleQuadrupleIntegral:
			StringIntegral = " @INTEGRAL "
			IfInString, StringIntegral, %OpCurrent%
				{
					MathString = %ExLeft%{\int\limits_{%Ex1%}^{%Ex2%}{%Ex3%}\;\mathrm{d}{%Ex4%}}%ExRight%
				}
		
	CheckIfDone:
		If debug != 0
			{
				SendInput {Raw}%MathString%
				SendInput {Enter}
			}
		IfInString, MathString, (														;Indeholder udtrykket en parentes?
			{
				goto, StartConvert
			}
		
		StringReplace, MathString, MathString, \lbrace, \left(, All						;Ingen parenteser tilbage. De elementer, som skal v�re parenteser, fixes
		StringReplace, MathString, MathString, \rbrace, \right), All
		StringReplace, MathString, MathString, @PLACEHOLDER,, All						;@PLACEHOLDER fjernes fra MathString
		StringReplace, MathString, MathString, @RPLACEHOLDER,, All
		
		StringReplace, MathString, MathString, % Chr(0x03B1), {\alpha}, All				;Gr�ske bogstaver
		StringReplace, MathString, MathString, % Chr(0x03B2), {\beta}, All
		StringReplace, MathString, MathString, % Chr(0x0393), {\Gamma}, All
		StringReplace, MathString, MathString, % Chr(0x03B3), {\gamma}, All
		StringReplace, MathString, MathString, % Chr(0x0394), {\Delta}, All
		StringReplace, MathString, MathString, % Chr(0x03B4), {\delta}, All
		StringReplace, MathString, MathString, % Chr(0x03B5), {\epsilon}, All
		StringReplace, MathString, MathString, % Chr(0x03B6), {\zeta}, All
		StringReplace, MathString, MathString, % Chr(0x03B7), {\eta}, All
		StringReplace, MathString, MathString, % Chr(0x0398), {\Theta}, All
		StringReplace, MathString, MathString, % Chr(0x03B8), {\theta}, All
		StringReplace, MathString, MathString, % Chr(0x03D1), {\vartheta}, All
		StringReplace, MathString, MathString, % Chr(0x03B9), {\iota}, All
		StringReplace, MathString, MathString, % Chr(0x03BA), {\kappa}, All
		StringReplace, MathString, MathString, % Chr(0x039B), {\Lambda}, All
		StringReplace, MathString, MathString, % Chr(0x03BB), {\lambda}, All
		StringReplace, MathString, MathString, % Chr(0x03BC), {\mu}, All
		StringReplace, MathString, MathString, % Chr(0x03BD), {\nu}, All
		StringReplace, MathString, MathString, % Chr(0x039E), {\Xi}, All
		StringReplace, MathString, MathString, % Chr(0x03BE), {\xi}, All
		StringReplace, MathString, MathString, % Chr(0x03A0), {\Pi}, All
		StringReplace, MathString, MathString, % Chr(0x03C0), {\pi}, All
		StringReplace, MathString, MathString, % Chr(0x03C1), {\rho}, All
		StringReplace, MathString, MathString, % Chr(0x03A3), {\Sigma}, All
		StringReplace, MathString, MathString, % Chr(0x03C3), {\sigma}, All
		StringReplace, MathString, MathString, % Chr(0x03C4), {\tau}, All
		StringReplace, MathString, MathString, % Chr(0x03A5), {\Upsilon}, All
		StringReplace, MathString, MathString, % Chr(0x03C5), {\upsilon}, All
		StringReplace, MathString, MathString, % Chr(0x03A6), {\Phi}, All
		StringReplace, MathString, MathString, % Chr(0x03D5), {\phi}, All
		StringReplace, MathString, MathString, % Chr(0x03C6), {\varphi}, All
		StringReplace, MathString, MathString, % Chr(0x03C7), {\chi}, All
		StringReplace, MathString, MathString, % Chr(0x03A8), {\Psi}, All
		StringReplace, MathString, MathString, % Chr(0x03C8), {\psi}, All
		StringReplace, MathString, MathString, % Chr(0x03A9), {\Omega}, All
		StringReplace, MathString, MathString, % Chr(0x03C9), {\omega}, All
		
		StringReplace, MathString, MathString, {cos}, {\cos}, All						;Trigonometriske funktioner
		StringReplace, MathString, MathString, {sin}, {\sin}, All
		StringReplace, MathString, MathString, {tan}, {\tan}, All
		StringReplace, MathString, MathString, {cot}, {\cot}, All
		StringReplace, MathString, MathString, {acos}, {\arccos}, All
		StringReplace, MathString, MathString, {asin}, {\arcsin}, All
		StringReplace, MathString, MathString, {atan}, {\arctan}, All
		StringReplace, MathString, MathString, {acot}, {\arccot}, All
		StringReplace, MathString, MathString, {cosh}, {\cosh}, All
		StringReplace, MathString, MathString, {sinh}, {\sinh}, All
		StringReplace, MathString, MathString, {tanh}, {\tanh}, All
		StringReplace, MathString, MathString, {coth}, {\coth}, All
		StringReplace, MathString, MathString, {sec}, {\sec}, All
		StringReplace, MathString, MathString, {csc}, {\csc}, All
		
		StringReplace, MathString, MathString, {ln}, {\ln}, All							;Logaritmefunktioner
		StringReplace, MathString, MathString, {log}, {\log}, All
		
		StringReplace, MathString, MathString, % Chr(0x221E), {\infty}, All				;Uendelighedstegn, gradtegn, havel�ge
		StringReplace, MathString, MathString, {deg}, {^{\circ}}, All
		StringReplace, MathString, MathString, % Chr(0xB0), {^{\circ}}, All
		StringReplace, MathString, MathString, #, {\#}, All
		StringReplace, MathString, MathString, \;\mathrm{^{\circ}}, \mathrm{^{\circ}}, All
		
		MathString := SepComma(MathString,SepDot2Comma)
		
		SendInput {Shift Up}{Alt Up}{Raw}%MathString%
return

Setup:
	If Ex1 = comma
		{
			If Ex2 != 0
				{
					SepDot2Comma = 1
					MsgBox Komma anvendes som decimaltalsseparator.
				}
			Else
				{
					SepDot2Comma = 0
					MsgBox Punktum anvendes som decimaltalsseparator.
				}
		}
	Else If Ex1 = debug
		{
			If Ex2 != 0
				{
					debug = 1
					MsgBox Debug-tilstand sl�et til.
				}
			Else
				{
					debug = 0
					MsgBox Debug-tilstand sl�et fra.
				}
		}
	return

SepComma(Ex,SepDot2Comma)
	{
		If SepDot2Comma = 1
			{
				ExStart = %Ex%
				n = 0
				NumClearLoop:
					StringReplace, Ex, Ex, %n%,, All
					n := n + 1
					If n > 9
						{
							goto, SepClear
						}
					goto, NumClearLoop
				SepClear:
					StringReplace, Ex, Ex, .,, All
				StringLen, ExLen, Ex
				If ExLen > 0
					{
						return ExStart
					}
				Else
					{
						StringReplace, ExEnd, ExStart, ., `,, All
						return ExEnd
					}
			}
		Else
			{
			return %Ex%
			}
	}

Error(error, errormsg, line, ms, mss, o, ot)
	{
		FormatTime, CurrentTime,, yyyy-MM-dd HH:mm:ss
		MsgBox Fejl %error%: %errormsg%`n`nFejlrapport gemt i %A_WorkingDir%\mathcad2latex.log
		FileAppend,
			(
			`n`nDato: %CurrentTime%`nFejl: %error%`nBesked: %errormsg%`nLinje: %line%`nMathString: %ms%`nMathStringStart: %mss%`nOperator: %o%`nOperatortype: %ot%
			), mathcad2latex.log
		reload
	}

>^>+R::
reload
return

>^>+E::
MsgBox, 1, Mathcad2LaTeX v1.4, Mathcad2LaTeX v1.4 afsluttes.
IfMsgBox OK
	{
		ExitApp
		return
	}
Else
	{
		return
	}