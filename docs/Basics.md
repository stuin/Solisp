## Basic Structure
As with most lisp-based languages, functions are written in pre-fix, inside parenthesis.
```
(function x y)
```

General mathmatical operations can be done as expected, and will be evaluated left to right.
```
(+ 1 2)
(- 4 1)
(* 3 1)
(/ 6 2)
(% 7 4)

(- 10 3 3 1)
(/ (* 3 3) (+ 1 1 1))
```
All of the above examples should return 3.

## Other syntax
Lists can be created using a few different functions, but curly braces can also be used to make it easier.

```
{1 2 3 4 5}
#This is a comment.
```

This will not be evaluated as a Function, but will instead be stored directly as a List.
Quotation marks can be used to define Strings.

```
"string"
"lots of words"
```
A single word without quotes will default to a Name, but if there is no defined value it can usually be used as a String.
A single letter in quotes will become a Char.

## Argument folding
In any case where the very last argument that a function asks for is a List, the function can accept that List in two different ways. Either a normal List can be provided, or all objects in the List can be provided directly as arguments.
You may have noticed that a normal mathmatical operator can accept more than just two arguments. In fact, all the basic mathmatical operators effectivly have the built-in functionality of foldl.
```
(+ 1 2 3 4 5)
(+ {1 2 3 4 5})
(+ (+ {1 2 3}) (+ 4 5))
(+ (+ 1 2) (+ 3 (+ 4 5)))
```
All these are equivalent.
It is important to note that any time there is only one argument in these cases, it is assumed to be a list. It may be required to put a String inside curly braces if it is the only value.

## Type conversion
All objects are stored as c++ structures with specific types, but converting between types is done automatically and dynamically. Each row shows what a type can be converted to.

Type 	|Stored	|Expr	|Name	|List 	|Bool	|Number |Char 	|String	|Function 	|
----	|----	|----	|----	|----	|----	|------	|----	|------	|-------- 	|
Expr 	|vector	|Yes	|No 	|Yes	|No 	|No 	|No 	|Append |No 		|
Name 	|string |No 	|Yes	|Chars	|Yes?	|Yes?	|Yes?	|Yes	|No 		|
List 	|vector	|Yes	|Append	|Yes	|No 	|No 	|No 	|Append |Yes		|
Bool	|char	|No 	|No		|Single	|Yes 	|Yes 	|Value	|Yes	|No 		|
Number	|int	|No 	|No		|Single	|Yes 	|Yes 	|Value	|Yes	|No 		|
Char	|char	|No 	|Yes	|Single |Value	|Value	|Yes	|Yes	|No 		|
String 	|string |No 	|Yes	|Chars	|Yes?	|Yes?	|Yes?	|Yes	|No 		|
Function|function|No 	|No		|No 	|No 	|No 	|No 	|<func> |Yes		|

## Variables
Anything can be stored in a variable, including all of the above types. Variables are dynamically typed, dynamically scoped, and case sensitive.

Names and Strings function very similar to each other, but Names will attempt to resolve as a variable before anything else.

To set a variable, the Set function can be used, followed by a Name and a value.
```
(Set x 3)
```
This will give x the value 3 in the current scope and additionally will return the value 3. Using the Set command, all variables are immutable, and cannot be changed. However, variables can be reassigned by replacing Set with Mutate.

## Constants
There are a few useful system constants.
```
true
false
newline
```

## Functions
Writing functions is quite straightforward, though the syntax is not the greatest in the world. If a existing function name is not provided, the system can attempt to convert a list into a function.
It is expected that the list will have only two arguments, the first of which being a list of arguments to provide to the function. This list can be empty, or have only one member, but the list must be there, and unless it contains a single character, should be contained in brackets.
The second argument should be the contents of the function, and can include any variables that are in the list of arguments, as well as any variable that is in scope when the function is run. Variables set inside the function will not be passed to the outside.
```
(Set power {
	{base exp}
	(* (Clone exp {base}))
})
```
Also note that an entire function can be placed into a variable, and that the variable name can then be used in a normal recursive strategy.
```
(Set factorial {
	{x}
	(If (> x 0)
		(+ x (factorial (- x 1)))
		0
	)
})
```

# Next
- [List of functions](https://github.com/stuin/Solisp/blob/master/docs/Functions.md)