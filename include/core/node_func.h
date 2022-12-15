#pragma once

#include "core/manager.h"
#include <string>
#include <vector>

namespace asyncflow
{
	namespace core
	{
		class Agent;		

		class NodeFunc
		{
		public:			
			virtual bool call(Agent* agent) = 0;
			virtual ~NodeFunc() = default;
		};

		typedef bool(Manager::*ControlFunc)(const std::vector<int>&);

		class ControlNodeFunc : public NodeFunc
		{
		public:
			bool call(core::Agent* agent) override;
			static NodeFunc* Create(ControlFunc, const std::vector<int>&);

		private:
			std::vector<int> params_;
			ControlFunc func;
		};

		class RepeatNodeFunc : public NodeFunc
		{
		public:
			bool call(core::Agent* agent) override;
			static NodeFunc* Create(const std::vector<int>&);
			static void OnNodeRunFlowEnd(int id, Node* node);
			void Clear(Chart* chart);
		private:
			std::vector<int> waiting_nodes_;
			bool Finished(Chart* chart);
		};

#ifdef FLOWCHART_DEBUG
		class BreakpointFunc : public NodeFunc
		{
		public:
			BreakpointFunc(NodeFunc* func):continue_flag_(false), original_nodefunc_(func) {}
			~BreakpointFunc() 
			{
				if (original_nodefunc_ != nullptr)
					delete original_nodefunc_;
			}
            
			bool call(core::Agent* agent) override;
			void SetExecute(bool flag) { continue_flag_ = flag; }
			NodeFunc* GetOriginalFunc() { return original_nodefunc_; }
            void SetOriginalNull() { original_nodefunc_ = nullptr; }

		private:
			NodeFunc* original_nodefunc_;
			bool continue_flag_;
		};
#endif
	}
}