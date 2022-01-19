#include "Subject.h"

Subject::Subject(std::string n, int t) {
    name = n;
    temp = t;
}

void Subject::setTemp(int n) {
    temp = n;
}

int Subject::getTemp() {
    return temp;
}

std::string Subject::getName() {
    return name;
}
