BEGIN{
	TRUE = 1
	FALSE = 0
	
	name = ""
	isdummy = FALSE
}
{
	if( $0 ~ /\.[Ss][Ee][Cc][Tt][Ii][Oo][Nn]/ ){
		if( isdummy ){
			printf( "SIZEOF_%s\t.ASSIGN\t%d\n\n", name, offset );
			isdummy = FALSE
			name = ""
		}
		
		n = split( $2, token, "," )
		if( n == 1 ){
			printf( "\t.section\t%s\n", token[1] )
		}
		else{
			if( token[2] ~ /[Dd][Uu][Mm][Mm][Yy]/ ){
				name = token[1]
				isdummy = TRUE
				offset = FALSE
			}
			else{
				if( token[2] ~ /[Cc][Oo][Dd][Ee]/ ){
					printf( "\t.section\t.text\n" )
				}
				else if( token[2] ~ /[Ss][Tt][Aa][Cc][Kk]/ ){
					printf( "\t.section\t.stack\n" )
				}
				else if( token[2] ~ /[Cc][Oo][Mm][Mm][Oo][Nn]/ ){
					printf( "\t.section\tCOMMON\n" )
				}
				else{
					printf( "\t.section\t.data\n" )
				}
				if( n == 3 ){
					if( token[3] ~ /[Aa][Ll][Ii][Gg][Nn]/ ){
						sub( /[Aa][Ll][Ii][Gg][Nn]=/,"",token[3] )
						printf( "\t.align\t%s\n", token[3] )
					}
				}
			}
		}
	}
	else{
		if( $0 ~ /SIZEOF[ \t]+/ ){
			sub( /SIZEOF[ \t]+/, "SIZEOF_" )
		}
		if(( isdummy )&&( $0 ~ /\.[Rr][Ee][Ss]/ )){
			split( $0, token, " " )
			printf( "%s\t.ASSIGN\t%d\n", token[1], offset )
			if( $0 ~ /\.[Ll]/ ){
				offset += 4
			}
			else if( $0 ~ /\.[Ww]/ ){
				offset += 2
			}
			else if( $0 ~ /\.[Bb]/ ){
				offset += 1
			}
			else{
				offset += 4
			}
		}
		else{
			print
		}
	}
}
