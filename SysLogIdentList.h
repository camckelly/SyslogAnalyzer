/*
    Copyright Cambridge "Cam" Kelly, camckelly@gmail.com
    
    TODO: Add license.
*/
#include <string>
#include <iostream>
#include <fstream>

#include "SysLogBucketCollection.h"

using namespace std;

namespace CambridgeSoftware
{

    class SysLogIdentList : public SysLogBucketCollection
    {
        public:

            void do_parse()
            {
                // 1. Open the text file, ie, /var/log/syslog.1
	            ifstream slf( settings.get_logname() );

	            if(slf.is_open()){
		            char cDelim = ' ';
		            string sline;

		            // 2. Loop While Not EOF
		            while( ! slf.eof() ) {
			            string token;
			            int tcnt = 0;

			            // 3. Get one line.
			            getline(slf,sline);
			            istringstream iss{sline};

			            // 4. Get the Column
			            while( ! iss.eof() && tcnt <= settings.get_settings() ){
				            getline(iss,token,cDelim);
				            if( token.size() > 0 )
					            tcnt++;
				            // cout << endl << "**TOKEN= " << token;
			            }
			            // cout << endl << token;
			            token_found(token);

		            }

	            } else {
	                stringstream errms{};
	                errms << "Unable to open log file " << settings.get_logname() ;
	                ls_errors.push_back(errms.str());
	            }

	            return ;
            }

    };
}
