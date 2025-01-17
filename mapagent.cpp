#include "mapagent.h"
QJsonObject MapAgent::toJson() const
{
    QJsonObject json;
    json["startX"] = start_x_;
    json["startY"] = start_y_;
    json["endX"] = end_x_;
    json["endY"] = end_y_;
    json["size"] = size_;
    json["distance"] = distance_;
    return json;
}
void MapAgent::fromJson(const QJsonObject& json)
{
    start_x_ = json["startX"].toInt();
    start_y_ = json["startY"].toInt();
    end_x_ = json["endX"].toInt();
    end_y_ = json["endY"].toInt();
    size_ = json["size"].toInt();
    distance_ = json["distance"].toInt();

}
