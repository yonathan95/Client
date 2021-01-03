#include "SocketReader.h"
#include <vector>
#include <connectionHandler.h>

SocketReader::SocketReader(ConnectionHandler &ch, std::mutex &mutex1, std::vector<std::string> &outputs1): connectionHandler(ch),mutex(mutex1),outputs(outputs1) {

}

void readFromSocket(){

}