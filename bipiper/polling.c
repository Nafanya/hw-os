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
#include <errno.h>
#include <signal.h>
#include <poll.h>

#include <stdio.h>

#include "helpers.h"
#include "bufio.h"

#define PERR(msg) { perror(msg); exit(EXIT_FAILURE); }
#define EXT(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

#define NT 0
#define RD 1
#define WR 2
#define RW 3

typedef struct pollfd pollfd;

const int MAX_PAIRS = 127;
const int N = 2 + 127 * 2;
const int BUF_SIZE = 4096;

int create_bind_sock(char* port) {
  struct addrinfo hints;
  struct addrinfo *addresses, *rp;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int sfd, s;
  s = getaddrinfo("0.0.0.0", port, &hints, &addresses);
  if (s == -1) EXT("getaddrinfo: %s\n", gai_strerror(s));
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

int main(int argc, char* argv[]) {
  if (argc != 3) EXT("polling PORT1 PORT2\n");
  int x;
  if (sscanf(argv[1], "%d", &x) != 1 || x < 0 || x > 65535) EXT("bad port");
  if (sscanf(argv[2], "%d", &x) != 1 || x < 0 || x > 65535) EXT("bad port");

  int srv[2] = {
    create_bind_sock(argv[1]),
    create_bind_sock(argv[2])
  };
  if (srv[0] == -1) PERR("server 1");
  if (srv[1] == -1) PERR("server 2");

  if (listen(srv[0], 128) == -1) PERR("listen 1");
  if (listen(srv[1], 128) == -1) PERR("listen 2");

  buf_t *bufs[MAX_PAIRS][2];
  for (int i = 0; i < MAX_PAIRS; i++) {
    for (int j = 0; j < 2; j++) {
      bufs[i][j] = buf_new(BUF_SIZE);
      if (bufs[i][j] == NULL) EXT("buf malloc");
    }
  }

  pollfd fds[N];
  int nfds = 2;
  memset(fds, 0, sizeof(pollfd) * N);
  fds[0].fd = srv[0];
  fds[1].fd = srv[1];
  fds[0].events = POLLIN;
  fds[1].events = 0;

  int flags[N];
  memset(flags, 0, sizeof(int) * N);
  int state = 0, waiting;

  for (;;) {
    int polls = poll(fds, nfds, -1);
    printf("new polls: %d\n", polls);
    if (polls == -1) {
      if (errno != EINTR) {PERR("poll");}
      else continue;
    }
    int oldnfds = nfds;
    if (state == 0 && (fds[0].revents & POLLIN)) {
      if (nfds + 2 <= 2*MAX_PAIRS) {
        printf("accept on 1st\n");
        int fd = accept(srv[0], NULL, NULL);
        fds[nfds].fd = fd;
        fds[nfds].events = POLLIN;
        flags[nfds] = 0;
        bufs[nfds/2][0]->size = bufs[nfds/2][1]->size = 0;
        nfds++;
        fds[0].events = 0;
        fds[1].events = POLLIN;
        state = 1;
      } else {
        fds[0].events = 0;
      }
    }
    if (state == 1 && (fds[1].revents & POLLIN)) {
      if (nfds + 1 <= 2*MAX_PAIRS) {
        printf("accept on 2nd\n");
        int fd = accept(srv[1], NULL, NULL);
        fds[nfds].fd = fd;
        fds[nfds].events = POLLIN;
        flags[nfds] = 0;
        nfds++;
        fds[1].events = 0;
        fds[0].events = POLLIN;
        state = 0;
      } else {
        fds[1].events = 0;
      }
    }
    for (int i = 2; i < oldnfds; i++) {
      int ind = i/2-1, dir = i % 2;
      if (fds[i].revents & POLLIN) {
        printf("POLLIN fds[%2d]\n", i);
        if (buf_fill_once(fds[i].fd, bufs[ind][dir]) <= 0) {
          shutdown(fds[i].fd, SHUT_RD);
          flags[i] |= RD;
          flags[i ^ 1] |= WR;
        }
      }
      if (fds[i].revents & POLLOUT) {
        printf("POLLOUT fds[%2d]\n", i);
        if (buf_flush(fds[i].fd, bufs[ind][dir ^ 1], buf_size(bufs[ind][dir ^ 1])) <= 0) {
          shutdown(fds[i].fd, SHUT_WR);
          flags[i] |= WR;
          flags[i ^ 1] |= RD;
        }
      }
      if (fds[i].revents & POLLHUP) {
        printf("POLLHUP fds[%2d]\n", i);
        //fds[i].events = 0;
        close(fds[i].fd);
        close(fds[i ^ 1].fd);
        fds[i] = fds[nfds - 2];
        fds[i ^ 1] = fds[nfds - 1];
        flags[i] = flags[nfds - 2];
        flags[i ^ 1] = flags[nfds - 1];
        buf_t *bf = bufs[(i-2)/2][0]; bufs[(i-2)/2][0] = bufs[(nfds-3)/2][0]; bufs[(nfds-3)/2][0] = bf;
               bf = bufs[(i-2)/2][1]; bufs[(i-2)/2][1] = bufs[(nfds-3)/2][1]; bufs[(nfds-3)/2][1] = bf;
        nfds -= 2;
      }
    }
    for (int i = oldnfds/2-1; i >= 0; i--) {
      fds[2*i+2].events = fds[2*i+3].events = 0;
      if (buf_size(bufs[i][0]) < buf_capacity(bufs[i][0]) && (flags[2*i+2] & RD) == 0) fds[2*i+2].events |= POLLIN;
      if (buf_size(bufs[i][0])                            && (flags[2*i+3] & WR) == 0) fds[2*i+3].events |= POLLOUT;

      if (buf_size(bufs[i][1]) < buf_capacity(bufs[i][1]) && (flags[2*i+3] & RD) == 0) fds[2*i+3].events |= POLLIN;
      if (buf_size(bufs[i][1])                            && (flags[2*i+2] & WR) == 0) fds[2*i+2].events |= POLLOUT;
    }
    for (int i = 2; i < nfds; i += 2) {
      if (flags[i] == RW || flags[i + 1] == RW || (flags[i] == flags[i + 1] && (flags[i] == RD || flags[i] == WR))) {
        close(fds[i].fd);
        close(fds[i + 1].fd);
        fds[i] = fds[nfds - 2];
        fds[i + 1] = fds[nfds - 1];
        flags[i] = flags[nfds - 2];
        flags[i + 1] = flags[nfds - 1];
        buf_t *bf = bufs[(i-2)/2][0]; bufs[(i-2)/2][0] = bufs[(nfds-3)/2][0]; bufs[(nfds-3)/2][0] = bf;
               bf = bufs[(i-2)/2][1]; bufs[(i-2)/2][1] = bufs[(nfds-3)/2][1]; bufs[(nfds-3)/2][1] = bf;
        nfds -= 2;
        fds[state].events = POLLIN;
        fds[state ^ 1].events = 0;
      }
    }
  }

  return EXIT_SUCCESS;
}
