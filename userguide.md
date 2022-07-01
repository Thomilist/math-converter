# User guide for math-converter-1.4.0

## Hotkeys

There is only one hotkey in math-converter: `CTRL + SHIFT + V`. This will attempt to convert plain text from the user's clipboard from the selected input math language to the selected output math language and then type it into the active window or place it in the user's clipboard.

## Supported math languages

### Input

List of supported math languages for input:

- Mathcad

When copying from Mathcad, be sure to copy from within a math field i.e. with the gray highlighting. Copying the entire or even several math fields with the blue highlighting is not compatible with math-converter.

### Output

List of supported math languages for output:

- Mathcad
- LaTeX
- MathML (presentation)

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

The supported input character set can be extended with additional, user-made `.json` files under `/resources/character-sets/`, which are loaded at program launch. Characters not in the character sets will be discarded. Additionally, configuration can be achieved at runtime via the terminal window the program runs in. Settings are saved to `/config/settings.conf`. The currently supported configuration commands include:

- Display help
- Open download page
- Set decimal separator
- Set output mode
- Set input language
- Set output language

### Display help

This command displays help about the configuration commands.

Command aliases: `h help ?`<br>
Available arguments: none<br>
Example: `help`

### Open download page

This command opens the Releases page on math-converter's Github for ease of access to the latest version.

Command aliases: `download`<br>
Available arguments: none<br>
Example: `download`

### Set decimal separator

This command sets the decimal separator to either a period or a comma. The default is a period.

Command aliases: `d dec ds sep`<br>
Available arguments: `period . comma ,`<br>
Example: `d comma`

### Set output mode

This command sets the output mode. When using the `Keystrokes` mode, the output is sent to the focused window via simulated keystrokes. Alternatively, the output can be placed in the clipboard by using the `Clipboard` mode, after which it can be pasted with the usual `CTRL + V` shortcut. Some programs provide autocompletion of user input, which may prevent the `Keystrokes` mode from functioning correctly. The `Clipboard` mode is intended to circumvent this.

Command aliases: `m mode`<br>
Available arguments: `keystrokes keys clipboard clip`<br>
Example: `m clip`

### Set input language

This command sets the input language. This is the language of the source math that the user copies to the clipboard. If the clipboard contents do not correspond to the input language setting, math-converter will not function correctly. 

Command aliases: `i in input`<br>
Available arguments: `Mathcad mathcad`<br>
Example: `i mathcad`

### Set output language

This command sets the output language. This is the math language to convert to for use at the destination.

Command aliases: `o out output`<br>
Available arguments: `Mathcad mathcad LaTeX Latex latex MathML mathml`<br>
Example: `o latex`

## Contact

For inquiries regarding math-converter, contact the author via email on [Thomas_98@hotmail.dk](mailto:Thomas_98@hotmail.dk) or by message on Discord to `Thomas#5904`.