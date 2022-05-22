//
// Created by sebastiend on 21/09/2018.
//

#ifndef OEOR_HOMEWORK2_MY_CLASSES_H
#define OEOR_HOMEWORK2_MY_CLASSES_H

#include "my_functions.h"
#include <string.h>
#include <stdlib.h>
#include <map>
using namespace std;

enum ordertype {LIMIT, MARKET};
enum order_action {add, modify, suppress};
enum message_type {none, logon, logout, new_order, execution, full_snapshot, heartbeat, incremental_snapshot};

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
    Order(long long timestamp_=0, bool is_buy_=true, unsigned int id_=0, unsigned int price_=0, unsigned int quantity_=0, const char * venue_="", const char * symbol_="",
            ordertype type_=ordertype::LIMIT);
    char * getVenue();
    char * getSymbol();
    unsigned int getID();
    ordertype getOrderType();
    void setType(ordertype e);
    unsigned int getQuantity();
    unsigned int getPrice();
    bool is_valid();
    void setVenue(const char * venue_);
    void setQuantity(unsigned int quantity_);
    void setSymbol(const char * symbol_);
    void setPrice(unsigned int price_);
    void setSide(bool is_buy_);
    void setOrderID(unsigned int id_);
    virtual unsigned int getOutstandingQuantity();
    virtual void setAction(unsigned int);
};


class PriceUpdate : public Order
{
private:
    order_action action;
public:
    PriceUpdate(long long timestamp_=0, bool is_buy_=true, unsigned int id_=0, unsigned int price_=0, unsigned int quantity_=0, const char * venue_="", const char * symbol_="",
            ordertype type_=ordertype::LIMIT);
    order_action getAction();
    void setAction(order_action oe);
};

class Message
{
private:
    message_type type;
    Order order=order_action::modify ;
    PriceUpdate price_update;
public:
    Message();
    string message_string;
    void setMessageType(message_type mt);
    bool setOrder(Order* new_ord);
    bool setPriceUpdate(PriceUpdate* pu);
    bool setMessage(string &msg_str);
    message_type getMessageType();
    Order& getOrder();
    PriceUpdate& getPriceUpdate();
};

class Parser{
public:
    virtual bool parse(string message_text, Message& message,string venue)=0;
};

class Composer{
public:
    virtual string compose(Message& message);
};

class FIX42Parser : public Parser{
public:

    virtual bool parse(string message_text, Message& message,string venue);
    virtual bool parse_incremental_refresh(string message_text, Message& message,string venue);
    virtual bool parse_new_order(string message_text, Message& message,string venue);
};

class SEBXParser : public Parser{
public:

    virtual bool parse(string message_text, Message& message,string venue);
    virtual bool parse_incremental_refresh(string message_text, Message& message,string venue);
    virtual bool parse_new_order(string message_text, Message& message,string venue);
};

class FIX42Composer:public Composer{
public:
    virtual string compose(Message& message);
};

class SEBXComposer:public Composer{
public:
    virtual string compose(Message& message);
};



class AppBase
{
protected:
    bool is_working;
public:
    AppBase();
    virtual void start() = 0;
    virtual void stop() = 0;

};

class Gateway : public AppBase
{
public:
    map<string, Parser *> list_parser;
    map<string, Composer *> list_composer;
    Message stored_message;
    void start();
    void stop();
    bool add_parser(string venue, Parser *parser);
    bool add_composer(string venue, Composer *composer);
    bool process_message_from_exchange_for_price_update(string venue, string message_to_parse);
    bool process_message_from_exchange_for_order(string venue, string message_to_parse);
    string send_message_for_order(string venue,Message& me);
    Message& return_stored_message();

};





#endif //OEOR_HOMEWORK2_MY_CLASSES_H


// different Message Type Values :
// {   A : logon
//     0 : Heartbeat
//     5 : logout
//     D : new_order
//     W : full_snapshot
//     X : incremental_snapshot
//     8 : execution}