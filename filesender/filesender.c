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

int send_file(int from, int to) {
  static const int BUF_SIZE = 4096;
  buf_t* buf = buf_new(BUF_SIZE);
  if (buf == NULL) return -1;
  for (;;) {
    ssize_t rd = buf_fill(from, buf, BUF_SIZE);
    if (rd == -1) {
      buf_free(buf);
      return -1;
    } else if (rd == 0) {
      buf_free(buf);
      return 0;
    }
    ssize_t wr = buf_flush(to, buf, rd);
    if (wr == -1) {
      return -1;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "filesender PORT FILENAME\n");
    return EXIT_FAILURE;
  }
  int sock = create_bind_sock(argv[1]);
  CHK(sock != -1, "Can't create socket");
  CHK(listen(sock, 128) != -1, "Can't listen socket");

  for (;;) {
    struct sockaddr client;
    socklen_t sz = sizeof(client);
    int fd = accept(sock, &client, &sz);
    CHK(fd != -1, "Can't accept new connection");

    pid_t pid = fork();
    if (pid == -1) {
      close(fd);
      return EXIT_FAILURE;
    } else if (pid == 0) {
      close(sock);
      int file = open(argv[2], O_RDONLY);
      send_file(file, fd);
      close(file);
      close(fd);
      return EXIT_SUCCESS;
    } else {
      close(fd);
    }
  }
  return EXIT_SUCCESS;
}
