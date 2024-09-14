#pragma once
#include "Utils/Memory.h"

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class IEngineClient
{
public:
	int GetMaxClients()
	{
		return M::CallVFunc<int, 34U>(this);
	}

	bool IsInGame()
	{
		return M::CallVFunc<bool, 35U>(this);
	}

	bool IsConnected()
	{
		return M::CallVFunc<bool, 36U>(this);
	}

	int GetLocalPlayer()
	{
		int nIndex = -1;

		M::CallVFunc<void, 47U>(this, std::ref(nIndex), 0);

		return nIndex + 1;
	}

	const char* GetLevelName()
	{
		return M::CallVFunc<const char*, 56U>(this);
	}

	const char* GetLevelNameShort()
	{
		return M::CallVFunc<const char*, 57U>(this);
	}

	const char* GetProductVersionString()
	{
		return M::CallVFunc<const char*, 81U>(this);
	}
};
