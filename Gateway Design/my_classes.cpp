//
// Created by sebastiend on 21/09/2018.
//

#include "my_classes.h"
#include <unordered_map>
#include <vector>

string getField(string &org,string fnd ){
    int start = org.find(fnd)+fnd.size();
    int end = start;
    while(org[end]!='|'){++end;}
    return org.substr(start,end-start); 
}

vector<string> string_split(string str,char sep){
    size_t start;
    size_t end = 0;
    vector <string> out(6,"") ;
    int i=0;
    while ((start = str.find_first_not_of(sep, end)) != string::npos)
    {
        end = str.find(sep, start);
        out[i]=str.substr(start, end - start);
        i+=1;
    }
    return out;
}

Order::Order(long long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_, unsigned int quantity_, const char * venue_, const char * symbol_,
                ordertype type_){
    is_buy=is_buy_;
    timestamp=timestamp_;
    id=id_;
    price=price_;
    quantity=quantity_;
    strcpy(venue,venue_);
    type=type_;
    strcpy(symbol,symbol_);
}


char* Order::getVenue(){
    return venue;
}

char* Order::getSymbol(){
    return symbol;
}

unsigned int Order::getID(){
    return id;
}

ordertype Order::getOrderType(){
    return type;
}

void Order::setType(ordertype e){
    type=e; 
}

unsigned int Order::getQuantity(){
    return quantity;
}

unsigned int Order::getPrice(){
    return price;
}

bool Order::is_valid(){
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

unsigned int Order::getOutstandingQuantity(){
    return 1;
}

void Order::setAction(unsigned int){};


//  Price update object - derived from order and contains additional order action //

PriceUpdate::PriceUpdate(long long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_, unsigned int quantity_, const char * venue_, const char * symbol_,
                ordertype type_): Order(timestamp_, is_buy_,id_,price_,quantity_,venue_, symbol_,type_){
    action = order_action::modify ;
}

order_action PriceUpdate::getAction(){
    return action;
}

void PriceUpdate::setAction(order_action oe) {
    action=oe;
}

// Message Object : contatins 3 other objects : message, order and price update //
// 3 setters and getters for the objects ///

Message::Message(){
    type = message_type::none ;
}

void Message::setMessageType(message_type mt){
    type=mt;
}

bool Message::setPriceUpdate(PriceUpdate* pu){
    price_update.setPrice(pu->getPrice());
    price_update.setQuantity(pu->getQuantity());
    price_update.setOrderID(pu->getID());
    price_update.setSymbol(pu->getSymbol());
    price_update.setVenue(pu->getVenue());
    price_update.setType(pu->getOrderType());
    return true;
}

bool Message::setOrder(Order* new_ord){
    order.setPrice(new_ord->getPrice());
    order.setQuantity(new_ord->getQuantity());
    order.setOrderID(new_ord->getID());
    order.setSymbol(new_ord->getSymbol());
    order.setVenue(new_ord->getVenue());
    order.setType(new_ord->getOrderType());
    return true;
}


message_type Message::getMessageType(){
    return type;
}

Order& Message::getOrder(){
    return order;
}

PriceUpdate& Message::getPriceUpdate(){
    return price_update;
}

// Composer : base composer - derived fix and sebx composer //

string Composer::compose(Message& message){
    string return_string= "";
    return return_string;
};

string FIX42Composer::compose(Message& message){
    string msg_string= "8=FIX.4.2|9=124|35=D|34=2|49=DONALD|52=20160613-22:52:37.227|56=VENUE|";
    msg_string += "11=" + to_string(message.getOrder().getID()) + "|21=3|38=";
    msg_string += to_string(message.getOrder().getQuantity()) + "|40=2|44=";
    msg_string += to_string(message.getOrder().getPrice()) + "|54=1|55=MQ|60=20160613-22:52:37.227|10=163|";
    return msg_string;
};

string SEBXComposer::compose(Message& message){
    string msg_string = "";
    msg_string += "NEWORDER|";
    msg_string += to_string(message.getOrder().getPrice()) + "|";
    msg_string += to_string(message.getOrder().getQuantity())+ "|";
    msg_string += "SEBX|";
    msg_string += (string)message.getOrder().getSymbol()+'|';
    return msg_string;
}

// Parsers - Base parser - derived fix and sebx parser //

    /// FIX PARSER ///

bool FIX42Parser::parse(string message_text, Message& message,string venue){
    char msgtype_val = getField(message_text,"35=")[0];
    message_type mt;
    switch(msgtype_val){
        case 'A' : mt = message_type::logon; break;
        case '0' : mt = message_type::heartbeat; break;
        case '5' : mt = message_type::logout; break;
        case 'D' : mt = message_type::new_order; break;
        case 'W' : mt = message_type::full_snapshot; break;
        case 'X' : mt = message_type::incremental_snapshot; break;
        case '8' : mt = message_type::execution; break;
        default  : mt = message_type::none;break;
    };
    message.setMessageType(mt);
    switch (mt){
        case message_type::incremental_snapshot: return parse_incremental_refresh(message_text,message,venue);
        case message_type::new_order: return parse_new_order(message_text,message,venue);
    }
    return true;

}

bool FIX42Parser::parse_incremental_refresh(string message_text, Message& message, string venue){

    unsigned int id        = stoi(getField(message_text,"37="));
    unsigned int quantity  = stoi(getField(message_text,"271="));
    unsigned int price     = stoi(getField(message_text,"270="));
    PriceUpdate* pu = new PriceUpdate(0,true,id,price,quantity,venue.c_str());
    message.setPriceUpdate(pu);
    delete pu;
    return true;    
};

bool FIX42Parser::parse_new_order(string message_text, Message& message, string venue){
    unsigned int id         = stoi(getField(message_text,"11="));
    unsigned int quantity   = stoi(getField(message_text,"38="));
    unsigned int price      = stoi(getField(message_text,"44="));
    string symbol           = getField(message_text,"55=");
    Order* new_ord = new Order(0,true,id,price,quantity,venue.c_str(),symbol.c_str());
    message.setOrder(new_ord);
    delete new_ord;
    return true; 
};

    /// SEBX PARSER ///

bool SEBXParser::parse(string message_text, Message& message,string venue){ 
    string msg_type = message_text.substr(0,message_text.find('|'));
    message_type mt;
    if(msg_type=="LOGON"){mt = message_type::logon;}
    else if(msg_type=="HEARTBEAT"){mt = message_type::heartbeat;}
    else if(msg_type=="LOGOUT"){mt = message_type::logout;}
    else if(msg_type=="NEWORDER"){mt = message_type::new_order; }
    else if(msg_type=="FULLSNAPSHOT"){mt = message_type::full_snapshot;}
    else if(msg_type=="INCREMENTALSNAPSHOT"){mt = message_type::incremental_snapshot;}
    else if(msg_type=="EXECUTION"){mt = message_type::execution;}
    else{mt = message_type::none;}
    message.setMessageType(mt);
    switch (mt){
        case message_type::incremental_snapshot: return parse_incremental_refresh(message_text,message,venue);
        case message_type::new_order: return parse_new_order(message_text,message,venue);
    }

    return true;
}

bool SEBXParser::parse_incremental_refresh(string message_text, Message& message,string venue){
    vector<string> fields   = string_split(message_text,'|');
    unsigned int quantity   = stoi(fields[2]);
    unsigned int price      = stoi(fields[1]);
    string symbol           = fields[4]; 
    venue                   = fields[3];
    PriceUpdate* pu         = new PriceUpdate(0,true,1,price,quantity,venue.c_str(),symbol.c_str());
    message.setPriceUpdate(pu);
    delete pu;
    return true;    
}

bool SEBXParser::parse_new_order(string message_text, Message& message,string venue){
    vector<string> fields   = string_split(message_text,'|');
    unsigned int quantity   = stoi(fields[2]);
    unsigned int price      = stoi(fields[1]);
    string symbol           = fields[4]; 
    venue                   = fields[3];
    Order* new_ord          = new Order(0,true,1,price,quantity,venue.c_str(),symbol.c_str());
    message.setOrder(new_ord);
    delete new_ord;
    return true;
}

// App Base - just a swithc to turn on and off the derived gateway class //
AppBase::AppBase(){
    is_working=false;
}

// defining the Gateway object //
void Gateway::start(){
    is_working = true;
}
void Gateway::stop(){
    is_working = false;
}

bool Gateway::add_parser(string venue, Parser *parser){
    if(is_working && list_parser.find(venue)==list_parser.end()){
        list_parser[venue]=parser;
        return true;
    }
    return false;
}

bool Gateway::add_composer(string venue, Composer *composer){
    if(is_working && list_composer.find(venue)==list_composer.end()){
        list_composer[venue]=composer;
        return true;
    }
    return false;
}


bool Gateway::process_message_from_exchange_for_price_update(string venue, string message_to_parse){
    if(is_working){
        return list_parser[venue]->parse(message_to_parse,stored_message,venue);
    }
    return false;
};

bool Gateway::process_message_from_exchange_for_order(string venue, string message_to_parse){
    if(is_working){
        return list_parser[venue]->parse(message_to_parse,stored_message,venue);
    }
    return false;
};

string Gateway::send_message_for_order(string venue,Message& me){
    string return_string = list_composer[venue]->compose(me);
    return return_string;
};

Message& Gateway::return_stored_message(){
    return stored_message;
}
