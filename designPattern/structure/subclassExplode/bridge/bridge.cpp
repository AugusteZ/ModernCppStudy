#include "bridge_Imp.h"
// class AbstractionImp;

class Abstraction {
public:
    AbstractionImp* mImp = nullptr;
    virtual void Operation_RunPythonInTerminal() = 0;
};

class RefinedAbstraction_Windows : public Abstraction{
public:
    void Operation_RunPythonInTerminal() {
        printf("windows start WSL\n");
        if (mImp)
            mImp->Operation_RunPythonInTerminal();
    }
};

class RefinedAbstraction_Linux : public Abstraction{
public:
    void Operation_RunPythonInTerminal() {
        printf("linux start cmd\n");
        if (mImp)
            mImp->Operation_RunPythonInTerminal();
    }
};


int main() {
    Abstraction* mOS = new RefinedAbstraction_Windows;
    AbstractionImp* mPython = new ConcreteAbstractionImp_Python3;
    mOS->mImp = mPython;
    mOS->Operation_RunPythonInTerminal();
    free(mPython);
    mPython = new ConcreteAbstractionImp_Python2;
    mOS->mImp = mPython;
    mOS->Operation_RunPythonInTerminal();
    free(mPython);
    free(mOS);
    return 0;
}