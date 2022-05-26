//
// Created by sebastiend on 06/10/2018.
//

#ifndef IEOR_HOMEWORK5_BOOKUPDATE_H
#define IEOR_HOMEWORK5_BOOKUPDATE_H

#include <string.h>

typedef unsigned int price_t;
typedef unsigned int quantity_t;
typedef unsigned int level_t;



class BookUpdate {
private:
    level_t level;
    price_t price;
    quantity_t quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
public:
    BookUpdate(level_t level_,
            price_t price_,
            quantity_t quantity_,
            const char *venue_,
            bool is_buy_,
            const char *symbol_):
            level(level_),
            price(price_),
            quantity(quantity_),
            is_buy(is_buy_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    price_t get_price(){return price;}
    quantity_t get_quantity(){return quantity;}
    level_t get_level(){return level;}
    const char * get_venue(){return venue;}
    bool get_is_buy(){return is_buy;}
    const char * get_symbol(){return symbol;}
};


#endif //IEOR_HOMEWORK5_BOOKUPDATE_H
