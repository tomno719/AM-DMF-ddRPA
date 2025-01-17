/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtDesigner/QDesignerCustomWidgetCollectionInterface"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    CustomWidget *widget;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *spInterval;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpinBox *spVolume;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QSpinBox *spSafeDistance;
    QVBoxLayout *verticalLayout;
    QPushButton *pbExportMap;
    QPushButton *pbInputMap;
    QPushButton *pbPahtPlanning;
    QLabel *label_3;
    QLabel *labStartsNum;
    QLabel *label_6;
    QLabel *labEndsNum;
    QLabel *label_7;
    QLabel *labPlanTime;
    QCheckBox *ckbStartOrderByX;
    QCheckBox *ckbStartOrderByY;
    QPushButton *pbLastStep;
    QPushButton *pbNextStep;
    QPushButton *pbStartSimulation;
    QPushButton *pbStopSimulation;
    QPushButton *pbResetSimulation;
    QLabel *label_4;
    QLabel *label_8;
    QCheckBox *ckbGoalOrderByX;
    QCheckBox *ckbGoalOrderByY;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *labCurrentX;
    QLabel *labCurrentY;
    QPushButton *pbInsertVideo1GoalObs;
    QPushButton *pbInsertVideo2GoalObs;
    QPushButton *pbInsertVideo3GoalObs;
    QSpinBox *spCurrentStepIndex;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(943, 997);
        QFont font;
        font.setPointSize(11);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widget = new CustomWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(471, 531));
        splitter->addWidget(widget);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(400, 752));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 371, 331));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(51, 21));

        horizontalLayout_2->addWidget(label);

        spInterval = new QSpinBox(layoutWidget);
        spInterval->setObjectName(QString::fromUtf8("spInterval"));
        spInterval->setMinimumSize(QSize(161, 21));
        spInterval->setMinimum(1);

        horizontalLayout_2->addWidget(spInterval);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(51, 21));

        horizontalLayout_3->addWidget(label_2);

        spVolume = new QSpinBox(layoutWidget);
        spVolume->setObjectName(QString::fromUtf8("spVolume"));
        spVolume->setMinimumSize(QSize(161, 21));
        spVolume->setMinimum(1);

        horizontalLayout_3->addWidget(spVolume);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(51, 21));

        horizontalLayout_4->addWidget(label_5);

        spSafeDistance = new QSpinBox(layoutWidget);
        spSafeDistance->setObjectName(QString::fromUtf8("spSafeDistance"));
        spSafeDistance->setMinimumSize(QSize(161, 21));
        spSafeDistance->setMinimum(1);

        horizontalLayout_4->addWidget(spSafeDistance);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pbExportMap = new QPushButton(layoutWidget);
        pbExportMap->setObjectName(QString::fromUtf8("pbExportMap"));
        pbExportMap->setMinimumSize(QSize(221, 31));

        verticalLayout->addWidget(pbExportMap);

        pbInputMap = new QPushButton(layoutWidget);
        pbInputMap->setObjectName(QString::fromUtf8("pbInputMap"));
        pbInputMap->setMinimumSize(QSize(221, 31));

        verticalLayout->addWidget(pbInputMap);

        pbPahtPlanning = new QPushButton(layoutWidget);
        pbPahtPlanning->setObjectName(QString::fromUtf8("pbPahtPlanning"));
        pbPahtPlanning->setMinimumSize(QSize(221, 31));

        verticalLayout->addWidget(pbPahtPlanning);


        verticalLayout_3->addLayout(verticalLayout);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 380, 91, 21));
        labStartsNum = new QLabel(tab_2);
        labStartsNum->setObjectName(QString::fromUtf8("labStartsNum"));
        labStartsNum->setGeometry(QRect(140, 380, 61, 21));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 430, 91, 21));
        labEndsNum = new QLabel(tab_2);
        labEndsNum->setObjectName(QString::fromUtf8("labEndsNum"));
        labEndsNum->setGeometry(QRect(140, 430, 61, 21));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 480, 141, 21));
        labPlanTime = new QLabel(tab_2);
        labPlanTime->setObjectName(QString::fromUtf8("labPlanTime"));
        labPlanTime->setGeometry(QRect(180, 480, 121, 21));
        ckbStartOrderByX = new QCheckBox(tab_2);
        ckbStartOrderByX->setObjectName(QString::fromUtf8("ckbStartOrderByX"));
        ckbStartOrderByX->setGeometry(QRect(20, 540, 151, 21));
        ckbStartOrderByX->setChecked(false);
        ckbStartOrderByY = new QCheckBox(tab_2);
        ckbStartOrderByY->setObjectName(QString::fromUtf8("ckbStartOrderByY"));
        ckbStartOrderByY->setGeometry(QRect(220, 540, 151, 21));
        pbLastStep = new QPushButton(tab_2);
        pbLastStep->setObjectName(QString::fromUtf8("pbLastStep"));
        pbLastStep->setGeometry(QRect(10, 610, 101, 51));
        pbNextStep = new QPushButton(tab_2);
        pbNextStep->setObjectName(QString::fromUtf8("pbNextStep"));
        pbNextStep->setGeometry(QRect(120, 610, 101, 51));
        pbStartSimulation = new QPushButton(tab_2);
        pbStartSimulation->setObjectName(QString::fromUtf8("pbStartSimulation"));
        pbStartSimulation->setGeometry(QRect(10, 670, 101, 51));
        pbStopSimulation = new QPushButton(tab_2);
        pbStopSimulation->setObjectName(QString::fromUtf8("pbStopSimulation"));
        pbStopSimulation->setEnabled(false);
        pbStopSimulation->setGeometry(QRect(120, 670, 101, 51));
        pbResetSimulation = new QPushButton(tab_2);
        pbResetSimulation->setObjectName(QString::fromUtf8("pbResetSimulation"));
        pbResetSimulation->setEnabled(false);
        pbResetSimulation->setGeometry(QRect(230, 670, 101, 51));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 520, 131, 16));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 560, 131, 16));
        ckbGoalOrderByX = new QCheckBox(tab_2);
        ckbGoalOrderByX->setObjectName(QString::fromUtf8("ckbGoalOrderByX"));
        ckbGoalOrderByX->setGeometry(QRect(20, 580, 151, 21));
        ckbGoalOrderByX->setChecked(false);
        ckbGoalOrderByY = new QCheckBox(tab_2);
        ckbGoalOrderByY->setObjectName(QString::fromUtf8("ckbGoalOrderByY"));
        ckbGoalOrderByY->setGeometry(QRect(220, 580, 151, 21));
        ckbGoalOrderByY->setChecked(true);
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(220, 390, 61, 31));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(220, 450, 61, 31));
        labCurrentX = new QLabel(tab_2);
        labCurrentX->setObjectName(QString::fromUtf8("labCurrentX"));
        labCurrentX->setGeometry(QRect(300, 390, 71, 21));
        labCurrentY = new QLabel(tab_2);
        labCurrentY->setObjectName(QString::fromUtf8("labCurrentY"));
        labCurrentY->setGeometry(QRect(300, 450, 71, 31));
        pbInsertVideo1GoalObs = new QPushButton(tab_2);
        pbInsertVideo1GoalObs->setObjectName(QString::fromUtf8("pbInsertVideo1GoalObs"));
        pbInsertVideo1GoalObs->setGeometry(QRect(20, 750, 311, 51));
        pbInsertVideo2GoalObs = new QPushButton(tab_2);
        pbInsertVideo2GoalObs->setObjectName(QString::fromUtf8("pbInsertVideo2GoalObs"));
        pbInsertVideo2GoalObs->setEnabled(false);
        pbInsertVideo2GoalObs->setGeometry(QRect(20, 820, 311, 51));
        pbInsertVideo3GoalObs = new QPushButton(tab_2);
        pbInsertVideo3GoalObs->setObjectName(QString::fromUtf8("pbInsertVideo3GoalObs"));
        pbInsertVideo3GoalObs->setEnabled(false);
        pbInsertVideo3GoalObs->setGeometry(QRect(20, 890, 311, 51));
        spCurrentStepIndex = new QSpinBox(tab_2);
        spCurrentStepIndex->setObjectName(QString::fromUtf8("spCurrentStepIndex"));
        spCurrentStepIndex->setGeometry(QRect(240, 610, 141, 51));
        spCurrentStepIndex->setMaximum(1000000);
        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(pbStartSimulation, SIGNAL(clicked()), MainWindow, SLOT(pbStartSimulationClick()));
        QObject::connect(pbResetSimulation, SIGNAL(clicked()), MainWindow, SLOT(pbResetSimulationClick()));
        QObject::connect(pbLastStep, SIGNAL(clicked()), MainWindow, SLOT(pbLastStepClick()));
        QObject::connect(spInterval, SIGNAL(valueChanged(int)), MainWindow, SLOT(spIntervalChange(int)));
        QObject::connect(pbExportMap, SIGNAL(clicked()), MainWindow, SLOT(pbImportMapClick()));
        QObject::connect(pbNextStep, SIGNAL(clicked()), MainWindow, SLOT(pbNextStepClick()));
        QObject::connect(pbStopSimulation, SIGNAL(clicked()), MainWindow, SLOT(pbStopSimulationClick()));
        QObject::connect(pbInputMap, SIGNAL(clicked()), MainWindow, SLOT(pbExportMapClick()));
        QObject::connect(pbPahtPlanning, SIGNAL(clicked()), MainWindow, SLOT(pbPathPlanningClick()));
        QObject::connect(pbInsertVideo1GoalObs, SIGNAL(clicked()), MainWindow, SLOT(pbInsertVideo1GoalObsClick()));
        QObject::connect(pbInsertVideo2GoalObs, SIGNAL(clicked()), MainWindow, SLOT(pbInsertVideo2GoalObsClick()));
        QObject::connect(pbInsertVideo3GoalObs, SIGNAL(clicked()), MainWindow, SLOT(pbInsertVideo3GoalObsClick()));
        QObject::connect(spVolume, SIGNAL(valueChanged(int)), MainWindow, SLOT(npdAgentSizeChanged(int)));
        QObject::connect(spSafeDistance, SIGNAL(valueChanged(int)), MainWindow, SLOT(spSafeDistanceChange(int)));
        QObject::connect(spCurrentStepIndex, SIGNAL(valueChanged(int)), MainWindow, SLOT(spCurrentStepIndexChanged(int)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PathPlanning", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\213\226\345\212\250\351\227\264\351\232\224", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\275\223\347\247\257", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\256\211\345\205\250\350\267\235\347\246\273", nullptr));
        pbExportMap->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\345\234\260\345\233\276", nullptr));
        pbInputMap->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\234\260\345\233\276", nullptr));
        pbPahtPlanning->setText(QCoreApplication::translate("MainWindow", "\350\247\204\345\210\222\350\267\257\345\276\204", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271\346\225\260\351\207\217", nullptr));
        labStartsNum->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\347\273\210\347\202\271\346\225\260\351\207\217", nullptr));
        labEndsNum->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\350\247\204\345\210\222\347\224\250\346\227\266(ms)", nullptr));
        labPlanTime->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        ckbStartOrderByX->setText(QCoreApplication::translate("MainWindow", "X \345\215\207\345\272\217", nullptr));
        ckbStartOrderByY->setText(QCoreApplication::translate("MainWindow", "Y \345\215\207\345\272\217", nullptr));
        pbLastStep->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\345\270\247", nullptr));
        pbNextStep->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\345\270\247", nullptr));
        pbStartSimulation->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213", nullptr));
        pbStopSimulation->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234", nullptr));
        pbResetSimulation->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\347\273\210\347\202\271", nullptr));
        ckbGoalOrderByX->setText(QCoreApplication::translate("MainWindow", "X \345\215\207\345\272\217", nullptr));
        ckbGoalOrderByY->setText(QCoreApplication::translate("MainWindow", "Y \345\215\207\345\272\217", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215X", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215Y", nullptr));
        labCurrentX->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labCurrentY->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pbInsertVideo1GoalObs->setText(QCoreApplication::translate("MainWindow", "\346\217\222\345\205\245Video1\351\232\234\347\242\215\347\202\271", nullptr));
        pbInsertVideo2GoalObs->setText(QCoreApplication::translate("MainWindow", "\346\217\222\345\205\245Video2\351\232\234\347\242\215\347\202\271", nullptr));
        pbInsertVideo3GoalObs->setText(QCoreApplication::translate("MainWindow", "\346\217\222\345\205\245Video3\351\232\234\347\242\215\347\202\271", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\350\267\257\345\276\204\350\247\204\345\210\222", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
