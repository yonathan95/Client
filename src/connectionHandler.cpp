#include <connectionHandler.h>
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port,std::map<std::string,short>): host_(host), port_(port), io_service_(), socket_(io_service_), sendingOpCode(0), gettingOpCode(0),opMap(map){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line);
}

bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line);
}
 

bool ConnectionHandler::getFrameAscii(std::string& frame) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    unsigned int counter = 0;
    char bytes[] opBytes = char bytes[2];
    char bytes[] messageBytes = char bytes[2];
    try {
	while(ch != '\0'){
		if(!getBytes(&ch, 1))
		{
			return false;
		}

		if(counter > 3 & ch != '\0'){
            frame.append(1, ch);
		}
		if (counter < 2){
            opBytes[counter] = ch;
		}
        if (counter > 1 & counter < 4){
            messageBytes[counter] = ch;
        }
        if (counter == 2){
            gettingOpCode = bytesToShort(opBytes);
            frame = frame + opMap[gettingOpCode];
        }
        if (counter == 4){
            messageOpCode = bytesToShort(messageBytes);
            frame = frame + " ";
            frame = frame + to_string(messageOpCode);
            if (gettingOpCode == 13) break;
        }
	}
    } catch (std::exception& e) {
	std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
	return false;
    }
    return true;
}
 
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame) {
    if (opCode == 1 | opCode == 2 | opCode == 3) {
        const char bytes[]
        arr = char
        bytes[frame.length() + 4];
        shortToBytes(opCode, &arr);
        unsigned int freeSlot = 2;
        for (char &c : frame) {
            if (c == " ") {
                arr[freeSlot] = '\0';
                freeSlot = freeSlot + 1;
            } else {
                arr[freeSlot] = c;
                freeSlot = freeSlot + 1;
            }
        }
        arr[freeSlot] = '\0';
        sendBytes(arr, frame.length() + 4);
    }
    else if (opCode == 4 | opCode == 11) {
        const char bytes[] arr = char bytes[2];
        shortToBytes(opCode, &arr);
        sendBytes(arr, 2);
    }
    else if (opCode == 8) {
        const char bytes[] arr = char bytes[frame.length() + 3];
        shortToBytes(opCode, &arr);
        unsigned int freeSlot = 2;
        for(char& c : frame) {
            arr[freeSlot] = c;
            freeSlot = freeSlot + 1;
        }
        arr[freeSlot] = '\0';
        sendBytes(arr, frame.length() + 3);
    }
    else {
        const char bytes[] arr = char bytes[4];
        shortToBytes(opCode, &arr);
        unsigned int freeSlot = 2;
        for(char& c : frame) {
            arr[freeSlot] = c;
            freeSlot = freeSlot + 1;
        }
        arr[freeSlot] = '\0';
        sendBytes(arr, 4);
    }
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

void ConnectionHandler::prepareLine(std::string &line){
    std::string code;
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    pos = s.find(delimiter);
    if (pos != s.npos){
        code = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        opCode = opMap[code];
    }
    else{
        opCode = opMap[line];
    }
}

void ConnectionHandler::shortToBytes(short num, char* bytesArr){
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

short ConnectionHandler::bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}
