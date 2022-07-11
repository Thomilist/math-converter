# User guide for math-converter-1.5.3

## Supported platforms

### Windows

Math-converter has been tested on Windows 10 version 21H2, but should be compatible with most modern versions of Windows. Three non-standard DLL-files are required (`libgcc_s_seh-1.dll`, `libstdc++-6.dll` and `libwinpthread-1.dll`,), but these are distributed alongside the program.

### Linux

Math-converter has been tested on Ubuntu 22.04 LTS, but should be compatible with most modern Linux distributions. The Linux-version of math-converter relies on the X Window System or X11 for hotkey, "clipboard" and keystroke functionality, and therefore only works with X11-based applications. Additionally, the commands to show the help text and the user guide rely on `xdg-open` to open URLs.

## Hotkeys

There is only one hotkey in math-converter: `CTRL + SHIFT + V`. This will attempt to convert plain text from the user's clipboard from the selected input math format to the selected output math format and then type it into the active window or place it in the user's clipboard.

## Supported math formats

### Input

List of supported math formats for input:

- Mathcad

When copying from Mathcad, be sure to copy from within a math field i.e. with the gray highlighting. Copying the entire or even several math fields with the blue highlighting is not compatible with math-converter.

### Output

List of supported math formats for output:

- Mathcad
- LaTeX
- MathML (presentation)
- UnicodeMath

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
- Set input format
- Set output format

### Display help

This command displays help about the configuration commands.

Command aliases: `h help ?`<br>
Available arguments: none<br>
Example: `h`

### Show user guide

This command opens the user guide on math-converter's Github.

Command aliases: `g guide userguide`<br>
Available arguments: none<br>
Example: `g`

### Open download page

This command opens the release page for the latest release on math-converter's Github for ease of access to the latest version. The program will automatically check for new releases on launch.

Command aliases: `download`<br>
Available arguments: none<br>
Example: `download`

### Set decimal separator

This command sets the decimal separator to either a period or a comma. The default is a period.

Command aliases: `d dec ds sep`<br>
Available arguments: `period . comma ,`<br>
Example: `d ,`

### Set output mode

This command sets the output mode. When using the `Keystrokes` mode, the output is sent to the focused window via simulated keystrokes. Alternatively, the output can be placed in the clipboard by using the `Clipboard` mode, after which it can be pasted with the usual `CTRL + V` shortcut. Some programs provide autocompletion of user input, which may prevent the `Keystrokes` mode from functioning correctly. The `Clipboard` mode is intended to circumvent this.

Command aliases: `m mode`<br>
Available arguments: `keystrokes keys clipboard clip`<br>
Example: `m clip`

### Set input format

This command sets the input format. This is the format of the source math that the user copies to the clipboard. If the clipboard contents do not correspond to the input format setting, math-converter will not function correctly. 

Command aliases: `i in input`<br>
Available arguments: `Mathcad mathcad`<br>
Example: `i mathcad`

### Set output format

This command sets the output format. This is the math format to convert to for use at the destination.

Command aliases: `o out output`<br>
Available arguments: `Mathcad mathcad LaTeX Latex latex MathML mathml UnicodeMath unicodemath Unicode unicode`<br>
Example: `o latex`

## Contact

For inquiries regarding math-converter, contact the author via email on [Thomas_98@hotmail.dk](mailto:Thomas_98@hotmail.dk) or by message on Discord to `Thomas#5904`.