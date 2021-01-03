#include <mutex>


#ifndef CLIENT_KEYBOARDREADER_H
#define CLIENT_KEYBOARDREADER_H

class KeyboardReader{
private:
    std::mutex & _mutex;
    std::vector<std::string> &_inputs;
public:
    KeyboardReader(std::mutex & mutex1, std::vector<std::string> &input);
    void readFromKeyboard();



};
#endif //CLIENT_KEYBOARDREADER_H
