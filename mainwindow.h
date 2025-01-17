#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QFileDialog>
#include<QMessageBox>
#include <QJsonDocument>
#include<QFile>
#include <QDebug>
#include"sippmapf.hpp"
#include "map.h"
#include "MapAgent.h"
#include<unordered_set>
#include <vector>
#include<QVector>
#include<thread>
#include<map>
#include<cmath>
#include"customwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
typedef SIPP<State, State, Action, int, Environment> sipp_t;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    void orderByXThenByY(QVector<QPoint>* points);
    void orderByXThenDescOrderByY(QVector<QPoint>* points);
    void descOrderByXThenByY(QVector<QPoint>* points);
    void descOrderByXThenDescOrderByY(QVector<QPoint>* points);
    std::pair<QPointF,QPointF> CalculateCentroid(QVector<QPoint>* starts, QVector<QPoint>* gaols) const;
    void destructStates(std::pair<QVector<QPoint>*, QVector<QPoint>*> result);
    void simulate();
    void setStartByStepIndex(int idx);
    QVector<QVector<QPoint>>paths_;
    int current_index_ = 0;
    std::map<int, QVector<QPoint>*>steps_;
    bool is_simulate_ = false;
    std::unordered_set<State> obstacles_;
    std::vector<State> start_states_;
    std::vector<State> goals_;
    bool hasCollision(std::map<int, QVector<QPoint>*>&steps,int agent_width,int agent_height,int safe_distance);
    void getJudgeStates(State& s, std::map<State, int>& states, int safe_distance, int w, int h);
    void writeSolutionsToFile(std::vector<PlanResult<State, Action, int>> &solutions);
private slots:
    void spIntervalChange(int t);
    void updateNum(int start, int end);
    void pbExportMapClick();
    void pbImportMapClick();
  
    void pbPathPlanningClick();
    void pbLastStepClick();
    void pbNextStepClick();
    void pbStartSimulationClick();
    void pbStopSimulationClick();
    void pbResetSimulationClick();
    void updateCurrentXY(int x,int y);
    void pbInsertVideo1GoalObsClick();
    void pbInsertVideo2GoalObsClick();
    void pbInsertVideo3GoalObsClick();
    void npdAgentSizeChanged(int size);
    void spSafeDistanceChange(int size);
    void spCurrentStepIndexChanged(int idx) ;
};
#endif // MAINWINDOW_H
