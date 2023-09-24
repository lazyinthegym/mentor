# Performance of comparing Strings in C++

I tested using four different ways of comparing two std::string objects in my_sort.cpp. 
I ran the algorithm 30 times for each case.

* Case #1 : `a <= b`
Average = 261.44537 Standard Deviation = 6.127322207312276 ms


* Case #2 : `a <= b.c_str()`
Average = 265.9375 Standard Deviation = 7.143647227898829 ms


* Case #3 : `strcmp(a.c_str(), b.c_str()) <= 0`
Average = 210.38657 Standard Deviation = 3.9911856440856694 ms


* Case #4 : `a.compare(b.c_str()) <=0`
Average = 201.24247 Standard Deviation = 5.9715788852046705 ms

Using`a.compare(b.c_str()) <=0` is 29% faster than `a <= b`. 

This is consistent with the findings in 
https://tinodidriksen.com/2011/02/cpp-string-compare-performance/