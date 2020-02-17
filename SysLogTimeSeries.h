/*
MIT License

Copyright (c) 2019 Cambridge C. Kelly

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>  // Because <chrono> is not for calendar operations.
#include <exception>
#include <iomanip>
#include <locale>

#include "SysLogBucketCollection.h"

using namespace std;

/*
	The purpose of this (unfinished) class is to calculate the time-series
	byMonth-hoizontal graph of log-events for each month
	byDay - events for each day...
*/


namespace CambridgeSoftware
{

class SysLogTimeSeries : public SysLogBucketCollection {
public:

    string cout_bucket_userfriendly(SysLogCountBucket pslcb)
    {
        return  settings.get_friendly_name() + " " + pslcb.get_ident();
    }

    void do_parse()
    {
        // cout.imbue(std::locale("en_US.utf8"));
        // 1. Open the file
        ifstream slf( settings.get_logname() );

        if(slf.is_open()) {
            string sline;

            // 2. Loop While Not EOF
            while( ! slf.eof() ) {
                string token;
                int tcnt = 0;
                struct tm tmrec;

                if(ls_errors.size() > i_max_errors) {
                    throw runtime_error("Too many errors. Stopping.");
                }

                zeroize(&tmrec);

                // 3. Get one line.
                getline(slf,sline);
                istringstream iss{sline};

                try {
                    // TODO: Determine actual column, '15'
                    stringstream ssdate{ sline.substr(0, 15) };


                    // 4. Manually Parse Date: Make time object
                    // cout << endl << ssdate.str() << endl;
                    while( ! iss.eof() && tcnt <= i_max_col ) {
                        getline(iss,token,settings.cDelim);
                        if( token.size() > 0 )
                        {
                            if(tcnt == i_month_col)
                                tmrec.tm_mon = convert_month(token);
                            else if(tcnt == i_mday_col)
                                tmrec.tm_mday = atoi( token.c_str() );
                            else if(tcnt == i_time_col)
                                parse_clock_time(&tmrec, token);
                            tcnt++;
                        }
                    }
                    // TODO: Get year from file's time-attribute; this will need to be function
                    // if ext4, then ctime, but it will be different for diff fs-types.
                    tmrec.tm_year = 2019 - 1900;
                    tmrec.tm_isdst = -1;
                    mktime( &tmrec );

                    // cout << put_time(&tmrec, "%D %T" ) << endl;

                    determine_bucket( tmrec );

                } catch(exception& exm) {
                    ls_errors.push_back( exm.what() );
                }
            } // end while

        } // end if open
    }

private:

    int convert_month(string psm)
    {
        // TODO: Account for locale;
        // TODO: These might be listed in a syslog header somewhere ??
        string list_months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

        for(int x=0; x<12; x++) {
            if( ! psm.compare( list_months[x] ) )
                return x;
        }

        // throw new exception("Month not recognized");

        return -1;
    }

    // Determine what the user wants to count.
    void determine_bucket(tm ptm)
    {
        char buffer[8];
        string tempstr{};

        switch(settings.get_settings()) {
        case byWeek:
            strftime(buffer, 8, "%W", &ptm );
            // cout << "Week is " << buffer << endl;
            tempstr = buffer;
            token_found( tempstr );
            break;

        case DayOfWeek:
            strftime(buffer, 8, "%a", &ptm );
            tempstr = buffer;
            token_found( tempstr );
            break;
        }
    }

    void parse_clock_time(tm *ptmr, string ps)
    {
        char cDelim = ':';
        istringstream _iss{ps};
        string sto;

        try {
            getline(_iss,sto, cDelim);
            ptmr->tm_hour = atoi(sto.c_str()) - 1;

            getline(_iss,sto, cDelim);
            ptmr->tm_min = atoi(sto.c_str());

            getline(_iss,sto, cDelim);
            ptmr->tm_sec = atoi(sto.c_str());
        } catch(exception& ex) {
            stringstream serr{};
            serr << "parse_clock_time() : Didn't understand time '" << ps << "'";
            ls_errors.push_back( serr.str() );
            throw;
        }

    }

    void zeroize(tm *tmrec)
    {
        tmrec->tm_sec = 0;
        tmrec->tm_min = 0;
        tmrec->tm_hour = 0;
        tmrec->tm_mday = 0;
        tmrec->tm_mon = 0;
        tmrec->tm_year = 0;
        tmrec->tm_wday = 0;
        tmrec->tm_yday = 0;
        tmrec->tm_isdst = 0;
        return;
    }


    // TODO: Detect which columns have the time-data by syslog config.
    int i_max_col = 3;
    int i_month_col = 0;
    int i_mday_col = 1;
    int i_time_col = 2;
};

}
