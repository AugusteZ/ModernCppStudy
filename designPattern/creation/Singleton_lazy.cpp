#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

class LockTool {
public:
    mutex mMtx;
    void lock() {
        mMtx.lock();
    }
    void unlock() {
        mMtx.unlock();
    }
};

class Singleton_lazy {
private:
Singleton_lazy() {
    printf("Instance created \n");
}
~Singleton_lazy() {}

public:
static LockTool* mMtx;
static Singleton_lazy* getInstance() {
    mMtx->lock();
    if (!mInstance) {
        mInstance = new Singleton_lazy();
    }
    mMtx->unlock();
    return mInstance;
}
static Singleton_lazy* mInstance;
};

Singleton_lazy* Singleton_lazy::mInstance = nullptr;
LockTool* Singleton_lazy::mMtx = new LockTool;

int main() {
    Singleton_lazy* mHungryPtr;
    Singleton_lazy* mHungryPtr2;
    printf("process started !\n");
    // Singleton_lazy::mMtx = mMtx;

    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    printf("waiting done.\n");
    mHungryPtr = Singleton_lazy::getInstance();
    mHungryPtr2 = Singleton_lazy::getInstance();
    printf("got instance\n");
    if (mHungryPtr != mHungryPtr2) {
        printf("fail 1 instance \n");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    printf("process exit\n");
    return 1;
}