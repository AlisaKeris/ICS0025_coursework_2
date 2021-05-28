#include <iostream>

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <future>
#include <cstdlib>

#include <thread>

#include "PipeClient.h"
#include "ServerUtils.h"

PipeClient::PipeClient(Data* data)
{
	verbose = true;
	hPipe = nullptr;
	stoppedGracefully = true;
	dataRef = data;
}


PipeClient::PipeClient(Data* data, bool verbose)
{
	this->verbose = verbose;
	hPipe = nullptr;
	stoppedGracefully = true;
	dataRef = data;
}

void PipeClient::performOperation(std::string command)
{
	std::cout << "Main thread received command: " << command << std::endl;
	if (command.compare("connect")==0) {
		connectToNamedPipe();
	}
	else if (command.compare("stop") == 0) {
		stop();
	} else if (command.compare("exit") == 0) {
		disconnect();
	} else if (command.compare("serveroff") == 0) {
		ServerUtils::killServer();
	}
	else if (command.compare("serveron") == 0) {
		ServerUtils::startServer();
	}
	else if (command.compare("ready") == 0) {
		for (int i = 0; i < 4; i++) {
			// send "ready" and insert newly received item
			getItemFromPipe();
		}
	}
	else {
		std::cout << "Unknown command" << std::endl;
	}
}

int PipeClient::connectToNamedPipe() {

	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPCSTR  lpszPipename = ICS0025_PIPENAME;

	printf("\nCONNECTING TO PIPE\n");

	if (stoppedGracefully) {
		ServerUtils::startServer();
	}

	// Try to open a named pipe; wait for it, if necessary. 
	while (1)
	{
		hPipe = CreateFileA(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			FILE_SHARE_WRITE|FILE_SHARE_READ,   // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			FILE_FLAG_OVERLAPPED,     // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE) {
			stoppedGracefully = false;
			break;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 
		if (!WaitNamedPipeA(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		} 
	}

	return 0;
}

void PipeClient::disconnect(void) {
	CloseHandle(hPipe);
	stoppedGracefully = true;
	ServerUtils::killServer();
	printf("DISCONNECTING FROM PIPE\n\nData Structure contents are:\n\n");
	dataRef->PrintAll();
}

void PipeClient::stop() {
	LPCSTR  lpvMessage = "stop";
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	std::string itemString = "";

	// Send stop message
	cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
	if (verbose)
		printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("Error trying to stop - WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return;
	}  

	printf("\nStop message sent to server, closing pipe.\n");
	disconnect();
}

void PipeClient::getItemFromPipe() {
	
	BOOL   fSuccess = FALSE;
	DWORD  dwMode;
	LPCSTR  lpszPipename = ICS0025_PIPENAME;
	std::string itemString = "";

	// The pipe connected; change to message-read mode. 
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return;
	}

	// TODO: separate thread
	std::thread writeThread([this]() {
		// Send a message to the pipe server. 
		LPCSTR  lpvMessage = "ready";
		BOOL fSuccess;
		DWORD cbWritten;
		DWORD cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
		//printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

		fSuccess = WriteFile(
			hPipe,                  // pipe handle 
			lpvMessage,             // message 
			cbToWrite,              // message length 
			&cbWritten,             // bytes written 
			NULL);                  // not overlapped 

		if (!fSuccess)
		{
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
			return;
		}
	});
	writeThread.join();

	// TODO: separate thread
	std::thread readThread([this]() {
		BOOL fSuccess;
		DWORD cbRead;
		TCHAR  chBuf[BUFSIZE];

		if(verbose)
			printf("\nMessage sent to server, receiving reply as follows:\n");

		fSuccess = ReadFile(
			hPipe,    // pipe handle 
			chBuf,    // buffer to receive reply 
			BUFSIZE * sizeof(TCHAR),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped  

		if (verbose && fSuccess) { printf("\"%s\"\n", chBuf); }
		
	});
	readThread.join();

	// TODO: join
	dataRef->InsertItem(itemString);
	return;
}
