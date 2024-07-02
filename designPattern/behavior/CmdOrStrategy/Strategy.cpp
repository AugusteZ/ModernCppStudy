#include <cstdio>
#include <cstdlib>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void excute() = 0;
};

class Strategy_allin : public Strategy {
public:
    void excute() {
        printf("塔塔开!\n");
    }
};

class Strategy_giveup : public Strategy {
public:
    void excute() {
        printf("点了点了点了.\n");
    }
};

class ChasingGame {
public:
    void play() {
        if (this->mStrategy) {
            mStrategy->excute();
        } else {
            printf("挂机，等着输.\n");
        }
    }
    void set_strategy(Strategy* strategy) {
        if (this->mStrategy) {
            delete this->mStrategy;
        }
        this->mStrategy = strategy;
    }
    Strategy* mStrategy = nullptr;
};

int main() {
    ChasingGame* mGame = new ChasingGame;
    mGame->play();
    Strategy* mStrategy = new Strategy_allin;
    mGame->set_strategy(mStrategy);
    mGame->play();
    mStrategy = new Strategy_giveup;
    mGame->set_strategy(mStrategy);
    mGame->play();
    return 1;
}
