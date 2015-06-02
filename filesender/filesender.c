#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <stdio.h>

#include "helpers.h"
#include "bufio.h"

#define CHK(_retcode, _msg)                     \
  if (_retcode == -1) {                         \
    perror(_msg);                               \
    return EXIT_FAILURE;                        \
  }

int send_file(int from, int to) {
  static const int BUF_SIZE = 4096;
  const char buf[BUF_SIZE];

  for (;;) {
    int rd = read_(from, (char*)buf, BUF_SIZE);
    if (rd == -1) {
      return -1;
    } else if (rd == 0) {
      return 0;
    } else {
      int cnt = 0;
      while (cnt < rd) {
        int wr = write_(to, (char*)(buf) + cnt, rd - cnt);
        if (wr == -1) {
          return -1;
        } else {
          cnt += wr;
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    write_(STDOUT_FILENO, "filesender PORT FILENAME\n", 25);
    return EXIT_FAILURE;
  }
  int port;
  if (sscanf(argv[1], "%d", &port) != 1) {
    write_(STDOUT_FILENO, "bad port\n", 9);
    return EXIT_FAILURE;
  }
  char* filename = argv[2];
  printf("port: %d\nfile: %s\n", port, filename);

  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  CHK(sock, "socket");

  const int one = 1;
  CHK(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)), "sockopt");
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr = {.s_addr = INADDR_ANY}
  };
  CHK(bind(sock, (struct sockaddr*)&addr, sizeof(addr)), "bind");
  CHK(listen(sock, 1), "listen");

  struct sockaddr_in client;
  socklen_t sz = sizeof(client);

  printf("Start loop\n");
  for (;;) {
    int fd = accept(sock, (struct sockaddr*)&client, &sz);
    printf("accept fd: %d\n", fd);
    printf("from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    CHK(fd, "accept");
    pid_t pid = fork();
    CHK(pid, "fork");
    if (pid == 0) {
      close(sock);
      sleep(5);
      int file = open(filename, O_RDONLY);
      CHK(file, "open file");
      CHK(send_file(file, fd), "send file");
      close(file);
      close(fd);
      return EXIT_SUCCESS;
    } else {
      close(fd);
    }
  }
  return EXIT_SUCCESS;
}
