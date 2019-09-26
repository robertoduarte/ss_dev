BEGIN {
	TRUE = 1
	FALSE = 0
	
	pool_flag = FALSE
	isoutput = FALSE
	isdelay = FALSE
	pool_l = ""
	pool_w = ""
	prev_line = ""
	next_line = ""
	entries = 0
}
{
	if( isoutput ){
		output_letteral_pool()
		isoutput  = FALSE
		pool_flag = FALSE
		
		print
	}
	else if( isdelay ){
		if( $0 ~ ":" )
			next_line = $0
		else
			prev_line = $0
		
		isdelay  = FALSE
		isoutput = TRUE
	}
	else{
		if( $0 ~ /mov\.w[ \t]+#/ ){
			source      = $2
			destination = $2
			gsub( /,.*/, "", source )
			gsub(   "#", "", source )
			gsub( /.*,/, "", destination )
			printf( "\tmov.w\tIMM_LABEL%d,%s\n", entries, destination )
			pool_w = pool_w"IMM_LABEL"entries":\n"
			pool_w = pool_w"\t.short\t"source"\n"
			entries++
			pool_flag = TRUE
		}
		else if( $0 ~ /mov\.l[ \t]+#/ ){
			source      = $2
			destination = $2
			gsub( /,.*/, "", source )
			gsub(   "#", "", source )
			gsub( /.*,/, "", destination )
			printf( "\tmov.l\tIMM_LABEL%d,%s\n", entries, destination )
			pool_l = pool_l"IMM_LABEL"entries":\n"
			pool_l = pool_l"\t.long\t"source"\n"
			entries++
			pool_flag = TRUE
		}
		else{
			if( pool_flag ){
				if(( $0 ~ /[ \t]+rts[ \t]*$/ )||( $0 ~ /[ \t]+bra[ \t]+.+$/ ))
					isdelay = TRUE
			}
			
			print
		}
	}
}

END{
	if( pool_flag )
		output_letteral_pool()
}

function	output_letteral_pool( void ){
	if( length( prev_line ) > 0 ){
		print	prev_line
		prev_line = ""
	}
	
	if( length( pool_w ) > 0 ){
		printf("\t.align\t2\n")
		print pool_w
		pool_w = ""
	}
	
	if( length( pool_l ) > 0 ){
		printf("\t.align\t4\n")
		print pool_l
		pool_l = ""
	}
	
	if( length( next_line ) > 0 ){
		print	next_line
		next_line = ""
	}
}

