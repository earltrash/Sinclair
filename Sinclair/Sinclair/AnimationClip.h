#pragma once
#include "pch.h"
struct Frame
{
    D2D1_RECT_U srcRect;   // 시트에서 잘라낼 영역

    float        duration; // 이 프레임이 유지되는 시간(초)

    float Width() const { return srcRect.right - srcRect.left; }
    float Height() const { return srcRect.bottom - srcRect.top; }

    D2D1_RECT_F ToRectF() const
    {
        return D2D1::RectF(
            static_cast<float>(srcRect.left),
            static_cast<float>(srcRect.top),
            static_cast<float>(srcRect.right),
            static_cast<float>(srcRect.bottom));
    }
};

struct Tag
{
    std::string m_name;
    int m_from;
    int m_to;
    std::string m_direction;
};

class AnimationClip
{
public:
    AnimationClip() = default;
    ~AnimationClip() {
        m_frames.clear();
        m_tags.clear();
        // m_sheet 얘는 comptr이라 괜찮을 듯 
    };

    // 텍스처 시트 연결 (AssetManager에서 한 번만 호출) //이거로 BITMAP 넘겨주는 듯 
    void SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> sheet)
    {
        m_sheet = std::move(sheet);
    }

    // AsepriteParser 등에서 프레임을 한 번만 채워 넣음
    void AddFrame(const Frame& frame)
    {
        m_frames.push_back(frame);
        m_totalDuration += frame.duration / 1000.0f;
    }

    void AddTag(const Tag& tag)
    {
        m_tags.push_back(tag);
    }

    void AddSceneInfo(const std::string& info)
    {
        m_sceneInfos.push_back(info);
    }

    const std::vector<std::string>& GetSceneInfos() const
    {
        return m_sceneInfos;
    }



    // 프레임 데이터 조회
    const std::vector<Frame>& GetFrames() const { return m_frames; }
    float                       GetTotalDuration() const { return m_totalDuration; }
    ID2D1Bitmap1* GetBitmap() const { return m_sheet.Get(); }

private:
    std::vector<Frame>                  m_frames;
    float                                m_totalDuration = 0.f;
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_sheet;
    std::vector<Tag>                    m_tags;
    std::vector<std::string> m_sceneInfos; //중복 에셋 불필요 로드를 방지하기 위함. 
};

