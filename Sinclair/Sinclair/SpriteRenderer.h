#pragma once
#include "pch.h"
#include "Component.h"
#include "AnimationClip.h"


class SpriteRenderer : public Component
{


public:
		SpriteRenderer() = default;
		~SpriteRenderer(); //asset 정리는 여기서 함. 

		void SetCurrentClip(const AnimationClip* clip);
		void AddClip(const std::string& aniName, const AnimationClip& clip);
		void Update(float deltaTime);



		const Frame& GetCurrentFrame() const;
		const AnimationClip& GetCurrentClip();
		AnimationClip* GetClip(const std::string& aniName);




private:
		std::unordered_map<std::string, AnimationClip> m_clips;
		const AnimationClip* m_clip;


		float                m_elapsed = 0.f;      // 재생 위치
		bool                 m_loop = true;     // 루프 재생 여부
};