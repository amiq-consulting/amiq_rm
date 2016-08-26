amiq_rm
=======

Register model library in C++

[Amiq Blog Related Article](http://www.amiq.com/consulting/2015/12/09/c-register-modeling-framework/)

This is a C++ library which can be used to model behavior of registers found in a chip. The user has the possibility to define fields in a registers
with different attributes (RO, RW, W1C, etc) and sizes. The registers are mapped then in "address_maps". Various topologies can be created by the user
(flat topologies - in which all registers are under one address map, hierarchical topologies - in which address_maps contain other address_maps which 
contain registers, etc).    

The library provides functions for field operations: adding fields, getting and setting the value of a field. Basic register operations on a register 
are supported: reset, reading and writing, setting and getting the value of the register. The difference between read/write and get/set is that the 
first pair takes into consideration the attributes of the fields (ex: a write to a READ-ONLY register will not take place), while the second pair will 
bypass the attributes and perform the operation (ex: to change the value of a status register, which is READ-ONLY, a set can be performed). There are 
hooks like pre_access() and post_access() in which side-effects of the register can be implemented.

Folder hierarchy description:
=============================

/build - example build folder & resources

/doc - contains the documentation for the library
	html - contains documentation from code - generated with doxygen
	amiq_rm_performance.ods - contains the graphs and the raw data from the perfomance benchmark for the main functions from the library
	
/src - contains source files of the library

/tests - contains the tests for the library 
	benchmark - contains the tests to create certain register topologies and measure the performance of the function calls of the library	
	unit_tests - contains a collection of unit tests to asure the sanity of the library; here can be found also some usecases and real 
	exmples of how to use the library
	
How to run the example:
=======================
$> cd amiq_rm/build
$> make -f makefile all
$> ./amiq_rm


