#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>

class AddPointDialog : public QDialog
{
    Q_OBJECT

    QPushButton * addButton,
                * cancelButton;

    QLabel * xLabel,
            * yLabel,
            * zLabel;

    QLineEdit * xEdit,
                *yEdit,
                * zEdit;


public:
    explicit AddPointDialog(QWidget * parent = nullptr);
    ~AddPointDialog();

    void setDefault(int x, int y, int z);

signals:
    void addValues(int, int, int);

public slots:
    void addPressed();
};

#endif // ADDPOINTDIALOG_H
