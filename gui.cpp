#include "gui.h"

GUI::GUI(QWidget *parent) :
    QWidget(parent)
{
    setWindowState(Qt::WindowMinimized | Qt::WindowMaximized);

    QDesktopWidget * desktop = QApplication::desktop();

    this->setMinimumSize(620, 300);
    this->setMaximumSize(desktop->availableGeometry().width(), desktop->availableGeometry().height());

    program = new Program();
    program->moveToThread(&programThread);
    connect(this, SIGNAL(keyPressed(int)), program, SLOT(keyPressed(int)));
    programThread.start();

    QPushButton * b = new QPushButton("Tab 2");
    dialog = new QDialog(this);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new MainTab(program), "Tab 1");
    tabWidget->addTab(new TestTab(), "Test");
    tabWidget->addTab(b, "Tab 2");



    // tu moze wyjebac segmenta
    program->setPointListPtr(static_cast<MainTab*>(tabWidget->widget(0))->getPointList());

    connect(tabWidget->widget(1), SIGNAL(initPressed()), program, SLOT(testRobotInit()));
    connect(tabWidget->widget(1), SIGNAL(startPressed()), program, SLOT(testRun()));
    connect(b, SIGNAL(pressed()), dialog, SLOT(exec()));

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    this->setLayout(layout);
}


void GUI::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e->key());

}

GUI::~GUI()
{
    programThread.quit();
    programThread.wait();
}
