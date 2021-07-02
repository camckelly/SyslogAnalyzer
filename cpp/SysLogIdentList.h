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
#include <string>
#include <iostream>
#include <fstream>

#include "SysLogBucketCollection.h"

using namespace std;

namespace CambridgeSoftware
{

class SysLogIdentList : public SysLogBucketCollection {
public:

    string cout_bucket_userfriendly(SysLogCountBucket pslcb)
    {
        return pslcb.get_ident();
    }

    void do_parse()
    {
        // 1. Open the text file, ie, /var/log/syslog.1
        ifstream slf( settings.get_logname() );

        if(slf.is_open()) {
            string sline;

            // 2. Loop While Not EOF
            while( ! slf.eof() ) {
                string token;
                int tcnt = 0;

                // 3. Get one line.
                getline(slf,sline);
                istringstream iss{sline};

                // 4. Get the Column
                while( ! iss.eof() && tcnt <= settings.get_settings() ) {
                    getline(iss,token,settings.cDelim);
                    if( token.size() > 0 )
                        tcnt++;
                }
                string::size_type n = token.find("[");
                if(n != string::npos)
                    token = token.substr(0, n);
                token_found(token);
            }

        } else {
            stringstream errms{};
            errms << "Unable to open log file " << settings.get_logname() << ", might need 'admin' privileges?";
            ls_errors.push_back(errms.str());
        }

        return ;
    }

};

}
