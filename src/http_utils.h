#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#define TRUE 1
#define FALSE 0
#define MAX_CONNECTIONS 3

char *wwwroot;
char *mime_file;

void init(int run_daemon, int *port, char **conf_file, char **log_file);
void handle(int connecting_socket);
int getRequestType(char *input);
int handleHttpGET(char *input, int connecting_socket);
int sendString(char *message, int socket);

#endif // HTTP_UTILS_H
