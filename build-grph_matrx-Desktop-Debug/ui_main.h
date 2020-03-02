/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *main)
    {
        if (main->objectName().isEmpty())
            main->setObjectName(QStringLiteral("main"));
        main->resize(400, 300);
        menuBar = new QMenuBar(main);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        main->setMenuBar(menuBar);
        mainToolBar = new QToolBar(main);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        main->addToolBar(mainToolBar);
        centralWidget = new QWidget(main);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        main->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(main);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        main->setStatusBar(statusBar);

        retranslateUi(main);

        QMetaObject::connectSlotsByName(main);
    } // setupUi

    void retranslateUi(QMainWindow *main)
    {
        main->setWindowTitle(QApplication::translate("main", "main", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class main: public Ui_main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
