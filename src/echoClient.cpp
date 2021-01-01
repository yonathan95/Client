#include <stdlib.h>
#include <connectionHandler.h>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    std::map<std::string,short> opMap = std::map<std::string,short>();
    opMap["ADMINREG"] = 1;
    opMap["STUDENTREG"] = 2;
    opMap["LOGIN"] = 3;
    opMap["LOGOUT"] = 4;
    opMap["COURSEREG"] = 5;
    opMap["KDAMCHECK"] = 6;
    opMap["COURSESTAT"] = 7;
    opMap["STUDENTSTAT"] = 8;
    opMap["ISREGISTERED"] = 9;
    opMap["UNREGISTER"] = 10;
    opMap["MYCOURSES"] = 11;
    opMap["ACK"] = 12;
    opMap["ERR"] = 13;
    ConnectionHandler connectionHandler(host, port, opMap);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
	
	//From here we will see the rest of the ehco client implementation:
    while (1) {
        const short bufsize = 16384;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);//read from keyboard in to the buf
		std::string line(buf); // create a string call line
		connectionHandler.prepareLine(line);
        std::cout<<line<<std::endl;
        int len=line.length();
        if (!connectionHandler.sendLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
		// connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
        std::cout << "Sent bytes to server" << std::endl;

 
        // We can use one of three options to read data from the server:
        // 1. Read a fixed number of characters
        // 2. Read a line (up to the newline character using the getline() buffered reader
        // 3. Read up to the null character
        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        
		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        if (connectionHandler.getCode() == 12){
            std::cout << "ACK" <<" "<< connectionHandler.getCode() <<" " << answer << std::endl;
        }
        if (connectionHandler.getCode() == 13){
            std::cout << "ACK 3" << std::endl;
            break;
        }
    }
    return 0;
}
