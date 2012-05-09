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
	CAddXPNode(SActivationInfo *pActInfo) { }

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

class CXPListenerNode : public CFlowBaseNode<eNCT_Instanced>, public IPlayerEventListener
{
private:
	SActivationInfo *m_pActInfo;

	enum EInputs
	{
		EIP_Enable,
		EIP_Disable
	};

	enum EOutputs
	{
		EOP_OnXPChanged,
		EOP_OnLevelUp
	};

public:
	CXPListenerNode(SActivationInfo *pActInfo) { }

	// IPlayerEventListener
	virtual void OnXPChange(IActor *pActor, int xp)
	{
		this->ActivateOutput(m_pActInfo, EOP_OnXPChanged, xp);
	}

	virtual void OnLevelChange(IActor *pActor, int level)
	{
		this->ActivateOutput(m_pActInfo, EOP_OnLevelUp, level);
	}
	// ~IPlayerEventListener

	IFlowNodePtr Clone(SActivationInfo *pActInfo)
	{
		return new CXPListenerNode(pActInfo);
	}

	virtual void GetMemoryUsage(ICrySizer *s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig &config)
	{
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig_Void("Enable"),
			InputPortConfig_Void("Disable"),
			{0}
		};

		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig<int>("OnXPGained"),
			OutputPortConfig<int>("OnLevelUp"),
			{0}
		};

		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
	{
		switch(event)
		{
			case eFE_Initialize:
				{
					m_pActInfo = pActInfo;
					break;
				}

			case eFE_Activate:
				{
					if(this->IsPortActive(pActInfo, EIP_Enable))
						CPlayer::GetHero()->RegisterPlayerEventListener(this);
					if(this->IsPortActive(pActInfo, EIP_Disable))
						CPlayer::GetHero()->UnregisterPlayerEventListener(this);
				}
		}
	}
};

REGISTER_FLOW_NODE("RPG:AddXP", CAddXPNode);
REGISTER_FLOW_NODE("RPG:XPListener", CXPListenerNode);