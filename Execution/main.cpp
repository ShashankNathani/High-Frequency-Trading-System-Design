#include <iostream>

#include "my_unit_test.h"
#include "Order.h"
#include "BookUpdate.h"
#include "MarketSimulator.h"
#include "OrderManager.h"
#include "TradingStrategy.h"


void reset_communication(
        std::queue<Order> &strategy_to_ordermanager,
        std::queue<ExecutionOrder> &ordermanager_to_strategy,
        std::queue<Order> &ordermanager_to_simulator,
        std::queue<ExecutionOrder> &simulator_to_ordermanager
        )
{
    while (!strategy_to_ordermanager.empty()) strategy_to_ordermanager.pop();
    while (!ordermanager_to_strategy.empty()) ordermanager_to_strategy.pop();
    while (!ordermanager_to_simulator.empty()) ordermanager_to_simulator.pop();
    while (!simulator_to_ordermanager.empty()) simulator_to_ordermanager.pop();
}

int main() {
    /* Create an order o1 on the stack with the following parameter
     *
     * (100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT);
     *
     * */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;

    Order o1(100,true,1,10,1000,"JPMX","EURUSD",ordertype::LIMIT);

    TEST_FUNCTION(o1.getID(),1);
    TEST_FUNCTION(o1.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1.getPrice(),10);
    TEST_FUNCTION(o1.getQuantity(),1000);
    TEST_STRING(o1.getVenue(),"JPMX");
    TEST_STRING(o1.getSymbol(),"EURUSD");
    TEST_FUNCTION(o1.is_valid(), true);
    o1.setVenue("\0");
    TEST_FUNCTION(o1.is_valid(), false);
    o1.setVenue("BARX");
    TEST_FUNCTION(o1.is_valid(), true);

    BookUpdate bu1(0,10,10000,"BARX",true,"MSFT");
    MarketSimulator simulator(strategy_to_ordermanager,
                              ordermanager_to_strategy,
                              ordermanager_to_simulator,
                              simulator_to_ordermanager);
    OrderManager order_manager(strategy_to_ordermanager,
                               ordermanager_to_strategy,
                               ordermanager_to_simulator,
                               simulator_to_ordermanager);
    ExecutionOrder eo1;
    TradingStrategy ts1(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);
    simulator.start();
    order_manager.start();
    ts1.start();

    TEST_FUNCTION(ts1.process_book_update(bu1),true);
    const Order o1verif=strategy_to_ordermanager.front();
    TEST_FUNCTION(o1verif.getID(),1);
    TEST_FUNCTION(o1verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1verif.getPrice(),10);
    TEST_FUNCTION(o1verif.getQuantity(),10000);
    TEST_STRING(o1verif.getVenue(),"BARX");
    TEST_STRING(o1verif.getSymbol(),"MSFT");

    TEST_FUNCTION(order_manager.handle_order(),true);

    const Order o2verif=ordermanager_to_simulator.front();
    TEST_FUNCTION(o2verif.getID(),1);
    TEST_FUNCTION(o2verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o2verif.getPrice(),10);
    TEST_FUNCTION(o2verif.getQuantity(),10000);
    TEST_STRING(o2verif.getVenue(),"BARX");
    TEST_STRING(o2verif.getSymbol(),"MSFT");


    simulator.handle_order();
    const ExecutionOrder o3verif=simulator_to_ordermanager.front();
    TEST_FUNCTION(o3verif.getID(),1);
    TEST_FUNCTION(o3verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o3verif.getPrice(),10);
    TEST_FUNCTION(o3verif.getQuantity(),10000);
    TEST_STRING(o3verif.getVenue(),"BARX");
    TEST_STRING(o3verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o3verif.getExecutionID(),0);
    TEST_FUNCTION(o3verif.getState(),orderstate::ACKNOWLEDGED);


    /* Simulator sends a ack */
    TEST_FUNCTION(order_manager.handle_execution_order(),true);
    const ExecutionOrder o4verif=ordermanager_to_strategy.front();
    TEST_FUNCTION(o4verif.getID(),1);
    TEST_FUNCTION(o4verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o4verif.getPrice(),10);
    TEST_FUNCTION(o4verif.getQuantity(),10000);
    TEST_STRING(o4verif.getVenue(),"BARX");
    TEST_STRING(o4verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o4verif.getExecutionID(),0);
    TEST_FUNCTION(o4verif.getState(),orderstate::ACKNOWLEDGED);
    ts1.process_market_response();
    /* Simulator sends a fill */
    TEST_FUNCTION(order_manager.handle_execution_order(),true);
    const ExecutionOrder o5verif=ordermanager_to_strategy.front();
    TEST_FUNCTION(o5verif.getID(),1);
    TEST_FUNCTION(o5verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o5verif.getPrice(),10);
    TEST_FUNCTION(o5verif.getQuantity(),10000);
    TEST_STRING(o5verif.getVenue(),"BARX");
    TEST_STRING(o5verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o5verif.getExecutionID(),0);
    TEST_FUNCTION(o5verif.getState(),orderstate::FILLED);
    ts1.process_market_response();
    TEST_FUNCTION(ts1.get_position("MSFT"),100000);




    /* New Book Update - When trying to get this one the market simulator
     * doesnt have the liquidity any more
     *
     */

    BookUpdate bu2(0,10,10000,"BARX",true,"AAA");
    TEST_FUNCTION(ts1.process_book_update(bu2),true);
    order_manager.handle_order();
    simulator.handle_order();
    order_manager.handle_execution_order();
    ts1.process_market_response();
    TEST_FUNCTION(ts1.get_number_of_rejections(),1);

    for(int k=0;k<8;k++)
    {
        BookUpdate bu3(0,10,10000,"BARX",true,"MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();                // SENDS RESPONSE FOR BOTH FILLED AND ACKNOWLEDGED ORDER
        order_manager.handle_execution_order();  //ORDER ACKNOWLEDGED
        order_manager.handle_execution_order();  //ORDER FILLED
        ts1.process_market_response();      // ACKNOWLEDGED ORDER TREATMENT
        ts1.process_market_response();      // FILLED ORDER TREATMENT
    }

    TEST_FUNCTION(ts1.get_number_of_rejections(),1);
    TEST_FUNCTION(ts1.get_number_of_fills(),9);
    TEST_FUNCTION(ts1.get_position("MSFT"),900000);

    for(int k=0;k<3;k++)
    {
        BookUpdate bu3(0,9,2000,"BARX",false,"MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_number_of_fills(),12);
    TEST_FUNCTION(ts1.get_position("MSFT"),846000);

    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),0);

    for(int k=0;k<3;k++){
        BookUpdate bu3(0, 9, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
    }

    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),3);
    simulator.handle_order();
    order_manager.handle_execution_order();
    ts1.process_market_response();

    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),2);
    TEST_FUNCTION(order_manager.get_number_of_open_orders(),3);

    simulator.stop();
    order_manager.stop();
    ts1.stop();
    reset_communication(strategy_to_ordermanager,
            ordermanager_to_strategy,
            ordermanager_to_simulator,
            simulator_to_ordermanager);
    simulator.start();
    order_manager.start();
    ts1.start();


    for(int k=0;k<12;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }


    TEST_FUNCTION(ts1.get_number_of_rejections(),1);

    for(int k=0;k<10;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_number_of_rejections(),1);

    for(int k=0;k<10;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_position("MSFT"),-64000);


    simulator.stop();
    order_manager.stop();
    ts1.stop();
    reset_communication(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);
    order_manager.reset_position();
    ts1.reset_position();
    simulator.start();
    order_manager.start();
    ts1.start();


    {
        BookUpdate bu3(0, 1, 10000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(order_manager.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_number_of_rejections(),1);

    {
        BookUpdate bu3(0, 100, 10000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }



    TEST_FUNCTION(order_manager.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_number_of_rejections(),2);

    TradingStrategy ts2(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);

    ts2.start();

    {
        BookUpdate bu3(0, 1, 20000, "BARX", false, "MSFT");
        ts2.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts2.process_market_response();
        ts2.process_market_response();
    }

    TEST_FUNCTION(order_manager.get_position("MSFT"),-30000);
    TEST_FUNCTION(ts2.get_position("MSFT"),-20000);
    TEST_FUNCTION(ts2.get_number_of_rejections(),0);

    /* Write the unit tests for amendment after this part */
        
    BookUpdate bu3(0, 1, 20000, "BARX", false, "MSFT");
    ts2.process_book_update(bu3);
    order_manager.handle_order();
    simulator.handle_order();
    order_manager.handle_execution_order();
    TEST_FUNCTION(order_manager.get_position("MSFT"),-50000);
    TEST_FUNCTION(ts2.get_position("MSFT"),-40000);
    ts2.ammend_book_update(30000);
    order_manager.handle_order();
    simulator.handle_order();
    order_manager.handle_execution_order();
    ts2.process_market_response();
    ts2.process_market_response();
    TEST_FUNCTION(ts2.get_position("MSFT"),-50000);
    TEST_FUNCTION(order_manager.get_position("MSFT"),-60000);
    TEST_FUNCTION(ts2.get_number_of_rejections(),0);




    PRINT_RESULTS();
    return 0;
}