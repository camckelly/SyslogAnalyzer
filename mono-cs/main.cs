using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace cambridgesoftware
{

        public class Program
        {
                public static int Main()
                {
                        Console.WriteLine(Environment.NewLine + "This is the command-line interface(cli) to SyslogAnalyzer.");

                        // 1) Create Object
                        SyslogAnalyzer sla = new SyslogAnalyzer();

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
