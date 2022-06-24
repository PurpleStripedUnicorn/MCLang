
# MCLang syntax

The MCLang syntax is based on `C`. Note, however, that not all `C` features are currently supported in MCLang and some never will be. Examples of programs can also be found in the `examples` folder in this GitHub project. We start with a basic example:

```
void func() {
    /say Hello World!
}
```

Compiling this code creates a datapack with a function `func`, that outputs `"Hello World!"` to the Minecraft chat when run. The function can be run in-game by using `/function dp:func`. Note that the namespace of the datapack is currently the default: `dp`.

## Functions

Functions in general can be created by the following syntax:

```
void myFunction() {
    // code...
}
```

Multiple functions can be put in the same file simply by putting them below each other.

## Comments

As already shown in the [Functions](#Functions) section, single-line comments can be inserted by using `//` and then the comment after it. Multi-line comments are also supported. These can be inserted with `/*` and `*/`, like so:

```
void myFunction() {
    /*
        Wow a comment!
        How amazing!
    */
}
```

**WARNING: Any start of a comment that is inside an [inserted minecraft command](#inserted-minecraft-commands) is ignored! Therefore comments cannot be placed behind inserted minecraft commands!**

## Inserted minecraft commands

Normal minecraft commands can be inserted anywhere in your code by placing `/`, *at the start of a new line*. Any whitespace before `/` is ignored. The following is a correct use of an inserted minecraft command:

```
// Correct
void myFunction() {
    as ("@p") {
        /say hello!
    }
}
```

However if there is already something before the `/` at the start of the line, this can lead to unpredictable behaviour!

```
// Incorrect
void myFunction() {
    as ("@p") { /say hello!
    }
}
```

```
// Also incorrect
void myFunction() {
    as ("@p") { /say hello! }
}
```

## Execute-like statements

In Minecraft, the `execute` command is used to achieve a myriad of different things. See the [Minecraft Wiki](https://minecraft.fandom.com/wiki/Commands/execute) for more information. Because this command is so useful, a special syntax is introduced in MCLang to make the use of this command easier.

An example of the use of the execute command would be `/execute as @p run say hello!`, which displays `"hello!"` in chat as if it was sent by the nearest player. This gets more compilcated when wanting to execute multiple commands with the same `execute` command. This is solved in MCLang with the following syntax:

```
void myAsStatement() {
    as ("@p") {
        /say Hello Alice!
        /say Hello Bob!
    }
}
```

This prevents having to make a lot of separate functions.
