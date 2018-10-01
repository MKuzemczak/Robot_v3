#include "gui.h"

GUI::GUI(QWidget *parent) :
    QWidget(parent)
{
    setWindowState(Qt::WindowMinimized/* | Qt::WindowMaximized*/);

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
    tabWidget->addTab(new TestTab(), "Test");
    tabWidget->addTab(new MainTab(), "Tab 1");
    tabWidget->addTab(b, "Tab 2");

    connect(tabWidget->widget(0), SIGNAL(initPressed()), program, SLOT(testRobotInit()));
    connect(tabWidget->widget(0), SIGNAL(startPressed()), program, SLOT(testRun()));
    connect(b, SIGNAL(pressed()), dialog, SLOT(exec()));

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    this->setLayout(layout);

/*    setWindowFlags(Qt::Window
    | Qt::WindowMinimizeButtonHint
    | Qt::WindowMaximizeButtonHint
    | Qt::WindowCloseButtonHint);
*/


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
