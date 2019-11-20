#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
  // create socket
  char inputBuffer[256] = {};
  char message[] = {"Hi,this is server.\n"};
  int sockfd = 0, forClientfd = 0;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd<0) {
    printf("Fail to create socket");
  }

  // connect socket
  struct sockaddr_in serverinfo, clientinfo;
  int addrlen = sizeof(clientinfo);
  bzero(&serverinfo, sizeof(serverinfo));

  serverinfo.sin_family = PF_INET;
  serverinfo.sin_addr.s_addr = INADDR_ANY;
  serverinfo.sin_port = htons(8700);
  bind(sockfd, (struct sockaddr*)&serverinfo, sizeof(serverinfo));
  listen(sockfd, 5);

  // loop
  for(;;) {
    forClientfd = accept(sockfd, (struct sockaddr*)&clientinfo, &addrlen);
    send(forClientfd, message, sizeof(message), 0);
    recv(forClientfd, inputBuffer, sizeof(inputBuffer), 0);
    printf("Get %s\n",inputBuffer);
  }

  return 0;
}
