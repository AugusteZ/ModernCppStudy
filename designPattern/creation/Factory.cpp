#include <cstdio>
#include <iostream>

using namespace std;

class AbstractProduct {
public:
virtual void do_something() = 0;
};

class ConcreteProductA_1 : public AbstractProduct {
public:
    ConcreteProductA_1() {
        printf("ConcreteProductA_1 created!\n");
    }
    void do_something() {
        printf("ConcreteProductA_1 do something.\n");
    }
};

class ConcreteProductA_2 : public AbstractProduct {
public:
    ConcreteProductA_2() {
        printf("ConcreteProductA_2 created!\n");
    }
    void do_something() {
        printf("ConcreteProductA_2 do something.\n");
    }
};

class ConcreteFactory {
public:
    AbstractProduct* createProduct(int type){
        AbstractProduct* mProduct = nullptr;
        switch (type) {
        case 1:
            mProduct = dynamic_cast<AbstractProduct*>(new ConcreteProductA_1);
            break;
        case 2:
            mProduct = dynamic_cast<AbstractProduct*>(new ConcreteProductA_2);
            break;
        default:
            printf("unkown product type");
        }
        return mProduct;
    }
};

int main() {
    ConcreteFactory* mFactory = new ConcreteFactory();
    AbstractProduct* mProduct = mFactory->createProduct(1);
    mProduct->do_something();
    return 0;
}