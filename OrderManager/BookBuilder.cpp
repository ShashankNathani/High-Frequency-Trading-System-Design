//
// Created by sebastiend on 19/10/2018.
//

#include "BookBuilder.h"


BookBuilder::BookBuilder(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_,
        std::queue<BookUpdate> &bookbuilder_to_strategy_
):AppBase(strategy_to_ordermanager_,
          ordermanager_to_strategy_,
          ordermanager_to_simulator_,
          simulator_to_ordermanager_,
          bookbuilder_to_strategy_)

{

}