/*
    Copyright Cambridge "Cam" Kelly, camckelly@gmail.com
    
    TODO: Add license.
*/
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include <regex>

using namespace std;

namespace CambridgeSoftware
{

    // TODO: Add cmd-line option locale.
    // TODO: Maybe Add an "alltimes" that will compute all 5 of the time-series calculations ?

    // TODO: Determine these enum integers by syslog.conf.
    enum SyslogColumnSelectEnum{ byMonth=0, DayOfMonth=1, byHour=2, hostName=3, Ident=4,
         // tmTest=97,
         byWeek=90, DayOfWeek=91 };

    class SyslogCountSettings
    {
        public:

            void check_parameters(int _parg,char* _pargv[])
            {
                i_args_ok = ! check_args_sane( _parg, _pargv );
                if(_parg > 2)
                    i_setting = parse_col_selected(_pargv[2]);
            }
            
            /* SyslogCountSettings(const SyslogCountSettings& pc) :
                i_args_ok(pc.i_args_ok), i_setting(pc.i_setting), s_logname(pc.s_logname){};  */
            
            int args_not_sane()
            {
                return !i_args_ok;
            }
            
            int get_settings()
            {
                return i_setting;
            }
            
            string get_logname()
            {
                return s_path_logname;
            }
            
        private:
            int i_args_ok = 0;
            int i_setting = Ident;
            string s_path_logname;
            vector<string> ls_errors{};
            
            int check_args_sane(int parg,char* pargv[])
            {
	            try{
	            // 1. argc[1] is valid file name
	            if( parg > 1 )
	            {
		            smatch m;
		            string _sfilename(pargv[1]);
		            s_path_logname = _sfilename;
		            int i_trim = _sfilename.rfind("/");
         
		            if( i_trim != string::npos ){
			            _sfilename = _sfilename.substr( i_trim+1 );
		            }

		            // 1.1 A-Z,a-z,0-9, "." and hyphen
		            regex rePattern("^([A-Za-z0-9_\\.\\-])+$");
		            if( ! regex_match(_sfilename, m, rePattern) )
			            return 1;

		            // 1.2 File name max 14 characters
		            if( _sfilename.size() > 14 )
			            return 1;

		            // 1.3  hyphen can't be 1st character
		            if( _sfilename[0] == '-' )
			            return 1;
	            }

	            // TODO: 2. If col-selected, is that column supported?? *They might change by syslog config.
	            // if (parg > 2)

	            }
	            catch(exception)
	            { return 1; }

	            return 0;
            }

            int parse_col_selected(string ps)
            {
	            string col = str_tolower(ps);
	            if( ! col.compare("month"))
		            return byMonth;
	            if( ! col.compare("hour"))
		            return byHour;
	            if( ! col.compare("day"))
		            return DayOfMonth;
	            if( ! col.compare("host"))
		            return hostName;
	            if( ! col.compare("ident"))
		            return Ident;
	            
	            if( ! col.compare("week"))
		            return byWeek;
	            if( ! col.compare("dayofweek"))
		            return DayOfWeek;

	            /* if( ! col.compare("test"))
	                return tmTest; */
                
                cout << "Didn't understand '" << ps << "' assuming 'ident' instead." << endl;
	            return Ident;
            }


            std::string str_tolower(std::string s) {
                std::transform(s.begin(), s.end(), s.begin(), 
                               [](unsigned char c){ return std::tolower(c); }
                              );
                return s;
            }


    };

}
