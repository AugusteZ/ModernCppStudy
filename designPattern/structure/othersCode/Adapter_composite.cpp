
#include <cstdio>
#include <cstdlib>
class Adaptee_rob {
public:
    virtual int doRob() = 0;
};
class Adaptee_robInFrance : public Adaptee_rob {
public:
    int doRob() {
        printf("Levez les mains, donnez moi votre argent!\n");
        return 1000;
    }
};

class Target_rob {
public:
    Adaptee_rob* mAdaptee = nullptr;
    virtual int giveMeYrMoney() = 0;
};

class Adaptor_rob : public Target_rob {
public:

    int giveMeYrMoney() {
        int money = 0;
        if (mAdaptee) {
            money = mAdaptee->doRob();
        } else {
            printf("we got nothing \n");
        }
        return money;
    }
};

int main() {
    Target_rob* mClinet = dynamic_cast<Target_rob*>(new Adaptor_rob());
    Adaptee_rob* mRobber = dynamic_cast<Adaptee_rob*>(new Adaptee_robInFrance());
    mClinet->mAdaptee = mRobber;
    int money = mClinet->giveMeYrMoney();
    printf("we got %d RMB!", money);
    return 0;
}