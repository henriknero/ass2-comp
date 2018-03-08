#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <map>
#include <string>
class Environment {
public:
    std::map<std::string,std::string> vars;
    Environment() {}
};


#endif

