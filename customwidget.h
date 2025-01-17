#pragma once
#include <QtWidgets/QtWidgets>
#include <QLineF>
#include <QVector>
#include"Map.h"
enum GridState
{
	//white middle
	None = 0,
	//blue left+ctrl
	Start = 1,
	// red  right + ctrl
	Goal = 2,
	//green middle+ctrl
	Obstacle = 3,
	//blue 
	StartOther = 4,
	//red
	GoalOther = 5,

};
class CustomWidget :public  QWidget
{
	Q_OBJECT
public:
	CustomWidget(QWidget* parent) :QWidget(parent)
	{
		initLines();
		setMouseTracking(true);
		initStates();
		restoreDragRect();
	}
	int drag_interval_ = 1;
	int rows_count_ = 512;
	std::pair<QVector<QPoint>*, QVector<QPoint>*>getStartGoalPoints();
	QVector<QPoint>* getObstaclePoints();
	void setMap(Map& m);
	void clearStartPoints();
	void setStartPoints(const QVector<QPoint>& starts);
	QVector<QVector<int>>* getStateClone();
	void appendObstaclePoints(const QVector<QPoint>& obstacles);
	int agent_width_ = 1;
	int agent_height_ = 1;
	int safe_distance_ = 1;
	void setAgentSize(int width, int height);
	void clearOtherPoints();
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
 signals:
	 void updateStartEndNum(int start, int end);
	 void updateCurrentXY(int x, int y);
private:
	QVector<QLineF>vertical_lines_;
	QVector<QLineF>horizontal_lines_;
	qreal scale_factor_ = 1.0;
	bool is_translate_ = false;
	float translate_x_ = 0.0f;
	float translate_y_ = 0.0f;
	QPointF click_down_point_;
	QPointF drag_end_point_;
	QPointF drag_start_point_;
	QVector<QVector<int>> state_;
	float grid_length_ = 5.0;
	bool is_multi_choice_ = false;
	QRectF drag_rect_;
	int drag_state_ = 0;
	std::pair<QPoint, QPoint> getRectStartEnds();
	void restoreDragRect();
	void initStates();
	void updateStartGoalNum();
	void initLines();
};
