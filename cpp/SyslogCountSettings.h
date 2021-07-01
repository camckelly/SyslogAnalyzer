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
// TODO: Add cmd-line option, year of the log, useful when the file was already

// Rsyslog writes log-entries to different column specifications, it calls "templates."
// The template supported here is RSYSLOG_TraditionalFileFormat
// file:///usr/share/doc/rsyslog-doc/html/configuration/templates.html#standard-template-for-writing-to-files
enum SyslogColumnSelectEnum { byMonth=0, DayOfMonth=1, byHour=2, hostName=3,
                              Ident=4, byWeek=90, DayOfWeek=91 };

enum SyslogOutputFormatEnum { GraphOutput, CsvOutput };

class SyslogCountSettings {
public:

    static const char cDelim = ' ';

    SyslogOutputFormatEnum outputFormat = SyslogOutputFormatEnum::CsvOutput;

    void check_cmdline_args(int _parg,char* _pargv[])
    {
        // 1. Check Sanity
        i_args_ok = ! check_args_sane( _parg, _pargv );
        // 2. Get column user wants counted
        if(_parg > 2)
            i_setting = parse_col_selected(_pargv[2]);
        // 3. Get Optional output-format
        if(_parg > 3 )
        {
            if( !(str_tolower(_pargv[3])).compare("csvoutput") )
                outputFormat = SyslogOutputFormatEnum::CsvOutput;
            else
                cout << "Ignoring cmdline arg, " << _pargv[3] << endl;
        }
        else
            outputFormat = SyslogOutputFormatEnum::GraphOutput;
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

    string get_friendly_name()
    {
        return s_userfriendly;
    }

private:
    int i_args_ok = 0;
    int i_setting = Ident;
    string s_path_logname;
    vector<string> ls_errors{};
    string s_userfriendly = "";

    int check_args_sane(int parg,char* pargv[])
    {
        try {
            // 1. argc[1] is valid file name
            if( parg > 1 ) {
                smatch m;
                string _sfilename(pargv[1]);
                s_path_logname = _sfilename;
                size_t i_trim = _sfilename.rfind("/");

                if( i_trim != string::npos ) {
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
        }
        catch(exception)
        {
            return 1;
        }

        return 0;
    }

    int parse_col_selected(string ps)
    {
        string col = str_tolower(ps);
        s_userfriendly = col + ":";

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


    string str_tolower(string s)
    {
        transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return tolower(c); } );

        return s;
    }

};

}
