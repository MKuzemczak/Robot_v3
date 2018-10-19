#include "terminal.h"

Terminal::Terminal(QWidget * parent) :
    QWidget (parent)
{
    textEdit = new QPlainTextEdit(this);
    textEdit->setReadOnly(true);

    clearButton = new QPushButton("Wyczyść", this);
    clearButton->setMaximumSize(100, 25);

    hideButton = new QPushButton("Schowaj", this);
    hideButton->setMaximumSize(100, 25);

    label = new QLabel("Komunikaty aplikacji", this);
    label->setMaximumSize(150, 40);

    QWidget * fillWidget = new QWidget;
    fillWidget->setMaximumHeight(40);

    QGridLayout * l = new QGridLayout;
    l->addWidget(label, 0, 0);
    l->addWidget(clearButton, 0, 1);
    l->addWidget(hideButton, 0, 2);
    l->addWidget(fillWidget, 0, 3, 1, 2);
    l->addWidget(textEdit, 1, 0, 1, 5);
    l->setRowStretch(0, 40);

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(hideButton, SIGNAL(clicked()), this, SLOT(hiding()));

    setLayout(l);

    textEdit->setFont (QFont ("Consolas", 9));
}

QPlainTextEdit * Terminal::getTextEdit()
{
    return textEdit;
}

void Terminal::hiding()
{
    if(!clearButton->isHidden())
    {
        clearButton->hide();
        textEdit->hide();
        hideButton->setText("Pokaż");
        setMaximumHeight(40);
        static_cast<QGridLayout*>(layout())->setColumnStretch(1, 1);
        static_cast<QGridLayout*>(layout())->setColumnStretch(3, 500);
        emit hidden();
    }
    else
    {
        setMaximumHeight(400);
        clearButton->show();
        textEdit->show();
        hideButton->setText("Schowaj");
    }
}

void Terminal::showing()
{

}

void Terminal::clear()
{
    textEdit->clear();
}

Terminal & operator << (Terminal & t, QString s)
{
    t.getTextEdit()->appendPlainText(s);

    return t;
}

Terminal & operator << (Terminal & t, char const * s)
{
    t.getTextEdit()->appendPlainText(QString(s));

    return t;
}

Terminal & operator << (Terminal & t, char s)
{
    t.getTextEdit()->appendPlainText(QString(&s));

    return t;
}

Terminal & operator << (Terminal & t, double s)
{
    t.getTextEdit()->appendPlainText(QString("%1").arg(s));

    return t;
}

Terminal & operator << (Terminal & t, int s)
{
    t.getTextEdit()->appendPlainText(QString("%1").arg(s));

    return t;
}

Terminal & operator << (Terminal & t, std::string s)
{
    t.getTextEdit()->appendPlainText(QString(s.c_str()));

    return t;
}

void Terminal::write(QString s)
{
    *this << s;
}

void Terminal::write(int i)
{
    *this << i;
}

void Terminal::write(double d)
{
    *this << d;
}

void Terminal::write(char const * s)
{
    *this << s;
}

void Terminal::write(char c)
{
    *this << c;
}

void Terminal::write(std::string s)
{
    *this << s;
}
