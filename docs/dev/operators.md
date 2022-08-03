
# Operators and precedence

Lower precedence means the operators will be evaluated earlier, for example `2 + 8 * 3 + 1` is equivalent to `(2 + (8 * 3)) + 1` because associativity of `+` is left-to-right and `*` has a lower precedence than `+`.

This document is based on [C++ Operator Precedence](https://en.cppreference.com/w/cpp/language/operator_precedence).

| Precedence | Operator | Description | Associativity |
| --- | --- | --- | --- |
| 1 | `a++`, `a--` | Suffix increment/decrement | Left-to-right |
| | `a()` | Function call | |
| 2 | `++a`, `--a` | Prefix increment/decrement | Right-to-left |
| | `+a`, `-a` | Unary plus and minus | |
| | `!a` | Not | |
| 3 | `a*b`, `a/b`, `a%b` | Multiplication, division and remainder | Left-to-right |
| 4 | `a+b`, `a-b` | Addition and subtraction | |
| 5 | `a<=b`, `a<b`, `a>b`, `a>=b` | Inequality operators | |
| 6 | `a==b`, `a!=b` | Equality operations | |
| 7 | `a&&b` | And | |
| 8 | `a\|\|b` | Or | Right-to-left |
| 9 | `a?b:c` | Ternary operator | |
| | `a=b` | Assignment | |
| | `a+=b`, `a-=b` | Assignment with addition and subtraction |
| | `a*=b`, `a/=b`, `a%=b` | Assignment with multiplication, division and remainder |

_Note: Some operators may not be implemented yet_