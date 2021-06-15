Syslog Analyzer Read-me
=======================

This program is useful for determing what services are making noise in the syslog, and then might warrant further investigation. It is different because it doesn't load the entire Syslog log file into RAM memory. Instead, it uses the C++ facility for file streaming, and will count matches as it goes along. You can be confident that you won't hog server resources when processing large log files.

The program can count the number of log entries within some predefined category; you choose the category. The list of categories include: "ident" column(reference RFC 5424), hour, month, week of the year, day of the week, and others. Presenty, the output ignores counts that are equal to 1, because the intent is to find overactive software making noise, but I have more ideas on that topic. The output is a horizonal line graph written to the console.

There is a C++11 language version in folder Cpp11, and a mono/C# version in folder mono-cs that runs, but doesn't filter to a category just yet.


Enjoy!

-Cam
