#pragma once

#include "XunFeiSpeech.h"
#include "XunFei.h"
#include "Async/AsyncWork.h"

class FSpeechTask : public FNonAbandonableTask
{

	friend class FAutoDeleteAsyncTask<FSpeechTask>;

	FSpeechTask()
	{
		UE_LOG(XunFeiLog, Log, TEXT("Speech Task be Create !"));
	}

	void DoWork();
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSpeechTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};