#include"core/node_func.h"

using namespace asyncflow::core;

bool ControlNodeFunc::call(Agent* agent)
{
	auto* mgr = agent->GetManager();
	return (mgr->*func)(params_);
}

NodeFunc* ControlNodeFunc::Create(ControlFunc func, const std::vector<int>& params)
{
	auto* f = new ControlNodeFunc;
	f->func = func;
	f->params_ = params;
	return f;
}

bool RepeatNodeFunc::call(core::Agent* agent)
{
	auto* mgr = agent->GetManager();
	auto* self = mgr->GetCurrentNode();
	auto* repeater = self->GetAttacher();
	if(repeater == nullptr)
	{
		repeater = new NodeRepeater(self);
		self->SetAttacher(repeater);	
	}
	
	int self_id = self->GetId();
	auto* chart = self->GetChart();

	for (int id : waiting_nodes_)
	{
		auto* node = chart->GetNode(id);
		node->SetRunFlowEnd([self_id](Node* node) { OnNodeRunFlowEnd(self_id, node); });
	}
	return true;
}

void RepeatNodeFunc::OnNodeRunFlowEnd(int id, Node* node)
{
	auto* chart = node->GetChart();
	auto* self = chart->GetNode(id);
	auto* repeat = static_cast<RepeatNodeFunc*>(self->GetData()->GetNodeFunc());
	if (repeat->Finished(chart))
	{
		auto* agent = chart->GetAgent();
		agent->WaitEvent(node, AsyncEventBase::START_EVENT);
		agent->GetManager()->Event(AsyncEventBase::START_EVENT, agent, nullptr, 0);
	}
}

bool RepeatNodeFunc::Finished(Chart* chart)
{
    for (const int id : waiting_nodes_)
    {
		auto* node = chart->GetNode(id);
		if(node->GetStatus() == Node::Status::Running)
		{
			return false;
		}
    }
	return true;
}

void RepeatNodeFunc::Clear(Chart* chart)
{
	for (const int id : waiting_nodes_)
	{
		auto* node = chart->GetNode(id);
		//TODO could repeaterNodeFunc dtor called after node?
		if(node != nullptr)
		    node->SetRunFlowEnd(nullptr);
	}	
}



#ifdef FLOWCHART_DEBUG
bool BreakpointFunc::call(Agent* agent)
{
	//continue to run the original function in the node
	if (continue_flag_)
	{
		continue_flag_ = false;
		return original_nodefunc_->call(agent);
	}
	auto* mgr = agent->GetManager();
	auto* node = mgr->GetCurrentNode();
	// if the chart is not in debug state, breakpoint can not work
	if (!node->GetChart()->IsDebug())
		return original_nodefunc_->call(agent);
	else
	{
		// add the node to the async manager
		node->SetStatus(Node::Running);
		mgr->GetAsyncManager().AddNode(node);
	}
	return true;
}
#endif