### Yeah, idk its important
* documentation

### max level
* string division and Multiplication

### next goals
* `if ... do ...` (now only `do ... if ...`)
* `for ... do ...` (now only `do ... for ...`)
* xOr
* raised to the nth power
* array of array
* implement `break` and `continue` (Node exists)
* references

### stil next goals
* save config
* multiword functions and variables
	- words (instead of 'identifier')
		-> ```this->currentToken.matches("AHAH");```
	- functions can take argument at any place in the function by using % to specify location: ```Number subtract % from % taking Number n1, Number n2: { return n2 - n1; }; ``` ->```subtract 3 from 2;```
	- if references exist: ```Number subtract % from % taking Number n1, Number reference n2: { n2 equals n2 - n1; }; ```</br>
	^ damn it, subtract exist, but you get the drill
* if expressions
* * typeof function
### bug fixes etc.
* fix division (and maybe others) `int` / `double` mismatch

### to do, but later
* imports
* binary </br>
  => stdlib</br>
    => print, scan</br>
  => default import (like vcruntime.h)</br>
    => squared, cubed</br>
    => other existing stuff</br>

### someday
* positions in nodes and interpreted values;
* update Error messages
* code refactoring?
	- less oop
