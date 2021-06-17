using System;
using System.IO;
using System.Text.RegularExpressions;

namespace cambridgesoftware
{
	public enum SyslogColumnSelectEnum{byMonth=0, DayOfMonth=1, byHour=2, hostName=3, Ident=4, byWeek=90, DayOfWeek=91};

	public class SyslogCountSettings
	{
		public const char cDelim = ' ';

		public bool ArgsAreSane
		{ get { return i_args_sane; } }
		bool i_args_sane = false;

		public string sLogName
		{ get { return s_path_logname; }}
		string s_path_logname = "";

		public SyslogColumnSelectEnum GetSettings
		{ get {return settings; }}
		SyslogColumnSelectEnum settings = SyslogColumnSelectEnum.Ident;

		public SyslogCountSettings(string[] pargs)
		{
			i_args_sane = check_args_sane(pargs);
			if(pargs.Length > 1)
                                settings = parse_col_selected(pargs[2]);
		}

		bool check_args_sane(string[] pargs)
		{
			try
			{
				if(pargs.Length < 1)
					return false;
				if(!File.Exists(pargs[1]))
					return false;
			}
			catch(Exception)
			{
				return false;
			}
			return true;
		}

		SyslogColumnSelectEnum parse_col_selected(string ps)
		{
			SyslogColumnSelectEnum result = SyslogColumnSelectEnum.Ident;
			switch(ps.ToLower().Trim())
			{
				case "month":
					result = SyslogColumnSelectEnum.byMonth;
				break;
				case "hour":
					result = SyslogColumnSelectEnum.byHour;
				break;
				case "day":
					result = SyslogColumnSelectEnum.DayOfMonth;
				break;
				case "host":
					result = SyslogColumnSelectEnum.hostName;
				break;
				case "ident":
					result = SyslogColumnSelectEnum.Ident;
				break;
				case "week":
					result = SyslogColumnSelectEnum.byWeek;
				break;
				case "dayofweek":
					result = SyslogColumnSelectEnum.DayOfWeek;
				break;
				default:
					// TODO: This assumes console program called this function:
					Console.WriteLine("Didn't understand '{0}', using 'ident' instead.", ps.Trim() );
				break;
			}

			return result;
		}
	}
}
