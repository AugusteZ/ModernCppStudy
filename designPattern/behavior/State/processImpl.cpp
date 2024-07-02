#include "process.h"
#include "state.h"
#include <cstdio>
#pragma once

Process::Process(int init_money) {
    this->balance = init_money;
    if (init_money > 0) {
        this->set_state(new State_rich);
    } else {
        this->set_state(new State_poor);
    }
}

int Process::query_money() {
    return this->balance;
}

void Process::set_state(State* state) {
    if (mState)
        delete this->mState;
    this->mState = state;
    this->mState->set_process(this);
}

void Process::recharge(int money) {
    bool result = this->mState->recharge(money);
    this->balance += money;
    if (result) {
        printf("尊贵的用户: 充值后，余额 %d\n", this->query_money());
    } else {
        printf("欠费的用户: 充值后，余额 %d\n", this->query_money());
    }
}

void Process::buy(int price) {
    if (this->mState->buy(price))
        printf("购买成功， 余额 %d\n", this->query_money());
    else
        printf("购买失败 请充值， 余额 %d\n", this->query_money());
}
