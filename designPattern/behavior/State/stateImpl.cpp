#include <cstdio>
#include <cstdlib>
#include "state.h"
#include "process.h"
#pragma once

bool State_rich::buy(int price) {
	this->process->balance -= price;
	int balance = this->process->query_money();
	if (balance <= 0) {
		this->process->set_state(new State_poor);
	}
	return true;
}

bool State_rich::recharge(int money) {
	int balance = this->process->query_money();
	if (money + balance > 0) {
		return true;
	}
	return false;
}


bool State_poor::buy(int price) {
	// int balance = this->process->query_money();
	return false;
}

bool State_poor::recharge(int money) {
	int balance = this->process->query_money();
	if (money + balance > 0) {
		this->process->set_state(new State_rich);
		return true;
	}
	return false;
}
