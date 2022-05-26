#include <iostream>

#include "my_unit_test.h"
#include "Order.h"
#include "BookUpdate.h"
#include "MarketSimulator.h"
#include "OrderManager.h"
#include "TradingStrategy.h"
#include "MDReader.h"
#include "BookBuilder.h"
#include <boost/random.hpp>
#include <iomanip>
typedef boost::mt19937 RNGType;
RNGType rng;


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
    std::queue<BookUpdate> bookbuilder_to_strategy;

    MDReader reader("D:\\fdrive\\ProgrammingForFE\\ieorclassproject7\\EUR_USD_Week1.csv",",",0); //MODIFY THIS LINE AND THE LINE 23 in the BookBuilder.h

    // Get the data from CSV File
    cout << "started" << endl;
    std::vector<BookUpdate> dataList = reader.getData();

    // Print the content of row by row on screen if you need
//    for(const BookUpdate &vec : dataList)
//    {
//        std::cout << vec.get_venue() << std::endl;
//        std::cout << vec.get_price() << std::endl;
//        break;
//    }

    /* STEP1: You need to modify the function getData to convert a string into a epoch time */
    /* Example: https://www.epochconverter.com */

    /* Unit test will test to see if the first line of the csv file is well parsed */
    /* You will need to provide all the unit tests you need to perform this verification */
    /* ONLY FOR THE FIRST LINE */
    TEST_FUNCTION(dataList[0].get_price(),(float)1.1595);
    TEST_FUNCTION(dataList[1].get_price(),(float)1.1598);
    TEST_FUNCTION(dataList[0].get_quantity(),1000000);
    TEST_STRING(dataList[0].get_venue(),"GAIN");
    TEST_STRING(dataList[0].get_symbol(),"EUR/USD");
    TEST_FUNCTION(dataList[0].get_is_buy(),true);
    TEST_FUNCTION(dataList[0].get_timestamp(),1535907608910);

    /* STEP2: You need to modify the class Signal to send a Signal to Long or Short based on
     * the 5-min and 20-min moving average.
     * You can create many more unit tests if necessary.
     * The goal here is to be sure that your moving average for 5 min and 20 min works
    */

    Signal sig1;
//    sig1.insert_book_update(BookUpdate(0,10,10000,"GAIN",true,"EURUSD",0));
//    sig1.insert_book_update(BookUpdate(0,12,10000,"GAIN",false,"EURUSD",0));
//    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",280));
//    sig1.insert_book_update(BookUpdate(0,15,10000,"GAIN",false,"EURUSD",280));
//    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",290));
//    sig1.insert_book_update(BookUpdate(0,15,10000,"GAIN",false,"EURUSD",290));
//    TEST_FUNCTION(sig1.get_5min_moving_average(),13.33);
//    sig1.insert_book_update(BookUpdate(0,14,10000,"GAIN",true,"EURUSD",300));
//    sig1.insert_book_update(BookUpdate(0,16,10000,"GAIN",false,"EURUSD",300));
//    TEST_FUNCTION(sig1.get_20min_moving_average(),13.75);
//    TEST_FUNCTION(sig1.go_long(),false);
//    TEST_FUNCTION(sig1.go_short(),false);


    /* STEP3: Generate Signal
     * if the short moving average becomes higher than the long moving average
     * you should go long so it means you will provide a signal to go long
     * if the short moving average becomes lower than the long moving average
     * you should go short so it means you will provide a signal to go short.
     * You will provide the unit tests to verify if the function
     * go_long() and go_short() works
    */
    boost::uniform_int<> rand_num( 10, 16 );
    boost::variate_generator<RNGType,boost::uniform_int<>> rand_price(rng, rand_num);
    double _20minavg = sig1.get_20min_moving_average(); double _5minavg = sig1.get_5min_moving_average();
    for(int i=0;i<dataList.size();i+=2){
        sig1.insert_book_update(dataList[i]);
        sig1.insert_book_update(dataList[i+1]);
//        int p = rand_price();
//        sig1.insert_book_update(BookUpdate(0,p,10000,"GAIN",true,"EURUSD",300+20*i));
//        sig1.insert_book_update(BookUpdate(0,p+1,10000,"GAIN",false,"EURUSD",300+20*i));
        cout.setf( ios::fixed );
        cout << "20 Min Moving Average :" << setprecision(8 )<<sig1.get_20min_moving_average() << "\t 5 Min Moving Average :" << setprecision(8 )<<sig1.get_5min_moving_average() ;

        if(sig1.get_20min_moving_average()>sig1.get_5min_moving_average() && _5minavg>_20minavg){
            cout << "\t GO SHORT" << endl;
            TEST_FUNCTION(sig1.go_long(),false);
            TEST_FUNCTION(sig1.go_short(),true);
        }else if(sig1.get_20min_moving_average()<sig1.get_5min_moving_average() && _5minavg<_20minavg){
            cout << "\t GO LONG" << endl;
            TEST_FUNCTION(sig1.go_long(),true);
            TEST_FUNCTION(sig1.go_short(),false);
        }else{
            cout << "\t NO POSITION "<< endl;
            TEST_FUNCTION(sig1.go_long(),false);
            TEST_FUNCTION(sig1.go_short(),false);
        }
        _5minavg = sig1.get_5min_moving_average(); _20minavg = sig1.get_20min_moving_average();
    }

    sig1.clear();

    /* STEP4: Modify trading strategy to use the class signal correctly
     * It means that you will use go_long and go_short.
     * You will send an order buy or sell based on the return of these functions
     * You will provide the unit tests to test this part
    */


    /* STEP5: Run your Trading Strategy in the back tester
     * In this part the Market Simulator will display the fills that your trading strategy
     * neeed. It won't reject any orders and always fill them
    */

    TradingStrategy ts1(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager,
                        bookbuilder_to_strategy);

    MarketSimulator simulator(strategy_to_ordermanager,
                              ordermanager_to_strategy,
                              ordermanager_to_simulator,
                              simulator_to_ordermanager,
                              bookbuilder_to_strategy);

    OrderManager order_manager(strategy_to_ordermanager,
                               ordermanager_to_strategy,
                               ordermanager_to_simulator,
                               simulator_to_ordermanager,
                               bookbuilder_to_strategy);

    BookBuilder book_builder(strategy_to_ordermanager,
                                ordermanager_to_strategy,
                                ordermanager_to_simulator,
                                simulator_to_ordermanager,
                                bookbuilder_to_strategy);

    /* The book builder fills the queue of BookUpdate
    * for simulation purpose
    */
    book_builder.start();
    simulator.start();
    order_manager.start();
    ts1.start();

    while(!bookbuilder_to_strategy.empty())
    {
        ts1.process_book_update();
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        ts1.process_execution();
        order_manager.handle_execution_order();
        ts1.process_execution();
    }




    /* STEP7: You need to modify the function get_pnl in MarketSimulator and TradingStrategy
     * to calculate the PNL or Just return the value. At the end of the execution you should the PNL
     * in both class be the same.
    */
    TEST_FUNCTION(ts1.get_pnl(),simulator.get_pnl());
    cout << "NET PROFIT IS : " << ts1.get_pnl() << endl;


    PRINT_RESULTS();
    return 0;
}
