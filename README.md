# QTcppIDE
A simple C/C++ IDE made with the Qt C++ framework.
Still WIP.

[Demo Video]

<!-- TOC -->

- [QTcppIDE](#qtcppide)
    - [Features](#features)
        - [Complete](#complete)
        - [Dropped Features](#dropped-features)
    - [Install](#install)
    - [How to use](#how-to-use)

<!-- /TOC -->

## Features

### Complete
- Text editor with line numbers and syntax highlighting.
- In-Editor help/documentation.
- File Open.
- Open folder with multiple files.
- Support to use multiple compilers/linkers.
    - C and C++ support (selectable).
    - Launch in external console.
- Restore settings on startup and restore defaults.
- Project starter templates.
- Toolbar
    - Undo and Redo. (edit menu)
- Search and Replace.
- Right click menu.
- Clean project folder
- View menu for zoom in and out.
- Error handling when launching procceses.
- File save.
- Line number goto.
- Switch between .cpp and .h file.
- Bottom status bar.
    - Line number and column display.
- Debugger support.
    - Breakpoint setting.
    - Support to use multiple debuggers.
- About markdown file.
- Documentation markdown file.
- Cross-platform: Linux and Windows.

### Dropped Features
- Automatically install compiler/debugger for Windows.
- *Code Folding.
- Change file open to QTreeView to support subfolders.
- Save as.
- Save before quit.

## Install

On Linux, ensure you have the `g++`, `gcc`, `gdb` or `lldb` packages installed from your package manager.


On Windows, before using the IDE you must install `w64devkit` to provide the compiler and debugger.
1. Install [w64devkit](https://github.com/skeeto/w64devkit), recommended to copy w64devkit folder directly to `C:\`.
2. Add the w64devkit install directory to your path: Open `cmd` as administrator and then run `set PATH=C:\path\to\w64devkit\bin;%PATH%` (change this path to the path where you installed the w64devkit folder first! For example `set PATH=C:\w64devkit\bin;%PATH%`).

## How to use
1. Open a folder containing C/C++ files or create a new project.
2. Choose which file to edit with the sidebar, edit files as needed.
3. Configure compiler for your OS, or just pick from preset.
4. Compile.
5. If using a debugging preset, you may click on the line numbers to create breakpoints and generate the breakpoint commands from the run menu.
6. Run, paste in the breakpoint commands if using a debugging preset.