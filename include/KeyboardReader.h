#include <mutex>
#include <condition_variable>


#ifndef CLIENT_KEYBOARDREADER_H
#define CLIENT_KEYBOARDREADER_H

class KeyboardReader{
private:
    std::mutex & _mutex;
    std::vector<std::string> &_inputs;
    bool &stopper;
    std::condition_variable &cv;
    volatile bool &finished;
public:
    KeyboardReader(std::mutex & mutex1, std::vector<std::string> &input, bool &stopper, std::condition_variable &_cv, volatile bool &finished);
    void readFromKeyboard();
};
#endif //CLIENT_KEYBOARDREADER_H
