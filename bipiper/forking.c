#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>

#include <stdio.h>

#include "helpers.h"
#include "bufio.h"

#define CHK(_cond, ...)                             \
  if (!(_cond)) {                                   \
    perror(NULL);                                   \
    fprintf(stderr, __VA_ARGS__);                   \
    exit(EXIT_FAILURE);                             \
  }

int create_bind_sock(char* port) {
  struct addrinfo hints;
  struct addrinfo *addresses, *rp;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int sfd, s;
  s = getaddrinfo("localhost", port, &hints, &addresses);
  CHK(s != 1, "getaddrinfo: %s\n", gai_strerror(s));
  for (rp = addresses; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) {
      continue;
    }
    int one = 1;
    s = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    if (s == -1) {
      continue;
    }
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
      break;
    }
    close(sfd);
  }
  if (rp == NULL) {
    return -1;
  }
  freeaddrinfo(addresses);
  return sfd;
}

void connect_clients(int from, int to) {
  static const int BUF_SIZE = 4096;
  buf_t* buf = buf_new(BUF_SIZE);
  CHK(buf != NULL, "malloc failed");
  for (;;) {
    ssize_t rd = buf_fill(from, buf, 1);
    if (rd == -1 || rd == 0) {
      break;
    }
    ssize_t wr = buf_flush(to, buf, buf_size(buf));
    if (wr == -1) {
      break;
    }
  }
  buf_free(buf);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "forking PORT1 PORT2\n");
    return EXIT_FAILURE;
  }
  int sock1 = create_bind_sock(argv[1]);
  CHK(sock1 != -1, "Can't create socket");
  CHK(listen(sock1, 128) != -1, "Can't listen socket");

  int sock2 = create_bind_sock(argv[2]);
  CHK(sock2 != -1, "Can't create socket");
  CHK(listen(sock2, 128) != -1, "Can't listen socket");

  for (;;) {
    int client1 = accept(sock1, NULL, NULL);
    CHK(client1 != -1, "Can't accept 1st client");
    int client2 = accept(sock2, NULL, NULL);
    CHK(client2 != -1, "Can't accept 2nd client");

    pid_t pid1 = fork();
    if (pid1 == -1) {
      return EXIT_FAILURE;
    } else if (pid1 == 0) {
      close(sock1);
      close(sock2);
      connect_clients(client1, client2);
      exit(EXIT_SUCCESS);
    }
    pid_t pid2 = fork();
    if (pid2 == -1) {
      kill(pid1, SIGKILL);
      exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
      close(sock1);
      close(sock2);
      connect_clients(client2, client1);
      exit(EXIT_SUCCESS);
    } else {
      close(client1);
      close(client2);
    }
  }
  return EXIT_SUCCESS;
}
