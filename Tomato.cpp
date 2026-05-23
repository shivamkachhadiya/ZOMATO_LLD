//
// Created by shiva on 23-05-2026.
//
#include <iostream>
#include <string>
#include <vector>
using namespace std;
// ==================
//   MenuItem
// ==================
class MenuItem {
private:
    string code;
    string name;
    int price;
public:
    MenuItem(string code,string name,int price):code(code),name(name),price(price){}
    string getName()const{return name;}
    string getCode()const{return code;}
    int getPrice()const {return price;}
};

// ==================
//   Restaurant
// ==================
class Restaurant {
private:
    int id;
    string name;
    string location;
    vector<MenuItem>menu;
public:
    Restaurant(int id,string name,string location)
                :id(id),name(name),location(location){}

    void addMenuItem(MenuItem item) {
        menu.push_back(item);
    }

    int                 getId()         const{return id;}
    string              getName()       const{return name;}
    string              getLocation()   const{return location;}
    vector<MenuItem>    getMenu()       const{return menu;}
};

// ==================
//   Cart
// ==================
class Cart {
private:
    vector<MenuItem>items;
    Restaurant* restaurant;
public:
    Cart():restaurant(nullptr){}
    void setRestaurant(Restaurant* r) {
        restaurant=r;
    }
    void addToCart(MenuItem item) {
        items.push_back(item);
    }
    int totalCost()const {
        int total=0;
        for (auto item:items) {
            total+=item.getPrice();
        }
        return total;
    }
    bool isEmpty()const {
        return items.empty();
    }
    Restaurant* getRestaurant()const{return restaurant;}
    vector<MenuItem>getItems()const{return items;}
};

//===============================TILL NOW RUN===================================
// int main() {
//     // (1) Restaurant banaya — apni jagah exist karta hai
//     Restaurant r1(1, "Pizza Hut", "Ahmedabad");
//
//     // (2) Cart banaya — abhi khali hai
//     Cart cart;
//
//     // (3) Cart ko bataya — "tu Pizza Hut ke liye hai"
//     cart.setRestaurant(&r1);  // & matlab "r1 ka address de do"
//     //                  ^
//     //                  |
//     //               & = address of
//
//     // (4) Item add kiya
//     MenuItem m1("P001", "Margherita", 299);
//     cart.addToCart(m1);
//
//     // (5) Total dekha
//     cout << cart.totalCost() << endl;  // 299 print hoga
// }


// ==================
//   User
// ==================
class User {
private:
    int id;
    string name;
    string address;
    Cart cart;          //  Cart by value — User OWNS cart

public:
    User(int id, string name, string address)
        : id(id), name(name), address(address) {}

    Cart& getCart() {
        return cart;    //  reference return
    }

    int    getId()      const { return id; }
    string getName()    const { return name; }
    string getAddress() const { return address; }
};

//Payment Strategy

class IpaymentStrategy {
public:
    virtual void pay(int amount)=0;
    virtual ~IpaymentStrategy()=default;
};
//
// virtual void pay() = 0;
// ^                ^
// |                |
// har child class    pure virtual —
// override KARE GA   object nahi ban sakta
//                sirf inherit kar sakte hain

//credit card
class CreditCard:public IpaymentStrategy {
public:
    void pay(int amount)override {
        cout<<"Paid Rs. "<<amount<<"using Credit Card!"<<endl;
    }
};

//netbanking
class NetBanking:public IpaymentStrategy {
public:
    void pay(int amount)override {
        cout<<"Paid Rs. "<<amount<<"using net banking !"<<endl;
    }
};

//UPI
class UPI:public IpaymentStrategy {
public:
    void pay(int amount)override {
        cout<<"Paid Rs. "<<amount<<"using UPI:"<<endl;
    }
};
//
// IPaymentStrategy  ← Interface (contract)
//        ↑
//   ┌────┼────┐
//   │    │    │
// Card  Net  UPI
//       Banking
//
// Teeno ne pay() implement kiya

class Order {
protected:
    int id;
    User user;
    Restaurant* restaurant;
    vector<MenuItem>items;
    IpaymentStrategy* strategy_;

public:
    Order(int id,User user,Restaurant* rest):id(id),user(user),restaurant(rest),strategy_(nullptr){}

    void setPaymentStrategy(IpaymentStrategy* s) {
        strategy_=s;
    }

    void addItem(MenuItem item) {
        items.push_back(item);
    }

    void pay() {
        if (strategy_==nullptr) {
            cout<<"No Payment Selected..."<<endl;
            return;
        }
        int total=0;
        for (auto item:items) {
            total+=item.getPrice();
        }
        strategy_->pay(total);
    }

    int getId()const{return id;}
    User getUser()const{return user;}
    Restaurant* getRestaurant()const{return restaurant;}
    vector<MenuItem>getItems()const{return items;}


    virtual string getType()const=0;
    virtual ~Order()=default;
};

// ==================
//   DeliveryOrder
// ==================

class DelivertOrder:public Order {
private:
    string deliveryAddress;
public:
    DelivertOrder(int id,User user,Restaurant* rest,string address)
            :Order(id,user,rest),deliveryAddress(address){}

    string getType()const override {
        return "DELIVERY...";
    }

    string getDeliveryAddress()const{return deliveryAddress;}

};

class PickupOrder:public Order {
private:
    string resAddress;
public:
    PickupOrder(int id,User user,Restaurant* rest,string address)
        :Order(id,user,rest),resAddress(address){}

    string getType() const override {
        return "PICKUP...";
    }

    string getResAddress()const{return resAddress;}
};