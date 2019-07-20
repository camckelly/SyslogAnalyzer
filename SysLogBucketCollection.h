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

	            int i_col_weight = 40;

	            if( ts.ws_col > 0)
	            {
		            i_col_weight = (int) ceil( pl.i_max_count / ts.ws_col);
		            if(i_col_weight < 1)
		                i_col_weight = 1;
		            else
		                i_col_weight = (int) ceil( i_col_weight * 1.05 );
	            }

	            for(auto lident : pl.bucket_list)
	            {
		            if( lident.MyCount() <= 1) continue;
		            int li_weight = ceil( lident.MyCount() / i_col_weight);

		            if( li_weight <= 1 ) continue;
		            // TODO: Pretty-fi output; SysLogCountBucket derived classes need "Pretty name", a 'virtual string get_prettyname()'.
		            outs << lident.ident << endl;

		            for(int il=0; il<li_weight; il++) outs << "*";
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
           	int i_max_count = 0;
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
