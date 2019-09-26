{
	if( $0 ~ /[ \t]+.+:[ \t]+\.equ[ \t]+.+/ ){
		sub( /[ \t]+/, "" )
	}
	
	gsub( /\$/, "." )
	
	if( $0 ~ /[^A-Za-z0-9"]+(SP)([^A-Za-z0-9"]+|$)/ )
		gsub( "SP", "sp" )
	if( $0 ~ /[^A-Za-z0-9"]+(SR)([^A-Za-z0-9"]+|$)/ )
		gsub( "SR", "sr" )
	if( $0 ~ /[^A-Za-z0-9"]+(PR)([^A-Za-z0-9"]+|$)/ )
		gsub( "PR", "pr" )
	if( $0 ~ /[^A-Za-z0-9"]+(GBR)([^A-Za-z0-9"]+|$)/ )
		gsub( "GBR", "gbr" )
	if( $0 ~ /[^A-Za-z0-9"]+(VBR)([^A-Za-z0-9"]+|$)/ )
		gsub( "VBR", "vbr" )
	if( $0 ~ /[^A-Za-z0-9"]+(PC)([^A-Za-z0-9"]+|$)/ )
		gsub( "PC", "pc" )
	if( $0 ~ /[^A-Za-z0-9"]+(MACH)([^A-Za-z0-9"]+|$)/ )
		gsub( "MACH", "mach" )
	if( $0 ~ /[^A-Za-z0-9"]+(MACL)([^A-Za-z0-9"]+|$)/ )
		gsub( "MACL", "macl" )
	if( $0 ~ /[^A-Za-z0-9]+R[0-9]([^A-Za-z]+|$)/ ){
		gsub( "R0", "r0" )
		gsub( "R1", "r1" )
		gsub( "R2", "r2" )
		gsub( "R3", "r3" )
		gsub( "R4", "r4" )
		gsub( "R5", "r5" )
		gsub( "R6", "r6" )
		gsub( "R7", "r7" )
		gsub( "R8", "r8" )
		gsub( "R9", "r9" )
	}
	
	if( $0 ~ /[^A-Za-z0-9"]+(CMP)([^A-Za-z0-9"]+|$)/ )
		gsub( "CMP", "cmp" )
	gsub( ",[ \t]*/[Ss]$", ",.s" )
	gsub( "/[Ss][ \t]+", ".s\t" )
	gsub( "/EQ[ \t]+", "/eq\t" )
	gsub( "/GE[ \t]+", "/ge\t" )
	gsub( "/GT[ \t]+", "/gt\t" )
	gsub( "/HI[ \t]+", "/hi\t" )
	gsub( "/HS[ \t]+", "/hs\t" )
	gsub( "/PL[ \t]+", "/pl\t" )
	gsub( "/PZ[ \t]+", "/pz\t" )
	gsub( "/STR[ \t]+", "/str\t" )
	
	if(( $0 ~ /[Mm][Oo][Vv]\.[LlWwBb][ \t]+r[0-9]+,r[0-9]+/ )){
		gsub( /[Mm][Oo][Vv]\.[LlWwBb]/, "mov\t" )
	}
	if( $0 ~ /mov\.b[ \t]+#/ ){
		gsub( "mov.b", "mov\t" )
	}
	
	gsub( /tas[ \t]+/, "tas.b\t" )
	gsub( /dmuls[ \t]+/, "dmuls.l\t" )
	gsub( /swap[ \t]+/, "swap.w\t" )
	
	gsub( /[ \t]+RTS/, "\trts" )
	gsub( /[ \t]+BRA[ \t]+/, "\tbra\t" )
	gsub( /MOV\.B/, "mov.b" )
	gsub( /MOV\.W/, "mov.w" )
	gsub( /MOV\.L/, "mov.l" )
	gsub( /STS\.L/, "sts.l" )
	gsub( /LDS\.L/, "lds.l" )
	gsub( /STC\.L/, "stc.l" )
	gsub( /LDC\.L/, "ldc.l" )
	
	gsub( /\.endm/, ".ENDM" )
	gsub( /\.macro/, ".MACRO" )
	gsub( /\.aif/, ".AIF" )
	gsub( /\.aelse/, ".AELSE" )
	gsub( /\.aendi/, ".AENDI" )
	gsub( /[ \t]+eq[ \t]+/, "\tEQ\t" )
	gsub( /[ \t]+ne[ \t]+/, "\tNE\t" )
	gsub( /[ \t]+gt[ \t]+/, "\tGT\t" )
	gsub( /[ \t]+ge[ \t]+/, "\tGE\t" )
	gsub( /[ \t]+lt[ \t]+/, "\tLT\t" )
	gsub( /[ \t]+le[ \t]+/, "\tLE\t" )
	gsub( /\.assignc/, ".ASSIGNC" )
	gsub( /\.assigna/, ".ASSIGNA" )
	gsub( /\.assign/, ".ASSIGN" )
	
	print
}
