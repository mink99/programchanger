REM Stringtable creator
DIM s AS STRING * 20


DIM SHARED bankcount AS INTEGER
DIM SHARED textcount AS INTEGER
DIM SHARED f20 AS STRING * 20



INPUT "please enter basename", fn$


IF _FILEEXISTS("e:\temp\strings.txt") THEN
    INPUT "appending Stringtable", gg$
END IF


OPEN "e:\temp\" + fn$ + ".rea" FOR INPUT AS #1
OPEN "e:\temp\" + fn$ + ".bnk" FOR OUTPUT AS #2
OPEN "e:\temp\" + fn$ + ".pat" FOR OUTPUT AS #3
OPEN "e:\temp\strings.txt" FOR APPEND AS #4
REM stringtable reclen: 0x1C


DO UNTIL EOF(1)
    LINE INPUT #1, a$
    a$ = LTRIM$(RTRIM$(a$))
    IF LEN(a$) > 0 THEN
        IF (LEFT$(a$, 1) <> "/") AND (LEFT$(a$, 1) <> ";") THEN
            IF handleBank(a$) = 0 THEN
                handlePatch a$
            END IF

        END IF
    END IF
LOOP


CLOSE #1
CLOSE #2
CLOSE #3
CLOSE #4


FUNCTION handleBank% (b$)

btemplate$ = "### ### ### &"

IF LEFT$(UCASE$(b$), 4) = "BANK" THEN
    tmp$ = RTRIM$(LTRIM$(MID$(b$, 5)))

    FOR i% = 1 TO LEN(tmp$)
        IF MID$(tmp$, i%, 1) = " " THEN
            l1$ = LEFT$(tmp$, i%)
            tmp$ = MID$(tmp$, i% + 1)
            EXIT FOR
        END IF
    NEXT

    FOR i% = 1 TO LEN(tmp$)
        IF MID$(tmp$, i%, 1) = " " THEN
            l2$ = LEFT$(tmp$, i%)
            tmp$ = MID$(tmp$, i% + 1)
            EXIT FOR
        END IF
    NEXT


    lb% = VAL(l1$)
    hb% = VAL(l2$)
    IF LEN(tmp$) > 12 THEN
        a$ = LEFT$(tmp$, 5) + "..." + RIGHT$(tmp$, 4)
        tmp$ = a$
    END IF

    stringtable tmp$
    PRINT #2, USING btemplate$; bankcount, lb%, hb%, tmp$

    bankcount = bankcount + 1

    handleBank% = 1
ELSE

    handleBank% = 0


END IF

END FUNCTION

REM ------------------------------------------------------

SUB handlePatch (b$)


btemplate$ = "## ### :&"

tmp$ = RTRIM$(LTRIM$(b$))
PRINT "pt 0", tmp$
FOR i% = 1 TO LEN(tmp$)
    IF MID$(tmp$, i%, 1) = " " THEN
        l1$ = LEFT$(tmp$, i%)
        tmp$ = MID$(tmp$, i% + 1)
        EXIT FOR
    END IF
NEXT
PRINT "pt (1)", tmp$

PRINT "L1", l1$

lb% = VAL(l1$)
IF LEN(tmp$) > 16 THEN
    a$ = LEFT$(tmp$, 7) + "..." + RIGHT$(tmp$, 6)
    tmp$ = a$
END IF

stringtable tmp$
PRINT #3, USING btemplate$; bankcount, lb%, tmp$


END SUB

SUB stringtable (a$)
btemplate$ = "### :&:"
textcount = textcount + 1
f20 = a$
PRINT #4, USING btemplate$; textcount; f20

END SUB

