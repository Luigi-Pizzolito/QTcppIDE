#ifndef SEARCH_REPLACE_H
#define SEARCH_REPLACE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class Search_Replace;
}

class Search_Replace : public QMainWindow
{
    Q_OBJECT

public:
    explicit Search_Replace(QTextEdit *search_object, QWidget *parent = nullptr);
    ~Search_Replace();
    Ui::Search_Replace *ui;

    void setCurrentTab(int page);
    bool Searchact(QTextEdit *,bool backward_op,bool whole_word_op,bool Case_se_op,QString keystr,bool all=false);
    void Replaceact(QTextEdit *Se_object,bool backward_op,bool whole_word_op,bool Case_se_op,bool Prompt_on_Re,QString keystr);
    void All_Replaceact(QTextEdit *Se_object,bool backward_op,bool whole_word_op,bool Case_se_op,QString keystr);

private:

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // SEARCH_REPLACE_H
