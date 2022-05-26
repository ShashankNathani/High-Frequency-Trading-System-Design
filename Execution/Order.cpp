//
// Created by sebastiend on 06/10/2018.
//

#include "Order.h"

const char * Order::getVenue() const{
    return venue;
}

const char * Order::getSymbol() const{
    return symbol;
}

unsigned int Order::getID() const{
    return id;
}

ordertype Order::getOrderType() const{
    return type;
}

void Order::setType(ordertype e){
    type=e;
}

unsigned int Order::getQuantity() const{
    return quantity;
}

unsigned int Order::getPrice() const{
    return price;
}

bool Order::is_valid() const{
    return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
}

void Order::setVenue(const char * venue_){
    strcpy(venue,venue_);
}

void Order::setQuantity(unsigned int quantity_){
    quantity=quantity_;
}

void Order::setSymbol(const char * symbol_){
    strcpy(symbol,symbol_);
}

void Order::setPrice(unsigned int price_){
    price=price_;
}

void Order::setSide(bool is_buy_){
    is_buy = is_buy_;
}

void Order::setOrderID(unsigned int id_){
    id=id_;
}

unsigned int Order::getTimeStamp() const{
    return timestamp;
}

bool Order::isBuy() const{
    return is_buy;
}