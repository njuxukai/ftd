#include "TestDB.h"
#include "DBWrapper.h"




int main(int argc, char* argv[])
{
	
	McoDBWrapper wrapper;
	while (char c = getchar())
	{
		if (c == '1')
		{
			testUserLogin(&wrapper);
		}
		if (c == '2')
		{
		}
	}
}
