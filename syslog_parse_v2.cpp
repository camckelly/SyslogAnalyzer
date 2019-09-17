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
#include <iostream>
#include <string>
// #include <locale>
#include <memory>

#include "SyslogCountSettings.h"
#include "SysLogIdentList.h"
#include "SysLogTimeSeries.h"

using namespace std;
using namespace CambridgeSoftware;

void print_usage()
{
    cout << "The purpose of this program is to count something of your choice" << endl;
    cout << "in the system log, 'syslog'. Output is a horizontal bar-graph" << endl;
    cout << "of asterisk's." << endl;
    cout << "Usage:" << endl;
    cout << "syslog_parse.x (path to a syslog file) (something to count, month|hour|day|host|ident|week|dayofweek)" << endl;
    cout << "syslog_parse.x /var/log/syslog.1" << endl;
    cout << "syslog_parse.x /var/log/syslog month" << endl;
    cout << "" << endl;
    return ;
}


int main(int argc,char* argv[])
{
    // Cmd-line option, otherwise, print_usage()
    if(argc <= 1) {
        print_usage();
        return 0;
    }

    // Prepare Confg Settings
    SyslogCountSettings config;
    config.check_parameters(argc,argv);

    if( config.args_not_sane() ) {
        cout << "Didn't understand those command-line options." << endl;
        return 0;
    }


    {
        unique_ptr<SysLogBucketCollection> counter;
        if(config.get_settings() > Ident)
        {
            counter = make_unique<SysLogTimeSeries>( );
        } else {
            counter = make_unique<SysLogIdentList>( );
        }
        try {
            counter->set_config( config );
            counter->do_parse();
            cout << *counter;
            cerr << counter->get_errors();
        } catch(exception& ex) {
            cout << ex.what() << endl;
            cerr << ex.what() << endl;
        }
    }

    return 0;
}

