#pragma once

#ifdef USE_FIBONACCI_HEAP
#include <boost/heap/fibonacci_heap.hpp>
#endif

#include <boost/functional/hash.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <unordered_map>
#include <unordered_set>
#include<array>
#include<map>
// #include "neighbor.hpp"
// #include "planresult.hpp"
#include "a_star.hpp"

namespace libMultiRobotPlanning {

    /*!
      \example sipp.cpp Simple example using a 2D grid world and
      up/down/left/right
      actions
    */

    /*! \brief SIPP Algorithm to find the shortest path with dynamic obstacles

    This class implements the SIPP algorithm. SIPP is an informed search algorithm
    that finds the shortest path for a given map and dynamic a-priori known
    obstacles.
    It can use a heuristic that needs to be admissible.

    Details of the algorithm can be found in the following paper:\n
    Mike Phillips and Maxim Likhachev:\n
    "SIPP:  Safe  Interval  Path  Planning  for  Dynamic  Environments". IEEE
    International Conference on Robotics and Automation (ICRA), 2011\n
    https://doi.org/10.1109/ICRA.2011.5980306

    This class can either use a fibonacci heap, or a d-ary heap. The latter is the
    default. Define "USE_FIBONACCI_HEAP" to use the fibonacci heap instead.

    \tparam State Custom state for the search. Needs to be copy'able
    \tparam Location Custom location type for the search. Needs to be copy'able
    \tparam Cost Custom Cost type (integer or floating point types)
    \tparam Environment This class needs to provide the custom A* logic. In
        particular, it needs to support the following functions:
      - `Cost admissibleHeuristic(const State& s)`\n
        This function can return 0 if no suitable heuristic is available.

      - `bool isSolution(const State& s)`\n
        Return true if the given state is a goal state.

      - `void getNeighbors(const State& s, std::vector<Neighbor<State, Action,
       int> >& neighbors)`\n
        Fill the list of neighboring state for the given state s.

      - `void onExpandNode(const State& s, int fScore, int gScore)`\n
        This function is called on every expansion and can be used for statistical
    purposes.

      - `void onDiscover(const State& s, int fScore, int gScore)`\n
        This function is called on every node discovery and can be used for
       statistical purposes.
    */

    template <typename State, typename Location, typename Action, typename Cost,
        typename Environment>
    class SIPP {
    public:
        struct interval
    	{
            interval(Cost start, Cost end) : start(start), end(end)
        	{}

            Cost start;
            Cost end;

            friend bool operator<(const interval& a, const interval& b)
        	{
                return a.start < b.start;
            }
        };

        struct compare
    	{
        public:
            bool operator()(const interval& a, const interval& b)
        	{
                return a.start > b.start;
            }
        };


    public:
        SIPP(Environment& environment) : m_env(environment), m_astar(m_env)
        {
	        
        }
        
        /**
         * \brief 
         * \param location 地图上的位置
         * \param intervals 碰撞区间 vector 类型
         */
        void setCollisionIntervals(const Location& location,
                                   const std::vector<interval>& intervals)
    	{
            m_env.setCollisionIntervals(location, intervals);
        }

        bool mightHaveSolution(const State& goal)
    	{
            return m_env.mightHaveSolution(goal);
        }
        std::ofstream intervalsFile;
        
        void writeIntervalsFile()
        {
            static int intervalsId = 1;
	        intervalsFile.open("p/intervals"+std::to_string(intervalsId)+".txt",std::ios::out);
            		intervalsId++;
			for (auto it = m_env.m_safeIntervals.begin(); it != m_env.m_safeIntervals.end(); it++)
			{
				intervalsFile <<"["<< it->first.x << "," << it->first.y <<"]"<< std::endl;
				for (int i = 0; i < it->second.size(); i++)
				{
					intervalsFile << it->second[i].start << "-" << it->second[i].end << std::endl;
				}
			}
			intervalsFile.close();

        }
       
        bool search(const State& startState, const Action& waitAction,
            PlanResult<State, Action, Cost>& solution, Cost startTime = 0)
    	{
            PlanResult<SIPPState, SIPPAction, Cost> astarsolution;
            solution.cost = 0;
            solution.fmin = 0;
            solution.actions.clear();
            solution.states.clear();
            size_t interval;
            if (!m_env.findSafeInterval(startState, startTime, interval)) 
            {
                return false;
            }
            bool success = m_astar.search(SIPPState(startState, interval), astarsolution, startTime);
            solution.cost = astarsolution.cost - startTime;
            solution.fmin = astarsolution.fmin;
            for (size_t i = 0; i < astarsolution.actions.size(); ++i) 
            {
                Cost waitTime =
                    astarsolution.actions[i].second - astarsolution.actions[i].first.time;
                if (waitTime == 0) 
                {
                    solution.states.push_back(
                        std::make_pair<>(astarsolution.states[i].first.state,
                            astarsolution.states[i].second));
                    solution.actions.push_back(
                        std::make_pair<>(astarsolution.actions[i].first.action,
                            astarsolution.actions[i].second));
                }
                else {
                    // additional wait action before
                    solution.states.push_back(
                        std::make_pair<>(astarsolution.states[i].first.state,
                            astarsolution.states[i].second));
                    solution.actions.push_back(std::make_pair<>(waitAction, waitTime));
                    solution.states.push_back(
                        std::make_pair<>(astarsolution.states[i].first.state,
                            astarsolution.states[i].second + waitTime));
                    solution.actions.push_back(
                        std::make_pair<>(astarsolution.actions[i].first.action,
                            astarsolution.actions[i].first.time));
                }
            }
            solution.states.push_back(
                std::make_pair<>(astarsolution.states.back().first.state,
                    astarsolution.states.back().second));
            //writeIntervalsFile();
            return success;
        }
       
    private:
        // public:
        struct SIPPState
    	{
            SIPPState(const State& state, size_t interval)
                : state(state), interval(interval) {}

            bool operator==(const SIPPState& other) const
        	{
                return std::tie(state, interval) == std::tie(other.state, other.interval);
            }

            friend std::ostream& operator<<(std::ostream& os, const SIPPState& s)
        	{
                return os << "(" << s.state << "," << s.interval << ")";
            }

            State state;
            size_t interval;
        };

        struct SIPPStateHasher {
            size_t operator()(const SIPPState& s) const
        	{
                size_t seed = 0;
                boost::hash_combine(seed, std::hash<State>()(s.state));
                boost::hash_combine(seed, s.interval);
                return seed;
            }
        };

        struct SIPPAction
    	{
            SIPPAction(const Action& action, Cost time) : action(action), time(time) {}

            Action action;
            Cost time;
        };

        // private:
		public:  struct SIPPEnvironment
    	{
            int width = 1;
            int height = 1;
            int safe_distance = 1;
          
            SIPPEnvironment(Environment& env) : m_env(env)
        	{}

            Cost admissibleHeuristic(const SIPPState& s)
        	{
                return m_env.admissibleHeuristic(s.state);
            }

            bool mightHaveSolution(const State& goal)
        	{
                const auto& si = safeIntervals(m_env.getLocation(goal));
                return m_env.isSolution(goal) &&
                    !si.empty() &&
                    si.back().end == std::numeric_limits<Cost>::max();
            }

            bool isSolution(const SIPPState& s)
        	{
                return m_env.isSolution(s.state) &&
                    safeIntervals(m_env.getLocation(s.state)).at(s.interval).end ==
                    std::numeric_limits<Cost>::max();
            }

            void getNeighbors(
                const SIPPState& s,
                std::vector<Neighbor<SIPPState, SIPPAction, Cost> >& neighbors)
        	{
                std::vector<Neighbor<State, Action, Cost> > motions;
                m_env.getNeighbors(s.state, motions);
                
                for (const auto& m : motions) 
                {
                    // std::cout << "gN " << m.state << std::endl;
                    Cost m_time = m.cost;
                    // std::cout << m_lastGScore;
                    Cost start_t = m_lastGScore + m_time;
                    Cost end_t = 
                       safeIntervals(m_env.getLocation(s.state)).at(s.interval).end;
                    //s.interval 代表当前的state s正在目前所在的安全区间
                    //Cost end_t = findSafeInterval(s.state, start_t, s.interval);
                    //const auto& sis = safeIntervals(m_env.getLocation(m.state));
                    //for (size_t i = 0; i < sis.size(); ++i) 
                    //{
                    //    const interval& si = sis[i];
                    //    // std::cout << "  i " << i << ": " << si.start << "," << si.end <<
                    //    // std::endl;
                    //    if (si.start - m_time > end_t || si.end < start_t) {
                    //        continue;
                    //    }
                    //    int t;
                    //    if (m_env.isCommandValid(s.state, m.state, m.action, m_lastGScore,
                    //        end_t, si.start, si.end, t)) 
                    //    {
                    //        // std::cout << "  gN: " << m.state << "," << i << "," << t << ","
                    //        // << m_lastGScore << std::endl;
                    //        neighbors.emplace_back(Neighbor<SIPPState, SIPPAction, Cost>(
                    //            SIPPState(m.state, i), SIPPAction(m.action, m.cost),
                    //            t - m_lastGScore));
                    //    }
                    //}

                    

                    std::vector<State>states;
                    getJudgeStates(m.state,states);
                    int t;
                    size_t interval_i;
                    if(isTimeMoveable(m.state,states,m_time,start_t,end_t,t,interval_i))
                    {
                        neighbors.emplace_back(Neighbor<SIPPState, SIPPAction, Cost>(
                            SIPPState(m.state, interval_i), SIPPAction(m.action, m.cost),
                            t - m_lastGScore));
                    }
                }
            }

            void onExpandNode(const SIPPState& s, Cost fScore, Cost gScore)
        	{
                // const auto& interval =
                // safeIntervals(m_env.getLocation(s.state)).at(s.interval);
                // std::cout << "expand: " << s.state << "," << interval.start << " to "
                // << interval.end << "(g: " << gScore << " f: " << fScore << ")" <<
                // std::endl;
                // This is called before getNeighbors(). We use the callback to find the
                // current cost (=time) of the expanded node
                m_lastGScore = gScore;
                m_env.onExpandNode(s.state, fScore, gScore);
            }

            void onDiscover(const SIPPState& s, Cost fScore, Cost gScore)
        	{
                // const auto& interval =
                // safeIntervals(m_env.getLocation(s.state)).at(s.interval);
                // std::cout << "discover: " << s.state << "," << interval.start << " to "
                // << interval.end << std::endl;
                m_env.onDiscover(s.state, fScore, gScore);
            }
            std::vector<interval> mergeTwoInterval(std::vector<interval>& list1, std::vector<interval>& list2)
        	{
                // write your code here
                std::vector<interval> res;
                if (list1.size() == 0 && list2.size() == 0) {
                    return res;
                }
             std::priority_queue<interval, std::vector<interval>, compare> pq;
                for (int i = 0; i < list1.size(); i++) {
                    pq.push(list1[i]);
                }
                for (int i = 0; i < list2.size(); i++) {
                    pq.push(list2[i]);
                }
                interval last = pq.top();
                pq.pop();
                while (!pq.empty()) 
                {
                    interval cur = pq.top();
                    pq.pop();
                    if (last.end < cur.start) 
                    {
                        res.push_back(last);
                        last = cur;
                    }
                    else 
                    {
                        last.end = last.end > cur.end ? last.end : cur.end;
                        
                    }
                }
                res.push_back(last);
                return res;
            }
        
            /**
             * \brief 设置一个location的碰撞区间和安全区间
             * \param location 
             * \param intervals 
             */
            void setCollisionIntervals(const Location& location,
                const std::vector<interval>& intervals)
            {
                std::vector<interval> m = intervals;
                m_CollisionIntervals[location] = m;
                auto merge = m_CollisionIntervals.find(location)->second;
                m_safeIntervals.erase(location);

                m_safeIntervals[location]; // create empty safe interval
                int start = 0;
                int lastEnd = 0;
                for (const auto& interval : merge)
                {
                    // std::cout << "  ci: " << interval.start << " - " << interval.end <<
                    // std::endl;
                    assert(interval.start <= interval.end);
                    assert(start <= interval.start);
                    // if (start + 1 != interval.start - 1) {
                    // std::cout << start << "," << interval.start << std::endl;
                    // assert(start + 1 < interval.start - 1);
                    if (start <= interval.start - 1)
                    {
                        m_safeIntervals[location].push_back({ start, interval.start - 1 });
                    }
                    // }
                    start = interval.end + 1;
                    lastEnd = interval.end;
                }
                if (lastEnd < std::numeric_limits<int>::max())
                {
                    // assert(start < std::numeric_limits<int>::max());
                    m_safeIntervals[location].push_back(
                        { start, std::numeric_limits<int>::max() });
                }
            }

            /**
             * \brief state的安全区间中 是否包含 time 包含 保存在interval中
             * \param state 
             * \param time 
             * \param interval 
             * \return 
             */
            bool findSafeInterval(const State& state, Cost time, size_t& interval)
            {
                const auto& si = safeIntervals(m_env.getLocation(state));
                for (size_t idx = 0; idx < si.size(); ++idx) 
                {
                    if (si[idx].start <= time && si[idx].end >= time) {
                        interval = idx;
                        return true; 
                    }
                }
                return false;
            }
            


            
        private:
            /**
             * \brief 返回location的安全区间 默认返回(0，max)
             * \param location 查找安全区间的地图位置
             * \return 安全区间
             */
            const std::vector<interval>& safeIntervals(const Location& location)
        	{
                static std::vector<interval> defaultInterval(
                    1, { 0, std::numeric_limits<Cost>::max() });
                const auto iter = m_safeIntervals.find(location);
                if (iter == m_safeIntervals.end()) 
                {
                    return defaultInterval;
                }
                return iter->second;
            }
            
            void getJudgeStates(const State& s,std::vector<State>&states)
            {
                states.clear();
                int startX = s.x - (safe_distance+width-1);
                int startY = s.y - (safe_distance+height-1);
                for (int i=0;i<2*height-1+2*safe_distance;i++)
                {
	                for (int j=0;j<2*width-1+2*safe_distance;j++)
	                {
                        State t(startX + j, startY + i);
                        if(m_env.stateValid(t))
                        {
                            states.emplace_back(t);
                        }
                        
	                }
                }
               
            }
           
            std::vector<interval> intervalIntersection(std::vector<interval>& firstList, std::vector<interval>& secondList)
            {
                std::vector<interval> result;
                int i = 0, j = 0;
                while (i < firstList.size() && j < secondList.size()) {
                    int start = std::max(firstList[i].start, secondList[j].start);
                    int end = std::min(firstList[i].end, secondList[j].end);
                    if (start <= end) {
                        result.push_back({ start, end });
                    }
                    if (firstList[i].end < secondList[j].end) {
                        i++;
                    }
                    else {
                        j++;
                    }
                }
                return result;

            }
        	bool isTimeMoveable(const State& s,std::vector<State>&states,int m_time,int start_t,int end_t,int&ot,size_t& interval_i)
            {
				//max  si.start,lastG+1
                std::vector<std::vector<interval>> allIntervals;

	            for (int i=0;i<states.size();i++)
	            {
                    const auto& sis = safeIntervals(m_env.getLocation(states[i]));
                    for (int j=0;j<sis.size();j++)
                    {
                        const auto& si = sis[j];
                        if (si.start - m_time > end_t || si.end < start_t)
                        {
	                        continue;
                        }
                        else
                        {
                           std::vector<interval> destination;
                            for (int z = j;z<sis.size();z++)
                            {
	                            destination.emplace_back(sis[z]);
                            }
                            allIntervals.emplace_back(destination);
                        	break;
                        }
                    }
	            }
                
                if(allIntervals.size()!=states.size())
				{
					return false;
				}
                if (end_t == std::numeric_limits<int>::max())
                    end_t = end_t - 1;
                interval interval1(start_t, end_t + 1);
                allIntervals.insert(allIntervals.begin(), std::vector<interval>{interval1});
                auto intersect = allIntervals[0] ;
                for (int i=1;i<allIntervals.size();i++)
                {
	                intersect = intervalIntersection(intersect,allIntervals[i]);
	                if (intersect.size()<=0)
	                {
                        return false;
	                }
                }
                if (intersect.size() <= 0)
                {
	                return false;
                }
                else
                {
                    
                
                    /*for (int i = 0; i < intersect.size();i++)
                    {
	                    if (intersect[i].start<=start_t&& intersect[i].end>=start_t)
	                    {
		                    maxFirst = intersect[i].start;
                            isFound = true;
                            break;
	                    }
                    }*/
                   
                   /* std::vector<interval> intersect1;
                    intersect1.emplace_back(interval1);
					auto rst = intervalIntersection(intersect, intersect1);*/
                    /*if (rst.size()>0)
                    {*/
                        int maxFirst = intersect[0].start;
                        findSafeInterval(s, maxFirst, interval_i);
                        ot = std::max(maxFirst, start_t);
                        /*auto disStates = getSafeDistanceStates(s, safe_distance, width, height);
                        auto dic1 = m_stateTimeMap.find(ot);
                        if (dic1!=m_stateTimeMap.end())
                        {
                            for (size_t i = 0; i < disStates.size(); i++)
                            {
	                            if (dic1->second.find(disStates[i])!=dic1->second.end())
	                            {
                                    return false;
	                            }
                            }
                        }
                        else
                        {
                            m_stateTimeMap.insert(std::make_pair<>(ot, std::map<State, int>()));
                        }*/
                        return true;
						
                   /* }
                    else
                    {
                        return false;
                    }
                    */
                }
                
                
              
                //在此处判断所有的判断的点是否存在公共区间 并得到公共区间 查找到公共区间的index 写回即为要插入的i sippstate的成员 interval
                
                
            }
        private:
            Environment& m_env;
            Cost m_lastGScore;
        	
        	std::unordered_map<Location, std::vector<interval> > m_CollisionIntervals;
        public:
            std::unordered_map<Location, std::vector<interval> > m_safeIntervals;
        };

		SIPPEnvironment m_env;
        
        AStar<SIPPState, SIPPAction, Cost, SIPPEnvironment, SIPPStateHasher> m_astar;
    	
    };

    

}  // namespace libMultiRobotPlanning
