# QTcppIDE
A simple C/C++ IDE made with the Qt C++ framework.
Still WIP.

<!-- TOC -->
- [Features](#features)
    - [Done](#done)
    - [Work in Progress](#work-in-progress)
    - [Planned](#planned)
- [How to use](#how-to-use)
- [How to use on Windows](#how-to-use-on-windows)
<!-- /TOC -->

## Features

### Done
- Text editor with line numbers and syntax highlighting.
- In-Editor help/documentation.
- File Open.
- Open folder with multiple files.
- Support to use multiple compilers/linkers.
    - C and C++ support (selectable).
    - Launch in external console.
- Restore settings on startup and restore defaults.
- Project starter templates.
- *Toolbar
    - Undo and Redo. (add edit menu)
- *Search and Replace.
- *Right click menu.
- Clean project folder
- View menu for zoom in and out.
- Error handling when launching procceses.
- File save.
- Line number goto.
- Switch between .cpp and .h file.
- Bottom status bar.
    - Line number and column display.

### Work in Progress
- *Code Folding.
- Change file open to QTreeView to support subfolders.
- Save as.
- save before quit.
- Debugger support.
    - Breakpoint setting.
    - Support to use multiple debuggers.

### Planned
- Automatically install compiler/debugger for Windows.
- *Cross-platform testing.
- About markdown file.
- Documentation markdown file.
- Alternative welcome screen markdown file.


## How to use
1. Open a folder containing C/C++ files or create a new project.
2. Choose which file to edit with the sidebar, edit files as needed.
3. Configure compiler for your OS, or just pick from preset.
4. Compile.
5. If using a debugging preset, you may click on the line numbers to create breakpoints and generate the breakpoint commands from the run menu.
6. Run, paste in the breakpoint commands if using a debugging preset.

## How to use on Windows
1. Install [w64devkit](https://github.com/skeeto/w64devkit), recommended to copy w64devkit folder directly to `C:\`.
2. Add the w64devkit install directory to your path: Open `cmd` as administrator and then run `set PATH=c:\path\to\w64devkit\bin;%PATH%`.
3. Open a folder containing C/C++ files or create a new project.
4. Choose which file to edit with the sidebar, edit files as needed.
5. Configure compiler for your OS, or just pick from preset.
6. Compile.
7. If using a debugging preset, you may click on the line numbers to create breakpoints and generate the breakpoint commands from the run menu.
8. Run, paste in the breakpoint commands if using a debugging preset.