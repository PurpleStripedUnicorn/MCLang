
# MCLang project coding standards

Please read this document before contributing to the MCLang project on GitHub.

## File/folder organization

All classes and functions are split into `.h` and `.cc` files. The `.h` files
contain only the declarations or forward declarations of functions and classes.
The `.cc` files contain the actual contents of the functions and class member
functions.

The `.h` and `.cc` files that correspond with each other have to be put in the
same folder.

The main program is located in `main.cc` and individual components in subfolders
of the `mclang` folder.

Each `.h` file has at the start:
```c++
#ifndef __UNIQUE_NAME_H__
#define __UNIQUE_NAME_H__
```
and at the end:
```c++
#endif
```
to prevent conflicts.

The first line of each `.cc` and `.h` file should be empty.

## Line size

All lines should be at most 80 characters long. This is a hard limit and should never be exceeded!

## Comments

Comments inside functions are only used when neccessary for clarification. Each
function and class member function declaration has to have a comment above it.
The comments above standalone functions have to be in the following format:
```c++
/**
 * [description]
 * @param param1 [description]
 * @param param2 [description]
 * @return [description]
 */
int myFunction(int param1, int param2);
```
Class member function comments need to be in the following format:
```c++
/**
 * [description]
 * @param param1 [description]
 * @param param2 [description]
 * @return [description]
 * @pre [required precondition (optional)]
 * @post [postcondition (given the precondition) (optional)]
 */
int myMemberFunction(int param1, int param2) const;
```
Note that the actual function declarations here are just examples.

Class member variables should also have comments, using single-line comments as
in the following example:
```c++
// A variable storing the key to the universe
int memberVar;
```

Global variables preferably also have single-line comments above them.

## Constants

The `const` keyword should be used as much as possible. Global constants can be
put both in the `.h` as well as in the `.cc` files.

## Spacing

For spacing in classes, look at the following example:
```c++
#ifndef __MYCLASS_H__
#define __MYCLASS_H__

class MyClass {

public:

    /**
     * Constructor
     */
    MyClass();

    /**
     * Member function
     * @return The value of `memberVar`
     */
    int memberFunc() const;

private:

    // Member variable
    int memberVar;

};

#endif
```
Inside functions, do not use any empty lines.

## Casing

For functions and variables (global, constants member, local, etc.) *camelCase*
is used. For class names use *PascalCase*. For `#define` variables use
*UPPERCASE_WITH_UNDERSCORES*.

## Class organization

Each class should have `public:` and (if there are private member variables)
`private:` keywords, optionally also `protected:`. No member should be above
these keywords.

## Includes

When using includes do not depend on one included `.h` file to include other
resources. So for exmaple when `fileB.h` includes `fileA.h`, and you need both
`fileA.h` and `fileB.h`, use
```c++
#include "fileA.h"
#include "fileB.h"
```
However also use as little includes as possible. If an issue can be resolved by
using a forward declaration or leaving out the include entirely, it should be
resolved this way. An exception here are standard libraries, so do not make a
forward declaration for `std::string`, but use `#include <string>`.

When using `.cc` files in combination with `.h` files, the `.cc` file should not
rely on the includes from the `.h` file. Likewise don't put any includes in the
`.h` file that are only needed in the `.cc`.

For example, when using `fileA.h`, `fileA.cc`, `fileB.h` and `fileB.cc` with
`ClassA` inside `fileA.h`. Then the contents `fileB.h` could look like:
```c++
#ifndef __CLASSB_H__
#define __CLASSB_H__

class ClassA;

class ClassB {

public:

    /**
     * Constructor
     */
    ClassB();

    /**
     * Destructor
     */
    ~ClassB();

    /**
     * Get the member object
     * @return A pointer to the member object, as stored in `obj`
     */
    ClassA *getObj() const;

private:

    // Member object
    ClassA *obj;

};

#endif
```
Notice there are no includes. The contents of `fileB.cc` could look like:
```c++
#include "classA.h"
#include "classB.h"

ClassB::ClassB() : obj(new classA()) {

}

ClassB::~ClassB() {
    delete obj;
}

ClassA *ClassB::getObj() const {
    return obj;
}
```
Note that here both `classA.h` and `classB.h` are included since they are
both required in this file. If `classB.h` were to include `classA.h`, the
`classA.h` include should still be present to avoid problems when code is
changed.

### Order of includes

Includes come before any C++ code. Listed first are all of the local `.h` files,
then all of the standard libraries used. Within the categories they are ordered
alphabetically.

## Indents

An indent is used after every `{`, with the exception of classes, where the
`private:` and similar keywords are put one indent to left of where they would
be.

## Some extra notes

- Make sure to delete all used pointers. Any code with memory leaks should not
  be accepted.
- Keep functions small and readable.
- Write tests or create examples for newly implemented features.
- If you have a suggestion for a feature or found a bug, create a GitHub issue.
