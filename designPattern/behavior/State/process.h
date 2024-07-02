
#include <cstdio>
#include <cstdlib>
#include "state.h"
#pragma once

class Process {
public:
    Process(int = 10);
    void set_state(State* state);
    void recharge(int money);
    void buy(int price);
    int query_money();
    int balance;
    State* mState = nullptr;
};

