//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_TRADINGSTRATEGY_H
#define IEOR_HOMEWORK5_TRADINGSTRATEGY_H
#include "AppBase.h"
#include "BookUpdate.h"
#include "Order.h"
#include <unordered_map>
#include <string>
#include <iostream>

class Signal{
public:
    bool is_tradeable(BookUpdate &bu){return true;}
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      unsigned int price_,
                      unsigned int quantity_,
                      const char * venue_,
                      const char * symbol_,
                      ordertype type_,
                      unsigned int id_);
    bool is_tradeable() {return tradeable;}
    void set_tradeable(bool is_tradable){tradeable=is_tradable;};
    Order & get_order() {return e;}
};

class TradingStrategy : public AppBase {
private:
    Signal signal;
    Execution execution;
    int order_id;
    std::unordered_map<std::string,int> positions;
    unsigned int number_of_rejections=0;
    unsigned int number_of_fills=0;

public:
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_),
              signal(),
              execution(),
              order_id(1),
              number_of_rejections(0),
              number_of_fills(0){}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
        // number_of_rejections = 0;
        // number_of_fills = 0;
        // order_id = 1;
    }

    bool ammend_book_update(unsigned int q);
    bool process_book_update(BookUpdate &bu,bool modify=false);
    bool process_execution();
    bool process_market_response();
    int get_position(std::string symbol);
    unsigned int get_number_of_rejections();
    unsigned int get_number_of_fills();
    void reset_position();

};


#endif //IEOR_HOMEWORK5_TRADINGSTRATEGY_H
