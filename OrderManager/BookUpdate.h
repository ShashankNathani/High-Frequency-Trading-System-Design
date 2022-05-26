//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_BOOKUPDATE_H
#define IEOR_HOMEWORK5_BOOKUPDATE_H

#include <string.h>
typedef double price_t;
typedef unsigned int quantity_t;
typedef unsigned int level_t;
typedef unsigned  long long timestamp_t;



class BookUpdate {
private:
    level_t level;
    price_t price;
    quantity_t quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
    timestamp_t epoch_time;
public:
    BookUpdate(level_t level_,
            price_t price_,
            quantity_t quantity_,
            const char *venue_,
            bool is_buy_,
            const char *symbol_,
           timestamp_t epoch_time_):
            level(level_),
            price(price_),
            quantity(quantity_),
            is_buy(is_buy_),
            epoch_time(epoch_time_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    price_t get_price() const {return price;}
    quantity_t get_quantity() const {return quantity;}
    level_t get_level() const {return level;}
    char * get_venue() {return venue;}
    bool get_is_buy() const {return is_buy;}
    char * get_symbol() {return symbol;}
    timestamp_t get_timestamp() const{return epoch_time;}
};



#endif //IEOR_HOMEWORK5_BOOKUPDATE_H
