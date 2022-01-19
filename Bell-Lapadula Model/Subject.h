#include <iostream>
#include <string>

class Subject {
    private:
        std::string name;
        int temp;
    public:
        Subject(std::string, int);
        void setTemp(int);
        int getTemp();
        std::string getName();

};
