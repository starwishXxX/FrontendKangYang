#pragma once

#include "XunFeiSpeech.h"
#include "XunFei.h"

static char *g_result = NULL;
static unsigned int g_buffersize = BUFFER_SIZE;
static HANDLE events[EVT_TOTAL] = { NULL,NULL,NULL };	// 识别状态定义

static void on_speech_begin();
static void on_speech_end(int reason);
static void on_result(const char *result, char is_last);

void on_speech_begin()
{
	if (g_result)
	{
		free(g_result);
	}
	g_result = (char*)malloc(BUFFER_SIZE);
	g_buffersize = BUFFER_SIZE;
	memset(g_result, 0, g_buffersize);

	UE_LOG(XunFeiLog, Log, TEXT("Start Listening!!!"));
}

void on_speech_end(int reason)
{
	if (reason == END_REASON_VAD_DETECT)
	{
		UE_LOG(XunFeiLog, Log, TEXT("Speaking Done!!!"));
	}
	else
	{
		UE_LOG(XunFeiLog, Error, TEXT("Recognizer error: %d!!!"), reason);
	}
}

// static functions
void on_result(const char *result, char is_last)
{
	if (result) {
		size_t left = g_buffersize - 1 - strlen(g_result);
		size_t size = strlen(result);
		if (left < size) {
			g_result = (char*)realloc(g_result, g_buffersize + BUFFER_SIZE);
			if (g_result)
				g_buffersize += BUFFER_SIZE;
			else {
				printf("mem alloc failed\n");
				return;
			}
		}
		strncat(g_result, result, size);
	}
}

FXunFeiSpeech::FXunFeiSpeech()
{

}

FXunFeiSpeech::FXunFeiSpeech(FString Log)
{
	UE_LOG(XunFeiLog, Warning, TEXT("%s"), *Log);
}

void FXunFeiSpeech::SetStart()
{
	SetEvent(events[EVT_START]);

	return;
}

void FXunFeiSpeech::SetStop()
{
	SetEvent(events[EVT_STOP]);

	return;
}

void FXunFeiSpeech::SetQuit()
{
	SetEvent(events[EVT_QUIT]);

	return;
}

void FXunFeiSpeech::speech_mic(const char* session_beging_params)
{
	int errcode;
	int i = 0;

	struct speech_rec iat;
	DWORD waiters;
	char isquit = 0;

	struct speech_rec_notifier recnotifier = {
		on_result,
		on_speech_begin,
		on_speech_end
	};

	errcode = speechrecoginzer->sr_init(&iat, session_beging_params, SR_MIC, DEFAULT_INPUT_DEVID, &recnotifier);	// 初始化识别参数
	if (errcode)
	{
		UE_LOG(XunFeiLog, Error, TEXT("Speech Recognizer init failed!!!"));
		return;
	}
	// 创建识别事件状态
	for (i = 0; i < EVT_TOTAL; ++i) {
		events[i] = CreateEvent(NULL, false, false, NULL);
	}
	// 监听识别状态
	while (1) {
		waiters = WaitForMultipleObjects(EVT_TOTAL, events, false, INFINITE);
		switch (waiters) {
		case WAIT_FAILED:
		case WAIT_TIMEOUT:
			UE_LOG(XunFeiLog, Warning, TEXT("TimeOut or Failed!!!"));
			break;
		case WAIT_OBJECT_0 + EVT_START:
			errcode = speechrecoginzer->sr_start_listening(&iat);
			if (errcode) {
				UE_LOG(XunFeiLog, Error, TEXT("Start Listen failed: %d!!!"), errcode);
				isquit = 1;
			}
			break;
		case WAIT_OBJECT_0 + EVT_STOP:
			errcode = speechrecoginzer->sr_stop_listening(&iat);
			if (errcode) {
				UE_LOG(XunFeiLog, Error, TEXT("Stop Listening failed: %d!!!"), errcode);
				isquit = 1;
			}
			break;
		case WAIT_OBJECT_0 + EVT_QUIT:
			speechrecoginzer->sr_stop_listening(&iat);
			isquit = 1;
			break;
		default:
			break;
		}
		if (isquit)
			break;
	}

	for (i = 0; i < EVT_TOTAL; ++i) {
		if (events[i])
			CloseHandle(events[i]);
	}

	speechrecoginzer->sr_uninit(&iat);
}

const char* FXunFeiSpeech::get_result() const
{
	return g_result;
}
