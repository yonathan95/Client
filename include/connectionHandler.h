#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <map>

using boost::asio::ip::tcp;

class ConnectionHandler {
private:
	const std::string host_;
	const short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	short sendingOpCode;
    short gettingOpCode;
    short opMessage;
    std::map <std::string, short> opMap;
 
public:
    ConnectionHandler(std::string host, short port, std::map<std::string,short> &map);

    virtual ~ConnectionHandler();
 
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is cesToRead);
 
	// Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);

    bool getBytes(char bytes[], unsigned int bytesToRead);
	
    // Read an ascii line from the server
    // Returns false in case connection closed before a newline can be read.
    bool getLine(std::string& line);
	
	// Send an ascii line from the server
    // Returns false in case connection closed before all the data is sent.
    bool sendLine(std::string& line);
 
    // Get Ascii data from the server until the delimiter character
    // Returns false in case connection closed before null can be read.
    bool getFrameAscii(std::string& frame);
 
    // Send a message to the remote host.
    // Returns false in case connection is closed before all the data is sent.
    bool sendFrameAscii(const std::string& frame);
	
    // Close down the connection properly.
    void close();

    void prepareLine(std::string &line);

    void shortToBytes(short num, char* bytesArr);

    void shortToBytesTwo(short num, char* bytesArr);

    short bytesToShort(char* bytesArr);

    short getOpMessage();

    short getGettingOpCode();


}; //class ConnectionHandler
 
#endif