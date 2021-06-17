using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace cambridgesoftware
{

        public class Program
        {
		public static void print_usage()
		{
			Console.WriteLine( "The purpose of this program is to count something of your choice" );
			Console.WriteLine( "in the system log, 'syslog'. Output is a horizontal bar-graph" );
			Console.WriteLine( "of asterisk's." );
			Console.WriteLine( "Usage:" );
			Console.WriteLine( "syslog_parse.x (path to a syslog file) (something to count, month|hour|day|host|ident|week|dayofweek)" );
			Console.WriteLine( "syslog_parse.x /var/log/syslog.1" );
			Console.WriteLine( "syslog_parse.x /var/log/syslog month" );
			Console.WriteLine( "" );
			return ;

		}

                public static int Main(string[] args)
                {
                        Console.WriteLine(Environment.NewLine + "This is the command-line interface(cli) to SyslogAnalyzer.");
			if(args.Length < 1)
			{
				print_usage();
				return 0;
			}
			SyslogCountSettings config = new SyslogCountSettings(args);

                        // 1) Create Object
                        SyslogAnalyzer sla = new SyslogAnalyzer(config, args[0]);

                        // 2) Run Analysis
                        sla.RunAnalysis();

                        // 3) Output Results
                        sla.OutputToConsole();

                        // 4)
                        Console.WriteLine(Environment.NewLine + "Done.");
                        return 0;
                }
        }

}
