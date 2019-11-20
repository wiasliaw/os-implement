#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
  // create socket
  int sockfd = 0;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd<0) {
    printf("Fail to create socket");
  }

  // connect socket
  struct sockaddr_in info;
  bzero(&info, sizeof(info));
  info.sin_family = PF_INET;
  info.sin_addr.s_addr = inet_addr("127.0.0.1");
  info.sin_port = htons(8700);

  int err = connect(sockfd, (struct sockaddr*)&info, sizeof(info));
  if (err<0) {
    printf("Connect error");
  }

  //Send a message to server
  char message[] = {"Hi there"};
  char receiveMessage[100] = {};
  send(sockfd,message,sizeof(message),0);
  recv(sockfd,receiveMessage,sizeof(receiveMessage),0);

  printf("%s",receiveMessage);
  printf("close Socket\n");
  close(sockfd);

  return 0;
}
