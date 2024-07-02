// 作用与对象结构的二次辨析
#include <cstdio>


class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void action_lunch() = 0;
    virtual void action_dinner() = 0;
};


class Visitor_Shanghai : public Visitor {
public:
    virtual void action_lunch() {
        printf("山西午餐吃炸酱面\n");
    }
    virtual void action_dinner() {
        printf("山西晚餐吃刀削面\n");
    }
};

class Visitor_ShanXi : public Visitor {
public:
    virtual void action_lunch() {
        printf("上海午餐吃酱鸭子\n");
    }
    virtual void action_dinner() {
        printf("上海晚餐吃剩下的酱鸭子\n");
    }
};

class Element {
public:
    virtual ~Element() = default;
    virtual void eat_meal() = 0;
    virtual void accept(Visitor* visitor) {
        mVisitor = visitor;
    }
    Visitor* mVisitor;
};

class Element_Lunch : public Element {
public:
    virtual void eat_meal() {
        if (mVisitor) {
            mVisitor->action_lunch();
        }
    }
};

class Element_dinner : public Element {
public:
    virtual void eat_meal() {
        if (mVisitor) {
            mVisitor->action_dinner();
        }
    }
};


int main() {
    Visitor* mShanxi = new Visitor_ShanXi;
    Visitor* mShanghai = new Visitor_Shanghai;
    Element* meal = new Element_Lunch;
    meal->accept(mShanghai);
    meal->eat_meal();
    delete meal;
    meal = new Element_dinner;
    meal->accept(mShanxi);
    meal->eat_meal();
    delete meal;
    delete mShanghai;
    delete mShanxi;

    return 1;
}


