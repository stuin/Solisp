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

This will not be evaluated as a function, just stored as a list.
In the same vein, quotation marks can be used to define strings, though any single word can be used as a string as well.

```
string
"string"
```
These are equivalent.
```
lots of words
"lots of words"
```
These are not, as the first one will only include the first word.
Finally, if a string is the same as an existing function or variable, a colon can be used to denote the literal string.
```
:Set -> Set
Set -> <func>
:: -> :
```

## Argument folding
In any case where the very last argument that a function asks for is a list, the function can accept that list in two different ways. Either a normal list can be provided, or all objects in the list can be provided directly as arguments.
You may have noticed that a normal mathmatical operator can accept more than just two arguments. In fact, all the basic mathmatical operators effectivly have the builtin functionality of foldl.
```
(+ 1 2 3 4 5)
(+ {1 2 3 4 5})
(+ (+ {1 2 3}) (+ 4 5))
(+ (+ 1 2) (+ 3 (+ 4 5)))
```
All these are equivalent.
It is important to note that any time there is only one argument in these cases, it is assumed to be a list. It may be required to put a string inside curly braces if it is the only value.

## Type conversion
All objects are stored as c++ structures with specific types, but converting between types is done automatically and dynamically. Each row shows what a type can be converted to.

Type 	|Stored	|Expr	|List 	|Bool	|Number |Char 	|String	|Function 	|
----	|----	|----	|----	|----	|----	|----	|----	|----		|
Expr 	|vector	|Yes	|Yes	|No 	|No 	|No 	|Append |No 		|
List 	|vector	|Yes	|Yes	|No 	|No 	|No 	|Append |Yes		|
Bool	|char	|No 	|Single	|Yes 	|Yes 	|Value	|Yes	|No 		|
Number	|int	|No 	|Single	|Yes 	|Yes 	|Value	|Yes	|No 		|
Char	|char	|No 	|Single |Value	|Value	|Yes	|Yes	|No 		|
String 	|string |No 	|Chars	|Yes?	|Yes?	|Yes?	|Yes	|No 		|
Function|function|No 	|No 	|No 	|No 	|No 	|<:func>|Yes		|

## Variables
Anything can be stored in a variable, including all of the above types. Variables are dynamically typed, dynamically scoped, and case sensitive.
Any time a string or char is evaluated, the system will check for an existing variable under that name, and return that instead if found. All builtin functions are provided through variables as well.
To set a variable, the Set function can be used, followed by a name and a value.
```
(Set x 3)
```
This will return and give x the value 3 in the current scope. Using the set command, all variables are immutable, and cannot be changed. However, variables can be reassigned by replacing Set with Mutate.

## Constants
There are a few useful system constants,
they should all be pretty obvious.
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