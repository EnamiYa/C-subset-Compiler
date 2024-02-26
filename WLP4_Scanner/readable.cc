.ALPHABET
a-z A-Z 0-9 
( ) { } [ ]
= ! < > + - * / % , ; &

.STATES
empty

NUM_0! NUM_$!

PLUS! MINUS! STAR! SLASH! PCT! COMMA! SEMI! AMP!
LPAREN! RPAREN!
LBRACE! RBRACE!
LBRACK! RBRACK!

LT! LE!
GT! GE!
BECOMES! EQ!
Z NE!

ID_W!

ID_WA! 
ID_WAI! WAIN!
ID_WA$! ID_WAI$!

ID_WH! 
ID_WHI! ID_WH$!
ID_WHIL! ID_WHI$!
WHILE! ID_WHIL$!

ID_W$!

ID_I! 
IF! ID_IN! ID_I$!
ID_IN$! INT!

ID_E! ID_EL! ID_E$! ID_ELS! ID_EL$! ELSE! ID_ELS$!

ID_P! ID_PR! ID_PRI! ID_PRIN! ID_PRINT! ID_PRINTL! PRINTLN! 
      ID_P$! ID_PR$! ID_PRI$! ID_PRIN$! ID_PRINT$! ID_PRINTL$!

ID_R! ID_RE! ID_RET! ID_RETU! ID_RETUR! RETURN!
      ID_R$! ID_RE$! ID_RET$! ID_RETU$! ID_RETUR$!

ID_N! ID_NU! ID_NUL! NULL!
      ID_N$! ID_NU$! ID_NUL$!

ID_n! ID_ne! NEW!
      ID_n$! ID_ne$!

ID_D! ID_DE! ID_DEL! ID_DELE! ID_DELET! DELETE!
      ID_D$! ID_DE$! ID_DEL$! ID_DELE$! ID_DELET$!

ID_D1! ID_D1+!

.TRANSITIONS
empty 0 NUM_0

empty 1-9 NUM_$
NUM_$ 0-9 NUM_$

empty + PLUS
empty - MINUS
empty * STAR 
empty / SLASH
empty % PCT
empty , COMMA
empty ; SEMI
empty & AMP
empty ( LPAREN
empty ) RPAREN
empty { LBRACE
empty } RBRACE
empty [ LBRACK
empty ] RBRACK

////////////////////////////////////////// W
empty w ID_W

ID_W 0-9 b-g i-z A-Z ID_W$

/// WAIN?
ID_W a ID_WA

ID_WA i ID_WAI
ID_WA 0-9 A-Z a-h j-z ID_WA$ // exclude i

ID_WAI n WAIN
ID_WAI 0-9 A-Z a-m o-z ID_WAI$

/// WHILE ?
ID_W h ID_WH

ID_WH i ID_WHI
ID_WH 0-9 A-Z a-h j-z ID_WH$ // exclude i

ID_WHI l ID_WHIL
ID_WHI 0-9 A-Z a-k m-z ID_WHI$

ID_WHIL e WHILE
ID_WHIL 0-9 A-Z a-d f-z ID_WHIL$

////////////////////////////////////////// I

empty i ID_I

ID_I n ID_IN
ID_IN t INT

ID_I f IF

ID_I 0-9 A-Z a-e g-m o-z ID_I$

////////////////////////////////////////// E

empty e ID_E

ID_E l ID_EL
ID_E 0-9 A-Z a-k m-z ID_E$

ID_EL s ID_ELS
ID_EL 0-9 A-Z a-r t-z ID_EL$

ID_ELS e ELSE
ID_ELS 0-9 A-Z a-d f-z  ID_ELS$

////////////////////////////////////////// P
empty p ID_P

ID_P r ID_PR
ID_P 0-9 A-Z a-q s-z ID_P$

ID_PR i ID_PRI
ID_PR 0-9 A-Z a-h j-z ID_PR$

ID_PRI n ID_PRIN
ID_PRI 0-9 A-Z a-m o-z ID_PRI$

ID_PRIN t ID_PRINT
ID_PRIN 0-9 A-Z a-s u-z ID_PRIN$

ID_PRINT l ID_PRINTL
ID_PRINT 0-9 A-Z a-k m-z ID_PRINT$

ID_PRINTL n PRINTLN
ID_PRINTL 0-9 A-Z a-m o-z ID_PRINTL$

////////////////////////////////////////// R
empty r ID_R

ID_R e ID_RE
ID_R 0-9 A-Z a-d f-z ID_R$


ID_RE t ID_RET
ID_RE 0-9 A-Z a-s u-z ID_RE$

ID_RET u ID_RETU
ID_RET 0-9 A-Z a-t v-z ID_RET$

ID_RETUR n RETURN
ID_RETUR 0-9 A-Z a-m o-z ID_RETUR$

////////////////////////////////////////// N CAPS

empty N ID_N

ID_N U ID_NU
ID_N 0-9 A-T V-Z a-z ID_N$

ID_NUL L NULL
ID_NUL 0-9 A-K M-Z a-z ID_NUL$

////////////////////////////////////////// N NOT_CAPS

empty n ID_n

ID_n e ID_ne
ID_n 0-9 A-Z a-d f-z ID_n$

ID_ne w NEW
ID_ne 0-9 A-Z a-v x-z ID_ne$

////////////////////////////////////////// N NOT_CAPS
empty d ID_D

ID_D e ID_DE
ID_D 0-9 A-Z a-d f-z ID_D$

ID_DE l ID_DEL
ID_DE 0-9 A-Z a-k m-z ID_DE$

ID_DEL e ID_DELE
ID_DEL 0-9 A-Z a-d f-z ID_DEL$

ID_DELE t ID_DELET
ID_DELE 0-9 A-Z a-s u-z ID_DELE$

ID_DELET e DELETE
ID_DELET 0-9 A-Z a-d f-z ID_DELET$

////////////////////////////////////////// OTHER IDS
empty a-c f-h j-m o q s-v x-z A-M O-Z ID_D1
ID_D1 0-9 A-Z a-z ID_D1+
ID_D1+ 0-9 A-Z a-z ID_D1+

empty < LT
LT = LE

empty > GT
GT = GE

empty = BECOMES
BECOMES = EQ

empty ! Z
Z = NE

.INPUT

