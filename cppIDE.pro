QT += widgets
requires(qtConfig(filedialog))

HEADERS         = \
                  codeeditor.h \
                  commandgen.h \
                  configgen.h \
                  console.h \
                  filemanager.h \
                  globaldefs.h \
                  highlighter.h \
                  linenumberarea.h \
                  mainwindow.h \
                  procrunner.h \
                  search_replace.h \
                  statusbar.h
SOURCES         = \
                  codeeditor.cpp \
                  commandgen.cpp \
                  configgen.cpp \
                  console.cpp \
                  filemanager.cpp \
                  highlighter.cpp \
                  linenumberarea.cpp \
                  mainwindow.cpp \
                  main.cpp \
                  procrunner.cpp \
                  search_replace.cpp \
                  statusbar.cpp

## install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
#INSTALLS += target

FORMS += \
    search_replace.ui
