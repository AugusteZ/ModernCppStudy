#include <cstdio>
#include <iostream>

class AbstractionImp {
public:
    virtual void Operation_RunPythonInTerminal() = 0;
};

// ConcreteAbstractionImp
class ConcreteAbstractionImp_Python2 : public AbstractionImp {
public:
    void Operation_RunPythonInTerminal() {
        printf("start python2 \n");
    }
};

class ConcreteAbstractionImp_Python3 : public AbstractionImp {
public:
    void Operation_RunPythonInTerminal() {
        printf("start python3 \n");
    }
};
