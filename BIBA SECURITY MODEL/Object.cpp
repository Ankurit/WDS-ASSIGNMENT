#include "Object.h"

Object::Object(std::string n, int v) {
    name = n;
    value = v;
}

void Object::setValue(int v) {
    value = v;
}

int Object::getValue() {
    return value;
}

std::string Object::getName() {
    return name;
}
