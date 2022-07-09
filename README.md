# math-converter

## Introduction

A simple program to convert text representations of math expressions between different formats, such as Mathcad and LaTeX. Originally conceived and written haphazardly in AutoHotkey during the writing of an engineering report at university, it has been reimagined in C++ with since-obtained knowledge to achieve greater code quality and improved modularity. While the original could only convert from Mathcad to LaTeX, the intention for the new math-converter is to be able to convert back and forth between several different plain-text representations of math.

## Features

**Hotkey activation**

While math-converter is running, a math conversion hotkey is enabled. After copying a math expression to the clipboard, simply press `CTRL + SHIFT + V` to convert the expression from the input format to the output format. Depending on the output mode, the converted math expression will either be placed in the clipboard or sent directly to the focused window via simulated keystrokes.

**Supported platforms**

- Windows
- Linux

**Input formats**

- Mathcad

**Output formats**

- Mathcad
- LaTeX
- MathML (presentation)
- UnicodeMath

## Further information

For more detailed information on the capabilities and usage of math-converter, see the [user guide](userguide.md).
