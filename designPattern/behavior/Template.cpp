#include <iostream>
using namespace std;

//做饮料模板
class TemplateDrink {
public:
    virtual ~TemplateDrink() = default;
    //煮水
    virtual void BoildWater() = 0;
    //冲泡
    virtual void Brew() = 0;
    //倒入杯中
    virtual void PourInCup() = 0;
    //加辅助材料
    virtual void AddSomething() = 0;

    //模板方法
    void Make() {
        BoildWater();
        Brew();
        PourInCup();
        AddSomething();
    }
};

class Coffee : public TemplateDrink {
    virtual void BoildWater() {
        cout << "煮纯净水" << endl;
    }
    virtual void Brew() {
        cout << "冲泡咖啡" << endl;
    }
    virtual void PourInCup() {
        cout << "咖啡倒入杯中" << endl;
    }
    virtual void AddSomething() {
        cout << "加牛奶" << endl;
    }
};


class Tea :public TemplateDrink {
    virtual void BoildWater() {
        cout << "煮山泉水" << endl;
    }
    virtual void Brew() {
        cout << "冲泡铁观音" << endl;
    }
    virtual void PourInCup() {
        cout << "茶水倒入杯中" << endl;
    }
    virtual void AddSomething() {
    }
};


int main()
{
    Tea* tea = new Tea;
    tea->Make();

    Coffee* coffee = new Coffee;
    coffee->Make();

    delete tea;
    delete coffee;
}

