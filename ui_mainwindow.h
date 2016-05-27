/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOPEN;
    QAction *actionOPEN_2;
    QAction *actionSAVE;
    QAction *actionSAVE_AS;
    QAction *actionCLOSE;
    QAction *actionEXIT;
    QAction *actionOPTIONS;
    QAction *actionABOUT;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QMenu *menuPARSER;
    QMenu *menuFILE;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(706, 533);
        actionOPEN = new QAction(MainWindow);
        actionOPEN->setObjectName(QStringLiteral("actionOPEN"));
        actionOPEN_2 = new QAction(MainWindow);
        actionOPEN_2->setObjectName(QStringLiteral("actionOPEN_2"));
        actionSAVE = new QAction(MainWindow);
        actionSAVE->setObjectName(QStringLiteral("actionSAVE"));
        actionSAVE_AS = new QAction(MainWindow);
        actionSAVE_AS->setObjectName(QStringLiteral("actionSAVE_AS"));
        actionCLOSE = new QAction(MainWindow);
        actionCLOSE->setObjectName(QStringLiteral("actionCLOSE"));
        actionEXIT = new QAction(MainWindow);
        actionEXIT->setObjectName(QStringLiteral("actionEXIT"));
        actionOPTIONS = new QAction(MainWindow);
        actionOPTIONS->setObjectName(QStringLiteral("actionOPTIONS"));
        actionABOUT = new QAction(MainWindow);
        actionABOUT->setObjectName(QStringLiteral("actionABOUT"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 1, 0, 1, 2);

        plainTextEdit_2 = new QPlainTextEdit(centralWidget);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(16);
        plainTextEdit_2->setFont(font);

        gridLayout->addWidget(plainTextEdit_2, 3, 0, 1, 2);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 4, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 706, 19));
        menuPARSER = new QMenu(menuBar);
        menuPARSER->setObjectName(QStringLiteral("menuPARSER"));
        menuFILE = new QMenu(menuBar);
        menuFILE->setObjectName(QStringLiteral("menuFILE"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFILE->menuAction());
        menuBar->addAction(menuPARSER->menuAction());
        menuPARSER->addAction(actionOPTIONS);
        menuPARSER->addAction(actionABOUT);
        menuFILE->addAction(actionOPEN);
        menuFILE->addAction(actionOPEN_2);
        menuFILE->addAction(actionSAVE);
        menuFILE->addAction(actionSAVE_AS);
        menuFILE->addAction(actionCLOSE);
        menuFILE->addAction(actionEXIT);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOPEN->setText(QApplication::translate("MainWindow", "NEW", 0));
        actionOPEN_2->setText(QApplication::translate("MainWindow", "OPEN", 0));
        actionSAVE->setText(QApplication::translate("MainWindow", "SAVE", 0));
        actionSAVE_AS->setText(QApplication::translate("MainWindow", "SAVE AS..", 0));
        actionCLOSE->setText(QApplication::translate("MainWindow", "CLOSE", 0));
        actionEXIT->setText(QApplication::translate("MainWindow", "EXIT", 0));
        actionOPTIONS->setText(QApplication::translate("MainWindow", "OPTIONS", 0));
        actionABOUT->setText(QApplication::translate("MainWindow", "ABOUT", 0));
        pushButton->setText(QApplication::translate("MainWindow", "COMPILE", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "CLEAR", 0));
        plainTextEdit->setPlainText(QApplication::translate("MainWindow", "#include <names.h>				\n"
"#include \342\200\234kpa_config.h\342\200\235				\n"
"				\n"
"#define HW_SEQ_MKT_2    0x0002				\n"
"#define HW_SEQ_CTRL_ROOT    0x0004	\n"
"\n"
"#define KPA_IP1 0xDA\n"
"#define KPA_IP2 0xDB\n"
"#define KPA_IP3 0xDC\n"
"#define KPA_IP4 0xDD\n"
"				\n"
"HW_SEQUENCE (HW_SEQ_CTRL_ROOT) {				\n"
"    KPA_2M2C_0:0, 				\n"
"    KPA_IP1_1:2,				\n"
"    KPA_IP1_1:3,				\n"
"    KPA_IP1_1:4,				\n"
"    @HW_SEQ_MKT_2      ; HW_SEQUENCE(HW_SEQ_MKT_2) becomes the part of the				\n"
"                                              ; current  sequence				\n"
"};				\n"
"				\n"
"HW_SEQUENCE (HW_SEQ_MKT_2) {				\n"
"    KPA_MKT_0:0,				\n"
"    KPA_MKT_0:1,				\n"
"    KPA_MKT_0:2,				\n"
"    KPA_MKT_0:3,				\n"
"    KPA_MKT_0:4,				\n"
"    KPA_MKT_0:5,				\n"
"    KPA_MKT_1:2,				\n"
"    KPA_MKT_1:3,				\n"
"    KPA_MKT_1:4				\n"
"};				\n"
"	\n"
"HW_SEQUENCE (HW_SEQ_MKT_2) {				\n"
"    KPA_MKT_2:0,				\n"
"    KPA_MKT_2:1,				\n"
"    KPA_MKT_2:2,				\n"
"    KPA_MKT_2:3,				\n"
"    KPA"
                        "_MKT_2:4,				\n"
"    KPA_MKT_2:5,				\n"
"    KPA_MKT_3:2,				\n"
"    KPA_MKT_3:3,				\n"
"    KPA_MKT_3:4				\n"
"};			\n"
"				\n"
"				\n"
"#define IP1_UPDATE EVENT_L00	\n"
"\n"
"module  (  KPA_IP1  ,  SEQ_CTRL_ROOT    )       {\n"
"\n"
"\n"
"\n"
"\n"
"	procedure ( IP1_UPDATE) {			\n"
"		A := Ctrl;		\n"
"		Ctrl := A & 0xFFFE;		\n"
"		rg[11] := Vx + dVx;		\n"
"		rg[10] := Vx - dVx;		\n"
"		rg[1] := 0xA710;		\n"
"		Ctrl := 0x0002 | iif ( A & 0x0020, A | 0x8000, A);		\n"
"	}\n"
"}\n"
"\n"
"module(KPA_IP2, HW_SEQ_CTRL){VAR_EVENTS{Ctrl ::: IP1_UPDATE}}\n"
"\n"
"module	(	KPA_IP3	, 		 HW_CTRL_ROOT	)	 {	\n"
"	uword Vx : rg:7, dVx:rg:8;			\n"
"	uword Ctrl:port:0;			\n"
"	word A, B, C;			\n"
"}\n"
"				\n"
"module (KPA_IP4, HW_SEQ_CTRL_ROOT) {				\n"
"	uword Vx : rg:7, dVx:rg:8;			\n"
"	uword Ctrl:port:0;			\n"
"	word AX, BX, C;			\n"
"				\n"
"	VAR_EVENTS {			\n"
"		Ctrl ::: IP1_UPDATE		\n"
"	}			\n"
"				\n"
"	procedure ( IP1_UPDATE) {			\n"
"		A := Ctrl;		\n"
"		Ctrl := A & 0xFFFE;		\n"
"		rg[11] := Vx + dVx;		\n"
""
                        "		rg[10] := Vx - dVx;		\n"
"		rg[1] := 0xA710;		\n"
"		Ctrl := 0x0002 | iif ( A & 0x0020, A | 0x8000, A);		\n"
"	}			\n"
"				\n"
"	procedure Run (EVENT_GRUN) {			\n"
"		Ctrl := 0;		\n"
"		Vx := 0;		\n"
"		dVx := 0;		\n"
"		B := 0;		\n"
"		while (1) {		\n"
"			rg[3+B] := 0;	\n"
"			if (B > 5)	\n"
"				break;\n"
"			else	\n"
"				B := B + 1;\n"
"			if (Ctrl & 0x0200) {	\n"
"				local_send_event (IP1_UPDATE);\n"
"				end_proc ();\n"
"			}	\n"
"		}		\n"
"		debug_prnt (\"Module IP1 was run at %02d:%02d\", m_pos, m_channel);		\n"
"	}			\n"
"}				\n"
"", 0));
        menuPARSER->setTitle(QApplication::translate("MainWindow", "PARSER", 0));
        menuFILE->setTitle(QApplication::translate("MainWindow", "FILE", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
