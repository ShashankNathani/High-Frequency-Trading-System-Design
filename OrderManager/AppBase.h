//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_APPBASE_H
#define IEOR_HOMEWORK5_APPBASE_H

#include <queue>
#include "Order.h"
#include "BookUpdate.h"

class AppBase
{
protected:
    bool is_working;
    std::queue<Order> &strategy_to_ordermanager;
    std::queue<ExecutionOrder> &ordermanager_to_strategy;
    std::queue<Order> &ordermanager_to_simulator;
    std::queue<ExecutionOrder> &simulator_to_ordermanager;
    std::queue<BookUpdate> &bookbuilder_to_strategy;

public:
    AppBase(std::queue<Order> &strategy_to_ordermanager_,
    std::queue<ExecutionOrder> &ordermanager_to_strategy_,
    std::queue<Order> &ordermanager_to_simulator_,
    std::queue<ExecutionOrder> &simulator_to_ordermanager_,
    std::queue<BookUpdate> &bookbuilder_to_strategy_):
            strategy_to_ordermanager(strategy_to_ordermanager_),
            ordermanager_to_strategy(ordermanager_to_strategy_),
            ordermanager_to_simulator(ordermanager_to_simulator_),
            simulator_to_ordermanager(simulator_to_ordermanager_),
            bookbuilder_to_strategy(bookbuilder_to_strategy_)
    {
        is_working=false;
    }
    virtual void start() = 0;
    virtual void stop() = 0;
                    
};


#endif //IEOR_HOMEWORK5_APPBASE_H
