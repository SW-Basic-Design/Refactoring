#pragma once

#include "includes.h"

#ifndef FRAME_HPP
#define FRAME_HPP

struct stRect {
	int nWidth;
	int nHeight;
};

class Frame {
public:
	Frame();

	HANDLE consoleHandler;
	stRect consoleInfo;
	HANDLE bufferHandler[2];
	int currentBufferIndex;
};
#endif
