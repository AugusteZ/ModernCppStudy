#include <cstdio>
#include <cstdlib>
#include <vector>

class Command {
public:
    virtual ~Command() = default;
    virtual void excute() = 0; 
};

class Command_EatApple : public Command {
public:
    void excute() {
        printf("eat an Apple\n");
    }
};

class Command_DrinkTea : public Command {
public:
    void excute() {
        printf("drink a cup of tea\n");
    }
};

class CmdQueue {
public:
    void enqueueCmd(Command* cmd) {
        this->cmds.push_back(cmd);
    }
    void handleCmd() {
        for (auto cmd : this->cmds) {
            cmd->excute();
        }
        cmds.clear();
    }
    std::vector<Command*> cmds;
};

int main() {
    CmdQueue* mQueue = new CmdQueue;
    Command_DrinkTea* cmd1 = new Command_DrinkTea;
    Command_EatApple* cmd2 = new Command_EatApple;
    mQueue->enqueueCmd(cmd1);
    mQueue->enqueueCmd(cmd2);

    printf("容器决定什么时候执行cmd");
    mQueue->handleCmd();
    delete cmd1;
    delete cmd2;
    delete mQueue;
    return 0;
}
