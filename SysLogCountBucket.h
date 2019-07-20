/*
    Copyright Cambridge "Cam" Kelly, camckelly@gmail.com
    
    TODO: Add license.
*/
#include <string>
#include <iostream>

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

            int MyCount(){ return count; }

            SysLogCountBucket& operator++() // Prefix 
	        {
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
                int count = 1;

    };

    /* ostream& operator<<(ostream& os,SysLogCountBucket& pslic)
    {
	    os << pslic.ident << endl << pslic.count << endl;
	    return os;
    } */

}

