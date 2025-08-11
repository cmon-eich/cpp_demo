# cpp_demo

After a really devastating experience during a job interview (including a way to easy test on C++)
I created this small code snippets and programms inspired from exercises I encountered at the university 
and stuff I was curious about while remembering to refresh my C++ skills.

On the project structure:
- **calculator**: Was the first subproject therefore the code style is little poor:
  - almost no error handling, 
  - using different classes for the Tokens would not be necessary at all 
  - in fact, I could have done it with strings only instead of any Token class
  - BUT it was a great chance to refresh my debugging-skills
- **playground**: Contains unrelated but executable snippets on issues dug into deeper while
refreshing my memory.
  - _operators_: yeah, one would not have redundant information like m and mm in a class.
  - _structs_: as simple as it looks
  - _variadic_: there are many ways to implement them
- **measurements**: is my attempt on a library for measures.
  By now I do not really know where I am headed with this.
- **sqlite**: is my attempt on an object relational mapper. Due to lack of time the ORM is very slimmed down and prone to errors.



**PS**: Why are there so little comments?  
Let me ask you in return: Is your code commented sufficiently? ;)
