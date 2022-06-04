# User guide for math-converter-1.1.0

## Hotkeys

There is only one hotkey in math-converter: `CTRL + SHIFT + V`. This will attempt to convert plain text from the user's clipboard from the selected input math language to the selected output math language and then type it into the active window.

## Supported math languages

### Input

List of supported math languages for input:

- Mathcad

When copying from Mathcad, be sure to copy from within a math field i.e. with the gray highlighting. Copying the entire or even several math fields with the blue highlighting is not compatible with math-converter.

### Output

List of supported math languages for output:

- LaTeX

## Supported math features

While not all math syntax is supported, a variety of commonly-used features are implemented. Mathcad is the starting point for the program, and the feature set reflects this.

- Addition, subtraction, multiplication and division
- Root and power
- Equality (evaluation, comparison and definition)
- Inequality (not equal, less than, greater than, less than or equal, greater than or equal)
- Parentheses
- Trigonometric functions (sin, cos, tan, cot, arcsin, arccos, arctan, arccot, sinh, cosh, tanh, coth, sec, csc)
- Factorial
- Absolute value
- Derivative and integral
- Matrix
- Vector product
- Sum and product
- Logical NOT, AND and OR
- Variable, unit and function formatting
- Text subscript
- Degree symbol
- Percentage
- Range and step range
- Element of
- Complex numbers

## Configuration

The supported input character set can be extended with additional, user-made `.json` files under `/resources/character-sets/`. Additionally, configuration can be achieved at runtime via the terminal window the program runs in. At the moment, configuration is not saved across sessions. The currently supported configuration commands include:

- Display help
- Set decimal separator

The commands to set input and output math languages are also present, but they currently have no effect.

### Display help

This command displays help about the configuration commands.

Command aliases: `h help ?`<br>
Available arguments: none<br>
Example: `help`

### Set decimal separator

This command sets the decimal separator to either a period or a comma. The default is a period.

Command aliases: `d dec ds sep`<br>
Available arguments: `period . comma ,`<br>
Example: `d comma`

## Contact

For inquiries regarding math-converter, contact the author via email on [Thomas_98@hotmail.dk](mailto:Thomas_98@hotmail.dk) or by message on Discord to `Thomas#5904`.