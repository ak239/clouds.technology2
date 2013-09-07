#include "stdafx.h"
#include "Utility.h"

#include <exception> 

void debugPrint(bool condition, char* text, int code)
{
	if (condition)
		fprintf(stderr, "Info: %s, with code %d\r\n", text, code);
}

void debugPrint(bool condition, char* text)
{
	if (condition)
		fprintf(stderr, "Info: %s\r\n", text);
}

void fatalPrint(bool condition, char* text, int code)
{
	if (condition)
	{
		fprintf(stderr, "Error: %s, with code %d\r\n", text, code);
		std::terminate();
	}
}

void fatalPrint(bool condition, char* text)
{
	if (condition)
	{
		fprintf(stderr, "Error: %s\r\n", text);
		std::terminate();
	}
}
