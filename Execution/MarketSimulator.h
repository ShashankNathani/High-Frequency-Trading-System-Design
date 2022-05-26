//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_MARKETSIMULATOR_H
#define IEOR_HOMEWORK5_MARKETSIMULATOR_H
#include "AppBase.h"
#include "Order.h"
#include <set>
#include <queue>
#include <string>
#include <iostream>

class MarketSimulator : public AppBase {
public:
    unsigned execution_id;
    std::set<std::string> list_symbols;
    MarketSimulator(
             std::queue<Order> &strategy_to_ordermanager_,
             std::queue<ExecutionOrder> &ordermanager_to_strategy_,
             std::queue<Order> &ordermanager_to_simulator_,
             std::queue<ExecutionOrder> &simulator_to_ordermanager_
            );
    bool handle_order();

    virtual void start() {is_working=true;}
    virtual void stop() {is_working=false;}

};


#endif //IEOR_HOMEWORK5_MARKETSIMULATOR_H
