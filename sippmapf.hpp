#pragma once
#include <iostream>
#include <map>
#include <unordered_set>
#include <iostream>
#include <fstream> 
#include <chrono>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include <QMessageBox>

#include "sipp.hpp"
using libMultiRobotPlanning::SIPP;
using libMultiRobotPlanning::Neighbor;
using libMultiRobotPlanning::PlanResult;

struct State {
    State(int x, int y) : x(x), y(y) {}

    bool operator==(const State& other) const {
        return std::tie(x, y) == std::tie(other.x, other.y);
    }

    bool operator!=(const State& other) const {
        return std::tie(x, y) != std::tie(other.x, other.y);
    }

    bool operator<(const State& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }

    friend std::ostream& operator<<(std::ostream& os, const State& s) {
        return os << "(" << s.x << "," << s.y << ")";
    }

    int x;
    int y;
};

namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State& s) const {
            size_t seed = 0;
            boost::hash_combine(seed, s.x);
            boost::hash_combine(seed, s.y);
            return seed;
        }
    };
}  // namespace std

enum class Action {
    Up,
    Down,
    Left,
    Right,
    Wait,
};
class Environment {
public:
    Environment(size_t dimx, size_t dimy, std::unordered_set<State> obstacles,
        State goal)
        : m_dimx(dimx),
        m_dimy(dimy),
        m_obstacles(std::move(obstacles)),
        m_goal(goal) {}

    float admissibleHeuristic(const State& s) {
        return std::abs(s.x - m_goal.x) + std::abs(s.y - m_goal.y);
    }

    bool isSolution(const State& s) { return s == m_goal; }

    State getLocation(const State& s) { return s; }

    void getNeighbors(const State& s,
        std::vector<Neighbor<State, Action, int> >& neighbors) {
        neighbors.clear();

        State up(s.x, s.y + 1);
        if (stateValid(up)) {
            neighbors.emplace_back(Neighbor<State, Action, int>(up, Action::Up, 1));
        }
        State down(s.x, s.y - 1);
        if (stateValid(down)) {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(down, Action::Down, 1));
        }
        State left(s.x - 1, s.y);
        if (stateValid(left)) {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(left, Action::Left, 1));
        }
        State right(s.x + 1, s.y);
        if (stateValid(right)) {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(right, Action::Right, 1));
        }
    }

    void onExpandNode(const State& /*s*/, int /*fScore*/, int /*gScore*/) {
        // std::cout << "expand: " << s << "g: " << gScore << std::endl;
    }

    void onDiscover(const State& /*s*/s, int /*fScore*/f, int /*gScore*/g) {
        // std::cout << "  discover: " << s << std::endl;
        
    }
    // s.state, m.state, m.action, m_lastGScore,
    //                                    end_t, si.start, si.end, t
    bool isCommandValid(
        const State& /*s1*/, const State& /*s2*/, const Action& /*a*/,
        int earliestStartTime,      // can start motion at this time
        int /*latestStartTime*/,    // must have left s by this time
        int earliestArrivalTime,    // can only arrive at (s+cmd)
        int /*latestArrivalTime*/,  // needs to arrive by this time at (s+cmd)
        int& t) {
        t = std::max<int>(earliestArrivalTime, earliestStartTime + 1);

        // TODO(whoenig): need to check for swaps here...

        // return t - 1 <= latestStartTime;
        return true;
    }
    bool stateValid(const State& s) {
        return s.x >= 0 && s.x < m_dimx && s.y >= 0 && s.y < m_dimy &&
            m_obstacles.find(s) == m_obstacles.end();
    }
    int m_dimx;
    int m_dimy;
    std::unordered_set<State> m_obstacles;
    State m_goal;

};
class EnvironmentWithSizeDistance
{
    
    using sipp_tt = SIPP<State, State, Action, int, EnvironmentWithSizeDistance>;
public:
    int width_;
    int height_;
    int safe_distance_;
    std::map<State, std::vector<sipp_tt::interval>> GlobalCollision;
    std::ofstream outputFile;
    std::ofstream obsFile;
    EnvironmentWithSizeDistance(size_t dimx, size_t dimy, std::unordered_set<State> obstacles,
        State goal)
        : dimx_(dimx),
        dimy_(dimy),
        obstacles_(std::move(obstacles)),
        goal_(goal)
    {
    	outputFile.open("log.txt");
    }
    void setAgentHeight(int height)
    {
        height_ = height;
    }
    void setAgentWidth(int width)
    {
        width_ = width;
    }
    void setSafeDistance(int safe_distance)
    {
        safe_distance_ = safe_distance;
    }
    float admissibleHeuristic(const State& s)
    {
        return std::abs(s.x - goal_.x) + std::abs(s.y - goal_.y);
    }

    bool isSolution(const State& s) { return s == goal_; }

    State getLocation(const State& s) { return s; }

    void getNeighbors(const State& s,
        std::vector<Neighbor<State, Action, int> >& neighbors)
    {
        neighbors.clear();

        State up(s.x, s.y + 1);
        if (stateValid(up) && isMoveable(up)) {
            neighbors.emplace_back(Neighbor<State, Action, int>(up, Action::Up, 1));
        }
        State down(s.x, s.y - 1);
        if (stateValid(down) && isMoveable(down))
        {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(down, Action::Down, 1));
        }
        State left(s.x - 1, s.y);
        if (stateValid(left) && isMoveable(left))
        {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(left, Action::Left, 1));
        }
        State right(s.x + 1, s.y);
        if (stateValid(right) && isMoveable(right))
        {
            neighbors.emplace_back(
                Neighbor<State, Action, int>(right, Action::Right, 1));
        }
    }

    void onExpandNode(const State& /*s*/, int /*fScore*/, int /*gScore*/)
	{
        // std::cout << "expand: " << s << "g: " << gScore << std::endl;
    }

    void onDiscover(const State& /*s*/s, int /*fScore*/f, int /*gScore*/g)
	{
        // std::cout << "  discover: " << s << std::endl;

        if (outputFile.is_open())
        {
	        outputFile << "  discover: " << s << std::endl;
        }
    }

    bool isCommandValid(
        const State& /*s1*/, const State& /*s2*/, const Action& /*a*/,
        int earliestStartTime,      // can start motion at this time
        int /*latestStartTime*/,    // must have left s by this time
        int earliestArrivalTime,    // can only arrive at (s+cmd)
        int /*latestArrivalTime*/,  // needs to arrive by this time at (s+cmd)
        int& t) {
        t = std::max<int>(earliestArrivalTime, earliestStartTime + 1);

        // TODO(whoenig): need to check for swaps here...

        // return t - 1 <= latestStartTime;
        return true;
    }
    bool isMoveable(const State& s)
    {
        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                if (!stateValid(State(s.x + i, s.y + j)))
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool stateValid(const State& s) {
        return s.x >= 0 && s.x < dimx_ && s.y >= 0 && s.y < dimy_ &&
            obstacles_.find(s) == obstacles_.end();
    }
    void writeObsFile(std::unordered_set<State>&ob)
    {
        static int obsId = 1;
        obsFile.open("p/obs"+std::to_string(obsId)+".txt");
        obsId++;
        for (auto v : ob)
        {
            obsFile << v.x << "," << v.y << std::endl;
        }
        obsFile.close();
	}

private:
    int dimx_;
    int dimy_;
    std::unordered_set<State> obstacles_;
    State goal_;
    
};
class SippMapf
{
    typedef SIPP<State, State, Action, int, EnvironmentWithSizeDistance> sipp_t;
public:
    SippMapf(int dimx, int dimy, std::unordered_set<State> obstacles, std::vector<State> startStates, std::vector<State> goals):DimX(dimx),DimY(dimy),Obstacles(obstacles),StartSoates(startStates),Goals(goals)
    {
	    
    }
    int DimX;
    int DimY;
    std::unordered_set<State> Obstacles;
    std::vector<State> Goals;
    std::vector<State> StartSoates;
    std::map<State, std::vector<sipp_t::interval>> GlobalCollision;
    std::vector<std::pair<int, int>>waitTimes;
    static bool cmp(sipp_t::interval a, sipp_t::interval b)
    {
        return a.start < b.start;
    }
    std::vector<sipp_t::interval> merge(std::vector<sipp_t::interval>& intervals)
	{
        if (intervals.size() <= 1) return intervals;//集合为空或者为一个区间
        std::vector<sipp_t::interval> res;
        std::sort(intervals.begin(), intervals.end(), cmp);//排序，规定使用这种按start升序排列
        int i = 0;
        sipp_t::interval temp = { intervals[0].start, intervals[0].end };//赋初值，temp为组合新的区间
        while (i < intervals.size())
        {
            temp.start = intervals[i].start;
            temp.end = intervals[i].end;
            while (i < intervals.size())
            {
                if (i < intervals.size() - 1 && temp.end >= (intervals[i + 1].start - 1))//防止数组访问越界i < intervals.size()-1；前后区间有交集
                {
                    if (temp.end < intervals[i + 1].end)//后面的区间没有包含进来
                    {
                        temp.end = intervals[i + 1].end;//将后面区间合并成更大的区间
                    }

                    i++;
                }
                else//前后区间没有交集
                {
                    i++;
                    break;
                }

            }
            res.push_back(temp);
        }

        return res;

    }
    void writeCollisionFile(std::map<State, std::vector<sipp_t::interval>> allCollisionIntervals)
    {
        QFile file("example.txt");

        
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
			for (const auto& collisionIntervals : allCollisionIntervals) {
                //sipp.setCollisionIntervals(collisionIntervals.first, collisionIntervals.second);
                stream << "(" << (collisionIntervals.first.x) << "," << (collisionIntervals.first.y) << "):";
                for (int j = 0; j < collisionIntervals.second.size(); j++)
                {
                    stream << "[" << collisionIntervals.second[j].start << ":" << collisionIntervals.second[j].end << "]->";
                }
                stream << endl;
            }
			file.close();
            qDebug() << "write file done";
        }
        else {
            qDebug() << "can not open file";
        }
    }
   
    void writeSolutionFile(PlanResult<State, Action, int>&solution)
	{
        static int solutionId = 1;
		std::fstream file("p/solution"+std::to_string(solutionId)+".txt",std::ios::app);
        solutionId++;
        if (file.is_open())
        {
            file << solution.states[0].first << "-" << solution.states[0].second << "," << "0" << std::endl;
            for (size_t j = 1; j < solution.states.size(); ++j)
            {
                file << solution.states[j].first << "-" << solution.states[j].second <<","<<solution.actions[j-1].second<< std::endl;
            }
            file.close();
        }
	}
    void writePorcessToFile(int i)
    {
        std::fstream file("p/solution" + std::to_string(i) + ".txt", std::ios::app);
        if (file.is_open())
        {
            file<<std::to_string(i)<<std::endl;
            file.close();
        }
    }
    bool Planning(std::vector<PlanResult<State, Action, int>>&solutions, double& plantime,int agent_width,int agent_height,int safe_distance)
    {
        bool planrst = true;
        auto start = std::chrono::high_resolution_clock::now();
      
        long group_max_cost = 0;
        long global_max_cost = 0;
        std::map<sipp_t::interval, int>mapIntervals;
        std::map<State, std::vector<sipp_t::interval>> allCollisionIntervals;
        bool firstPlanFailed = false;
        for (int i = 0; i < Goals.size(); ++i)
        {
			currentAgentId = i;
            
            std::unordered_set<State>obs;
            std::copy(Obstacles.begin(), Obstacles.end(), std::inserter(obs, obs.end()));
            int w = agent_width;
            int h = agent_height;
            int safed = safe_distance;
            insertObstacleAfter(i, StartSoates, obs, w, h, safed);
            insertObstacleBefore(i, Goals, obs, w, h, safed);
            EnvironmentWithSizeDistance env(DimX, DimY, obs, Goals[i]);
            
            env.setAgentHeight(agent_width);
            env.setAgentWidth(agent_height);
            env.setSafeDistance(safe_distance);
            sipp_t sipp(env);
            //env.writeObsFile(obs);
            sipp.m_env.safe_distance = safe_distance;
            sipp.m_env.width = agent_width;
            sipp.m_env.height = agent_height;
            for (auto& collisionIntervals : GlobalCollision) 
            {
                sipp.setCollisionIntervals(collisionIntervals.first,collisionIntervals.second);
            }
            PlanResult<State, Action, int> solution;
            bool success = sipp.search(StartSoates[i], Action::Wait, solution,global_max_cost);
            if (success) 
            {
                std::cout << "Planning successful! Total cost: " << solution.cost
                    << std::endl;

               
                solutions.emplace_back(solution);

                auto lastState = solution.states[0];
                for (size_t z = 1; z < solution.states.size(); ++z)
                {
                    if (solution.states[z].first != lastState.first)
                    {

                        int col = lastState.first.x-safed-env.width_+1;
                        int row = lastState.first.y-safed-env.height_+1;
                        for (int j = 0; j < env.width_ + (safed+env.width_-1)*2; ++j)
                        {
                            for (int k = 0; k < env.height_ + (safed+env.height_-1) * 2; ++k)
                            {
                                int tx = col + j;
                                int ty = row + k;
                                State st(tx, ty);
                                if (tx < DimX && tx >= 0 && ty >= 0 && ty < DimY && Obstacles.find(st) == Obstacles.end())
                                {
                                    AddToGlobalCollision(st, sipp_t::interval(lastState.second, solution.states[z].second - 1));
                                }
                            }
                        }
                        lastState = solution.states[z];
                    }
                }
                //cost += solution.cost;
                group_max_cost= solution.cost> group_max_cost ?solution.cost: group_max_cost;
                //std::cout << solution.states.size() << "########" << std::endl;
                //writeSolutionFile(solution);
                writePorcessToFile(currentAgentId);
                firstPlanFailed = false;
            }
            else
            {
                /*std::cout << "Planning NOT successful!" << std::endl;
                std::ofstream errFile("p/error.txt",std::ios::app);
                if (errFile.is_open())
                {
                    errFile<<std::to_string( i)<<std::endl;
                    errFile.close();
                }
                planrst = false;
               
                return planrst;*/
                if (firstPlanFailed)
                {
                    planrst = false;
                    return planrst;
                }
                firstPlanFailed = true;
                std::ofstream logFile("p/groupfile.txt",std::ios::app);
                if (logFile.is_open()) {
                    logFile << "group "<<std::to_string(currentAgentId) << std::endl;
                    logFile.close();
                }
                i--;
                global_max_cost += (group_max_cost+1);
                
                waitTimes.emplace_back(std::make_pair(i+1, group_max_cost+1));
                group_max_cost = 0;
                GlobalCollision.clear();
            }
            //
            
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        plantime = elapsed.count();
        if (waitTimes.size() > 0)
        {
            waitTimes.emplace_back(std::make_pair(solutions.size()-1, 0));
            int wait_time = 0;
            for (int i = 0; i < waitTimes.size() - 1; i++)
            {
                wait_time += waitTimes[i].second;
                int solutiuon_idx_start = waitTimes[i].first;//agent start index
                int solutiuon_idx_end = waitTimes[i + 1].first;//agent end index
                for (int j=solutiuon_idx_start;j<solutiuon_idx_end;j++)
                {
	                State wait_state = solutions[j].states[0].first;
					for (int k = 0; k < wait_time; k++)
					{
						solutions[j].states.insert(solutions[j].states.begin(),std::make_pair(wait_state,1));
                        solutions[j].actions.insert(solutions[j].actions.begin(), std::make_pair(Action::Wait,1));
					}
                }


            }
            State wait_state = solutions[solutions.size() - 1].states[0].first;
            for (int k=0;k<wait_time;k++)
            {
                solutions[solutions.size() - 1].states.insert(solutions[solutions.size() - 1].states.begin(), std::make_pair(wait_state, 1));
                solutions[solutions.size() - 1].actions.insert(solutions[solutions.size() - 1].actions.begin(), std::make_pair(Action::Wait, 1));
            }
        }
        return planrst;
    }
    void AddToGlobalCollision(const State& s,sipp_t::interval t)
    {
        auto it = GlobalCollision.find(s);
	    if (it == GlobalCollision.end())
	    {
            std::vector<sipp_t::interval>ts;
            ts.emplace_back(t);
            GlobalCollision[s] = ts;
	    }
        else
        {
            it->second.emplace_back(t);
            it->second = merge(it->second);
        }
    }
    int getCurrentAgentId()
	{
		return currentAgentId;
	}
    void insertObstacleAfter(int current_Idx,std::vector<State>&start_states,std::unordered_set<State>&obs,int agent_width,int agent_height,int safe_distance)
	{
        for (int j = current_Idx + 1; j < start_states.size(); j++)
        {
            int col = start_states[j].x - safe_distance ;
            int row = start_states[j].y - safe_distance ;
            for (int k = 0; k < agent_width + (safe_distance) * 2; k++)
            {
                for (int z = 0; z < agent_height + (safe_distance ) * 2; z++)
                {
                    int tx = col + k;
                    int ty = row + z;
                    State st(tx, ty);
                    if (tx < DimX && tx >= 0 && ty >= 0 && ty < DimY && Obstacles.find(st) == Obstacles.end())
                    {
                        obs.insert(st);
                    }
                }
            }
        }
	}
    void insertObstacleBefore(int current_Idx, std::vector<State>& goal_states, std::unordered_set<State>&obs, int agent_width, int agent_height, int safe_distance)
	{
        for (int j = 0; j < current_Idx; j++)
        {
            int col = goal_states[j].x - safe_distance ;
            int row = goal_states[j].y - safe_distance ;
            for (int k = 0; k < agent_width + (safe_distance ) * 2; k++)
            {
                for (int z = 0; z < agent_height + (safe_distance ) * 2; z++)
                {
                    int tx = col + k;
                    int ty = row + z;
                    State st(tx, ty);
                    if (tx < DimX && tx >= 0 && ty >= 0 && ty < DimY && Obstacles.find(st) == Obstacles.end())
                    {
                        obs.insert(st);
                    }
                }
            }
        }
	}
private:
    void AppendLog(std::string log)
    {
        std::ofstream file("p/log.txt", std::ios::app);
        if(file.is_open())
        {
            file << log<<std::endl;
            file.close();
        }

    }
    int currentAgentId = 0;
};
