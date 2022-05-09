# Gish
A interpreted programming language with word based syntax.</br>
I am still working on this, it is far from done. I created this language to be more beginner friendly and easy to read. The documentation is also currently not completely up to date.


## 1. Installation
Download the build [here](https://github.com/Cosemuckel/Gish/releases/tag/v0.1) and run it. 

## 2. Interface
#### Menu </br>
  1. `run file, return to this screen` execute a gish file, and keep defined variables and functions after execution
  2. `run file, delete all data and return to this screen` execute a gish file and delete all defined variables and functions after execution
  3. `run file and then continue in terminal mode` execute a gish file and continue directly in interpreter prompt without deleting any data
  4. `start terminal mode` start the interpreter prompt to enter code manually
  5. `reset all data` delete all defined variables and functions
  6. `open config` open gish settings
#### Config
  (open with '6' on main menu)</br>
  1. `Output lexing output`(`true/false`) show the generated list of tokens from the input. This is probably not useful to you, its more for debugging reasons
  2. `Output parsing output`(`true/false`) show the generated syntax tree. This is probably not useful to you, its more for debugging reasons
  3. `Output interpreter output`(`true/false`) shows the lexing output of every executed instruction. This is probably not useful to you, its more for debugging reasons (note: many of them will be zero)
  4. `Output last result output`(`true/false`) show the interpreter output of the last executed instruction. This is useful if you want to see the values of variables or calculations without using print, because you can just type eg. `420 + 69` and get `489` directly outputed
  5. `show all defined Symbols` lists all the currently defined variables and functions

## 3. Getting started

### Basic syntax rules
##### Comments
Comments are snippets that are ignored by the Interpreter, you can use them in the follwing way:
  - `//This is a comment` <- Everthing followed by a double slash until the next line break is commented.
  ```
  //This is a comment
  This is not a comment anymore
  ```

#### Semicolons
All instructions have to be sepperated by a semicolon: `;`. Linebreaks do therefore not matter. (Except for singleline comments)

### Types
There are 4 basic types in gish:
 - `Number` for int and double: `0123456789.42069`
 - `String` for text sequences declared inside double quotes : `"This is a String"`
 - `Boolean` for `true`/`false`
 - `Array` to store an of any of the basic types declared inside square brackets: `[2424, 2443, 5745.35345]`

### Arithmetic operations
Types can be operated with by the following operators in the shown combinations. The mathematical orders of operation apply. If you are not sure, in which order your operations are evaluated, you can view the parsing output. Activate it in the config.
  - Addition: `+` or `plus`
    * `Number plus Number`
    * `String plus String` returns a string With the second string appended to the first string
    * More than two operators can be added after the same principles: `"Str1" + "Str2" + "Str3"` -> `"Str1Str2Str3"`
  - Subtraction `-` or `minus`
    * `Number minus Number`
  - Multiplication `*` or `times`
    * `Number times Number`
    * `String times Number` or `Number times String` multiplies the String `n` times: `"Str" * 3` -> `"StrStrStr"`
  - Division `/` or `over`
    * `Number over Number`
    * `String over Number` cuts the string in `n` pieces and keeps the first: `"StrStrStr" / 3` -> `"Str"`
  - Factorial `!` or `factorial`
    * `Number factorial`
  - Power `^`
    * `Number ^ Number`
  - And `and`
    * `Boolean and Boolean` returns true if all Booleans are true
      - See more under ``Comparaisons``
      - And can be chained: `true and true and false` -> `false`
  - Or `or`
    * `Boolean or Boolean` returns true if one or more of the Booleans is true
      - See more under ``Comparaisons``
      - Or can be chained: `false or true or false` -> `true`

### Comparaison
Types can be compared to each other by the following operators, a Boolean is returned
All combinations can be negated by using `Not`
  - Equals: `is equal to`
    * `Number is equal to Number` checks if the values are equal
      - Note: `42 is equal to 42.0` returns `true`
    * `String is equal to String` checks whether all characters are the same
    * `Boolean is equal to Boolean` checks whether the two have the same value
    * `Array is equal to Array` checks if all elements of the arrays are the same, using their comparaison rules
    * Negation: `Object1 is` `not` `equal to Object2`
  - Is smaller/greater than: `is smaller/greater than`
    * `Number is smaller/greater than Number`
    * Negation: `Number1 is` `not` `smaller than Number2`
  - Is smaller than or equal to: `is smaller/greater or equal to`
    * `Number is smaller/greater or equal to Number`
    * Negation: `Number1 is` `not` `greater or equal to Number2`
  - Combining comparaisons
    * comparaisons can be combined using `and` or `or`
      - Examples:
        * `42 is smaller than 69` `and` `420 is greater than 69` -> `true`
        * `42 is smaller than 69` `and` `69 is greater than 420` -> `false`
        * `42 is smaller than 69` `or` `69 is greater than 420` -> `true`

### Variables
Variables are used to store Values and can later be modified. To declare a variable specify its type, followed by a name and a value:
  - eg:  `Number num equals 3.3;` <- The Variable num has a value of 3.3 </br>
  The Value must match the variable type.
  - declare Arrays the follwing way: `Array of type Number arr equals [42, 35, 69];`
    * note: Arrays of arrays are currently not yet supported

To use a variable in operations simply type its name. </br>
If you want to see the value of the variable, you can just type its name if you have `Last interpreter output` enabled in the [config](#Config). You can also see all currently defined Variables and their values by using the `show all defined Symbols` in the [config](#Config). </br>
To undefine a variable use `undefine the variable VAR_NAME;`

#### Operating on variables
To change the value of a variable, use `set VAR_NAME equal to VALUE;` The Value must match the variable type. </br>
To directly operate on a variable you can use the following keywords:
 - `add VALUE to VAR_NAME`
 - `subtract VALUE from VAR_NAME`
 - `multiply VAR_NAME by VALUE`
 - `divide VAR_NAME by VALUE`
 - `cube VAR_NAME`
 - `square VAR_NAME`

The operation rules from [Arithmetic operations](#Arithmetic-operations) apply.

#### Variable-functions
  * `typeof` returns a string containing the type of the variable
  * `sizeof` returns the size of an Array or String
  * `index` returns the element at the `nth` index of an Array:
  ```
  Array of type Number arr equals [3, 4, 5, 6];
  Number n equals index 2 of arr;
  print n;
  ```
  Output: `5`
  * `character` returns the `nth` character of a String, just like `index`

### Methods/Functions
Methods are snippets of code, that can be called as often as you wish, without having to type it out every time. Methods can take multiple arguments, which they process. They can return values which can then be used. If a method does not return a value it has to be declared as type `Void`.
To declare a Function specify the return type, the name, arguments and the method body:
  - `Number sum taking Number n1, Number n2: { return n1 + n2; };` <- The method sum takes two Numbers `n1` and `n2` as arguments and gives back the sum of these two.
  - `Void doSomething taking nothing: { //Does something };` <- the method doSomething takes no arguments and does not give anything back.

Call a method by typing its name, and then list its arguments:
  - `sum(10, 20);` <- the method sum is called with the arguments `10` and `20` </br> The parenthesis are optional, but if you are not using parenthesis and get some kind of error calling the function, you might use them, or check the parsing output.
   * `sum 10, 20;` does the same thing as `sum(10, 20);`
   * if you want to operate on the function output, while not using parenthesis, you must put the whole function call in parenthesis to not operate on the last argument:
     - `sum 10, 20 * 2;` -> `sum 10, 40;`
     - `(sum 10, 20) * 2;` -> `30 * 2;`
     - `sum(10, 20) * 2;` -> `30 * 2;` I recommend doing this in that case.

Like variables, you can undefine Functions: `undefine the function FUNC_NAME;`
