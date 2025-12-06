#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

fd_set active_fd;
fd_set write_fd;
fd_set read_fd;
int maxfd = 0;
int ids[65535];
char *buffer[65535];

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
    write(2, "Fatal Error\n", 12);
    exit(1);
}

void broadcast(char *msg, int sender) {
    for (int fd = 0; fd <= maxfd; ++fd) {
        if (FD_ISSET(fd, &write_fd) && fd != sender)
            send(fd, msg, strlen(msg), 0);
    }
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

void accept_connection(int listener) {
    static int client_counter = 0;

    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    int connection = accept(listener, (struct sockaddr *)&addr, &size);
    if (connection == -1)
        fatal();

    FD_SET(connection, &active_fd);
    ids[connection] = client_counter++;
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

int main(int ac, char **av) {
    if (ac != 2) {
        write(2, "Wrong number of arguments\n", 26);
        return 1;
    }

    FD_ZERO(&active_fd);

    int port = atoi(av[1]);
    if (port < 0 || port > 65535)
        fatal();
    int listener = setup_connection(port);

    while (1) {
        write_fd = read_fd = active_fd;

        int read = select(maxfd+1, &read_fd, &write_fd, NULL, NULL);
        if (read == -1)
            fatal();
        if (read == 0)
            continue;

        for (int fd = 0; fd <= maxfd; ++fd) {
            if (!FD_ISSET(fd, &read_fd))
                continue;

            if (fd == listener)
                accept_connection(listener);
            else {
                int recv_buffer[1024];
                ssize_t msg_len = recv(fd, recv_buffer, 1023, 0);
                if (msg_len <= 0) {
                    disconnect(fd);
                    break;
                } else {
                    recv_buffer[msg_len] = '\0';
                    char prefix[64];
                    char *line;
                    while (extract_message(&buffer[fd], &line)) {
                        sprintf(prefix, "client %d: ", ids[fd]);
                        broadcast(prefix, fd);
                        broadcast(line, fd);
                        free(line);
                    }
                }
            }
        }
    }
}