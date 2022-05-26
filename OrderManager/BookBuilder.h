//
// Created by sebastiend on 19/10/2018.
//

#ifndef IEOR_HOMEWORK5_BOOKBUILDER_H
#define IEOR_HOMEWORK5_BOOKBUILDER_H
#include "AppBase.h"
#include "MDReader.h"

class BookBuilder: public AppBase {
public:
    BookBuilder(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    );
    virtual void start(){
        /* Tips: if you find the execution too long when testing you should specify the number of lines
         * you want to read by changing 0 by the number of lines
        */
        MDReader reader("D:\\fdrive\\ProgrammingForFE\\ieorclassproject7\\EUR_USD_Week1.csv",",",0); //MODIFY THIS LINE AND THE LINE 39 in main.cpp

        // Get the data from CSV File
        std::vector<BookUpdate> dataList = reader.getData();

        // Send the book update to the trading strategy
        for(const BookUpdate &bu : dataList)
        {
            bookbuilder_to_strategy.push(bu);
        }
    };
    virtual void stop(){

    };

};


#endif //IEOR_HOMEWORK5_BOOKBUILDER_H
