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
#include <vector>
#include <unordered_map>

class Signal{
private :
    int offset=0;
    std::vector<timestamp_t> timestamp_list;
    std::unordered_map<timestamp_t,std::vector<double>> orders_map;
    int _5min_idx       = 0;
    int _20min_idx      = 0;
    double _5minMovSum  = 0;
    double _20minMovSum = 0;
    int prev_sign       = 0;
    bool _long          = false;
    bool _short         = false;
    double bestAsk      = 0;
    double bestBid      = 0;

public:
    void insert_book_update(BookUpdate bu);
    double get_5min_moving_average();
    double get_20min_moving_average();
    bool go_long();
    bool go_short();
    int getPreviousSign();
    bool is_tradeable(){return go_short() || go_long();};
    void clear();
    double getBestBid();
    double getBestAsk();
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      double price_,
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
    std::unordered_map<std::string,double> positions;
    unsigned int number_of_rejections;
    unsigned int number_of_fills;
    unsigned int pnl;

public:
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_,
              bookbuilder_to_strategy_),
              signal(),
              execution(),
              order_id(1),
              number_of_rejections(0),
              number_of_fills(0),
              pnl(0){}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
    }


//    bool process_book_update(BookUpdate &bu,bool modify= false);
    bool process_book_update();
    bool process_execution();
    bool process_market_response();
    int get_position(std::string symbol);
    unsigned int get_number_of_rejections();
    unsigned int get_number_of_fills();
    void reset_position();
    void adjustPosition(const char* symbol,double price,unsigned int quantity,bool isBuy);
    double get_pnl();

};


#endif //IEOR_HOMEWORK5_TRADINGSTRATEGY_H
