//
// Created by sebastiend on 06/10/2018.
//

#include "TradingStrategy.h"


bool Execution::insert_order(long timestamp_,
                  bool is_buy_,
                  unsigned int price_,
                  unsigned int quantity_,
                  const char * venue_,
                  const char * symbol_,
                  ordertype type_,
                  unsigned int id_)
{
    e.setSide(is_buy_);
    e.setPrice(price_);
    e.setQuantity(quantity_);
    e.setVenue(venue_);
    e.setType(type_);
    e.setSymbol(symbol_);
    e.setOrderID(id_);
    return true;
};

bool TradingStrategy::process_book_update(BookUpdate &bu,bool modify){
    if(!is_working)
        return false;
    if (signal.is_tradeable(bu)){
        if(!modify){
            execution.insert_order(0,bu.get_is_buy(),bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
        }else{
            execution.insert_order(0,bu.get_is_buy(),bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::MODIFY,order_id++);
        }
        execution.set_tradeable(true);
        if(positions.find(bu.get_symbol())==positions.end()){
            positions[bu.get_symbol()]=bu.get_quantity()*bu.get_price()*(2*(int)bu.get_is_buy()-1);
        }else{
            positions[bu.get_symbol()]= positions[bu.get_symbol()] + bu.get_quantity()*bu.get_price()*(2*(int)bu.get_is_buy()-1);
        }
    }
    return process_execution();
}

bool TradingStrategy::process_execution() {
    if(!is_working)
        return false;
    Order order;
    if (execution.is_tradeable()) {
        order.setType(execution.get_order().getOrderType());
        order.setVenue(execution.get_order().getVenue());
        order.setQuantity(execution.get_order().getQuantity());
        order.setPrice(execution.get_order().getPrice());
        order.setOrderID(execution.get_order().getID());
        order.setSymbol(execution.get_order().getSymbol());
        order.setSide(execution.get_order().isBuy());
        execution.set_tradeable(false);
        strategy_to_ordermanager.push(order);
    }
    return true;
};

bool TradingStrategy::process_market_response()
{
    if(ordermanager_to_strategy.empty())
        return true;
    ExecutionOrder &order = ordermanager_to_strategy.front();
    ordermanager_to_strategy.pop();
    if(order.getState()==REJECTED){
        number_of_rejections+=1;
        positions[order.getSymbol()]= positions[order.getSymbol()] - order.getQuantity()*order.getPrice()*(2*(int)order.isBuy()-1);
    }else if(order.getState()==FILLED){
        number_of_fills+=1;
    }else {
        
    }

    return true;
}

bool TradingStrategy::ammend_book_update(unsigned int q){
    ExecutionOrder &order = ordermanager_to_strategy.front();
    ordermanager_to_strategy.pop();
    if(order.getState()==ACKNOWLEDGED){
        BookUpdate bu(0, order.getPrice(),q-order.getQuantity(), order.getSymbol(), order.isBuy(), order.getSymbol());
        process_book_update(bu,true);
    }else{
        return false;
    }

}

int TradingStrategy::get_position(std::string symbol)
{
    return positions[symbol];
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return number_of_rejections;
}

unsigned int TradingStrategy::get_number_of_fills() {
    return number_of_fills;
}

void TradingStrategy::reset_position(){
    positions.clear();
}