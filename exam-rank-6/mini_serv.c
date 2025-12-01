# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/select.h>

int setup_listener(int port);
void accept_connection(int listener);
void disconnect(int connection);
void broadcast(char *msg, int author);
void fatal();
int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);

fd_set active_fds;
fd_set read_fds;
fd_set write_fds;
int maxfd = 0;
int ids[65535];
char *buffers[65535];

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(2, "Wrong number of arguments\n", 26);
        return 1;
    }

    FD_ZERO(&active_fds);
    int port = atoi(argv[1]);
    if (port < 0 || port > 65535)
        fatal();
    int listener = setup_listener(port);

    while (1)
    {
        write_fds = read_fds = active_fds;

        int ready = select(maxfd + 1, &read_fds, &write_fds, NULL, NULL);
        if (ready == -1)
            fatal();
        if (ready == 0)
            continue;

        for (int fd = 0; fd <= maxfd; ++fd)
        {
            if (!FD_ISSET(fd, &read_fds))
                continue;

            if (fd == listener)
                accept_connection(listener);
            else
            {
                char recv_buf[1024];
                ssize_t msg_len = recv(fd, recv_buf, 1023, 0);
                if (msg_len <= 0)
                {
                    disconnect(fd);
                    break;
                }
                else
                {
                    recv_buf[msg_len] = '\0';
                    buffers[fd] = str_join(buffers[fd], recv_buf);
                    char prefix[64];
                    char *line;
                    while (extract_message(&buffers[fd], &line))
                    {
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

void broadcast(char *msg, int author)
{
    for (int fd = 0; fd <= maxfd; ++fd)
        if (FD_ISSET(fd, &write_fds) && fd != author)
            send(fd, msg, strlen(msg), 0);
}

void disconnect(int connection)
{
    char msg[64];
    sprintf(msg, "server: client %d just left\n", ids[connection]);
    broadcast(msg, connection);

    FD_CLR(connection, &active_fds);
    close(connection);
    free(buffers[connection]);

    if (connection == maxfd)
        while (maxfd >= 0 && !FD_ISSET(maxfd, &active_fds))
            --maxfd;
}

void accept_connection(int listener)
{
    static int client_counter = 0;

    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    int connection = accept(listener, (struct sockaddr *)&addr, &size);
    if (connection == -1)
        return;

    FD_SET(connection, &active_fds);
    ids[connection] = client_counter++;
    buffers[connection] = NULL;

    if (connection > maxfd)
        maxfd = connection;

    char msg[64];
    sprintf(msg, "server: client %d just arrived\n", ids[connection]);
    broadcast(msg, connection);
}

int setup_listener(int port)
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1)
        fatal();

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = htons(port);
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        fatal();

    if (listen(listener, SOMAXCONN) == -1)
        fatal();

    FD_SET(listener, &active_fds);

    if (listener > maxfd)
        maxfd = listener;

    return listener;
}

void fatal()
{
    write(2, "Fatal error\n", 12);
    exit(1);
}

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
