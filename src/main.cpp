#include "../incl/Server.hpp"
#include "../incl/Socket.hpp"

/* int	main(int argc, char **argv)
{

	// PARSE CONFIG FILE

	// SET UP SERVER

	// PARSE INSTRUCTIONS

	// EXECUTE INSTRUCTIONS

	return (EXIT_SUCCESS);
}
 */

/* int main (void)
{
	
	std::string requestString("POST /api/users HTTP/1.1\r\nHost: www.example.com\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36\nContent-Type: application/json\nContent-Length: 44\nConnection: keep-alive\n\n{\"username}\": \"jdoe\", \"password\": \"password123\"}");
	std::string empty("");
	Server serverA;

	serverA.parseRequest(empty);
	serverA.printRequest();
	serverA.parseRequest(requestString);
	serverA.printRequest();
} */

#include <stdlib.h>
#include <cstring>

/* int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

	(void)argv;
	(void)argc;
    
    const char *hello = "Hello from server";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 3000 );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
		(void)valread;
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
} */

int main()
{
	Socket	new_socket;
	Server	servster;

	while (1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		int newConnFd = servster.acceptConnection();
		char buffer[30000] = {0};
		read(newConnFd, buffer, 30000);
		printf("%s\n",buffer );
		write(newConnFd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nhello" , strlen("hello"));
		printf("------------------Hello message sent-------------------\n");

	}
}