#pragma once
#include "pch.h"
#include "string"
#include "Status.h"

extern bool isDragging;
extern POINT dragStart;

Status_fundamental StringToF(std::string& val);
Wearable_part StringToWP(std::string& val);
Need_Moment StringToNM(std::string& val);
