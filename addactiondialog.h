#ifndef ADDACTIONDIALOG_H
#define ADDACTIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>

#include "actiontype.h"

class AddActionDialog : public QDialog
{
    Q_OBJECT

    QComboBox * typeList;

    QLineEdit * startPointEdit,
                * midPointEdit,
                * endPointEdit,
                * timeEdit,
                * newAngleEdit;

    QLabel * startPointLabel,
            * midPointLabel,
            * endPointLabel,
            * timeLabel,
            * newAngleLabel;

    QPushButton * addButton, * cancelButton;

public:
    AddActionDialog(QWidget * parent = nullptr);

    void enableAll();
    void disableAll();
    void setType(ActionType type);
    void setStart(QString s);
    void setMid(QString s);
    void setEnd(QString s);
    void setTime(QString s);
    void setAngle(QString s);
    void setInfo(QStringList s);

signals:
    void addValues(ActionType type, QString info);

public slots:
    void typeChanged(int index);
    void addClicked();

};

#endif // ADDACTIONDIALOG_H
