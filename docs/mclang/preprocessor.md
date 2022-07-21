
# MCLang preprocessor

The preprocessor for MCLang is very similar to the C preprocessor, in that directives start with `#`. Note that not all C preprocessor directives are supported in the MCLang preprocessor. Below is a list of all of the supported directives and functionalities from the preprocessor.

The preprocessor also removes all line endings that are preceded by a backslash `\`, for example:
```
#define COOL_STRING \
"Hello!"
```
is the same as
```
#define COOL_STRING "Hello!"
```
Without the backslash this would throw an error, as a preprocessor directive reads only the current line!

## `#include`

Include a different file. The path given is relative to the parent folder of the current file. Below an example of an include in a file `main.mcl`:
```
#include "myfile.mcl"

void main() {
    /say Calling boop!
    boop();
    /say Called boop!
}
```
Where the included file could be something like the following:
```
void boop() {
    /say Boop!
}
```
The directory structure would when be:
```
+ Parent folder
| + main.mcl
| + myfile.mcl
```
### Example of more directory structure

When having a more complex directory structure like the one below, you can move borth up and down the folder structure.
```
+ parent
| + folder1
| | + fileA.mcl
| | + fileB.mcl
| + folder2
| | + fileC.mcl
```
When wanting to include `fileA.mcl` from `fileC.mcl`, you can use `#include "../folder1/fileA.mcl"`.

*Warning: Using absolute paths in includes has undefined behiour and should therefore not be done!*

*Warning: Always use slashes `/` as directory separators, even in Windows!*

## `#define`

Define a set of tokens that should replace the given identifier. Only replaces the identifier if it is read by the preprocessor *after* the `#define` is read.

The following is an example of the use of `#define`:
```
#define PURPLESTRIPED "@a[name=PurpleStriped]"

void main() {
    as (PURPLESTRIPED)
        /say I'm PurpleStriped
}
```
This will be converted by the preprocessor to the following code:
```
void main() {
    as ("@a[name=PurpleStriped")
        /say I'm PurpleStriped
}
```