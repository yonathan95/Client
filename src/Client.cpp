#include <stdlib.h>
#include <connectionHandler.h>
#include <thread>
#include <KeyboardReader.h>
#include <mutex>
#include <vector>
#include <iostream>
#include <SocketReader.h>

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
    bool stopThreads = false;
    std::condition_variable _cv;
    volatile bool finished = false;
    KeyboardReader keyboardReader(mutex1, inputs, stopThreads, _cv, finished);
    SocketReader socketReader(connectionHandler, mutex2, outputs, stopThreads, finished);
    try{
        std::thread th1(&KeyboardReader::readFromKeyboard, &keyboardReader);
        std::thread th2(&SocketReader::readFromSocket, &socketReader);
        while (1) {
            if(!inputs.empty()){
                std::string line;
                {std::lock_guard<std::mutex> lock(mutex1);
                    line = inputs[0];
                    inputs.erase(inputs.begin());}
                connectionHandler.prepareLine(line);
                if (!connectionHandler.sendLine(line)) {
                    break;
                }
            }
            if (!outputs.empty()){
                std::string answer;
                {std::lock_guard<std::mutex> lock(mutex2);
                    answer = outputs[0];
                    outputs.erase(outputs.begin());}
                std::cout << answer << "\n";
                if ((connectionHandler.getGettingOpCode() == 12) & (connectionHandler.getOpMessage() == 4)){
                    {std::lock_guard<std::mutex> lock(mutex1);
                    stopThreads = true;}
                    _cv.notify_all();
                    break;
                }
                if ((connectionHandler.getGettingOpCode() == 13) & (connectionHandler.getOpMessage() == 4)){
                    std::lock_guard<std::mutex> lock(mutex1);
                    stopThreads = true;}
                    _cv.notify_all();
                }
            }
        th1.join();
        th2.join();
    }catch (std::exception &e){}
    return 0;
}
