#pragma once

#define BUFSIZE 512
#define ICS0025_PIPENAME R"(\\.\pipe\ICS0025)"

class PipeClient {
public:
	int connectToNamedPipe(void);
};