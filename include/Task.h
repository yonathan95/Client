#include <mutex>


#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

class Task{
private:
    std::mutex & _mutex;
    std::vector<std::string> _inputs;
public:
    Task(std::mutex & mutex1, std::vector<std::string> &input);
    void readFromKeyboard();
    void readFromSocket();



};
#endif //CLIENT_TASK_H
