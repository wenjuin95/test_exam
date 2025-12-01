#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

fd_set active_fd;
fd_set read_fd;
fd_set write_fd;
int maxfd = 0;
int ids[65535];
int *buffer[65535];

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

void fatal() {
	write(2, "Fatal error\n", 12);
	exit(1);
}

void broadcast(char *msg, int sender) {
	for (int fd = 0; fd <= maxfd; ++fd)
		if (FD_ISSET(fd, &write_fd) && fd != sender)
			send(fd, msg, strlen(msg), 0);
}

int setup_connection(int port) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		fatal();

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(2130706433);
	addr.sin_port = htons(port);

	if ((bind(listener, (struct sockaddr *)&addr, sizeof(addr))) == -1)
		fatal();

	if (listen(listener, SOMAXCONN) == -1)
		fatal();

	FD_SET(listener, &active_fd);
	if (listener > maxfd)
		maxfd = listener;
	return listener;
}

void accept_listener(int listener) {
	static int client_count = 0;

	struct sockaddr_in addr;
	socklen_t size = sizeof(addr);
	int connection = accept(listener, (struct sockaddr *)&addr, &size);
	if (connection == -1)
		fatal();

	FD_SET(connection, &active_fd);
	ids[connection] = client_count++;
	buffer[connection] = NULL;
	if (connection > maxfd)
		maxfd = connection;

	char msg[64];
	sprintf(msg, "server: client %d just arrived\n", ids[connection]);
	broadcast(msg, connection);
}

void disconnect(int connection) {
	char msg[64];
	sprintf(msg, "server: client %d just left\n", ids[connection]);
	broadcast(msg, connection);

	FD_CLR(connection, &active_fd);
	close(connection);
	free(buffer[connection]);

	if (connection == maxfd)
		while (maxfd >= 0 && !FD_ISSET(maxfd, &active_fd))
			--maxfd;
}
