#pragma once

#include "Painter.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Shaders.h"

Painter::Painter(HWND hwnd, ViewWave& viewWave, ViewFreq& viewFreq, int samplesPerBuffer, int samplesPerSecond, int fftPoints) {
	this->_hwnd = hwnd;
	this->_viewFreq = viewFreq;
	this->_viewWave = viewWave;
	this->_samplesPerBuffer = samplesPerBuffer;
	this->_samplesPerSecond = samplesPerSecond;
	this->_fftPoints = fftPoints;
	this->_bitsPerSample = 16;
	this->_pFftTransformer(fftPoints, samplesPerSecond);
	this->_pRecorder(samplesPerBuffer, samplesPerSecond);
}

void Painter::Run() {
	for (;;) {
		_event.Wait();
		if (_isDying) {
			return;
		}
		
		Lock lock(_mutex);
		if (_pRecorder->IsBufferDone()) {
			LokWaveInData();
		}
	}
}

void Painter::FlushThread() {
	_event.Release();
}

void Painter::LokWaveInData() {
	SimpleIter iter(_pRecorder.GetAccess());
	if (!_pRecorder->BufferDone()) {
		return;
	}
	_pFftTransformer->CopyIn(iter);
	_pFftTransformer->Transform();
	_viewFreq.Update(_pFftTransformer.GetAccess());
	_viewWave.Update(_pFftTransformer.GetAccess());
}

void ViewWave::Update(Fft const& fftTransformer) {
	// TODO define the ddrawing function for the frequencies
	// this function will be called each time the recorder sends in data
	// create windows in main, make context, then call the draw function here
}