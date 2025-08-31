#pragma once
#pragma once
#include <iostream>
#include <filesystem>
#include "AnimationClip.h" 

struct Clip_Asset;
using Filepath = std::filesystem::path;
using AnimationClips = std::vector<std::pair<std::string, AnimationClip>>;

class AsepriteParser
{
public:
    AsepriteParser() = default;
    ~AsepriteParser() = default;
public:
    Clip_Asset Load(Filepath& jsonPath);
};

