#pragma once
#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "SimpleMathHelper.h"
using namespace D2DTM;

class Object //�߻�Ǵ� ����ü�� �ǰڴµ�
{
public:
		Object() = default;
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

public:

		//position ���� update�� ���ؼ� �ٲ��� �ϰ��� 

		//���� ������Ʈ ������ ���� ���� �����̱� ������, �� ���� �� ���� �ƿ� ���� ����. ����� ����
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args);
		template<typename T>
		T* GetComponent() const;

		//void ComponentClear(); //�̰� ����

protected:

		std::vector<std::unique_ptr<Component>> m_Components;

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
#pragma endregion
