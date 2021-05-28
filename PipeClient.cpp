#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <future>
#include <cstdlib>  

#include "PipeClient.h"

int PipeClient::connectToNamedPipe() {

	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPCSTR  lpszPipename = ICS0025_PIPENAME;


	// Try to open a named pipe; wait for it, if necessary. 
	while (1)
	{
		hPipe = CreateFileA(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE) {
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
}

void PipeClient::stop() {
	LPCSTR  lpvMessage = "stop";
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
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
	}

	// Send a message to the pipe server. 
	cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
	printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
	}

	printf("\nStop message sent to server, closing pipe.\n");
	disconnect(); 

}

std::string PipeClient::getItemFromPipe() {
	LPCSTR  lpvMessage = "ready";
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
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
		return itemString;
	}

	// Send a message to the pipe server. 
	cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
	printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return itemString;
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");

	fSuccess = ReadFile(
		hPipe,    // pipe handle 
		chBuf,    // buffer to receive reply 
		BUFSIZE * sizeof(TCHAR),  // size of buffer 
		&cbRead,  // number of bytes read 
		NULL);    // not overlapped  

	if (fSuccess) { printf("\"%s\"\n", chBuf); }

	// copy chBuf into itemString ??

	return itemString;
}
