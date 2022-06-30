
# MCLang - Programming Minecraft Datapacks

[![GitHub license](https://badgen.net/github/license/PurpleStripedUnicorn/MCLang)](https://github.com/PurpleStripedUnicorn/MCLang/blob/master/LICENSE)
[![Twitter](https://badgen.net/badge/icon/twitter/cyan?icon=twitter&label)](https://twitter.com/striped_purple)
[![GitHub latest commit](https://badgen.net/github/last-commit/PurpleStripedUnicorn/MCLang/main)](https://GitHub.com/PurpleStripedUnicorn/MCLang/commit/)

MCLang is programming language which can be converted to minecraft commands, using the MCLang compiler in this project.

*Disclaimer: This project is not affiliated with Mojang or Minecraft in any way.*

## Building the project

- **Linux:** The compiler can be built on Linux by using `make`. *
- **Windows:** To build the project for Windows, MinGW needs to be installed. Then `make os=WINDOWS` should be run. Note that this command should be run on Linux as well, and not on Windows! The executable can then be sent to a Windows machine after compilation. **

This will generate either `build/main` or `build\main.exe`, depending on the operating system. This program is the compiler.

_* Compilation has only been tested on Ubuntu 20.04.3 LTS, g++ version 9.3.0._

_** Compilation has only been tested on Ubuntu 20.04.3 LTS, gcc version 9.3-win32._

## Compiling a file

An MCLang program file can be compiled with following command format:
```
<executable-name> <input-filename> [<command-line-options>]
```
As stated above, the executable name is either `build/main` or `build\main.exe`, unless changed. The datapack output will by default be put in a folder named `out_datapack` in the current working directory. This can be changed by using the `-o` option. The default namespace for the datapack is `dp`. This can be changed by using the `-n` option. For a full list of command line arguments, use `-h` or `--help`.

## Syntax

The syntax is very similar to that of C. Minecraft commands can be inserted by using `/` at the start of a line. An example program is shown below:
```
void main() {
    /say Hey hello
    if ("entity @e[name=PurpleStriped]") {
        /say Hello PurpleStriped!
        /say How is your day?
    } else {
        as ("@p") {
            /say I'm a butterfly!
        }
    }
}
```
After compilation and putting the generated datapack in the correct folder, this function can be run in Minecraft with `/function dp:main`. Refer to the [Syntax documentation](/docs/mclang/syntax.md) for more information.
