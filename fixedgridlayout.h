#ifndef FIXEDGRIDLAYOUT_H
#define FIXEDGRIDLAYOUT_H

#include <QGridLayout>

class QWidget;

/**

class FixedGridLayout
QGridView with fixed size cells, MUST be constructed with at least column width, row height and
parent QWidget* are optional. addItem() and addLayout() methods are made private so that only
widgets can be added, because only widgets can have fixed sizes, and that is required for this
class to work properly. If a layout needs to be added, just create a QWidget and use setLayout()
to give it your desired layout, then add the widget to FixedGridLayout.
IMPORTANT! When using this class it's VERY important that you don't
manually set the size of widgets being inserted, otherwise the outcome will not be as expected.
*/
class FixedGridLayout : public QGridLayout {

    int width, height;

    // Make addItem() and addLayout() private so only widgets can be added
    void addItem(QLayoutItem*, int, int, int, int, Qt::Alignment);
    void addLayout(QLayout*, int, int, Qt::Alignment);
    void addLayout(QLayout*, int, int, int rowSpan, int columnSpan, Qt::Alignment);

public:

    FixedGridLayout(int col_width, int row_height = -1, QWidget *parent = nullptr);
    ~FixedGridLayout();

    void addWidget(QWidget *widget, int row, int col, Qt::Alignment alignment = nullptr);
    void addWidget(QWidget *widget, int row, int col, int row_span, int col_span,
                   Qt::Alignment alignment = 0);

};

#endif // FIXEDGRIDLAYOUT_H
