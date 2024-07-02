#include <cstdio>
#include <iostream>

class Product {
public:
    std::string name;
    int level;
    void do_print() {
        printf("name = %s, level = %d \n", name.c_str(), level);
    }
};

class Builder {
public:
    Product* mProduct = nullptr;
    void set_product(Product* product) {
        this->mProduct = product;
    }
    Product* get_product() {
        return this->mProduct;
    }
    void build_name(std::string name) {
        if (this->mProduct)
            this->mProduct->name = name;
    }
    void build_level(int level) {
        if (this->mProduct)
            this->mProduct->level = level;
    }

};

class Director {
public:
    Builder* mBuilder = nullptr;
    void set_builder(Builder* builder) {
        this->mBuilder = builder;
    }
    void do_build(std::string name, int level) {
        Product* product = this->mBuilder->get_product();

        this->mBuilder->build_name(name);
        this->mBuilder->build_level(level);
    }
};


int main() {
    Product* mProduct = new Product;
    Builder* mBuilder = new Builder;
    Director* mDirector = new Director;
    mBuilder->set_product(mProduct);
    mDirector->set_builder(mBuilder);
    mDirector->do_build("张三", 100);
    mProduct->do_print();


    return 0;
}