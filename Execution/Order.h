//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_ORDER_H
#define IEOR_HOMEWORK5_ORDER_H

#include <string.h>
enum ordertype {LIMIT, MARKET, MODIFY};
enum orderstate {OPEN, ACKNOWLEDGED, FILLED, CANCELLED, REJECTED};


class Order{
private:
    long timestamp; // epoch time: the number of seconds that have elapsed since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970,.
    bool is_buy; // buy or sell
    unsigned int id; // order id – unique identifier
    unsigned int price;
    unsigned int quantity;
    char venue[20]; // company name where this order comes from
    char symbol[20];
    ordertype type;
public:
    Order(long timestamp_=0,
          bool is_buy_=true,
          unsigned int id_=0,
          unsigned int price_=0,
          unsigned int quantity_=0,
          const char * venue_="",
          const char * symbol_="",
          ordertype type_=ordertype::MARKET)
    {
        is_buy=is_buy_;
        timestamp=timestamp_;
        id=id_;
        price=price_;
        quantity=quantity_;
        strcpy(venue,venue_);
        type=type_;
        strcpy(symbol,symbol_);
    }

    const char * getVenue() const;
    const char * getSymbol() const;
    unsigned int getID() const;
    ordertype getOrderType() const;
    void setType(ordertype e);
    bool is_valid() const;
    unsigned int getQuantity() const;
    unsigned int getPrice() const;
    void setVenue(const char * venue_);
    void setQuantity(unsigned int quantity_);
    void setSymbol(const char * symbol_);
    void setPrice(unsigned int price_);
    void setSide(bool is_buy_);
    void setOrderID(unsigned int id_);
    unsigned int getTimeStamp() const;
    bool isBuy() const;

};


class ExecutionOrder : public Order
{
private:
    orderstate state;
    unsigned int execution_id;
public:
    ExecutionOrder():Order(),state(orderstate::OPEN) {}
    ExecutionOrder(const Order &o):Order(o.getTimeStamp(),
            o.isBuy(),o.getID(),o.getPrice(),o.getQuantity(),
            o.getVenue(),o.getSymbol(),o.getOrderType()){};
    orderstate getState() const {return state;};
    void setState(orderstate e){state=e;}
    void setExecutionID(unsigned int id){execution_id=id;}
    unsigned int getExecutionID() const { return execution_id;}
};

#endif //IEOR_HOMEWORK5_ORDER_H
