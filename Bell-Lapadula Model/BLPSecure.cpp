#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include "ReferenceMonitor.h"


// Stores an instruction from the instruction line
struct instruction {
    std::string command;
    std::string subjName;
    std::string objName;
    int secLevel;
    int value;
};

int main(int argc, char ** argv) {

    // Basic program validation such as requirements and correct filenames...
    if(argc != 2) {
        std::cout << "<Incorrect number of arguments. Run using ./BLPSecure filename. Exiting...>\n";
        return 0;
    }
    std::ifstream inFile;
    inFile.open(argv[1]);
    if(!inFile.is_open()) {
        std::cout << "<Invalid file name. Exiting...>\n";
        return 0;
    }

    // Begin taking instructions
    ReferenceMonitor monitor;

    // For every line in our instruction list, tokenize it into pieces to store in the instruction struct
    std::string line;
    std::string token;
    instruction fileLine;
    int tokenCount;
    bool illegal;

    while(std::getline(inFile, line)) {
        std::istringstream handler(line);
        tokenCount = 1;
        illegal = false;
        fileLine.secLevel = -1;
        // Tokenize the line and analyze it to determine commands/information
        while(std::getline(handler, token, ' ')) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            // Determine the command
            if(tokenCount == 1) {
                if(token == "addsub")
                    fileLine.command = "addsub";
                else if(token == "addobj")
                    fileLine.command = "addobj";
                else if(token == "read")
                    fileLine.command = "read";
                else if(token == "write")
                    fileLine.command = "write";
                else
                    illegal = true;
            }

            // Determine the arguments of the command with tokenCount 2-4
            else if(tokenCount == 2) {
                if(fileLine.command == "addsub" || fileLine.command == "write" || fileLine.command == "read")
                    fileLine.subjName = token;
                else if(fileLine.command == "addobj")
                    fileLine.objName = token;
            }

            else if(tokenCount == 3) {
                if(fileLine.command == "addsub" || fileLine.command == "addobj") {
                    if(token == "low")
                        fileLine.secLevel = 0;
                    else if(token == "medium")
                        fileLine.secLevel = 1;
                    else if(token == "high")
                        fileLine.secLevel = 2;
                    else
                        illegal = true;
                }
                else if(fileLine.command == "read" || fileLine.command == "write") {
                    fileLine.objName = token;
                }
            }

            else if(tokenCount == 4) {
                try {
                    fileLine.value = std::stoi(token);
                }
                catch(const std::invalid_argument&) {
                    illegal = true;
                }
            }

            tokenCount++;
        }
        // One more check, for verifying arguments to addobj/addsub
        if(fileLine.command == "addsub" || fileLine.command == "addobj") {
            if(fileLine.secLevel == -1)
                illegal = true;
        }

        // Valid command, now need to pass to ReferenceMonitor to handle it
        if(!illegal) {
            if(fileLine.command == "addsub") {
                monitor.addSubject(fileLine.subjName, fileLine.secLevel);
                std::cout << "Subject Added       : ";
            }
            else if(fileLine.command == "addobj") {
                monitor.addObject(fileLine.objName, fileLine.secLevel);
                std::cout << "Object Added        : ";
            }
            else if(fileLine.command == "read") {
                if(monitor.executeRead(fileLine.subjName, fileLine.objName))
                    std::cout << "Access Granted      : ";
                else
                    std::cout << "Access Denied       : ";
            }
            else if(fileLine.command == "write") {
                if(monitor.executeWrite(fileLine.subjName, fileLine.objName, fileLine.value))
                    std::cout << "Access Granted      : ";
                else
                    std::cout << "Access Denied       : ";
            }
        }

        else {
            std::cout << "Illegal Instruction : ";
        }
        std::cout << line << std::endl;

        fileLine = {};
        if(monitor.getCounter() == 10)
            monitor.printState();
    }

    monitor.printState();
    return 0;
}
