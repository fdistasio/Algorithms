#pragma once
#include "channel.hpp"

void source(Channel& out, int N);
void filterA(Channel& in, Channel& out);
void filterB(Channel& in, Channel& out);
void sink(Channel& in);
void sequential(int N);