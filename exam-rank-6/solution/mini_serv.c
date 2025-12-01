#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/select.h> // select, fd_set
#include <stdlib.h> // calloc, malloc, free, exit
#include <stdio.h> // sprintf

// *buf = strdup("hello\nhow are you\nbye");
// 1st call to extract_message():
	// Finds first \n after "hello"
	// *msg = "hello\n"
	// *buf = "how are you\nbye"
// 2nd call:
	// *msg = "how are you\n"
	// *buf = "bye"
// 3rd call:
	// No \n found
	// Returns 0, waits for more data
// this function extracts complete sentences (ending in \n) from the
// buffer into msg. buffer is updated so that its pointer starts past
// the sentence that was already extracted.
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

// Variable				Type		Purpose
// cliC					int			Current number of connected clients
// mFD					int			Maximum fd to pass into select()
// ids[65535]			int[]		Maps file descriptors to client IDs
// mbuf[65535]			char *[]	Accumulates partial messages per client
// rbuf[1001]			char[]		Temporary receive buffer (1KB max)
// wbuf[42]				char[]		Temporary send buffer for system messages
// rFDs, wFDs, aFDs		fd_set		For select(): read, write, all active fds

int cliC = 0, mFD = 0, ids[65535];
char rbuf[1001], wbuf[42], *mbuf[65535];
fd_set rFDs, wFDs, aFDs;

// error message to be printed in case any of the system calls fail.
void err()
{
	write(STDERR_FILENO, "Fatal error\n", 12);
	exit(1);
}

// helper function to get the server socket fd.
// the function will also update mFD straightaway.
int getSock()
{
	mFD = socket(AF_INET, SOCK_STREAM, 0);
	if (mFD < 0)
		err();
	FD_SET( mFD, &aFDs );
	return mFD;
}

// broadcast a message to all the other clients other than the author (auth)
void sysMsg(char *msg, int auth)
{
	for (int fd = 0; fd <= mFD; fd++)
	{
		if (FD_ISSET(fd, &wFDs) && fd != auth)
			send(fd, msg, strlen(msg), 0);
	}
}

void usrMsg(int fd)
{
	char *msg;

	while(extract_message(&(mbuf[fd]), &msg))
	{
		sprintf(wbuf, "client %d: ", ids[fd]);
		sysMsg(wbuf, fd);
		// best to separate, because wbuf is only 42 bytes
		// msg is part of mbuf, no overflow issue.
		sysMsg(msg, fd);
		// remember to free msg
		free(msg);
	}
}

// adds a client to the fd_set, and initializes its fd and message buffer.
// sprintf works just like printf except it writes to a buffer. it also allows
// format specifiers for string substitution.
void addCli(int fd)
{
	if (fd > mFD)
		mFD = fd;
	ids[fd] = cliC++;
	mbuf[fd] = 0;
	FD_SET(fd, &aFDs);

	sprintf(wbuf, "server: client %d just arrived\n", ids[fd]);
	sysMsg(wbuf, fd);
}

// free the message buffer (str_join uses malloc), remove the client from the fd_set
// and close the file descriptor.
void delCli(int fd)
{
	sprintf(wbuf, "server: client %d just left\n", ids[fd]);
	sysMsg(wbuf, fd);

	free(mbuf[fd]);
	FD_CLR(fd, &aFDs);
	close(fd);
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		write(STDERR_FILENO, "Wrong number of arguments\n", 26);
		exit(1);
	}

	FD_ZERO( &aFDs);
	int sFD = getSock();

	struct sockaddr_in servA;
	socklen_t servS = sizeof(servA);
	bzero(&servA, servS);

	// assign IP, PORT
	servA.sin_family = AF_INET;
	servA.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servA.sin_port = htons(atoi(av[1]));

	// Binding newly created socket to given IP and verification
	if ((bind(sFD, (const struct sockaddr *)&servA, servS)) != 0)
		err();

	if (listen(sFD, SOMAXCONN) != 0)
		err();

	while (1)
	{
		rFDs = wFDs = aFDs;

		int slct = select(mFD + 1, &rFDs, &wFDs, 0, 0);
		if (slct < 0)
			err();
		if (slct == 0)
			continue;

		for (int cFD = 0; cFD <= mFD; cFD++)
		{
			// check on rFDs and not aFDs because only rFDs will
			// be modified by the select call.
			if (!FD_ISSET(cFD, &rFDs))
				continue;

			if (cFD == sFD)
			{
				int nFD = accept(sFD, (struct sockaddr *)&servA, &servS);

				if (nFD >= 0)
				{
					addCli(nFD);
					// the loops runs on a static set_fd.
					// if there are any updates to the set_fd, break out of the loop.
					break;
				}
			}
			else
			{
				// read buffer is 1001 bytes; final byte for null terminator.
				int bytC = recv(cFD, rbuf, 1000, 0);
				// if client has hung up, delete the client.
				if (bytC <= 0)
				{
					delCli(cFD);
					break;
				}
				else
				{
					rbuf[bytC] = 0;
					// append the current read buffer to the existing message buffer of that fd.
					mbuf[cFD] = str_join(mbuf[cFD], rbuf);
					// send out the usrMsg to all the other clients.
					usrMsg(cFD);
				}
			}
		}
	}
}
