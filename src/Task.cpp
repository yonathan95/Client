#include <connectionHandler.h>
#include <mutex>
#include <Task.h>
#include <thread>

Task::Task(std::mutex mutex, std::vector<std::string> vector):_mutex(mutex),_inputs(vector) {}


void Task::readFromKeyboard() {
    while(1){
        try{
            const short bufsize = 16384;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);//read from keyboard in to the buf
            std::string line(buf); // create a string call line
            _inputs.push_back(line);
        }
        catch (std::exception& e) {break;}
    }
}

void  Task::readFromSocket() {



}


