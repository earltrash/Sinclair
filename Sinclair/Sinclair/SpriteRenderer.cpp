#include "pch.h"
#include "SpriteRenderer.h"



SpriteRenderer::~SpriteRenderer()
{
    m_clips.clear();
}

void SpriteRenderer::SetCurrentClip(const AnimationClip* clip)
{
    m_clip = clip;
}

//Object 에셋 넣을 때 이거로 넣으면 될듯 
void SpriteRenderer::AddClip(const std::string& aniName, const AnimationClip& clip)
{
    m_clips[aniName] = clip;
}

void SpriteRenderer::Update(float deltaTime)
{
    if (!m_clip) return;
    m_elapsed += deltaTime;

    float total = m_clip->GetTotalDuration(); // 다형성...
    if (m_loop)
    {
        if (m_elapsed >= total)
            m_elapsed = std::fmod(m_elapsed, total);
    }

    else
    {
        if (m_elapsed > total)
            m_elapsed = total;
    }
}

const Frame& SpriteRenderer::GetCurrentFrame() const
{
    float accum = 0.f;
    for (auto& frame : m_clip->GetFrames())
    {
        accum += frame.duration / 1000.0f;
        if (m_elapsed < accum)
        {
            return frame;
        }
    }
    return m_clip->GetFrames().back();
}

const AnimationClip& SpriteRenderer::GetCurrentClip()
{
    return *m_clip;
}

AnimationClip* SpriteRenderer::GetClip(const std::string& aniName)
{
    auto it = m_clips.find(aniName);
    return (it != m_clips.end()) ? &it->second : nullptr;
}
