#include <cstdio>
#include <iostream>
#include <string>
#include <list>

class Observer;
class Subscriber;

class Subject {
public:
    virtual ~Subject() = default;
    virtual void registObserver(Observer*) = 0;
    virtual void removeObserver(Observer*) = 0;
    virtual void notifyObserver() = 0;
    std::list<Observer*> mObservers;
};

class Subscriber {
public:
    Subscriber(std::string name) : name(name){};
    std::string name;
    void watchVideo() {
        printf("%s start watching . \n", this->name.c_str());
    }
};

class Observer {
public:
    void addSubscriber(Subscriber*);
    void removeSubscriber(Subscriber*);
    void notifySubscriber();
    std::list<Subscriber*> mSubcribers;
};

class Subject_Bilibili : public Subject {
public:
    virtual void registObserver(Observer* observer) {
        this->mObservers.push_back(observer);
    };
    virtual void removeObserver(Observer* observer) {
        this->mObservers.remove(observer);
    };
    virtual void notifyObserver() {
        printf("bilibili update! \n");
        for (auto observer : this->mObservers) {
            observer->notifySubscriber();
        }
    }
};

void Observer::addSubscriber(Subscriber* subscriber) {
    this->mSubcribers.push_back(subscriber);
}
void Observer::removeSubscriber(Subscriber* subscriber) {
    this->mSubcribers.remove(subscriber);
}
void Observer::notifySubscriber() {
    printf("好的，这就通知\n");
    for (auto subscriber : this->mSubcribers) {
        subscriber->watchVideo();
    }
}


int main() {
    Subject* biblibili = new Subject_Bilibili;
    Observer* mObserver = new Observer;
    Subscriber* A = new Subscriber("张三");
    Subscriber* B = new Subscriber("李四");
    Subscriber* C = new Subscriber("王五");

    biblibili->registObserver(mObserver);
    mObserver->addSubscriber(A);
    mObserver->addSubscriber(B);
    mObserver->addSubscriber(C);
    mObserver->removeSubscriber(C);

    biblibili->notifyObserver();


    return 1;
}