BEGIN{
	INCS = split( INCLUDE, INCDIR, ";" )
}

{
	remove_comment( $0 )
}

function remove_comment( line, path, n, dir ){
	gsub( /[ \t]*;.*$/, "", line )
	if( line ~ /^.+$/ ){
		if ( line ~ /^[ \t]*\.include[ \t]+/ ){
			gsub( /^[ \t]*\.include[ \t]+/, "", line )
			gsub( "\"", "", line )
			path = line
			if( !isexist( path ) ){
				path = ""
				for( n = 0; n < INCS; n++ ){
					path = INCDIR[n+1]"/"line
					if( isexist( path ) )
						break;
					
					path = ""
				}
				
				if( !path ){
					dir = ""
					path = line
					do{
						print "�t�@�C����������܂���: " path > "con"
						print "�f�B���N�g�����w�肵�Ă�������." > "con"
						close( "con" )
						
						if(( getline dir < "con" ) > 0 ){
							if( dir ~ /\/$/ )
								path = dir line
							else
								path = dir"/"line
						}
						else{
							close( "con" )
							exit
						}
					}while( !isexist( path ) )
					
					if( dir )
						INCDIR[++INCS] = dir
				}
			}
			
			while(( getline _line < path ) > 0 )
				remove_comment( _line )
			
			close( path )
		}
		else{
			gsub( /[ \t]*,[ \t]*/, ",", line )
			gsub( /[ \t]+$/, "", line )
			print line
		}
	}
}

function	isexist( path, s, ret ){
	if(( getline s < path ) == -1 )
		ret = 0
	else
		ret = 1;
	
	close( path )
	
	return	ret
}
