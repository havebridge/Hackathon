#include "server.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	Server server("127.0.0.1", 8888);

	server.Init();

	return 0;
}