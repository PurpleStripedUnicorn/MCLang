
# Preprocessor design

The preprocessor component has as an input the main filename to read the program
from, and as output a special text object, which can contain markers. These
markers can for example contain information about the current file, line/column
numbers, or other useful information.

## Reading in files

When a file is starting to be read, it is added to a stack, along with some
information that needs to be kept track of, such as the current line/column
numbers.