#include "Frame.h"

Frame::Frame() : consoleHandler(nullptr), bufferHandler{ nullptr, nullptr }, currentBufferIndex(0), consoleInfo({ 0,0 }) {}