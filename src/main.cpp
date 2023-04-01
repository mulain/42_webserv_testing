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




#include <cstring>

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
		const char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nhello, u lil bish";
		write(newConnFd, msg , strlen(msg));
		printf("------------------Hello message sent-------------------\n");

	}
}