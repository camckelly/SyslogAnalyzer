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
#ifndef SYSLOGBUCKETCOLLECTION_H

#define SYSLOGBUCKETCOLLECTION_H

#include <vector>
#include <iterator>
#include <string>
#include <sys/ioctl.h>
#include <math.h>

#include "SysLogCountBucket.h"

using namespace std;

namespace CambridgeSoftware
{


    class SysLogBucketCollection
    {
        public:
            virtual void do_parse() = 0;
            
            void set_config(SyslogCountSettings pconfig)
            {
                settings = pconfig;
            }
            
	        vector<string> ls_errors{};


	        friend ostream& operator<<(ostream& outs,SysLogBucketCollection& pl)
	        {
	            struct winsize ts;
	            ioctl(0, TIOCGWINSZ, &ts);

	            uintmax_t i_col_weight = 40;

	            if( ts.ws_col > 0)
	            {
		            i_col_weight = (uintmax_t) ceil( pl.i_max_count / ts.ws_col);
		            if(i_col_weight < 1)
		                i_col_weight = 1;
		            else
		                i_col_weight = (uintmax_t) ceil( i_col_weight * 1.05 );
	            }

	            for(auto lident : pl.bucket_list)
	            {
		            if( lident.MyCount() <= 1) continue;
		            uintmax_t li_weight = ceil( lident.MyCount() / i_col_weight);

		            if( li_weight <= 1 ) continue;
		            // TODO: Pretty-fi output; SysLogCountBucket derived classes need "Pretty name", a 'virtual string get_prettyname()'.
		            outs << lident.ident << endl;

		            for(uintmax_t il=0; il<li_weight; il++) outs << "*";
		            outs << endl;
	            }

		        outs << endl;

		        return outs;
	        }
            
            string get_errors()
            {
                stringstream ss;
                for(auto s : ls_errors)
                {
                    ss << s << endl;
                }
                return ss.str();
            }


        protected:
            // int settings = Ident;
            SyslogCountSettings settings;
            
        	vector<SysLogCountBucket> bucket_list{};
           	uintmax_t i_max_count = 0;
	        const int i_min_ignor = 1; // TODO: This could be overiden with cmd-line option.

            // TODO: max num of error's could be height of the terminal/console, ioctl()
            int i_max_errors = 24;

	        void token_found(string ps)
	        {
		        if( settings.get_settings() == byHour )
			        ps = ps.substr(0,2);

		        auto found = find(begin(bucket_list), end(bucket_list), ps);
		        if( found != bucket_list.end() )
		        {
		            ++(*found);
	                if( (*found).MyCount() > i_max_count )
                        i_max_count = (*found).MyCount();
	                return ;
		        }
		        
		        // 2. Otherwise, add new ident
		        SysLogCountBucket temp(ps);
		        bucket_list.push_back(temp);
	        }

    };

}
#endif
