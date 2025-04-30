// /********************************************************************************
// ** Form generated from reading UI file 'MainWindow.ui'
// **
// ** Created by: Qt User Interface Compiler version 6.9.0
// **
// ** WARNING! All changes made in this file will be lost when recompiling UI file!
// ********************************************************************************/

// #ifndef UI_MAINWINDOW_H
// #define UI_MAINWINDOW_H

// #include <GLWidget.h>
// #include <QtCore/QVariant>
// #include <QtGui/QAction>
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QComboBox>
// #include <QtWidgets/QGroupBox>
// #include <QtWidgets/QMainWindow>
// #include <QtWidgets/QMenu>
// #include <QtWidgets/QMenuBar>
// #include <QtWidgets/QPushButton>
// #include <QtWidgets/QSpinBox>
// #include <QtWidgets/QStatusBar>
// #include <QtWidgets/QWidget>

// QT_BEGIN_NAMESPACE

// class Ui_MainWindow
// {
// public:
//     QWidget *centralwidget;
//     GLWidget *openGLWidget;
//     QPushButton *pushButton;
//     QPushButton *pushButton_2;
//     QComboBox *comboBox;
//     QPushButton *pushButton_3;
//     QGroupBox *groupBox;
//     QSpinBox *spinBox;
//     QMenuBar *menubar;
//     QMenu *menuSketcher;
//     QStatusBar *statusbar;

//     void setupUi(QMainWindow *MainWindow)
//     {
//         if (MainWindow->objectName().isEmpty())
//             MainWindow->setObjectName("MainWindow");
//         MainWindow->resize(1109, 891);
//         centralwidget = new QWidget(MainWindow);
//         centralwidget->setObjectName("centralwidget");
//         openGLWidget = new GLWidget(centralwidget);
//         openGLWidget->setObjectName("openGLWidget");
//         openGLWidget->setGeometry(QRect(180, 50, 691, 651));
//         pushButton = new QPushButton(centralwidget);
//         pushButton->setObjectName("pushButton");
//         pushButton->setGeometry(QRect(190, 720, 151, 31));
//         pushButton_2 = new QPushButton(centralwidget);
//         pushButton_2->setObjectName("pushButton_2");
//         pushButton_2->setGeometry(QRect(720, 720, 141, 31));
//         comboBox = new QComboBox(centralwidget);
//         comboBox->addItem(QString());
//         comboBox->addItem(QString());
//         comboBox->addItem(QString());
//         comboBox->addItem(QString());
//         comboBox->addItem(QString());
//         comboBox->setObjectName("comboBox");
//         comboBox->setGeometry(QRect(60, 10, 161, 22));
//         pushButton_3 = new QPushButton(centralwidget);
//         pushButton_3->setObjectName("pushButton_3");
//         pushButton_3->setGeometry(QRect(60, 40, 61, 24));
//         groupBox = new QGroupBox(centralwidget);
//         groupBox->setObjectName("groupBox");
//         groupBox->setGeometry(QRect(900, 40, 161, 251));
//         spinBox = new QSpinBox(groupBox);
//         spinBox->setObjectName("spinBox");
//         spinBox->setGeometry(QRect(20, 40, 101, 22));
//         MainWindow->setCentralWidget(centralwidget);
//         menubar = new QMenuBar(MainWindow);
//         menubar->setObjectName("menubar");
//         menubar->setGeometry(QRect(0, 0, 1109, 22));
//         menuSketcher = new QMenu(menubar);
//         menuSketcher->setObjectName("menuSketcher");
//         MainWindow->setMenuBar(menubar);
//         statusbar = new QStatusBar(MainWindow);
//         statusbar->setObjectName("statusbar");
//         MainWindow->setStatusBar(statusbar);

//         menubar->addAction(menuSketcher->menuAction());

//         retranslateUi(MainWindow);

//         QMetaObject::connectSlotsByName(MainWindow);
//     } // setupUi

//     void retranslateUi(QMainWindow *MainWindow)
//     {
//         MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
//         pushButton->setText(QCoreApplication::translate("MainWindow", "Savefile", nullptr));
//         pushButton_2->setText(QCoreApplication::translate("MainWindow", "Load OBJ", nullptr));
//         comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Shapes", nullptr));
//         comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Beizer", nullptr));
//         comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Cube", nullptr));
//         comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Cylinder", nullptr));
//         comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Sphere", nullptr));

//         pushButton_3->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
//         groupBox->setTitle(QCoreApplication::translate("MainWindow", "Transformations", nullptr));
//         menuSketcher->setTitle(QCoreApplication::translate("MainWindow", "Sketcher", nullptr));
//     } // retranslateUi

// };

// namespace Ui {
//     class MainWindow: public Ui_MainWindow {};
// } // namespace Ui

// QT_END_NAMESPACE

// #endif // UI_MAINWINDOW_H
