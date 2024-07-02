
#include <cstdio>
class AbstractProduct {
public:
    virtual void do_something() = 0;
    virtual ~AbstractProduct() {
        printf("AbstractProduct 虚析构\n");
    }
};

class AbstractProductA : public AbstractProduct {
public:
    // virtual void do_something() = 0;
    virtual void do_something_special_A() = 0;
    virtual ~AbstractProductA() {
        printf("AbstractProductA 虚析构\n");
    }
};

class AbstractProductB : public AbstractProduct {
public:
    virtual void do_something_special_B() = 0;
    virtual ~AbstractProductB() {
        printf("AbstractProductB 虚析构\n");
    }
};

class ConcreteProductA : public AbstractProductA {
public:
    void do_something() {
        printf("ConcreteProductA  do_something\n");
    }
    void do_something_special_A() {
        printf("ConcreteProductA  do_something_special_A\n");
    } 
};

class ConcreteProductB : public AbstractProductB {
public:
    void do_something() {
        printf("ConcreteProductB  do_something\n");
    }
    void do_something_special_A() {
        printf("ConcreteProductB  do_something_special_B\n");
    } 
};

class AbstractFactory {
public:
    virtual AbstractProduct* createProduct(int type) = 0;
    virtual ~AbstractFactory() {
        printf("AbstractFactory 虚析构\n");
    }
};

class ConcreteFactoryA : public AbstractFactory {
public:
    AbstractProduct* createProduct(int type) {
        AbstractProduct* mProduct = nullptr;
        mProduct = dynamic_cast<AbstractProduct*>(new ConcreteProductA);
        return mProduct;
    }
};

int main() {
    AbstractFactory* mFactory = nullptr;
    AbstractProduct* mProduct = nullptr;
    mFactory = new ConcreteFactoryA();
    mProduct = mFactory->createProduct(1);
    mProduct->do_something();
    delete mProduct;
    delete mFactory;

    // mFactory = new ConcreteFactoryB();

    return 1;
}