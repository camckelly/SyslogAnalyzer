/*
    Copyright Cambridge "Cam" Kelly, camckelly@gmail.com
    
    TODO: Add license.
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

