//
// Created by sebastiend on 18/10/2018.
//
// date time handling in boost : https://stackoverflow.com/questions/3786201/how-to-parse-date-time-from-string

#ifndef IEOR_HOMEWORK5_MARKETDATAREADER_H
#define IEOR_HOMEWORK5_MARKETDATAREADER_H
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "BookUpdate.h"
#include "functions.h"
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
#include <iomanip>
using namespace std;

class MDReader
{
    const std::string fileName;
    const std::string delimeter;
    const unsigned int number_of_line;
    const bool is_header;

public:
    MDReader(std::string filename,
            std::string delm = ",",
            unsigned int number_of_line_ = 10,
            bool is_header_ = true) :
            fileName(filename),
            delimeter(delm),
            number_of_line(number_of_line_),
            is_header(is_header_)
    { }

    /*
    * Parses through csv file line by line and returns the data
    * in vector of vector of strings.
    */
    std::vector<BookUpdate> getData()
    {
        unsigned int current_number_of_line=0;
        std::ifstream file(fileName);

        if (!file)
        {
            cerr << "File could not be opened!\n"; // Report error
            cerr << "Error code: " << strerror(errno); // Get some info as to why
            exit(1);
        }


        // Iterate through each line and split the content using delimeter
        bool is_header_handled=false;
        std::vector<BookUpdate> dataList;
        std::string line = "";

        int count = 1;
        while (getline(file, line))
        {
            if(is_header and !is_header_handled)
            {
                is_header_handled=true;
                continue;
            }
            line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
            std::vector<std::string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
            // converting time string to unisigned int //
            timestamp_t epoch = string_to_timestamp(vec[3]);
            std::cout.setf( ios::fixed );
//            cout << "TimeStamp :" << vec[3] << "\tEpoch : "<< epoch<< endl;
            // using the epoch as our unsigned int timestamp //
            BookUpdate b1(0,std::stod(vec[4]),1000000,"GAIN",true,vec[2].c_str(),epoch);
            BookUpdate b2(0,std::stod(vec[5]),1000000,"GAIN",false,vec[2].c_str(),epoch);
            dataList.push_back(b1);
            dataList.push_back(b2);

            if(number_of_line!=0 and current_number_of_line++>=number_of_line)
            {
                break;
            }
            if(count>1000){break;}
            count+=1;
        }
        // Close the File
        file.close();

        return dataList;
    }
};


#endif //IEOR_HOMEWORK5_MARKETDATAREADER_H
