#include"customwidget.h"
 std::pair<QVector<QPoint>*, QVector<QPoint>*> CustomWidget::getStartGoalPoints()
 {
	 auto* starts = new QVector<QPoint>();
	 auto* goals = new QVector<QPoint>();
	 for (int i = 0; i < state_.size(); i++)
	 {
		 for (int j = 0; j < state_[i].size(); j++)
		 {
			 if (state_[i][j] == GridState::Start)
			 {
				 starts->append(QPoint(i, j));
			 }
			 else if (state_[i][j] == GridState::Goal)
			 {
				 goals->append(QPoint(i, j));
			 }
		 }
	 }
	 return std::make_pair(starts, goals);
 }
 QVector<QPoint>* CustomWidget::getObstaclePoints()
 {

	 auto* obs = new QVector<QPoint>();
	 for (int i = 0; i < state_.count(); i++)
	 {
		 for (int j = 0; j < state_[i].count(); j++)
		 {
			 if (state_[i][j] == GridState::Obstacle)
			 {
				 obs->append(QPoint(i, j));
			 }

		 }
	 }
	 return obs;

 }
 void CustomWidget::setMap(Map& m)
 {

	 for (int i = 0; i < state_.size(); i++)
	 {
		 for (int j = 0; j < state_[i].size(); j++)
		 {
			 state_[i][j] = GridState::None;
		 }
	 }
	 for (int i = 0; i < m.agents_.size(); i++)
	 {
		 


		 for (int k=0;k<agent_width_;k++)
		 {
			 for (int l=0;l<agent_height_;l++)
			 {
				 state_[m.agents_[i].end_x_+k][m.agents_[i].end_y_+l] = GridState::GoalOther;
			 }
		 }
		 state_[m.agents_[i].end_x_][m.agents_[i].end_y_] = GridState::Goal;
		 for (int k = 0; k < agent_width_; k++)
		 {
			 for (int l = 0; l < agent_height_; l++)
			 {
				 state_[m.agents_[i].start_x_ + k][m.agents_[i].start_y_ + l] = GridState::StartOther;
			 }
		 }
		 state_[m.agents_[i].start_x_][m.agents_[i].start_y_] = GridState::Start;
	 }

	 for (int i = 0; i < m.obstacles_.size(); i++)
	 {
		 state_[m.obstacles_[i].x()][m.obstacles_[i].y()] = GridState::Obstacle;

	 }
	 updateStartGoalNum();
	 update();


 }
 void CustomWidget::clearStartPoints()
 {

	 for (int i = 0; i < state_.size(); i++)
	 {
		 for (int j = 0; j < state_[i].size(); j++)
		 {
			 if (state_[i][j] == GridState::Start)
			 {
				 state_[i][j] = GridState::None;
			 }
		 }
	 }
	 update();

 }
 void CustomWidget::setStartPoints(const QVector<QPoint>& starts)
 {
	 if (starts.size()>0)
	 {
		 for (auto p : starts)
		 {
			 state_[p.x()][p.y()] = GridState::Start;
		 }
		 update();
	 }
	 

 }
 void CustomWidget::paintEvent(QPaintEvent* event) 
 {
	
     QPainter painter(this);
	 painter.setRenderHint(QPainter::Antialiasing);
	 painter.setPen(QPen(QBrush(Qt::black), 2));

	 painter.drawLines(vertical_lines_);
	 painter.drawLines(horizontal_lines_);
	 auto c = Qt::white;

	 for (int i = 0; i < state_.size(); i++)
	 {
		 for (int j = 0; j < state_[i].size(); j++)
		 {
			 switch (state_[i][j])
			 {
			 case 1:
				 c = Qt::blue;
				 break;
			 case 2:
				 c = Qt::red;
				 break;
			 case 3:
				 c = Qt::green;
				 break;
			 case 4:
				 c = Qt::blue;
				 break;
			 case 5:
				 c = Qt::red;
				 break;
			 }



			 if (state_[i][j] != 0)
			 {
				 painter.fillRect(QRectF(i * grid_length_ + vertical_lines_[0].x1() + 1, j * grid_length_ + vertical_lines_[0].y1() + 1, grid_length_ - 2, grid_length_ - 2), QBrush(c));
			 }


		 }
	 }
	 if (is_multi_choice_)
	 {
		 auto x = drag_end_point_.x() < drag_start_point_.x() ? drag_end_point_.x() : drag_start_point_.x();
		 auto y = drag_end_point_.y() < drag_start_point_.y() ? drag_end_point_.y() : drag_start_point_.y();
		 auto w = abs(drag_end_point_.x() - drag_start_point_.x());
		 auto h = abs(drag_end_point_.y() - drag_start_point_.y());
		 drag_rect_.setX(x);
		 drag_rect_.setY(y);
		 drag_rect_.setWidth(w);
		 drag_rect_.setHeight(h);
		 QColor fillColor = QColor(255, 0, 0, 128);
		 painter.fillRect(drag_rect_, QBrush(fillColor));

	 }


 }
 void CustomWidget::mousePressEvent(QMouseEvent* event)
 {
	 if (event->button() == Qt::LeftButton && event->modifiers() == Qt::AltModifier)
	 {
		 is_translate_ = true;
		 click_down_point_ = mapFromGlobal(QCursor::pos());
	 }
	 else if ((event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton || event->button() == Qt::RightButton) && event->modifiers() == Qt::ControlModifier)
	 {
		 is_multi_choice_ = true;
		 drag_start_point_ = mapFromGlobal(QCursor::pos());
		 drag_state_ = event->button() == Qt::LeftButton ? GridState::Start : event->button() == Qt::MiddleButton ? GridState::Obstacle : GridState::Goal;
	 }
	 else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier)
	 {
		 is_multi_choice_ = true;
		 drag_start_point_ = mapFromGlobal(QCursor::pos());
		 drag_state_ = GridState::None;
	 }
	 updateStartGoalNum();
 }
 void CustomWidget::mouseMoveEvent(QMouseEvent* event)
 {
	 QPointF mousePos = mapFromGlobal(QCursor::pos());
	 if (is_translate_)
	 {
		 translate_x_ = mousePos.x() - click_down_point_.x();
		 translate_y_ = mousePos.y() - click_down_point_.y();
		 for (int i = 0; i < vertical_lines_.count(); i++)
		 {
			 vertical_lines_[i].setP1(QPointF(vertical_lines_[i].p1().x() + translate_x_, vertical_lines_[i].p1().y() + translate_y_));
			 vertical_lines_[i].setP2(QPointF(vertical_lines_[i].p2().x() + translate_x_, vertical_lines_[i].p2().y() + translate_y_));
		 }
		 for (int i = 0; i < horizontal_lines_.count(); i++)
		 {
			 horizontal_lines_[i].setP1(QPointF(horizontal_lines_[i].p1().x() + translate_x_, horizontal_lines_[i].p1().y() + translate_y_));
			 horizontal_lines_[i].setP2(QPointF(horizontal_lines_[i].p2().x() + translate_x_, horizontal_lines_[i].p2().y() + translate_y_));
		 }
		 update();
		 click_down_point_.setX(mousePos.x());
		 click_down_point_.setY(mousePos.y());

	 }
	 if (is_multi_choice_)
	 {
		 drag_end_point_.setX(mapFromGlobal(QCursor::pos()).x());
		 drag_end_point_.setY(mapFromGlobal(QCursor::pos()).y());
		 update();
	 }
	 int cur_x = (mousePos.rx()- vertical_lines_[0].x1()) / grid_length_;
	 int cur_y = (mousePos.ry() - vertical_lines_[0].y1()) / grid_length_;
	 if(cur_x >= 0 && cur_x < rows_count_ && cur_y >= 0 && cur_y < rows_count_)
	 {
		 emit updateCurrentXY(cur_x, cur_y);
	 }
 }
 void CustomWidget::mouseReleaseEvent(QMouseEvent* event)
 {
	 if (is_translate_)
	 {
		 is_translate_ = false;
		 return;
	 }
	 if (is_multi_choice_)
	 {
		 is_multi_choice_ = false;
		 if (drag_rect_.x() != 0 && drag_rect_.y() != 0 && drag_rect_.width() != 0 && drag_rect_.height()
			 != 0)
		 {
			 auto region = getRectStartEnds();

			 int interval = drag_state_ == None ? 1 : drag_interval_;
			 if (region.first.x() != 0 || region.first.y() != 0 || region.second.x() != 0 || region.second.y() != 0)
			 {
				 for (int i = region.first.x(); i <= region.second.x(); i += interval)
				 {
					 for (int j = region.first.y(); j <= region.second.y(); j += interval)
					 {
						 if (i >= 0 && i < rows_count_ && j >= 0 && j < rows_count_)
						 {
							for (int k = 0; k <agent_width_; k++)
							{
								for (int l = 0; l < agent_height_; l++)
								{
									if (drag_state_==1)
									{
										state_[j+k][i+l] = 4;
									}
									else if (drag_state_ == 2)
									{
										state_[j+k][i+l] = 5;
									}
									
								}
							}
							 state_[j][i] = drag_state_;

						 }
					 }
				 }
			 }

			 update();
			 updateStartGoalNum();

		 }

		 return;
	 }
 }
 void CustomWidget::wheelEvent(QWheelEvent* event)
 {
	 QPointF mouse_pos = mapFromGlobal(QCursor::pos());

	 // 更新缩放因子
	 scale_factor_ = event->delta() > 0 ? 1.25 : 0.8;
	 QPointF scalePoint;
	 scalePoint.setX(mouse_pos.x());
	 scalePoint.setY(mouse_pos.y());
	 for (int j = 0; j <= rows_count_; j++)
	 {
		 auto x1 = vertical_lines_[j].x1() * scale_factor_ + scalePoint.x() * (1 - scale_factor_);
		 auto y1 = vertical_lines_[j].y1() * scale_factor_ + scalePoint.y() * (1 - scale_factor_);
		 auto x2 = vertical_lines_[j].x2() * scale_factor_ + scalePoint.x() * (1 - scale_factor_);
		 auto y2 = vertical_lines_[j].y2() * scale_factor_ + scalePoint.y() * (1 - scale_factor_);
		 vertical_lines_[j].setP1(QPointF(x1, y1));
		 vertical_lines_[j].setP2(QPointF(x2, y2));
	 }

	 for (int j = 0; j <= rows_count_; j++)
	 {
		 auto x1 = horizontal_lines_[j].x1() * scale_factor_ + scalePoint.x() * (1 - scale_factor_);
		 auto y1 = horizontal_lines_[j].y1() * scale_factor_ + scalePoint.y() * (1 - scale_factor_);
		 auto x2 = horizontal_lines_[j].x2() * scale_factor_ + scalePoint.x() * (1 - scale_factor_);
		 auto y2 = horizontal_lines_[j].y2() * scale_factor_ + scalePoint.y() * (1 - scale_factor_);
		 horizontal_lines_[j].setP1(QPointF(x1, y1));
		 horizontal_lines_[j].setP2(QPointF(x2, y2));
	 }
	 grid_length_ *= scale_factor_;

	 update();
	 restoreDragRect();
 }
 void CustomWidget::mouseDoubleClickEvent(QMouseEvent* event)
 {
	 int v = 0;
	 if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)
	 {
		 v = 1;
	 }
	 else if (event->button() == Qt::RightButton && event->modifiers() == Qt::ControlModifier)
	 {
		 v = 2;
	 }
	 else if (event->button() == Qt::MiddleButton && event->modifiers() == Qt::ControlModifier)
	 {
		 v = 3;
	 }
	 auto xd = mapFromGlobal(QCursor::pos()).x() - vertical_lines_[0].x1();
	 auto yd = mapFromGlobal(QCursor::pos()).y() - vertical_lines_[0].y1();
	 int col = (int)(xd / grid_length_);
	 int row = (int)(yd / grid_length_);
	 if (row >= 0 && row < rows_count_ && col >= 0 && col < rows_count_)
	 {
		 state_[col][row] = v;
		 update();
	 }
	 updateStartGoalNum();
 }
 std::pair<QPoint, QPoint> CustomWidget::getRectStartEnds()
 {
	 auto start_col = (drag_rect_.x() - vertical_lines_[0].x1()) / grid_length_;
	 auto start_row = (drag_rect_.y() - vertical_lines_[0].y1()) / grid_length_;

	 auto end_col = (drag_rect_.x() + drag_rect_.width() - vertical_lines_[0].x1()) / grid_length_;
	 auto end_row = (drag_rect_.y() + drag_rect_.height() - vertical_lines_[0].y1()) / grid_length_;
	 std::pair<QPoint, QPoint> rt = std::make_pair(QPoint((int)start_row, (int)start_col), QPoint((int)end_row, (int)end_col));
	 return rt;
 }
 void CustomWidget::restoreDragRect()
 {
	 drag_rect_.setX(0);
	 drag_rect_.setY(0);
	 drag_rect_.setWidth(0);
	 drag_rect_.setHeight(0);

 }
 void CustomWidget::initStates()
 {
	 for (int i = 0; i < rows_count_; i++)
	 {
		 QVector<int>row;
		 for (int j = 0; j < rows_count_; j++)
		 {
			 row.append(GridState::None);
		 }
		 state_.append(row);
	 }
 }
 void CustomWidget::updateStartGoalNum()
 {


	 auto start_num = 0;
	 auto end_num = 0;
	 for (int i = 0; i < state_.size(); i++)
	 {
		 for (int j = 0; j < state_[i].size(); j++)
		 {
			 if (state_[i][j] == GridState::Start)
			 {
				 start_num++;
			 }
			 else if (state_[i][j] == GridState::Goal)
			 {
				 end_num++;
			 }
		 }
	 }
	 emit updateStartEndNum(start_num, end_num);


 }
 void CustomWidget::initLines()
 {
	 for (int i = 0; i <= rows_count_; i++)
	 {
		 vertical_lines_.append(QLineF(i * grid_length_, 0, i * grid_length_, grid_length_ * rows_count_));
		 horizontal_lines_.append(QLineF(0, i * grid_length_, grid_length_ * rows_count_, i * grid_length_));
	 }
 }
 QVector<QVector<int>>* CustomWidget::getStateClone()
 {

	 auto*clone = new QVector<QVector<int>>();
	 for (auto vec : state_)
	 {
		 QVector<int> v;
		 for (auto s :vec)
		 {
			 v.append(s);
		 }
		 clone->append(v);
	 }
	 return clone;
 }
 void CustomWidget::appendObstaclePoints(const QVector<QPoint>& obstacles)
 {
	 for (int i = 0; i < obstacles.size(); i++)
	 {
		 if (obstacles.at(i).x() >= 0 && obstacles.at(i).x() < rows_count_ && obstacles.at(i).y() >= 0 && obstacles.at(i).y() < rows_count_)
		 {
			 state_[obstacles.at(i).x()][obstacles.at(i).y()] = GridState::Obstacle;
		 }
	 }
 }
void CustomWidget::setAgentSize(int w, int h)
{
	agent_width_ = w;
	agent_height_ = h;
}
void CustomWidget::clearOtherPoints()
{
	for (int i=0;i<state_.size();i++)
	{
		for (int j=0;j<state_[i].size();j++)
		{
			if (state_[i][j]==4|| state_[i][j] == 5)
			{
				state_[i][j] == 1;
			}
		}
	}
	update();
}
