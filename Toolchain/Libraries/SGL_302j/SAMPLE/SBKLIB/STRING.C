/*
   for String.h
*/

#define	NULL	0
#define	size_t	unsigned long

char	*_builtin_strcpy( char *s, const char *ct )
{
    char *pt;

    pt = s;

    do {
	*(pt++) = *(ct++);
    } while( ct != NULL );


    return s;
}

size_t	strlen( const char *s )
{
    size_t	i=0;

    while( *(s++) != NULL )
      i++;

    return i;
}

char	*strncpy( char *s, char *ct, size_t n )
{
    size_t	i=0, len;
    char	*pt;

    pt = s;

    len = strlen( ct );

    for( i=0; i<len; i++ )
      *(pt++) = *(ct++);
    *pt = NULL;

    for( i = len; i< n; i++ )
	  *(pt++) = NULL;
    *pt = NULL;

    return s;
}

char	*strcat( char *s, const char *ct )
{
    char	*pt;

    pt = s;
    pt += strlen( pt );
    do {
	*(pt++) = *(ct++);
    } while( *ct != NULL );

    return s;
}

char	*strncat( char *s, char *ct, size_t n )
{
    char	*pt;
    size_t	i=0;

    pt = s;
    pt  += strlen( pt );
    for( ;( *ct != NULL ) && ( (i++) < n ); )
      *(pt++) = *(ct++);

    *ct = NULL;

    return s;
}

int	 _builtin_strcmp( const char *cs, const char *ct )
{
    while( *cs != NULL ) {
	if ( *cs < *(ct++) )
	  {
	      return -1;
	  }
	else
	  {
	      if (  *(cs++) > *(ct++) )
		{
		    return 1;
		}
	  }
    }

    return 0;
}

int	strncmp( const char *cs, const char *ct, size_t n )
{
    size_t i = 0;

    while( ( *cs != NULL ) && ( i++ < n ) ) {
	if ( *cs < *(ct++) )
	  {
	      return -1;
	  }
	else
	  {
	      if (  *(cs++) > *(ct++) )
		{
		    return 1;
		}
	  }
    }

    return 0;
}

char	*strchr( const char *cs, int c )
{
    int	i, len;

    len = strlen( cs );
    for( i=0; i<len; i++, cs++ )
      {
	if ( *cs == c )
	  {
	      return (char *)cs;
	  }
    }

    return NULL;
}

char	*strrchr( const char *cs, int c )
{
    int	i, len;

    len = strlen( cs );
    cs  = cs + len - 1;
    for( i=0; i<len; i++, cs-- )
      {
	if ( *cs == c )
	  {
	      return (char *)cs;
	  }
    }

    return NULL;
}

void	*memcpy( void *s, const void *ct, size_t n )
{
    void	*pt;
    int		i;

    pt = s;

    for( i=0; i<n; i++ )
      *( ((char *)pt)++ ) = *( ( (char *)ct )++ );

    return (char *)s;
}

void	*memove( void *s, const void *ct, size_t n )
{
    void	*pt;
    int		i;

    pt = s;
    if ( (int)s < (int)ct )
      {
	  for( i=0; i<n; i++ )
	    *( ((char *)pt)++ ) = *( ( (char *)ct )++ );

      }
    else
      {
	  (char *)pt += n - 1;
	  (char *)ct += n - 1;
	  for( i=0; i<n; i++ )
	    *( ((char *)pt)-- ) = *( ( (char *)ct )-- );
      }


    return s;
}

void	*memset( void *s, int c, size_t n )
{
    int		i;
    void	*pt;

    pt = s;
    for(i=0; i<n; i++)
      {
	  *((char *)pt)++ = c;
      }

    return s;
}
