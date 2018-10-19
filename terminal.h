#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

class Terminal : public QWidget
{
    Q_OBJECT

    QPlainTextEdit * textEdit;
    QPushButton * clearButton,
                * hideButton;
    QLabel * label;

public:
    Terminal(QWidget * parent = nullptr);

    QPlainTextEdit * getTextEdit();

signals:
    void hidden();

public slots:
    void clear();
    void hiding();
    void showing();
    void write(QString);
    void write(int);
    void write(double);
    void write(char const *);
    void write(char);
    void write(std::string);

};

Terminal & operator << (Terminal & t, QString s);
Terminal & operator << (Terminal & t, double s);
Terminal & operator << (Terminal & t, int s);
Terminal & operator << (Terminal & t, char const * s);
Terminal & operator << (Terminal & t, char s);

#endif // TERMINAL_H
