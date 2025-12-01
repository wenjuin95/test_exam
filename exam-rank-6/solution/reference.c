#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

int			cliC = 0, mFD = 0, ids[ 65535 ];
char		rbuf[ 1001 ], wbuf[ 42 ], *mbuf[ 65535 ];
fd_set	rFDs, wFDs, aFDs;

void	err()
{
	write( 2, "Fatal error\n", 12 );
	exit( 1 );
}

int		getSock()
{
	mFD = socket( AF_INET, SOCK_STREAM, 0 );
	if( mFD < 0 )
		err();
	FD_SET( mFD, &aFDs );
	return( mFD );
}

void	sysMsg( char *msg, int auth )
{
	for ( int fd = 0; fd <= mFD; fd++ )
	{
		if ( FD_ISSET( fd, &wFDs ) && fd != auth )
			send( fd, msg, strlen( msg ), 0 );
	}
}

void	usrMsg( int fd )
{
	char	*msg;

	while( extract_message( &( mbuf[ fd ]), &msg ))
	{
		sprintf( wbuf, "client %d: ", ids[ fd ]);
		sysMsg( wbuf, fd );
		sysMsg( msg, fd );
		free( msg );
	}
}

void	addCli( int fd )
{
	if ( fd > mFD )
		mFD = fd;
	ids[ fd ] = cliC++;
	mbuf[ fd ] = 0;
	FD_SET( fd, &aFDs );

	sprintf( wbuf, "server: client %d just arrived\n", ids[ fd ]);
	sysMsg( wbuf, fd );
}

void	delCli( int fd )
{
	sprintf( wbuf, "server: client %d just left\n", ids[ fd ]);
	sysMsg( wbuf, fd );

	free( mbuf[ fd ] );
	FD_CLR( fd, &aFDs );
	close( fd );
}

int main(int ac, char **av )
{
	if( ac != 2 )
	{
		write( 2, "Wrong number of arguments\n", 26 );
		exit( 1 );
	}

	FD_ZERO( &aFDs );
	int sFD = getSock();

	struct sockaddr_in servA;
	socklen_t servS = sizeof( servA );
	bzero( &servA, servS );

	servA.sin_family = AF_INET;
	servA.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servA.sin_port = htons( atoi( av[ 1 ]));

	if( bind( sFD, ( const struct sockaddr * )&servA, servS ) != 0 )
		err();

	if( listen( sFD, SOMAXCONN ) != 0 )
		err();

	while( 1 )
	{
		rFDs = wFDs = aFDs;

		int slct = select( mFD + 1, &rFDs, &wFDs, 0, 0 );

		if ( slct < 0 )  err();
		if ( slct == 0 ) continue;

		for( int cFD = 0; cFD <= mFD; cFD++ )
		{
			if( !FD_ISSET( cFD, &rFDs ))
				continue;

			if( cFD == sFD )
			{
				int nFD = accept( sFD, ( struct sockaddr * )&servA, ( socklen_t * )&servS );

				if( nFD >= 0 )
				{
					addCli( nFD );
					break;
				}
			}

			else
			{
				int bytC = recv( cFD, rbuf, 1000, 0 );

				if( bytC <= 0 )
				{
					delCli( cFD );
					break;
				}

				else
				{
					rbuf[ bytC ] = 0;
					mbuf[ cFD ] = str_join( mbuf[ cFD ], rbuf );
					usrMsg( cFD );
				}
			}
		}
	}
}
