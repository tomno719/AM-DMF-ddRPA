#include "map.h"
QJsonObject Map::toJson() const
{
    QJsonObject json;
    QJsonArray agentArray;
    for (const MapAgent& agent : agents_)
    {
        agentArray.append(agent.toJson());
    }
    json["agents"] = agentArray;
    QJsonArray obstacleArray;
    for (const QPoint& obstacle : obstacles_)
    {
        QJsonObject obstacleJson;
        obstacleJson["x"] = obstacle.x();
        obstacleJson["y"] = obstacle.y();
        obstacleArray.append(obstacleJson);
    }
    json["obstacles"] = obstacleArray;
    return json;

}
void Map::fromJson(const QJsonObject& json)
{
    agents_.clear();
    QJsonArray agentArray = json["agents"].toArray();
    for (const QJsonValue& agentValue : agentArray)
    {
        QJsonObject agentJson = agentValue.toObject();
        MapAgent agent;
        agent.fromJson(agentJson);
        agents_.append(agent);
    }

    obstacles_.clear();
    QJsonArray obstacleArray = json["obstacles"].toArray();
    for (const QJsonValue& obstacleValue : obstacleArray)
    {
        QJsonObject obstacleJson = obstacleValue.toObject();
        int x = obstacleJson["x"].toInt();
        int y = obstacleJson["y"].toInt();
        obstacles_.append(QPoint(x, y));
    }
}
