{
	if( $0 ~ /[ \t]+.+:[ \t]+\.[Ee][Qq][Uu][ \t]+.+/ )
		sub( /[ \t]+/, "" )
	if( $0 ~ /[ \t]+.+:[ \t]+\.[Aa][Ss][Ss][Ii][Gg][Nn][ \t]+.+/ )
		sub( /[ \t]+/, "" )
	
	print
}

END{
	print	"\t.end"
}
