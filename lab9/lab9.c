// client.c

/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)
1. What is the address of the server it is trying to connect to (IP address and
port number).
  - IP address: 127.0.0.1
  - Port number: 8000
2. Is it UDP or TCP? How do you know?
  - This is a TCP because at line 38, when declaring the type in socket it is
SOCK_STREAM. Indicating that it is a TCP.
3. The client is going to send some data to the server. Where does it get this
data from? How can you tell in the code?
  - It gets its data from the read() in STDIN_FILENO at line 58.
4. How does the client program end? How can you tell that in the code?
  - When the client presses Enter on a blank line at the terminal making
num_read = 1, closes the socker with close(sfd) and exits cleanly with
exit(EXIT_SUCCESS).
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}

// server.c
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // TODO: print the message received from client
  // TODO: increase total_message_count per message

  int client_fd;
  int id;

  client_fd = client->cfd;
  id = client->client_id;

  // printf("client fd: %d\n", client_fd);
  // printf("client id: %d\n", id);

  char buf[BUF_SIZE];
  char out[BUF_SIZE + 64];
  ssize_t num_read;
  int len;

  len = snprintf(out, sizeof(out),
                 "New client created! ID %d, on socket FD %d\n", id, client_fd);
  write(STDOUT_FILENO, out, len);

  while ((num_read = read(client_fd, buf, BUF_SIZE)) > 0) {

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    int msg_num = total_message_count;
    pthread_mutex_unlock(&count_mutex);

    len = snprintf(out, sizeof(out), "MSG # %3d; Client ID %d: %.*s", msg_num,
                   id, (int)num_read, buf);
    write(STDOUT_FILENO, out, len);
  }

  len = snprintf(out, sizeof(out), "Ending thread for client %d\n", id);
  write(STDOUT_FILENO, out, len);

  close(client_fd);
  free(client);

  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
    int client_fd = accept(sfd, NULL, NULL);
    if (client_fd == -1) {
      handle_error("accept failed");
    }
    pthread_mutex_lock(&client_id_mutex);
    int id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    struct client_info *client = malloc(sizeof(struct client_info));
    client->cfd = client_fd;
    client->client_id = id;

    pthread_t tid;
    if (pthread_create(&tid, NULL, handle_client, client) != 0) {
      handle_error("pthread_create failed");
    }
    if (pthread_detach(tid) != 0) {
      handle_error("pthread_detach failed");
    }
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
