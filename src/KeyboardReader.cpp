#include <connectionHandler.h>
#include <mutex>
#include <KeyboardReader.h>
#include <thread>
#include <vector>


KeyboardReader::KeyboardReader(std::mutex & mutex, std::vector<std::string> & input, std::atomic_bool &stopper): _mutex(mutex), _inputs(input), stopper(stopper) {}


void KeyboardReader::readFromKeyboard() {
    while(!stopper){
        const short bufsize = 16384;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);//read from keyboard in to the buf
        std::string line(buf); // create a string call line
        std::lock_guard<std::mutex> lock(_mutex);
        _inputs.push_back(line);
    }
}


