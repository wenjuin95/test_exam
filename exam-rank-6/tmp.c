#include <sys/socket.h>
#include <sys/select.h> // select, fd_set
#include <netinet/in.h>
#include <stdlib.h> // calloc, malloc, free, exit
#include <stdio.h> // sprintf
#include <unistd.h>
#include <string.h>

void del_client(int client);
int add_client(int listener);
void usr_msg(int author);
void broadcast(char *msg, int author);
int setup_listener(int port);
void fatal();
int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);

fd_set active_fds;
fd_set read_fds;
fd_set write_fds;
int maxfd = 0;
int ids[65535];
char *client_buffers[65535];

int main(int ac, char **av)
{
    if (ac != 2)
    {
        write(STDERR_FILENO, "Wrong number of arguments\n", 26);
        exit(1);
    }

    FD_ZERO(&active_fds);
    int listener = setup_listener(atoi(av[1]));

    while (1)
    {
        read_fds = write_fds = active_fds;

        int ready = select(maxfd + 1, &read_fds, &write_fds, 0, 0);
        if (ready < 0)
            fatal();
        if (ready == 0)
            continue;

        for (int fd = 0; fd <= maxfd; fd++)
        {
            if (!FD_ISSET(fd, &read_fds))
                continue;

            if (fd == listener)
            {
                if (add_client(listener) == -1)
                break;
            }
            else
            {
                char receive_buffer[1001];
                int msg_len = recv(fd, receive_buffer, 1000, 0);
                if (msg_len <= 0)
                {
                    del_client(fd);
                    break;
                }
                else
                {
                    receive_buffer[msg_len] = 0;
                    client_buffers[fd] = str_join(client_buffers[fd], receive_buffer);
                    usr_msg(fd);
                }
            }
        }
    }
}

void usr_msg(int author)
{
    char msg[64];
    char *line;

    while(extract_message(&(client_buffers[author]), &line))
    {
        sprintf(msg, "client %d: ", ids[author]);
        broadcast(msg, author);
        broadcast(line, author);
        free(line);
    }
}

void broadcast(char *msg, int author)
{
    for (int fd = 0; fd <= maxfd; fd++)
        if (FD_ISSET(fd, &write_fds) && fd != author)
            send(fd, msg, strlen(msg), 0);
}

int add_client(int listener)
{
    static int client_counter = 0;

    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    int client = accept(listener, (struct sockaddr *)&addr, &size);
    if (client == -1)
        return -1;

    FD_SET(client, &active_fds);
    if (client > maxfd)
        maxfd = client;

    ids[client] = client_counter++;
    client_buffers[client] = 0;

    char msg[64];
    sprintf(msg, "server: client %d just arrived\n", ids[client]);
    broadcast(msg, client);

    return client;
}

void del_client(int client)
{
    char msg[64];
    sprintf(msg, "server: client %d just left\n", ids[client]);
    broadcast(msg, client);

    free(client_buffers[client]);
    FD_CLR(client, &active_fds);
    close(client);

    if (client == maxfd)
        while (maxfd >= 0 && !FD_ISSET(maxfd, &active_fds))
            --maxfd;
}

int setup_listener(int port)
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
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
    write(STDERR_FILENO, "Fatal error\n", 12);
    exit(1);
}

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
