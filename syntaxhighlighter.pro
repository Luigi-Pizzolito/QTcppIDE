QT += widgets
requires(qtConfig(filedialog))

HEADERS         = \
                  highlighter.h \
                  linenumberarea.h \
                  mainwindow.h \
                  qtextedithighlighter.h
SOURCES         = \
                  highlighter.cpp \
                  linenumberarea.cpp \
                  mainwindow.cpp \
                  main.cpp \
                  qtextedithighlighter.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target
