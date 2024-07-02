#include <cstdio>
#include <iostream>
#include <string>
#include <list>

class Mediator;

class Client {
public:
    virtual ~Client() = default;
    void registMediator(Mediator* mediator);
    void callSomeTeam(int type);
    virtual void responce() = 0;
    Mediator* mMediator;
    int type;
    std::string name;
};

class Client_Boss : public Client {
public:
    virtual ~Client_Boss() = default;
    Client_Boss(std::string name, Mediator* mediator) {
        Client::name = name;
        Client::type = 99;
        registMediator(mediator);
    }
    virtual void responce() {
        printf("Boss %s here! what do u want?\n", name.c_str());
    }
};

class Client_Engineer : public Client {
public:
    virtual ~Client_Engineer() = default;
    Client_Engineer(std::string name, Mediator* mediator) {
        Client::name = name;
        Client::type = 1;
        registMediator(mediator);
    }
    virtual void responce() {
        printf("Engineer %s here! how can i help u?\n", name.c_str());
    }
};

class Mediator {
public:
    void registClient(Client* client) {
        this->mClient.push_back(client);
    }
    void removeClient(Client* client) {
        this->mClient.remove(client);
    }
    void callClient(int type) {
        printf("callClient type = %d \n", type);
        for (auto client : this->mClient) {
            if (client->type == type) {
                client->responce();
            }
        }
    }
    std::list<Client*> mClient;
};


void Client::callSomeTeam(int type) {
    if (this->mMediator)
        this->mMediator->callClient(type);
    else
        printf("unset Mediator\n");
}

void Client::registMediator(Mediator* mediator){
    this->mMediator = mediator;
    mediator->registClient(this);
}


int main() {
    Mediator* mMediator = new Mediator;
    Client_Boss* mBoss = new Client_Boss("fajie", mMediator);
    Client_Engineer* mEngineer1 = new Client_Engineer("pengyi 1", mMediator);
    Client_Engineer* mEngineer2 = new Client_Engineer("pengyi 2", mMediator);

    mBoss->callSomeTeam(1);

    printf("10mins after\n");

    mEngineer1->callSomeTeam(99);

    return 1;
}

