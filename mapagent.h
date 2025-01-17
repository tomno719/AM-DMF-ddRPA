#pragma once
#include <QJsonObject>
class MapAgent
{
public:
	int start_x_;
	int start_y_;
	int end_x_;
	int end_y_;
	int size_;
	int distance_;
	QJsonObject toJson() const;

	void fromJson(const QJsonObject& json);
	MapAgent()
	{
		start_x_ = 0;
		start_y_ = 0;
		end_x_ = 0;
		end_y_ = 0;
		size_ = 0;
		distance_ = 0;
	}
	~MapAgent() = default;
};
