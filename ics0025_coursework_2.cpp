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

	// start server
	ServerUtils::startServer();

	Data data = Data();
	data.InsertItem('A', 0, "itemA0", Date(1, 1, 2021));
	
	// connect
	PipeClient client = PipeClient();
	client.connectToNamedPipe();
	
	// work
	std::string itemString;
	for (int i = 0; i < 4; i++) {
		// send "ready" and receive
		itemString = client.getItemFromPipe();
		// TODO: insert newly received item
		data.InsertItem(itemString);
	} 
	// assert(data.CountItems() == 5);

	// TODO: stop
	/*client.stop();

	// TODO: reconnect
	client.connectToNamedPipe();

	// work
	for (int i = 0; i < 4; i++) {
		// send "ready" and receive
		itemString = client.getItemFromPipe();
		// TODO: insert newly received item
		data.InsertItem(itemString);
	}
	// assert(data.CountItems() == 9);

	*/
	client.disconnect();
	ServerUtils::killServer();
	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> exit
*/
void testCase2() {
	std::cout << std::endl << "** Test Case 2 **" << std::endl; 
	
	PipeClient client = PipeClient();
	client.connectToNamedPipe(); // should indicate pipe could not be opened

	std::cout << std::endl << "** PASS **" << std::endl << std::endl;
}

/*
  Launch the client -> connect -> let to work a bit -> switch off the server -> connect(error) -> exit
*/
void testCase3() {
	std::cout << std::endl << "** Test Case 3 **" << std::endl;

	Data data = Data();

	// start server
	ServerUtils::startServer();
	
	// connect
	PipeClient client = PipeClient();
	client.connectToNamedPipe();

	// work
	std::string itemString;
	for (int i = 0; i < 4; i++) {
		itemString = client.getItemFromPipe();
		data.InsertItem(itemString);
	}
	// assert(data.CountItems() == 4);
	
	// switch off server
	ServerUtils::killServer();

	client.connectToNamedPipe(); // should indicate error

	std::cout << std::endl<< "** PASS **" << std::endl << std::endl;
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

int main()
{
	testCase1();
	//testCase2();
	//testCase3();
	//testCase4();
}
