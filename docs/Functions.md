# Functions
Full list of builtin functions, ordered in the same vauge way as in the code.
All are case sensitive.

Name 	|Return |Arguments 			|Description									|
----	|----	|----				|----											|
\+		|Num	|\[Num\] 			|Addition										|
\-		|Num	|\[Num\] 			|Subtraction									|
\*		|Num	|\[Num\] 			|Multiplication									|
/		|Num	|\[Num\] 			|Division										|
%		|Num	|\[Num\] 			|Modulus										|
\>		|Bool 	|\[Num\] 			|Greater than									|
\>=		|Bool 	|\[Num\] 			|Greater than or equal 							|
<		|Bool 	|\[Num\] 			|Less than or equal 							|
<=		|Bool 	|\[Num\] 			|Less than or equal 							|
Not 	|Bool 	|Bool 				|Not gate 										|
==		|Bool	|\[Any\]			|Is equal 										|
\!=		|Bool	|\[Any\]			|Is not equal 									|
Type	|String |Any 				|Returns string representing type of object 	|
Join	|String	|\[String\],<String>|Concat strings together with delimeter			|
Append	|\[Any\]|\[\[Any\]\]		|Append lists together 							|
Seq		|\[Num\]|Num,Num 			|List numbers from first to second 				|
Remove	|\[Any\]|Any,\[Any\]		|Remove all that equal first from list 			|
Tail	|\[Any\]|\[Any\]			|Remove first element from list 				|
Clone	|\[Any\]|Num,\[Any\]		|Copy entire list and append it n times 		|
Reverse |\[Any\]|\[Any\]			|Reverse entire list 							|
If 		|Any 	|Bool,Any,<Any>		|If true, second, else third/0 					|
Step	|Any 	|\[Any\]			|Evaluate all elements, return last one 		|
Switch	|Any 	|\[\[Bool,Any\]\]	|Return end of the first list to start with true|
Get 	|Any 	|Num,\[Any\]		|Return nth element in list 					|
Length 	|Num 	|\[Any\]			|Calculate length of list 						|
Get-Max |Num 	|\[Max\]			|Get max element in list 						|
Map 	|\[Any\]|String,\[Any\],Any |For each element in list, evaluate third 		|
MapI 	|\[Any\]|String,String,\[Any\],Any |Map but with a second index variable 	|
Repeat  |Num    |Bool				|Counts how many times the bool is true 		|
Set 	|Any 	|String,Any 		|Set value of string to third, return third		|
Mutate 	|Any 	|String,Any 		|Set but not pure/safe							|


[Basic Structure](https://github.com/stuin/Solisp/blob/master/docs/Basics.md)