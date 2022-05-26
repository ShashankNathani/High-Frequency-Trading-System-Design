//
// Created by sebastiend on 06/10/2018.
//

#include "OrderManager.h"


bool OrderManager::handle_execution_order(){
    if(!is_working)
        return false;
    if (simulator_to_ordermanager.empty())
        return true;

    const ExecutionOrder &e = simulator_to_ordermanager.front();
    simulator_to_ordermanager.pop();
    ordermanager_to_strategy.push(e);

    return true;
};


unsigned int OrderManager::get_number_of_open_orders(){
    // order manager to simulator will always contain pair of filled and acknowledged . Open order will thus be floor(size/2) or size/2 + size%2
    return (simulator_to_ordermanager.size()/2) + (simulator_to_ordermanager.size()%2) + ordermanager_to_simulator.size();
}


unsigned int OrderManager::get_number_of_non_acknowledged_orders()
{
    return ordermanager_to_simulator.size();
}

int OrderManager::get_position(std::string symbol)
{
    return positions[symbol];
}

bool OrderManager::handle_order(){
    if(!is_working)
        return false;
    if (strategy_to_ordermanager.empty())
        return true;
    Order &e = strategy_to_ordermanager.front();

    if(e.getQuantity()>10 && e.getPrice()*e.getQuantity()<1000000){
        if(e.getOrderType()==MODIFY){
            positions[e.getSymbol()] = positions[e.getSymbol()] + e.getQuantity()*e.getPrice()*(2*(int)e.isBuy()-1); 
        }else if(positions.find(e.getSymbol())==positions.end()){
            positions[e.getSymbol()]=e.getQuantity()*e.getPrice()*(2*(int)e.isBuy()-1);
        }else{
            positions[e.getSymbol()]= positions[e.getSymbol()] + e.getQuantity()*e.getPrice()*(2*(int)e.isBuy()-1);
        }
    }

    strategy_to_ordermanager.pop();

    ordermanager_to_simulator.push(e);

    return true;
};                                                                  
