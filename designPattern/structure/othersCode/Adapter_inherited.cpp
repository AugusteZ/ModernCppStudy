
#include <cstdio>
class Target_rob {
public:
    virtual int giveMeYrMoney() = 0;
};

class Adaptee_robInFrance {
public:
    int doRob() {
        printf("Levez les mains, donnez moi votre argent!\n");
        return 1000;
    }
};

class Adaptor_rob : public Target_rob, Adaptee_robInFrance {
public:
    int giveMeYrMoney() {
        return doRob();
    }
};

int main() {
    Target_rob* mClinet = dynamic_cast<Target_rob*>(new Adaptor_rob());
    int money = mClinet->giveMeYrMoney();
    printf("we got %d RMB!", money);
}
