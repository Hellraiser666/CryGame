#include "StdAfx.h"
#include "BaseNode.h"
#include "Player.h"

class CAddXPNode : public CBaseNode
{
private:
	enum EInputs
	{
		EIP_Activate,
		EIP_Amount
	};

public:
	CAddXPNode(SActivationInfo *pActInfo)
	{

	}

	virtual void GetConfiguration(SFlowNodeConfig &config)
	{
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig_Void("Activate"),
			InputPortConfig<int>("Amount", 0),
			{0}
		};

		config.pInputPorts = inputs;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void OnActivate()
	{
		if(this->IsActive(EIP_Activate))
			CPlayer::GetHero()->AddXP(this->GetPortValue<int>(EIP_Amount));
	}
};

REGISTER_FLOW_NODE("RPG:AddXP", CAddXPNode);