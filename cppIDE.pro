QT += widgets
requires(qtConfig(filedialog))

HEADERS         = \
                  configgen.h \
                  console.h \
                  filemanager.h \
                  highlighter.h \
                  linenumberarea.h \
                  mainwindow.h \
                  procrunner.h \
                  qtexteditlineshighlighted.h
SOURCES         = \
                  configgen.cpp \
                  console.cpp \
                  filemanager.cpp \
                  highlighter.cpp \
                  linenumberarea.cpp \
                  mainwindow.cpp \
                  main.cpp \
                  procrunner.cpp \
                  qtexteditlineshighlighted.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

FORMS +=
