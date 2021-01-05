#include <connectionHandler.h>
#include <mutex>
#include <KeyboardReader.h>
#include <vector>
#include <condition_variable>

/**
* Constructor
*/
KeyboardReader::KeyboardReader(std::mutex & mutex, std::vector<std::string> & input, bool &stopper, std::condition_variable &_cv, volatile bool &finished): _mutex(mutex), _inputs(input), stopper(stopper), cv(_cv), finished(finished) {}

/**
* Read from the keyboard
*/
void KeyboardReader::readFromKeyboard() {
    while(1){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);//read from keyboard in to the buf
        std::string line(buf); // create a string call line
        {std::lock_guard<std::mutex> lock(_mutex);
        _inputs.push_back(line);}
        if (line == "LOGOUT"){
            {std::unique_lock<std::mutex> lk(_mutex);
            this->cv.wait(lk,[this]{return stopper;});
            lk.unlock();}
            if (finished){
                break;
            }
            else{
                stopper = false;
            }
        }
    }
}



