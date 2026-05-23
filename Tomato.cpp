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

    string getType() const override {
        return "DELIVERY";
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

//==============  FACTORY  ===============================================================

class IOrderFactory {
public:
    virtual Order* createOrder(int id,User user,Restaurant* rest)=0;
    virtual ~IOrderFactory()=default;
};

class NowOrderFactory : public IOrderFactory {
public:
    Order* createOrder(int id, User user, Restaurant* rest) override {
        string type;
        string address;

        cout << "Delivery ya Pickup? (D/P): ";
        cin >> type;

        if (type == "D") {
            cout << "Delivery address do: ";
            cin >> address;
            return new DelivertOrder(id, user, rest, address);
        } else {
            cout << "Pickup address do: ";
            cin >> address;
            return new PickupOrder(id, user, rest, address);
        }
    }
};

class ScheduleOrderFactory:public IOrderFactory {
private:
    string scheduleTime;
public:
    ScheduleOrderFactory(string time):scheduleTime(time){}
    Order* createOrder(int id, User user, Restaurant *rest) override {
        string type;
        string address;
        cout<<"Schedule time: "<<scheduleTime<<endl;
        cout<<"Delivery ya Pickup (D/P): ";
        cin>>type;

        if (type=="D") {
            cout<<"Pickup address do:";
            cin>>address;
            return new DelivertOrder(id,user,rest,address);
        }else {
            cout<<"pickup address do:";
            cin>>address;
            return new PickupOrder(id,user,rest,address);
        }
    }
};

// ========================
//   RestaurantManager
//   (Singleton)
// ========================
class RestaurantManager {
private:
    // (1) private constructor
    RestaurantManager() {}

    // (2) copy band karo
    RestaurantManager(const RestaurantManager&) = delete;
    RestaurantManager& operator=(const RestaurantManager&) = delete;

    // (3) ek hi instance
    static RestaurantManager* instance;

    vector<Restaurant> restaurants;

public:
    // (4) global access point
    static RestaurantManager* getInstance() {
        if (instance == nullptr) {
            instance = new RestaurantManager();
        }
        return instance;
    }

    void addRestaurant(Restaurant r) {
        restaurants.push_back(r);
    }

    vector<Restaurant> searchByLoc(string loc) {
        vector<Restaurant> result;
        for (auto r : restaurants) {
            if (r.getLocation() == loc) {
                result.push_back(r);
            }
        }
        return result;
    }

    vector<Restaurant> getRestaurants() const { return restaurants; }
};

// (5) static member — class ke bahar define karo
RestaurantManager* RestaurantManager::instance = nullptr;


// ========================
//   OrderManager
//   (Singleton)
// ========================
class OrderManager {
private:
    OrderManager() {}
    OrderManager(const OrderManager&) = delete;
    OrderManager& operator=(const OrderManager&) = delete;

    static OrderManager* instance;

    vector<Order*> orders;    // (1) Order* pointer — kyunki Order abstract hai!

public:
    static OrderManager* getInstance() {
        if (instance == nullptr) {
            instance = new OrderManager();
        }
        return instance;
    }

    void addOrder(Order* order) {
        orders.push_back(order);
    }

    void listOrders() const {
        if (orders.empty()) {
            cout << "Koi order nahi!" << endl;
            return;
        }
        for (auto order : orders) {
cout << "-----------------" << endl;
            cout << "Order ID : " << order->getId() << endl;
            cout << "Type     : " << order->getType() << endl;
            cout << "User     : " << order->getUser().getName() << endl;
        }
    }
};

// (2) static member define karo
OrderManager* OrderManager::instance = nullptr;


// ========================
//   NotificationService
// ========================
class NotificationService {
private:
    Order* order;

public:
    NotificationService(Order* o) : order(o) {}

    void notifyUser() const {
        cout << "================================" << endl;
        cout << "      ORDER CONFIRMATION!       " << endl;
        cout << "================================" << endl;
        cout << "Hey " << order->getUser().getName() << "!" << endl;
        cout << "Order Type : " << order->getType() << endl;
        cout << "Order ID   : " << order->getId() << endl;
        cout << "Restaurant : " << order->getRestaurant()->getName() << endl;
        cout << "Items      : " << order->getItems().size() << endl;
        cout << "================================" << endl;
    }
};

// ========================
//   Tomato
//   (Orchestration Class)
// ========================
class Tomato {
public:
    void run() {
        // (1) Restaurants setup karo
        setupRestaurants();

        // (2) User banao
        User user(1, "Rahul", "Ahmedabad");
        cout << "Welcome " << user.getName() << "!" << endl;

        // (3) Restaurants dhundho
        cout << "\nRestaurants near you:" << endl;
        vector<Restaurant> list = RestaurantManager::getInstance()
                                    ->searchByLoc("Ahmedabad");

        // (4) Restaurants dikhao
        for (int i = 0; i < list.size(); i++) {
            cout << i << ". " << list[i].getName() << endl;
        }

        // (5) Restaurant choose karo
        int rChoice;
        cout << "Restaurant choose karo: ";
        cin >> rChoice;
        Restaurant selected = list[rChoice];

        // (6) Menu dikhao
        cout << "\nMenu:" << endl;
        vector<MenuItem> menu = selected.getMenu();
        for (int i = 0; i < menu.size(); i++) {
            cout << i << ". "
                 << menu[i].getName()
                 << " - Rs." << menu[i].getPrice()
                 << endl;
        }

        // (7) Item choose karo
        int iChoice;
        cout << "Item choose karo: ";
        cin >> iChoice;
        user.getCart().setRestaurant(&selected);
        user.getCart().addToCart(menu[iChoice]);

        // (8) Factory choose karo
        cout << "\n1. Order Now" << endl;
        cout << "2. Schedule Order" << endl;
        int fChoice;
        cout << "Choose: ";
        cin >> fChoice;

        IOrderFactory* factory = nullptr;
        if (fChoice == 1) {
            factory = new NowOrderFactory();
        } else {
            string time;
            cout << "Schedule time do: ";
            cin >> time;
            factory = new ScheduleOrderFactory(time);
        }

        // (9) Order banao
        Order* order = factory->createOrder(
            1, user, &selected
        );

        // (10) Cart items order mein daalo
        for (auto item : user.getCart().getItems()) {
            order->addItem(item);
        }

        // (11) Payment choose karo
        cout << "\n1. Credit Card" << endl;
        cout << "2. Net Banking" << endl;
        cout << "3. UPI" << endl;
        int pChoice;
        cout << "Payment choose karo: ";
        cin >> pChoice;

        IpaymentStrategy* payment = nullptr;
        if (pChoice == 1)      payment = new CreditCard();
        else if (pChoice == 2) payment = new NetBanking();
        else                   payment = new UPI();

        // (12) Payment karo
        order->setPaymentStrategy(payment);
        order->pay();

        // (13) Order save karo
        OrderManager::getInstance()->addOrder(order);

        // (14) Notify karo
        NotificationService notification(order);
        notification.notifyUser();

        // (15) Sab orders dikhao
        cout << "\nAll Orders:" << endl;
        OrderManager::getInstance()->listOrders();

        // (16) cleanup
        delete factory;
        delete payment;
    }

private:
    void setupRestaurants() {
        Restaurant r1(1, "Pizza Hut", "Ahmedabad");
        r1.addMenuItem(MenuItem("P001", "Margherita", 299));
        r1.addMenuItem(MenuItem("P002", "Pepperoni",  499));

        Restaurant r2(2, "Burger King", "Ahmedabad");
        r2.addMenuItem(MenuItem("B001", "Whopper",    349));
        r2.addMenuItem(MenuItem("B002", "Veg Burger", 199));

        RestaurantManager::getInstance()->addRestaurant(r1);
        RestaurantManager::getInstance()->addRestaurant(r2);
    }
};

// ========================
//   main()
// ========================
int main() {
    Tomato app;
    app.run();
    return 0;
}