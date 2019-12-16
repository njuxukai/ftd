#include <iostream>
#include "MdPublisher.h"
int main(int argc, char** argv)
{
	std::string file = "";
	if (argc < 2)
	{
		file = "sample_publisher.cfg";
	}
	else
	{
		file = argv[1];
	}
	MdPublisher publisher(file);
	publisher.start();

	while (1 == 1)
	{
		char c = getchar();
	}
}