#pragma once

#include <windows.h> 
#include "Data.h"

#define BUFSIZE 512
#define ICS0025_PIPENAME R"(\\.\pipe\ICS0025)"

class PipeClient {
private:
	HANDLE hPipe;
	Data* dataRef;
	bool stoppedGracefully;
	bool serverStarted;
	bool verbose;
public:
	PipeClient(Data* data);
	PipeClient(Data* data, bool verbose);
	void performOperation (std::string command);
	int connectToNamedPipe(void);
	void getItemFromPipe();
	void disconnect(void);
	void stop();
};