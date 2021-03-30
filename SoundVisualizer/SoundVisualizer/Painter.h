#pragma once
#include "ActiveObject.h"

class Painter : public ActiveObject {
	Painter(
		HWND hwnd,
		ViewWave& viewWave,
		ViewFreq& viewFreq,
		int samplesPerBuffer,
		int samplesPerSecond,
		int fftPoints
	);
	BOOL ReInit(
		int samplesPerBuffer,
		int samplesPerSecond,
		int fftPoints,
		int bitsPerSample
	);
	BOOL Start();
	void Stop();
	int HzToPoint(int hz) {
		Lock lock(_mutex);
		return _pFftTransformer->HzToPoint(hz); // call the Fast Furier Transformer to map the frequencies of the sound sample to points
	}
	int Points() {
		Lock lock(_mutex);
		return _pFftTransformer_Points(); // get the Points from the FastFurier Transformer
	}

private:
	void InitThread();
	void Run();
	void FlushThread();

	void LokWaveInData();

private:
	ViewWave& _viewWave;
	ViewFreq& _viewFreq;

	int _samplesPerBuffer;
	int _samplesPerSecond;
	int _fftPoints;
	int _bitsPerSample;

	HWND _hwnd;

	Mutex _mutex;
	Event _event;

	PtrRecorder _pRecorder;
	PtrFft _pFftTransformer;
};