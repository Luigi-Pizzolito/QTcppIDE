QT += widgets
requires(qtConfig(filedialog))

HEADERS         = \
                  console.h \
                  highlighter.h \
                  linenumberarea.h \
                  mainwindow.h \
                  qtexteditlineshighlighted.h
SOURCES         = \
                  console.cpp \
                  highlighter.cpp \
                  linenumberarea.cpp \
                  mainwindow.cpp \
                  main.cpp \
                  qtexteditlineshighlighted.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

FORMS +=
