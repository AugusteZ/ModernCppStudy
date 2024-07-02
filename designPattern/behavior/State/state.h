#include <cstdio>
#include <cstdlib>
#pragma once
class Process;

class State {
public:
    virtual ~State() = default;
    void set_process(Process* process) {
        this->process = process;
    }
    Process* get_process() {
        return this->process;
    }
	virtual bool buy(int) = 0; 
	virtual bool recharge(int) = 0; 
    Process* process;
};


class State_rich : public State {
public:
	bool buy(int) override;
	bool recharge(int) override;
};


class State_poor : public State {
public:
	bool buy(int) override;
	bool recharge(int) override;
};






