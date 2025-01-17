#pragma once
#include <QPointF>
#include <QtCore/QList>
#include <QJsonObject>
#include<QJsonArray>
#include "mapagent.h"
#include "sippmapf.hpp"
#include "mapagent.h"
class Map
{
public:
	QList<MapAgent>agents_;
	QList<QPoint>obstacles_;
    QJsonObject toJson() const;
	void fromJson(const QJsonObject& json);
	Map() = default;
	~Map() = default;
	Map(std::unordered_set<State> obs,
		std::vector<State> start_states,
		std::vector<State> goals)
	{
		for (const auto b:obs)
		{
			obstacles_.append(QPoint(b.x,b.y));
		}
		for (int i=0;i<start_states.size();i++)
		{
			MapAgent ma;
			ma.start_x_ = start_states[i].x;
			ma.start_y_ = start_states[i].y;
			ma.end_x_ = goals[i].x;
			ma.end_y_ = goals[i].y;
			agents_.append(ma);
		}
	}
};
