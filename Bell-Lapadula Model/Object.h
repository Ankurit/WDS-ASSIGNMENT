#include <iostream>
#include <string>

class Object {
    private:
        std::string name;
        int value;
    public:
        Object(std::string, int);
        void setValue(int);
        int getValue();
        std::string getName();
};
