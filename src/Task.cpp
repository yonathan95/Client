#include <connectionHandler.h>
#include <mutex>
#include <Task.h>
#include <thread>
#include <vector>


Task::Task(std::mutex & mutex, std::vector<std::string> & input):_mutex(mutex),_inputs(input) {}


void Task::readFromKeyboard() {
    while(1){
        try{
            const short bufsize = 16384;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);//read from keyboard in to the buf
            std::string line(buf); // create a string call line
            std::lock_guard<std::mutex> lock(_mutex);
            _inputs.push_back(line);
        }
        catch (std::exception& e) {break;}
    }
}

void  Task::readFromSocket() {



}


