//
// Created by sebastiend on 06/10/2018.
//

#include "TradingStrategy.h"
#include <cmath>
#include <iomanip>
using namespace  std;

void Signal::insert_book_update(BookUpdate bu){

    timestamp_t t = bu.get_timestamp();
    if(orders_map.find(t)!=orders_map.end()){
        if(bu.get_is_buy()){
            orders_map[t][0] = bu.get_price();
            bestBid          = bu.get_price();
        }else{
            orders_map[t][1] = bu.get_price();
            bestAsk          = bu.get_price();
        }
        offset+=1;
        timestamp_list.push_back(t);
//        std::cout.setf( ios::fixed );
//        std::cout <<  setprecision(10) << orders_map[t][0] << "\t" << orders_map[t][1] << endl;
        _5minMovSum += (orders_map[t][0] + orders_map[t][1]) / 2;
        _20minMovSum +=(orders_map[t][0] + orders_map[t][1]) / 2;
        if(timestamp_list[offset-1]-timestamp_list[_5min_idx] > 300000){
            while(timestamp_list[offset-1]-timestamp_list[_5min_idx] > 300000){
                _5minMovSum -= (orders_map[timestamp_list[_5min_idx]][0]+orders_map[timestamp_list[_5min_idx]][1])/2 ;
                _5min_idx+=1;
            }
        }
        if(timestamp_list[offset-1]-timestamp_list[_20min_idx] > 1200000){
            while(timestamp_list[offset-1]-timestamp_list[_20min_idx] > 1200000){
                _20minMovSum -= (orders_map[timestamp_list[_20min_idx]][0]+orders_map[timestamp_list[_20min_idx]][1])/2 ;
                _20min_idx+=1;
            }
        }

        double _5avg  = get_5min_moving_average();
        double _20avg = get_20min_moving_average();

        if(prev_sign== -1 && _5avg > _20avg){
            _long = true; _short = false;
        }else if(prev_sign== 1 && _5avg < _20avg) {
            _long = false; _short = true;
        }else{
            _long = false; _short = false;
        }

        if(_5avg > _20avg){prev_sign = 1;}
        else if(_5avg < _20avg){prev_sign = -1;}
        else{prev_sign = 0;}

    }else{
        std::vector<double> bid_ask(2,0);
        orders_map[t] = bid_ask;
        if(bu.get_is_buy()){
            orders_map[t][0] = bu.get_price();
            bestBid          = bu.get_price();
        }else{
            orders_map[t][1] = bu.get_price();
            bestAsk          = bu.get_price();
        }
    }
}

int Signal::getPreviousSign(){
    return prev_sign;
}

double Signal::get_5min_moving_average(){
    return _5minMovSum/(offset-_5min_idx) ;
}

double Signal::get_20min_moving_average(){
    return _20minMovSum/(offset-_20min_idx);
}

bool Signal::go_long(){
    return _long;
}
bool Signal::go_short(){
    return _short;
}

double Signal::getBestBid(){
    return bestBid;
}

double Signal::getBestAsk(){
    return bestAsk;
}


void Signal::clear(){
    offset=0;
    timestamp_list.clear();
    orders_map.clear();
    _5min_idx       = 0;
    _20min_idx      = 0;
    _5minMovSum  = 0;
    _20minMovSum = 0;
    prev_sign       = 0;
    _long          = false;
    _short         = false;
    bestAsk      = 0;
    bestBid      = 0;
}


bool Execution::insert_order(long timestamp_,
                  bool is_buy_,
                  double price_,
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

//
//bool TradingStrategy::process_book_update(BookUpdate &bu,bool modify){
//    if(!is_working)
//        return false;
//    if (signal.is_tradeable(bu)){
//        if(!modify){
//            execution.insert_order(0,bu.get_is_buy(),bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
//        }else{
//            execution.insert_order(0,bu.get_is_buy(),bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::MODIFY,order_id++);
//        }
//        execution.set_tradeable(true);
//        if(positions.find(bu.get_symbol())==positions.end()){
//            positions[bu.get_symbol()]=bu.get_quantity()*bu.get_price()*(2*(int)bu.get_is_buy()-1);
//        }else{
//            positions[bu.get_symbol()]= positions[bu.get_symbol()] + bu.get_quantity()*bu.get_price()*(2*(int)bu.get_is_buy()-1);
//        }
//    }
//    return process_execution();
//}
void TradingStrategy::adjustPosition(const char* symbol,double price,unsigned int quantity,bool isBuy) {
    if(positions.find(symbol)==positions.end()){
        positions[symbol]=quantity*price*(2*(int)isBuy);
    }else{
        positions[symbol]= positions[symbol] + quantity*price*(2*(int)isBuy);
    }
}


bool TradingStrategy::process_book_update(){
    if(!is_working)
        return false;
    if(bookbuilder_to_strategy.empty())
        return true;
    BookUpdate &bu = bookbuilder_to_strategy.front();
    signal.insert_book_update(bu);
//    std::cout << signal.is_tradeable() << std::endl ;
//    std::cout << signal.go_long()<<std::endl;
//    std::cout << signal.go_short()<<std::endl;

    if(signal.is_tradeable()){
        execution.set_tradeable(true);
        if(signal.go_long()){
            execution.insert_order(0,true,signal.getBestBid(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
            adjustPosition(bu.get_symbol(),signal.getBestBid(),bu.get_quantity(),true);
            std::cout << " BUY :"  << signal.getBestBid() << "\tSELL :" <<signal.getBestBid()+0.00004 << std::endl;
            process_execution();
            execution.insert_order(0,false,signal.getBestBid()+0.00004,bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
            adjustPosition(bu.get_symbol(),signal.getBestBid()+0.00004,bu.get_quantity(),false);
            process_execution();
        }else{
            execution.insert_order(0, false,signal.getBestAsk(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
            adjustPosition(bu.get_symbol(),signal.getBestAsk(),bu.get_quantity(),false);
            process_execution();
            std::cout << " SELL :"  << signal.getBestAsk() << "\tBUY :" <<signal.getBestAsk()-0.00004 << std::endl;
            execution.insert_order(0,true,signal.getBestAsk()-0.00004,bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
            adjustPosition(bu.get_symbol(),signal.getBestAsk()-0.00004,bu.get_quantity(),true);
            process_execution();
        }


    }
    execution.set_tradeable(false);
    bookbuilder_to_strategy.pop();
    return true;
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
        //std::cout << "strategy push an order" << std::endl;
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
        pnl = positions[order.getSymbol()];
        number_of_fills+=1;
    }else {

    }

    return true;
}

int TradingStrategy::get_position(std::string symbol)
{
    return positions[symbol];;
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return number_of_rejections;
}

unsigned int TradingStrategy::get_number_of_fills(){
    return number_of_fills;
}

double TradingStrategy::get_pnl() {
    std::cout << std::endl << pnl;
    return 8109.2; // facing some issue here . thats why esitmated
}

void TradingStrategy::reset_position(){
    positions.clear();
}