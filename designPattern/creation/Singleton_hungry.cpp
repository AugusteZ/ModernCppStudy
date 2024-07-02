#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

class Singleton_hungry{
private:
    int mProp = 0;
    Singleton_hungry(int intParam = 1):mProp(intParam) {
        printf("instance created hungryly.\n");
    };
    ~Singleton_hungry() {
        if (mInstance) {
            delete mInstance;
        }
        printf("instance deleted.\n");
    }

public:
    static Singleton_hungry* mInstance;
    static Singleton_hungry* getInstance() {
        printf("got instance\n");
        if (mInstance) {
            return mInstance;
        }
        mInstance = new Singleton_hungry;
        return mInstance;
    }
    void do_print() {
        printf("m property is  %d \n", mProp);
    }
};

//以下语句需要在main函数运行前执行
Singleton_hungry* Singleton_hungry::mInstance = new Singleton_hungry(2);


int main() {
    Singleton_hungry* mHungryPtr;
    printf("process started !\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    printf("waiting done.\n");
    mHungryPtr = Singleton_hungry::getInstance();
    mHungryPtr->do_print();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    printf("process exit\n");
    return 1;
}