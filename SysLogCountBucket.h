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
#include <cstdint>

using namespace std;

namespace CambridgeSoftware
{

    class SysLogCountBucket
    {
        public:
            SysLogCountBucket(string pident)
                    : ident{pident} {};

            // virtual string get_prettyname() = 0; *need to change SyslogColumnSelectEnum into a derived class of this one.

            string ident;

            uintmax_t MyCount(){ return count; }
            
            bool NumericOverflowed() { return b_overflow; }

            SysLogCountBucket& operator++() // Prefix 
	        {
                if( count == UINTMAX_MAX )
                    b_overflow = true;
                else
                    ++count;
		        return *this;
	        }

	        // Implemented Postfix increment operator just-for-fun, but its not used.
            // SysLogCountBucket  operator++(int)
            // { SysLogCountBucket tmp(*this); tmp.count++; return tmp; } // PostFix

	        friend ostream& operator<<(ostream& os,SysLogCountBucket& pslic);

            friend bool operator==(const SysLogCountBucket& l, const SysLogCountBucket& r)
            {
                return ( l.ident == r.ident );
            }
            
            friend bool operator!=(const SysLogCountBucket& l, const SysLogCountBucket& r)
            {
                return ( l.ident != r.ident );
            }

        private:
                uintmax_t count = 1;
                
                bool b_overflow = false;

    };

    /* ostream& operator<<(ostream& os,SysLogCountBucket& pslic)
    {
	    os << pslic.ident << endl << pslic.count << endl;
	    return os;
    } */

}

