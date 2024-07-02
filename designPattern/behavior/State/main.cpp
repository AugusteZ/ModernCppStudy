
#include <cstdio>
#include <cstdlib>
#include "processImpl.cpp"
#include "stateImpl.cpp"


int main() {

    Process* mProcess = new Process(10);
    mProcess->buy(101);
    mProcess->buy(1);
    mProcess->recharge(10);
    mProcess->buy(1);

    mProcess->recharge(1000);
    mProcess->buy(100);
    delete mProcess;

    return 1;
}
