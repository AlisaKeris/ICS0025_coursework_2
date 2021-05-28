#pragma once

#include <windows.h> 
#include <string.h>

#define BUFSIZE 512
#define ICS0025_PIPENAME R"(\\.\pipe\ICS0025)"

class PipeClient {
private:
	HANDLE hPipe;
public:
	int connectToNamedPipe(void);
	std::string getItemFromPipe();
	void disconnect(void);
	void stop();
};