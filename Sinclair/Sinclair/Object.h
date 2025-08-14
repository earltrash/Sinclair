#pragma once
#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "SimpleMathHelper.h"
#include "RenderInfo.h"

using namespace D2DTM;

class Object //�߻�Ǵ� ����ü�� �ǰڴµ�
{
public:
	Object() : m_renderInfo{}, m_transform(&m_renderInfo) {}
		virtual ~Object();
		

		virtual void Update() 
		{
			for (auto& obj : m_Components)
			{
				obj->Update();
			}
		};
		virtual void FixedUpdate(float dt) 
		{
			for (auto& obj : m_Components)
			{
				obj->FixedUpdate(dt);
			}
		}
		virtual void Render() {};

		virtual void OnEvent(const std::string& ev)
		{
			for (auto& compo : m_Components)
			{
				compo->OnEvent(ev);
			}
		}

		void SetActive(bool b) { activated = b; };
		bool isActive() { return activated; }
public:
		
		void SetPosition(Vec2 position)
		{
				m_transform.SetPosition(position);
		} 

		Transform& GetTransform()
		{
				return m_transform;
		}

		RenderInfo* GetRenderInfo()
		{
			return &m_renderInfo;
		}

public:

		//position ���� update�� ���ؼ� �ٲ��� �ϰ��� 

		//���� ������Ʈ ������ ���� ���� �����̱� ������, �� ���� �� ���� �ƿ� ���� ����. ����� ����
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args);
		template<typename T>
		T* GetComponent() const;
		template<typename T>
		T* GetComponent(int num);

		void ComponentClear() { m_Components.clear(); }
protected:

		std::vector<std::unique_ptr<Component>> m_Components;

		RenderInfo m_renderInfo;
		Transform m_transform;

		bool activated = true;
};



//���� ���� ���� ���� Component �Լ� ģ����
#pragma region Component
template<typename T, typename ...Args>
inline T* Object::AddComponent(Args && ...args)
{
		static_assert(std::is_base_of<Component, T>::value, "Component�� �ڽ��̾�� �Ѵٴ� ���� ");

		auto comp = std::make_unique<T>(std::forward<Args>(args)...);

		comp->SetOwner(this);

		T* ptr = comp.get();

		m_Components.emplace_back(std::move(comp));

		return ptr;
}

template<typename T>
inline T* Object::GetComponent() const //�̰� �� ���Ÿ� Component ������ �� �ʿ䰡 ���� ����?
{
		for (auto& com : m_Components)
		{
				if (auto ptr = dynamic_cast<T*>(com.get()))
				{
						return ptr;
				}
		}

		return nullptr;
}
template<typename T>
inline T* Object::GetComponent(int num)
{
	if (m_Components[num])
	{
		auto ptr = dynamic_cast<T*>(m_Components[num].get());
		return ptr;
	}
	else 
		return nullptr;
}
#pragma endregion
