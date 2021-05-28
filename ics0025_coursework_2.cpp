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
		PipeClient client = PipeClient(&data, true, true);
		std::string command;
		std::thread cl;

		std::cout << std::endl << "Type a command >> ";
		while (true) {
			std::cin >> command;
			cl = std::thread([&client, command] {client.performOperation(command); });
			cl.detach();
		}
		cl.join();

	}

}

