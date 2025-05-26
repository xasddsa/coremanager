/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *overallCpuFrequencyLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *cpuGovernorLabel;
    QComboBox *cpuGovernorComboBox;
    QLabel *cpuCoresLabel;
    QListWidget *cpuCoresListWidget;
    QPushButton *applyChangesButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 300);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        overallCpuFrequencyLabel = new QLabel(centralwidget);
        overallCpuFrequencyLabel->setObjectName("overallCpuFrequencyLabel");

        verticalLayout->addWidget(overallCpuFrequencyLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        cpuGovernorLabel = new QLabel(centralwidget);
        cpuGovernorLabel->setObjectName("cpuGovernorLabel");

        horizontalLayout->addWidget(cpuGovernorLabel);

        cpuGovernorComboBox = new QComboBox(centralwidget);
        cpuGovernorComboBox->setObjectName("cpuGovernorComboBox");

        horizontalLayout->addWidget(cpuGovernorComboBox);


        verticalLayout->addLayout(horizontalLayout);

        cpuCoresLabel = new QLabel(centralwidget);
        cpuCoresLabel->setObjectName("cpuCoresLabel");

        verticalLayout->addWidget(cpuCoresLabel);

        cpuCoresListWidget = new QListWidget(centralwidget);
        cpuCoresListWidget->setObjectName("cpuCoresListWidget");

        verticalLayout->addWidget(cpuCoresListWidget);

        applyChangesButton = new QPushButton(centralwidget);
        applyChangesButton->setObjectName("applyChangesButton");

        verticalLayout->addWidget(applyChangesButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "CPU Speed Manager", nullptr));
        overallCpuFrequencyLabel->setText(QCoreApplication::translate("MainWindow", "Overall CPU Frequency: [placeholder_freq]", nullptr));
        cpuGovernorLabel->setText(QCoreApplication::translate("MainWindow", "CPU Governor/Frequency:", nullptr));
        cpuCoresLabel->setText(QCoreApplication::translate("MainWindow", "CPU Cores:", nullptr));
        applyChangesButton->setText(QCoreApplication::translate("MainWindow", "Apply Changes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
