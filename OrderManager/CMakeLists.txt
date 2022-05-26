cmake_minimum_required(VERSION 3.12)
project(finm_project7)

set(CMAKE_CXX_STANDARD 14)

add_executable(finm_project7 main.cpp my_unit_test.cpp functions.cpp OrderManager.cpp OrderManager.h MarketSimulator.cpp MarketSimulator.h functions.h TradingStrategy.cpp TradingStrategy.h Order.cpp Order.h BookUpdate.cpp BookUpdate.h AppBase.cpp AppBase.h MDReader.cpp MDReader.h BookBuilder.cpp BookBuilder.h)
