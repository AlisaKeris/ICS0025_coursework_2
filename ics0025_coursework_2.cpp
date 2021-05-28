#include <iostream>
#include <assert.h>
#include <fstream>

#include "Data.h"
#include "ServerUtils.h"
#include "PipeClient.h"
#include <thread>
#include <future>
#include <conio.h>

#define AUTOMATED_TESTS true

void runTestCase(std::string filename) {
	std::cout << std::endl << "** Running: " << filename << " **" << std::endl;

	// setup
	Data data = Data();
	PipeClient client = PipeClient(&data);

	// read test case commands from file
	std::ifstream infile(filename);
	std::string command;
	while (infile >> command)
	{
		client.performOperation(command);
	}

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
} 
  
int main()
{
	if (AUTOMATED_TESTS) {

		// Switch on the server -> launch the client -> connect -> let to work a bit -> stop -> connect -> let to work a bit -> exit
		runTestCase("TestCase1.txt");
		
		// Launch the client -> exit
		runTestCase("TestCase2.txt");
		
		// Launch the client -> connect -> let to work a bit -> switch off the server -> connect(error) -> exit
		runTestCase("TestCase3.txt");
		
		// Switch on the server -> launch the client -> connect(error?) -> connect -> let to work a bit -> connect(error)
		//	-> let to work a bit -> stop -> stop(error) -> exit
		runTestCase("TestCase4.txt");
	
	} else {
		// Run normally as console app

		Data data = Data();
		PipeClient client = PipeClient(&data, false);
		std::string command;
		while (true) {
			std::cout << std::endl << "Type a command >> ";
			std::cin >> command;
			std::async(std::launch::async, [&client, command] {client.performOperation(command); });

			if (command.compare("exit") == 0) {
				std::cout << std::endl << "Press any key to close...";
				_getch();
				PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
			}
		}

	}

}


// TODO: Reading from and writing to server must be asynchronous
// TODO: thread for listening to keyboard, non-blocking console input
// client must obey exit at any moment??

