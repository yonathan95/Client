#include <stdlib.h>
#include <connectionHandler.h>
#include <thread>
#include <KeyboardReader.h>
#include <mutex>
#include <vector>
#include <iostream>
#include <SocketReader.h>


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
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
        return 1;
    }
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::mutex mutex1;
    std::mutex mutex2;
    std::atomic_bool stopThreads(false);
    KeyboardReader task(mutex1, inputs, stopThreads);
    std::thread th1(&KeyboardReader::readFromKeyboard, &task);
    SocketReader socketReader(connectionHandler, mutex2, outputs, stopThreads);
    std::thread th2(&SocketReader::readFromSocket, &socketReader);
    while (1) {
        if(inputs.size() != 0){
            std::string line;
            {std::lock_guard<std::mutex> lock(mutex1);
                line = inputs[0];
                inputs.erase(inputs.begin());}
            connectionHandler.prepareLine(line);
            if (!connectionHandler.sendLine(line)) {
                break;
            }
        }
        if (outputs.size() != 0){
            std::string answer;
            {std::lock_guard<std::mutex> lock(mutex2);
                answer = outputs[0];
                outputs.erase(outputs.begin());}
            std::cout << answer << std::endl;
            if ((connectionHandler.getGettingOpCode() == 12) & (connectionHandler.getOpMessage() == 4)){
                stopThreads.exchange(true);
                break;
            }
        }
    }
    return 0;
}
