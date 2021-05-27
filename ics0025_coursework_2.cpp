#include <iostream>
#include <assert.h>

#include "Data.h"
#include "ServerUtils.h"
#include "PipeClient.h"

/*
  Switch on the server -> launch the client -> connect -> let to work a bit -> stop -> connect -> let to work a bit -> exit
*/
void testCase1() {
	std::cout << std::endl << "** Test Case 1 **" << std::endl;
	ServerUtils::startServer();

	PipeClient client = PipeClient();
	client.connectToNamedPipe();

	ServerUtils::killServer();
	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> exit
*/
void testCase2() {
	std::cout << std::endl << "** Test Case 2 **" << std::endl; 
	
	assert(false);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> connect -> let to work a bit -> switch off the server -> connect(error) -> exit
*/
void testCase3() {
	std::cout << std::endl << "** Test Case 3 **" << std::endl;
	
	assert(false);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
  Switch on the server -> launch the client -> connect(error?) -> connect -> let to work a bit -> connect(error) 
  -> let to work a bit -> stop -> stop(error) -> exit
*/
void testCase4() {
	std::cout << std::endl << "** Test Case 4 **" << std::endl;

	assert(false);

	std::cout << "** PASS **" << std::endl << std::endl;
}

int main()
{
	testCase1();
	//testCase2();
	//testCase3();
	//testCase4();
}
