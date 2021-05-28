#include <iostream>
#include <assert.h>
#include <fstream>

#include "Data.h"
#include "ServerUtils.h"
#include "PipeClient.h"
#include <thread>
#include <future>
#include <conio.h>

#define AUTO_TEST true

/*
  Switch on the server -> launch the client -> connect -> let to work a bit -> stop -> connect -> let to work a bit -> exit
*/
void testCase1() {
	std::cout << std::endl << "** Test Case 1 **" << std::endl;

	// setup
	Data data = Data();
	PipeClient client = PipeClient(&data);

	// read test case commands from file
	std::ifstream infile("TestCase1.txt");
	std::string command;
	while (infile >> command)
	{
		client.performOperation(command);
	}

	//assert(data.CountItems() == 8); // 8 should have been created

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> exit
*/
void testCase2() {
	std::cout << std::endl << "** Test Case 2 **" << std::endl;

	// setup
	Data data = Data();
	PipeClient client = PipeClient(&data);

	// read test case commands from file
	std::ifstream infile("TestCase2.txt");
	std::string command;
	while (infile >> command)
	{
		client.performOperation(command);
	}

	assert(data.CountItems() == 0); // no data should be created

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> connect -> let to work a bit -> switch off the server -> connect(error) -> exit
*/
void testCase3() {
	std::cout << std::endl << "** Test Case 3 **" << std::endl;

	// setup
	Data data = Data();
	PipeClient client = PipeClient(&data);

	// read test case commands from file
	std::ifstream infile("TestCase3.txt");
	std::string command;
	while (infile >> command)
	{
		client.performOperation(command);
	}

	//assert(data.CountItems() == 4); // 4 should have been created

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

/*
  Switch on the server -> launch the client -> connect(error?) -> connect -> let to work a bit -> connect(error)
  -> let to work a bit -> stop -> stop(error) -> exit
*/
void testCase4() {
	std::cout << std::endl << "** Test Case 4 **" << std::endl;

	assert(false);

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

static std::string getAnswer()
{
	std::string answer;
	std::cin >> answer;
	return answer;
}

int main()
{
	if (AUTO_TEST) {
		testCase1();
		testCase2();
		testCase3();
		//testCase4();
	}
	else {
		Data data = Data();
		PipeClient client = PipeClient(&data, false);
		std::string command;
		while (true) {
			std::cout << std::endl << "Type a command >> ";
			std::cin >> command;
			std::async(std::launch::async, [&client, command] {client.performOperation(command); });

			if (command.compare("exit")==0) {
				std::cout << std::endl << "Press any key to close...";
				_getch();
				PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
			}
		} 
	}

}
