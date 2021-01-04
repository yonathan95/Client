#include <connectionHandler.h>
#include <boost/lexical_cast.hpp>
 
using boost::asio::ip::tcp;
using std::string;

ConnectionHandler::ConnectionHandler(string host, short port,std::map<std::string,short> &map): host_(host), port_(port), io_service_(), socket_(io_service_), sendingOpCode(0), gettingOpCode(0),opMessage(0),opMap(map){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
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
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
            tmp += socket_.write_some(boost::asio::buffer(bytes, bytesToWrite - tmp), error);
        }
		        if(error)
		                throw boost::system::system_error(error);

    } catch (std::exception& e) {
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
    char ch = '1';
    unsigned int counter = 0;
    char opBytes [2] ;
    char messageBytes [2] ;
    int i = 0;
    try {
        while((i < 4) | (ch != '\0')){
            if(!getBytes(&ch, 1))
            {
                return false;
            }
            if ((counter == 4) & ((opMessage <= 9) & (opMessage >= 6))){
                frame += "\n";
            }
            if((counter > 3) & (ch != '\0')){
                frame.append(1, ch);
            }
            if (counter < 2){
                opBytes[counter] = ch;
            }
            if ((counter > 1) & (counter < 4)){
                messageBytes[counter - 2] = ch;
            }
            counter = counter + 1;
            if (counter == 2){
                gettingOpCode = bytesToShort(opBytes);
                if (gettingOpCode == 12) frame += "ACK";
                else frame += "ERROR";

            }
            if (counter == 4){
                opMessage = bytesToShort(messageBytes);
                frame += " ";
                frame += std::to_string(opMessage);
                if (gettingOpCode == 13) break;
            }
            i = i + 1;
	    }
    } catch (std::exception& e) {
        return false;
    }
    return true;
}
 
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame) {
    if ((sendingOpCode == 1) | (sendingOpCode == 2) | (sendingOpCode == 3)) {
        char arr[frame.length() + 3];
        shortToBytes(sendingOpCode, &arr[0]);
        unsigned int freeSlot = 2;
        for (const char &c : frame) {
            if (c == ' ') {
                arr[freeSlot] = '\0';
                freeSlot = freeSlot + 1;
            } else {
                arr[freeSlot] = c;
                freeSlot = freeSlot + 1;
            }
        }
        arr[freeSlot] = '\0';
        return sendBytes(arr, frame.length() + 3);
    }
    else if ((sendingOpCode == 4) | (sendingOpCode == 11)) {
        char arr[2];
        shortToBytes(sendingOpCode, &arr[0]);
        return sendBytes(arr, 2);
    }
    else if (sendingOpCode == 8) {
        char arr[frame.length() + 3];
        shortToBytes(sendingOpCode, &arr[0]);
        unsigned int freeSlot = 2;
        for(const char& c : frame) {
            arr[freeSlot] = c;
            freeSlot = freeSlot + 1;
        }
        arr[freeSlot] = '\0';
        return sendBytes(arr, frame.length() + 3);
    }
    else {
        char arr[4];
        short courseNum = boost::lexical_cast<short>(frame);
        shortToBytes(sendingOpCode, &arr[0]);
        shortToBytesTwo(courseNum,&arr[0]);
        return sendBytes(arr, 4);
    }
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
    }
}

void ConnectionHandler::prepareLine(std::string &line){
    std::string code;
    std::string delimiter = " ";
    std::string token;
    size_t pos = line.find(delimiter);
    if (pos != line.npos){
        code = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        sendingOpCode = opMap[code];
    }
    else{
        sendingOpCode = opMap[line];
    }
}

void ConnectionHandler::shortToBytes(short num, char* bytesArr){
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

void ConnectionHandler::shortToBytesTwo(short num, char* bytesArr){
    bytesArr[2] = ((num >> 8) & 0xFF);
    bytesArr[3] = (num & 0xFF);
}

short ConnectionHandler::bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

short ConnectionHandler::getOpMessage(){
    return opMessage;
}

short ConnectionHandler::getGettingOpCode(){
    return gettingOpCode;
}