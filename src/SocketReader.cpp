#include "SocketReader.h"
#include <vector>
#include <connectionHandler.h>

SocketReader::SocketReader(ConnectionHandler &ch, std::mutex &mutex1, std::vector<std::string> &outputs1, bool &stopper, volatile bool &finished): connectionHandler(ch),mutex(mutex1),outputs(outputs1), stopper(stopper), finished(finished) {

}

/**
* Read from the socket
*/
void SocketReader::readFromSocket(){
    while(!stopper){
        std::string answer;
        if (!connectionHandler.getLine(answer)) break;
        if (answer == "ACK 4") {
            finished = true;
            std::lock_guard<std::mutex> lock(mutex);
            outputs.push_back(answer);
            break;
        }
        std::lock_guard<std::mutex> lock(mutex);
        outputs.push_back(answer);
    }
}