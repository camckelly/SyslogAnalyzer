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

		SyslogCountSettings config = null;

                #endregion

                #region PublicInterface

                /// <summary>
                /// Constructor
                /// </summary>
                /// <param>Optional, Path to syslog file, defaults to /var/log/syslog</param>
                public SyslogAnalyzer(SyslogCountSettings pconfig, string ps = @"/var/log/syslog")
		{
                        s_path = ps;
			config = pconfig;
                }

                public void RunAnalysis()
                {
			// TODO: Left off here. Use 'config' class-member.
                        // 1) Open file
                        using(StreamReader flog = new StreamReader(s_path))
                        {
                                string line;
                                const string pattern = @"\s*([\w|\.|\-]+)";

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
			int result = Convert.ToInt32( (i_max / Console.WindowWidth) * 1.05 );
			if(result < 1)
				result = 1;
			return result;
                }

                public void OutputToConsole()
                {
                        /* foreach(string sk in counts.Keys)
                                Console.WriteLine("{0} = {1}",sk, counts[sk]);
                        return ; */
                        if(counts==null || counts.Count < 1 || i_max < 1)
                        {
                                Console.WriteLine("Hum, nothing found.");
                                return ;
                        }

                        int i_col_weight = ConsoleColumnWeight();
                        foreach(string sk in counts.Keys)
                        {
                                int my_weight = (int) Math.Ceiling( (double) counts[sk] / i_col_weight );
				if(my_weight > 1)
				{
					Console.WriteLine("{0}{1}{2}", sk, Environment.NewLine, new string('*', my_weight ));
					Console.WriteLine();
				}
                        }

                        return ;
                }

                #endregion
        }
}
