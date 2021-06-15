Syslog Analyzer Read-me
=======================

This program is useful for determing what services are making noise in the syslog, and then might warrant further investigation. It is different because it doesn't load the entire Syslog log file into RAM memory. Instead, it uses the C++ facility for file streaming, and will count matches as it goes along. You can be confident that you won't hog server resources when processing large log files.

The program does this by counting the number of log entries within some predefined category; you choose the category. The list of categories include: "ident" column(reference RFC 5424), hour, month, week of the year, day of the week, and others. The output ignores counts that are equal to 1, because the intent is to find overactive software making noise, but I have some ideas about how to analyze those single-records and hope to implement them in a future revision.

There is a C++11 version in folder 'cpp', and a Mono-Project C# version in folder 'mono-cs' that runs but doesn't filter yet.

The output is a horizonal line graph written to the Console.


Enjoy!

-Cam
