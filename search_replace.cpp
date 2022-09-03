#include "search_replace.h"
#include "ui_search_replace.h"


#include <QString>
#include <QMap>
#include <QMessageBox>

Search_Replace::Search_Replace(QTextEdit *search_object, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Search_Replace)
{


    ui->setupUi(this);

    ui->forward->setChecked(true);


    connect(ui->FindButton,&QPushButton::clicked,this,[=](){
       Searchact(search_object,ui->backward->isChecked(),ui->whole_words->isChecked(),ui->Case->isChecked(),ui->keywords->text());
    });
    connect(ui->CloseButton,&QPushButton::clicked,this,&QWidget::close);
    connect(ui->CloseButton_2,&QPushButton::clicked,this,&QWidget::close);
    connect(ui->ReplaceButton,&QPushButton::clicked,this,[=](){
        Replaceact(search_object,ui->Re_backward->isChecked(),ui->Re_whole_words->isChecked(),ui->Re_Case->isChecked(),
                   ui->replace_prompt->isChecked(),ui->keywords_2->text());
    });
    connect(ui->all_replace,&QPushButton::clicked,this,[=](){
       All_Replaceact(search_object,ui->Re_backward->isChecked(),ui->Re_whole_words->isChecked(),ui->Re_Case->isChecked(),
                   ui->keywords_2->text());
    });
}
void Search_Replace::setCurrentTab(int page)
{
    ui->Search_Re_tab->setCurrentIndex(page-1);
}

bool Search_Replace::Searchact(QTextEdit *Se_object,bool backward_op,bool whole_word_op,bool Case_se_op,QString keystr,bool all)
{
    bool found;
    if(backward_op)
    {
       if(whole_word_op)
       {
           if(Case_se_op)
               found=Se_object->find(keystr,QTextDocument::FindBackward|QTextDocument::FindWholeWords|QTextDocument::FindCaseSensitively);
               else
               found=Se_object->find(keystr,QTextDocument::FindBackward|QTextDocument::FindWholeWords);
       }
       else
       {
           if(Case_se_op)
               found=Se_object->find(keystr,QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
               else
               found=Se_object->find(keystr,QTextDocument::FindBackward);
       }
    }
    else
    {
       if(whole_word_op)
       {
           if(Case_se_op)
               found=Se_object->find(keystr,QTextDocument::FindWholeWords|QTextDocument::FindCaseSensitively);
               else
               found=Se_object->find(keystr,QTextDocument::FindWholeWords);
       }
       else
       {
           if(Case_se_op)
               found=Se_object->find(keystr,QTextDocument::FindCaseSensitively);
               else
               found=Se_object->find(keystr);
       }
    }
    if((found==false)&&(all==false))
       {
        QString inf="Search string \""+keystr+"\" not found.";

        QMessageBox::information(this,"information",inf);


    }
    return found;
}
void Search_Replace::Replaceact(QTextEdit *Se_object,bool backward_op,bool whole_word_op,bool Case_se_op,bool Prompt_on_Re,QString keystr)
{
    QString tar_text=ui->Replace_tar->text();

    QString re_prompt="Replace this occurence of \""+keystr+"\"?";


        if(Searchact(Se_object,backward_op,whole_word_op,Case_se_op,keystr))
        {

        QTextCursor replace_cursor=Se_object->textCursor();


            if(Prompt_on_Re)
              {
                  QMessageBox::StandardButton result = QMessageBox::question(this,"确认Confirm",re_prompt);
                  if(result==QMessageBox::Yes)
                   {
                     replace_cursor.removeSelectedText();
                     replace_cursor.insertText(tar_text);
                   }
               }

             else
               {
                replace_cursor.removeSelectedText();
                replace_cursor.insertText(tar_text);
               }
        }

   }

void Search_Replace::All_Replaceact(QTextEdit *Se_object,bool backward_op,bool whole_word_op,bool Case_se_op,QString keystr)
{
    QString tar_text=ui->Replace_tar->text();
    while(Searchact(Se_object,backward_op,whole_word_op,Case_se_op,keystr,true))
    {

    QTextCursor replace_cursor=Se_object->textCursor();
    replace_cursor.removeSelectedText();
    replace_cursor.insertText(tar_text);
    }
    Se_object->moveCursor(QTextCursor::Start);

}


Search_Replace::~Search_Replace()
{
    delete ui;
}

void Search_Replace::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key::Key_Escape) {
        //close when esc pressed
        close();
    }
}
