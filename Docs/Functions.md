[Basic Structure](https://github.com/stuin/Solisp/blob/master/Docs/Basics.md)

# Functions
Full list of builtin functions, ordered in the same vauge way as in the code.
All are case sensitive, a Bool is just a number.

Name 	|Return |Arguments 			|Description									|
----	|----	|----				|----											|
+		|Num	|[Num] 				|Addition										|
-		|Num	|[Num] 				|Subtraction									|
*		|Num	|[Num] 				|Multiplication									|
/		|Num	|[Num] 				|Division										|
%		|Num	|[Num] 				|Modulus										|
\>		|Bool 	|[Num] 				|Greater than									|
\>=		|Bool 	|[Num] 				|Greater than or equal 							|
<		|Bool 	|[Num] 				|Less than or equal 							|
<=		|Bool 	|[Num] 				|Less than or equal 							|
==		|Bool	|[Any]				|Is equal 										|
!=		|Bool	|[Any]				|Is not equal 									|
Join	|String	|[String],<String>	|Append strings together 						|
Append	|[Any] 	|[[Any]]			|Append lists together 							|
Seq		|[Num] 	|Num,Num 			|List numbers from first to second 				|
Remove	|[Any] 	|Any,[Any]			|Remove all that equal first from list 			|
Tail	|[Any] 	|[Any]				|Remove first element from list 				|
Clone	|[Any] 	|Num,[Any]			|Copy entire list and append it n times 		|
Reverse |[Any]	|[Any]				|Reverse entire list 							|
If 		|Any 	|Bool,Any,<Any>		|If true, second, else third/0 					|
Step	|Any 	|[Any]				|Evaluate all elements, return last one 		|
Switch	|Any 	|[[Bool,Any]]		|Return end of the first list to start with true|
Get 	|Any 	|Num,[Any]			|Return nth element in list 					|
Map 	|[Any] 	|String,[Any],Any   |For each element in list, evaluate third 		|
Set 	|Any 	|String,Any 		|Set value of string to third, return third		|
Mutate 	|Any 	|String,Any 		|Set but not pure/safe							|
