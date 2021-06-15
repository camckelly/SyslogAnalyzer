using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace cambridgesoftware
{
        /// <summary>
        /// Opens a Linux syslog file and counts entries
        /// </summary>
        public class SyslogAnalyzer
        {
                #region PrivateProperties

                // Path to the syslog file to be analyzed
                string s_path = "";

		// Collection of each "ident", and their number of records in the syslog file
                Dictionary<string,int> counts = new Dictionary<string,int>();

		// The highest number of records; used to calculate a console-column weight, for Console output.
                int i_max = 0;

                #endregion

                #region PublicInterface

                /// <summary>
                /// Constructor
                /// </summary>
                /// <param>Optional, Path to syslog file, defaults to /var/log/syslog</param>
                public SyslogAnalyzer(string ps = @"/var/log/syslog")
                {
                        s_path = ps;
                }

                public void RunAnalysis()
                {
                        // 1) Open file
                        using(StreamReader flog = new StreamReader(s_path))
                        {
                                string line;
                                const string pattern = @"\s*(\w+)";

                                // 2) Loop foreach line
                                while((line = flog.ReadLine()) != null)
                                {
                                        string[] tokens = line.Split(' ');
                                        if(tokens.Length > 4)
                                        {
                                                string ident = (Regex.Match(tokens[4], pattern)).Value;
                                                if(counts.ContainsKey(ident))
                                                        counts[ident] = counts[ident] + 1;
                                                else
                                                        counts[ident] = 1;

                                                if(counts[ident] > i_max)
                                                        i_max = counts[ident];
                                        }
                                }
                        }
                }

                public int ConsoleColumnWeight()
                {
                        return (int) Math.Ceiling( (double) (i_max / Console.WindowWidth) * 1.05 );
                }

                public void OutputToConsole()
                {
                        /* foreach(string sk in counts.Keys)
                                Console.WriteLine("{0} = {1}",sk, counts[sk]);
                        return ; */
                        if(counts.Count < 1 || i_max < 1)
                        {
                                Console.WriteLine("Hum, nothing found.");
                                return ;
                        }

                        int i_col_weight = ConsoleColumnWeight();
                        foreach(string sk in counts.Keys)
                        {
                                int my_weight = (int) Math.Ceiling( (double) counts[sk] / i_col_weight );
                                Console.WriteLine("{0}{1}{2}", sk, Environment.NewLine, new string('*', my_weight ));
                                Console.WriteLine();
                        }

                        return ;
                }

                #endregion
        }
}
