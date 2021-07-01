# Syslog Analyzer

This program is useful for analyzing syslog files, and may help determine what warrants further investigation. It is different because it won't hog server resources when processing large log files. It doesn't load the entire log file into memory. Instead, it uses the C++ facility for file streaming, and will count matches as it goes along. 

The program does this by counting the number of log entries within some predefined category; you choose the category. The list of categories include: "ident" column(reference RFC 5424), hour, month, week of the year, day of the week, and others. The output doesn't include counts that are equal to 1, because the intent is to find overactive software making noise, but I have some ideas about how to analyze those single-records and hope to implement them in a future revision.

The output is a horizonal-graph written to the Console.

There is a C++11 version in folder 'cpp', and an unfinished Mono-Project C# version in folder 'mono-cs' that runs, but only counts the 'ident' column.
I don't plan to finish the Mono Project version; it's here just-for-fun and for comparison.

# How To Compile
It should compile with any C++11 compiler, but I've been using "g++", like this :
> $ g++ -o ./syslog_parse.x ./syslog_parse_v2.cpp

For the Mono Project Version, the following works for Mono Version 4.6 :
> $ mcs -out:syslog_analyzer.exe main.cs SyslogAnalyzer.cs SyslogCountSettings.cs

# How To Run
On most Linux systems, you'll need elevated permissions to access the logs, and that means preceeding the command with "sudo" or whatever your system offers.
> $ ./syslog_pase.x /var/log/syslog

Enjoy!

# Background
The inspiration for this program was actually another that I wrote some time ago on a Microsoft Windows system. A coworker was trying to open a log file (CSV, 'comma separated values') with Excel, but Excel would stall/hang, and crash before it even had a chance to realize the file exceeded Excel's maximum number of rows. The file was too large. So I wrote a program using C++ knowing the execution-time would be better than C# and the .NET Framework.

Microsoft's C# lanugage is an excellent option for your typical business-applications and I prefer it. The Mono Project C# version here was just an afterthought for comparing execution-time, and just for fun.

# Measuring Execution Time
The 'time' command can measure execution-time, and you'll want to be sure the log file hasn't changed between test runs. So I would copy it, and then analyze that copy.
> $ time syslog_parse.x syslog-copy.csv

Then, you can compare with the Mono Project version of the program.
> $ time syslog_analyze.exe syslog-copy.csv

If powershell is available to you, the cmdlet(pronounced "command let"), looks something like this:
> C:\> measure-command -Expression { syslog_analyze.exe syslogcopy.csv }

The difference will seem insignificant for Linux syslog files, but it matters when working with much larger files.

regards,

-Cam
