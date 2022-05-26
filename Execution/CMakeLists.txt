cmake_minimum_required(VERSION 3.12)
project(finm_project6)

set(CMAKE_CXX_STANDARD 14)

add_executable(finm_project6 main.cpp my_unit_test.cpp my_unit_test.h OrderManager.cpp OrderManager.h MarketSimulator.cpp MarketSimulator.h TradingStrategy.cpp TradingStrategy.h Order.cpp Order.h BookUpdate.cpp BookUpdate.h AppBase.cpp AppBase.h)
