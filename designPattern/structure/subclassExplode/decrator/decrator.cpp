#include <cstdio>
#include <iostream>
#include <iostream>
#include <string>

class DecoratorBase {
public:
    DecoratorBase* mImp = nullptr;
    virtual void Operation() = 0;
};

class ImpParty : public DecoratorBase {
public:
    std::string bro = "";
    ImpParty(std::string bro) {
        this->bro = bro;
    }
    void setBro(std::string name) {
        this->bro = name;
    }
    void Operation() {
        printf("Party start, hi %s\n", bro.c_str());
        if (mImp) {
            printf("lets drink!\n");
            mImp->Operation();
        }
    }
};

class ImpSoftDrink : public DecoratorBase {
public:
    std::string softdrink = "";
    ImpSoftDrink(std::string softdrink) {
        this->softdrink = softdrink;
    }
    void Operation() {
        printf("lets drink %s", softdrink.c_str());
    }
};

int main() {
    ImpParty* mParty = new ImpParty("John");
    ImpSoftDrink* mDrink = new ImpSoftDrink("cola");
    mParty->mImp = mDrink;
    mParty->Operation();
    delete mDrink;
    delete mParty;
    return 0;
}
