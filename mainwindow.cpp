#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(updateStartEndNum(int,int)), this, SLOT(updateNum(int,int)));
    connect(ui->widget, SIGNAL(updateCurrentXY(int, int)), this, SLOT(updateCurrentXY(int, int)));
    ui->pbStopSimulation->setVisible(false);
    ui->pbResetSimulation->setVisible(false);
    ui->pbInsertVideo2GoalObs->setVisible(false);
    ui->pbInsertVideo3GoalObs->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::spIntervalChange(int t)
{
    ui->widget->drag_interval_ = t;

}
void MainWindow::updateNum(int start, int end)
{
    ui->labStartsNum->setText(QString::number(start));
    ui->labEndsNum->setText(QString::number(end));
}
std::pair<QPointF, QPointF> MainWindow::CalculateCentroid(QVector<QPoint>*starts, QVector<QPoint>* gaols) const
{
    int sxnum = 0;
    int synum = 0;
    for (int i = 0; i < starts->count(); i++)
    {
        sxnum += starts->at(i).x();
        synum += starts->at(i).y();
    }
    QPointF s(sxnum*1.0f / starts->count(), synum*1.0f / starts->count());
    int exnum = 0;
    int eynum = 0;
    for (int i = 0; i < gaols->count(); i++)
    {
        exnum += gaols->at(i).x();
        eynum += gaols->at(i).y();
    }
    QPointF e(sxnum*1.0f / gaols->count(), synum*1.0f / gaols->count());
    return std::make_pair(s, e);
}

void MainWindow::pbImportMapClick()
{

	QString file_name = QFileDialog::getOpenFileName(nullptr, tr("choice file"));
    if (!file_name.isEmpty()) {
        QFile file(file_name);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray jsonData = file.readAll();
            file.close();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            Map map;
            map.fromJson(doc.object());
            ui->widget->setMap(map);
        }
    }
	else 
    {
        qDebug() << tr("no choice file!");
    }
   
}

void MainWindow::destructStates(std::pair<QVector<QPoint>*, QVector<QPoint>*> result)
{
	auto pair = CalculateCentroid(result.first, result.second);
	auto s = pair.first;
	auto e = pair.second;
	if (!ui->ckbStartOrderByX->isChecked()&& !ui->ckbStartOrderByY->isChecked())
	{
		descOrderByXThenDescOrderByY(result.first);
		
	}
	else if (ui->ckbStartOrderByX->isChecked() && ui->ckbStartOrderByY->isChecked())
	{
		orderByXThenByY(result.first);
		
	}
	else if (!ui->ckbStartOrderByX->isChecked() && ui->ckbStartOrderByY->isChecked())
	{
		descOrderByXThenByY(result.first);
		
	}
	else
	{
		orderByXThenDescOrderByY(result.first);
		
	}


    if (!ui->ckbGoalOrderByX->isChecked() && !ui->ckbGoalOrderByY->isChecked())
    {
        descOrderByXThenDescOrderByY(result.second);

    }
    else if (ui->ckbGoalOrderByX->isChecked() && ui->ckbGoalOrderByY->isChecked())
    {
        orderByXThenByY(result.second);

    }
    else if (!ui->ckbGoalOrderByX->isChecked() && ui->ckbGoalOrderByY->isChecked())
    {
        descOrderByXThenByY(result.second);

    }
    else
    {
        orderByXThenDescOrderByY(result.second);

    }

}

void MainWindow::pbExportMapClick()
{
    auto result = ui->widget->getStartGoalPoints();
    if(result.first->size()==result.second->size()&&result.first->size()>0)
    {
       
        destructStates(result);
        Map m;
        for (int i=0;i<result.first->size();i++)
        {
            MapAgent agent;
            agent.start_x_ = result.first->at(i).x();
            agent.start_y_ = result.first->at(i).y();
            agent.end_x_ = result.second->at(i).x();
            agent.end_y_ = result.second->at(i).y();
            agent.distance_ = ui->spSafeDistance->value();
            agent.size_ = ui->spVolume->value();
            m.agents_.append(agent);
        }
        auto* obs = ui->widget->getObstaclePoints();
        for (int i=0;i<obs->size();i++)
        {
            m.obstacles_.append(obs->at(i));
        }
        QJsonDocument doc(m.toJson());
        QByteArray jsonBytes = doc.toJson();
        QFile file("output_test.json");
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(jsonBytes);
            file.close();
            qDebug() << tr("JSON file saved successfully!");
        }
        else
        {
            qDebug() << tr("Failed to save JSON file.");
        }
    }
    else
    {
        QMessageBox::information(this, tr("info"), tr("The number of starting points and ending points is not consistent!"));
    }
    
    
}
void MainWindow::descOrderByXThenByY(QVector<QPoint>* points)
{
    std::sort(points->begin(), points->end(), [](QPoint& p1, QPoint& p2)
    {
        if (p1.x() > p2.x()) 
        {
            return true; // X Öµ½µÐò
        }
        else if (p1.x() == p2.x()) 
        {

            return p1.y() < p2.y(); //Y ÉýÐò
        }
        return false;
    });
}
void MainWindow::descOrderByXThenDescOrderByY(QVector<QPoint>* points)
{
    std::sort(points->begin(), points->end(), [](QPoint& p1, QPoint& p2)
    {
        if (p1.x() > p2.x()) {
            return true; // X Öµ½µÐò
        }
        else if (p1.x() == p2.x()) {

            return p1.y() > p2.y(); //Y ½µÐò
        }
        return false;
    });
}
void MainWindow::orderByXThenByY(QVector<QPoint>* points)
{
    std::sort(points->begin(), points->end(), [](QPoint& p1, QPoint& p2)
    {
        if (p1.x() < p2.x()) {
            return true; // X ÖµÉýÐò
        }
        else if (p1.x() == p2.x()) {

            return p1.y() < p2.y(); //Y ÉýÐò
        }
        return false;
    });
}
void MainWindow::orderByXThenDescOrderByY(QVector<QPoint>* points)
{
    std::sort(points->begin(), points->end(), [](QPoint& p1, QPoint& p2)
    {
        if (p1.x() < p2.x()) {
            return true; // X ÖµÉýÐò
        }
        else if (p1.x() == p2.x()) {

            return p1.y() > p2.y(); //Y ½µÐò
        }
        return false;
    });
}
void MainWindow::pbPathPlanningClick()
{
    
    auto result = ui->widget->getStartGoalPoints();
    if (result.first->size() == result.second->size() && result.first->size() > 0)
    {
		destructStates(result);
        for (int i=0;i<result.first->size();i++)
        {
            start_states_.emplace_back(State(result.first->at(i).x(), result.first->at(i).y()));
            goals_.emplace_back(State(result.second->at(i).x(), result.second->at(i).y()));
        }
        auto* obs = ui->widget->getObstaclePoints();
        for (int i = 0; i < obs->size(); i++)
        {
            obstacles_.insert(State(obs->at(i).x(), obs->at(i).y()));
        }
    }

    SippMapf solver(ui->widget->rows_count_, ui->widget->rows_count_,obstacles_,start_states_,goals_);
    std::vector<PlanResult<State, Action, int>> solutions;
    double time = 0;
    auto rst = solver.Planning(solutions,time,ui->widget->agent_width_,ui->widget->agent_height_,ui->widget->safe_distance_);
    
    if (!rst)
    {
        QMessageBox::information(this, tr(""), tr("plan not success"));
    }
    paths_.clear();
    ui->labPlanTime->setText(QString::number(time));
   
    for (int i = 0; i < solutions.size(); i++)
    {
        QVector<QPoint>path;
        auto& solution = solutions[i];
        path.append(QPoint(solution.states[0].first.x, solution.states[0].first.y));
        for (int j = 1; j < solution.states.size(); j++)
        {
            for (int z = 0; z < solution.actions[j - 1].second; z++)
            {
                path.append(QPoint(solution.states[j].first.x, solution.states[j].first.y));
            }
        }
        paths_.append(path);

    }
    int maxlen = 0;
    for (int i = 0; i < paths_.size(); i++)
    {
        if (paths_[i].size() > maxlen)
        {
            maxlen = paths_[i].size();
        }
    }
    for (int i = 0; i < paths_.size(); i++)
    {
        if (paths_[i].size() < maxlen)
        {
            int x = paths_[i].back().x();
            int y = paths_[i].back().y();
            int s = maxlen - paths_[i].size();
            for (int j = 0; j < s; j++)
            {
                paths_[i].append(QPoint(x, y));
            }
        }
    }
    if (!rst)
    {
        int s = solutions.size();
	    for (int i=0;i<start_states_.size() - solutions.size();i++)
	    {
            QVector<QPoint>ps;
            for (int j=0;j<maxlen;j++)
            {
                ps.append(QPoint(start_states_[i+s].x, start_states_[i + s].y));
            }
            paths_.append(ps);
	    }
    }
    current_index_ = 0;
    steps_.clear();
    std::map<int, QVector<QPoint>*>steps;
    for (int i = 0; i < maxlen; i++)
    {
        QVector<QPoint>* ps = new QVector<QPoint>();
        QVector<QPoint>* ps1 = new QVector<QPoint>();
        for (int j = 0; j < paths_.size(); j++)
        {

            ps->append(paths_[j][i]);
            //ps->append(QPoint( paths_[j][i].x()+1, paths_[j][i].y()));
            for (int c=0;c<ui->spVolume->value();c++)
            {
	            for (int k=0;k< ui->spVolume->value();k++)
	            {
		            ps1->append(QPoint(paths_[j][i].x() + c, paths_[j][i].y() + k));
	            }
            }
        }
        steps[i] = ps1;
        steps_[i] = ps;
    }
    current_index_ = 0;
    auto r = hasCollision(steps_,ui->widget->agent_width_,ui->widget->agent_height_,ui->widget->safe_distance_);
    
    steps_=steps;
    obstacles_.clear();
    start_states_.clear();
    goals_.clear();
    if (r)
    {
        QMessageBox::information(this, tr(""), tr("plan has collision"));
        
    }
    current_index_++;
    setStartByStepIndex(current_index_);
    current_index_--;
    setStartByStepIndex(current_index_);

}
void MainWindow::setStartByStepIndex(int idx)
{
    ui->widget->clearStartPoints();

    ui->widget->setStartPoints(*steps_[idx]);
}
void MainWindow::pbLastStepClick()
{
    
	if (current_index_>=1)
	{
        current_index_--;
        setStartByStepIndex(current_index_);
	}
}
void MainWindow::pbNextStepClick()
{
	if(current_index_<=steps_.size()-2)
	{
        current_index_++;
        setStartByStepIndex(current_index_);
	}
}

void MainWindow::pbStartSimulationClick()
{
    if(steps_.size() == 0)
	{
		QMessageBox::information(this, tr(""), tr("no plan"));
		return;
	}
    else
    {
        std::thread t(&MainWindow::simulate, this);
        is_simulate_ = true;
        ui->widget->clearOtherPoints();
        t.detach();
    }
   

}
void MainWindow::pbStopSimulationClick()
{
    is_simulate_ = false;
}
void MainWindow::pbResetSimulationClick()
{
    is_simulate_ = false;
    current_index_ = 0;
    Map m(obstacles_,start_states_,goals_);
    ui->widget->setMap(m);
    
}
void MainWindow::simulate()
{
    current_index_++;
    setStartByStepIndex(current_index_);
    current_index_--;
    setStartByStepIndex(current_index_);
	while (is_simulate_&& current_index_ <= steps_.size() - 2)
	{
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        current_index_++;
        setStartByStepIndex(current_index_);
	}
}
bool MainWindow::hasCollision(std::map<int, QVector<QPoint>*>&steps, int agent_width, int agent_height, int safe_distance)
{
    bool result = false;
    std::vector<int>fails;
    if(safe_distance>0)
    {
        for (const auto& pair : steps)
        {
            
            auto* points = pair.second; 
            for (int i=0;i<points->size();i++)
            {
                auto p1 = points->at(i);
                std::map<State, int>states;
                State s(p1.x(), p1.y());
                getJudgeStates(s, states, safe_distance, agent_width, agent_height);
                for (int j=i+1;j<points->size();j++)
                {
                    auto p2 = points->at(j);
                    State sf(p2.x(), p2.y());
                    if (states.find(sf)!=states.end())
                    {
	                    result = true;
						fails.push_back(pair.first);
                        
						return result;
                    }
                    /*if (std::abs(p1.x() - p2.x())<=safe_distance&& std::abs(p1.y() - p2.y())<=safe_distance)
                    {
                        result = true;
                        fails.push_back(pair.first);
                    }*/
                    
                }
            }
        }
    }
    else
    {
	    
    }
    return result;
}
void MainWindow::updateCurrentXY(int x, int y)
{
	ui->labCurrentX->setText(QString::number(x));
	ui->labCurrentY->setText(QString::number(y));
}
void MainWindow::getJudgeStates(State& s, std::map<State,int>& states,int safe_distance,int w,int h)
{
	states.clear();
    int startX = s.x - (safe_distance + w - 1);
    int startY = s.y - (safe_distance + h - 1);
    for (int i = 0; i < 2*h-1+2*safe_distance; i++)
    {
        for (int j = 0; j <2*w-1+2*safe_distance; j++)
        {
            State t(startX + j, startY + i);
            
            states.insert(std::make_pair(t,1));
            

        }
    }
	
}
void MainWindow::pbInsertVideo1GoalObsClick()
{
    auto* ends = ui->widget->getStartGoalPoints().second;
    int w = ui->widget->agent_width_;
    int h = ui->widget->agent_height_;
    int d = ui->widget->safe_distance_;
    QVector<QPoint>obs;
    for (int i = 0; i < ends->size(); i++)
    {
        int sx = ends->at(i).x() - (d);
        int sy = ends->at(i).y() - (2*d+h);
        for (int j = 0; j < w+d*2; j++)
        {
            for (int k = 0; k < h+d*2; k++)
            {
                obs.append(QPoint(sx + j, sy + k));
            }
        }
    }
    ui->widget->appendObstaclePoints(obs);

    ui->widget->update();


}
void MainWindow::pbInsertVideo2GoalObsClick()
{
    auto* ends = ui->widget->getStartGoalPoints().second;
    QVector<QPoint>obs;
    for (int i = 0; i < ends->size(); i++)
    {
        int sx = ends->at(i).x() - 1 ;
        int sy = ends->at(i).y() - 4;
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                obs.append(QPoint(sx + j, sy + k));
            }
        }
    }
    ui->widget->appendObstaclePoints(obs);

    ui->widget->update();
}
void MainWindow::pbInsertVideo3GoalObsClick()
{
    auto* ends = ui->widget->getStartGoalPoints().second;
    QVector<QPoint>obs;
    for (int i = 0; i < ends->size(); i++)
    {
        int sx = ends->at(i).x() - 2;
        int sy = ends->at(i).y() - 6;
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 6; k++)
            {
                obs.append(QPoint(sx + j, sy + k));
            }
        }
    }
    ui->widget->appendObstaclePoints(obs);

    ui->widget->update();
}
void MainWindow::npdAgentSizeChanged(int size)
{
		ui->widget->setAgentSize(size,size);
}
void MainWindow::spSafeDistanceChange(int size)
{
    ui->widget->safe_distance_ = size;
}
void MainWindow::writeSolutionsToFile(std::vector<PlanResult<State, Action, int>>& solutions)
{
	
	
}
void MainWindow::spCurrentStepIndexChanged(int idx)
{
	/*current_index_ = idx;
	if (current_index_)
	{
		
	}
	*/
	if (idx>=0&&idx< steps_.size())
	{
        current_index_ = idx;
        setStartByStepIndex(current_index_);
	}
}
