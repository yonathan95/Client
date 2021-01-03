//
// Created by spl211 on 03/01/2021.
//

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

class Task{
private:
    std::mutex &_mutex;
    std::vector<std::string> &_inputs;
public:
    void readFromKeyboard();
    void readFromSocket();


    Task(std::mutex &mutex, std::vector<std::string> vector);
};
#endif //CLIENT_TASK_H
