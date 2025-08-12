#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "Inventory.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"

#include "ItemManager.h"

#include "Object.h"

class EquipmentWindow : public UIWindow
{
public:

	// �ʱ� ��ġ�� ũ�� ����
	EquipmentWindow()
		: UIWindow(UIWindowType::EquipmentWindow, { 0, 0 }, { 524, 766 })
	{
		// ��� ���� ��ġ �� ũ�� �ʱ�ȭ
		// ���� ��ǥ ��� m_position�� �������� ��� ��ǥ�� ����
		m_slotPositions[Wearable_part::Helmet] = { 222, 138 };
		m_slotSizes[Wearable_part::Helmet] = { 74, 74 };

		m_slotPositions[Wearable_part::Weapon] = { 370, 517 };
		m_slotSizes[Wearable_part::Weapon] = { 74, 74 };

		m_slotPositions[Wearable_part::Upper] = { 222, 290 };
		m_slotSizes[Wearable_part::Upper] = { 74, 74 };

		m_slotPositions[Wearable_part::Glove] = { 74, 365 };
		m_slotSizes[Wearable_part::Glove] = { 74, 74 };

		m_slotPositions[Wearable_part::Under] = { 222, 442 };
		m_slotSizes[Wearable_part::Under] = { 74, 74 };

		m_slotPositions[Wearable_part::Shoes] = { 222, 593 };
		m_slotSizes[Wearable_part::Shoes] = { 74, 74 };

		m_slotPositions[Wearable_part::Cape] = { 370, 365 };
		m_slotSizes[Wearable_part::Cape] = { 74, 74 };

		m_slotPositions[Wearable_part::Ring] = { 74, 214 };
		m_slotSizes[Wearable_part::Ring] = { 74, 74 };

		m_slotPositions[Wearable_part::Neckless] = { 370, 214 };
		m_slotSizes[Wearable_part::Neckless] = { 74, 74 };

		UI_Renderer* uiRenderer = AddComponent<UI_Renderer>();
		if (uiRenderer)
		{
			// �ʿ��� UI ��Ʈ�ʵ��� ResourceManager���� �ε��Ͽ� UI_Renderer�� ����
			// ResourceManager�� �ش� Ű�� ��Ʈ���� �ε�
			uiRenderer->SetBitmap("equipment_window_bg", ResourceManager::Get().GetTexture("EquipmentWindowBg"));
			//uiRenderer->SetBitmap("title_bar_bg", ResourceManager::Get().GetTexture("titlebarbg"));
			uiRenderer->SetBitmap("close_button", ResourceManager::Get().GetTexture("CloseButton"));
			uiRenderer->SetBitmap("close_button_hover", ResourceManager::Get().GetTexture("CloseButton"));
			//uiRenderer->SetBitmap("empty_slot", ResourceManager::Get().GetTexture("SlotNormal"));

			// ���� ������ ��Ʈ�� �ε�
			uiRenderer->SetBitmap("icon_helmet_slot", ResourceManager::Get().GetTexture("SlotHelmet"));
			uiRenderer->SetBitmap("icon_weapon_slot", ResourceManager::Get().GetTexture("SlotWeapon"));
			uiRenderer->SetBitmap("icon_armor_slot", ResourceManager::Get().GetTexture("SlotArmor"));
			uiRenderer->SetBitmap("icon_pants_slot", ResourceManager::Get().GetTexture("SlotPants"));
			uiRenderer->SetBitmap("icon_glove_slot", ResourceManager::Get().GetTexture("SlotGlove"));
			uiRenderer->SetBitmap("icon_shoes_slot", ResourceManager::Get().GetTexture("SlotShoes"));
			uiRenderer->SetBitmap("icon_necklace_slot", ResourceManager::Get().GetTexture("SlotNecklace"));
			uiRenderer->SetBitmap("icon_earring_slot", ResourceManager::Get().GetTexture("Slotearring"));
			uiRenderer->SetBitmap("icon_cape_slot", ResourceManager::Get().GetTexture("SlotCape"));
		}
		
		m_itemDatabase = std::make_unique<ItemDatabase>();
		m_itemBank = std::make_unique<ItemBank>();

		// ��� ����
		auto helmet_slot = std::make_unique<Object>();
		auto hInfo = helmet_slot->GetRenderInfo();
		hInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotHelmet").Get());
		helmet_slot->GetTransform().SetPosition({ 222, 138 });
		AddEffect(helmet_slot.get());
		m_slots.emplace(Wearable_part::Helmet, std::move(helmet_slot));

		// ���� ����
		auto weapon_slot = std::make_unique<Object>();
		auto wInfo = weapon_slot->GetRenderInfo();
		wInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotWeapon").Get());
		weapon_slot->GetTransform().SetPosition({ 370, 517 });
		AddEffect(weapon_slot.get());
		m_slots.emplace(Wearable_part::Weapon, std::move(weapon_slot));

		// ���� ����
		auto upper_slot = std::make_unique<Object>();
		auto upInfo = upper_slot->GetRenderInfo();
		upInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotArmor").Get());
		upper_slot->GetTransform().SetPosition({ 222, 290 });
		AddEffect(upper_slot.get());
		m_slots.emplace(Wearable_part::Upper, std::move(upper_slot));

		// �尩 ����
		auto glove_slot = std::make_unique<Object>();
		auto gInfo = glove_slot->GetRenderInfo();
		gInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotGlove").Get());
		glove_slot->GetTransform().SetPosition({ 74, 365 });
		AddEffect(glove_slot.get());
		m_slots.emplace(Wearable_part::Glove, std::move(glove_slot));

		// ���� ����
		auto under_slot = std::make_unique<Object>();
		auto underInfo = under_slot->GetRenderInfo();
		underInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotPants").Get());
		under_slot->GetTransform().SetPosition({ 222, 442 });
		AddEffect(under_slot.get());
		m_slots.emplace(Wearable_part::Under, std::move(under_slot));

		// �Ź� ����
		auto shoes_slot = std::make_unique<Object>();
		auto sInfo = shoes_slot->GetRenderInfo();
		sInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotShoes").Get());
		shoes_slot->GetTransform().SetPosition({ 222, 593 });
		AddEffect(shoes_slot.get());
		m_slots.emplace(Wearable_part::Shoes, std::move(shoes_slot));

		// ���� ����
		auto cape_slot = std::make_unique<Object>();
		auto cInfo = cape_slot->GetRenderInfo();
		cInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotCape").Get());
		cape_slot->GetTransform().SetPosition({ 370, 365 });
		AddEffect(cape_slot.get());
		m_slots.emplace(Wearable_part::Cape, std::move(cape_slot));

		// ���� ����
		auto ring_slot = std::make_unique<Object>();
		auto rInfo = ring_slot->GetRenderInfo();
		rInfo->SetBitmap(ResourceManager::Get().GetTexture("Slotearring").Get());
		ring_slot->GetTransform().SetPosition({ 74, 214 });
		AddEffect(ring_slot.get());
		m_slots.emplace(Wearable_part::Ring, std::move(ring_slot));

		// ����� ����
		auto neck_slot = std::make_unique<Object>();
		auto nInfo = neck_slot->GetRenderInfo();
		nInfo->SetBitmap(ResourceManager::Get().GetTexture("SlotNecklace").Get());
		neck_slot->GetTransform().SetPosition({ 370, 214 });
		AddEffect(neck_slot.get());
		m_slots.emplace(Wearable_part::Neckless, std::move(neck_slot));
	}
	void AddEffect(Object* slot);
	void Update() override;
	void FixedUpdate(float dt) override
	{
		for (auto& slot : m_slots)
		{
			slot.second->FixedUpdate(dt);
		}
	}
	void Render() override;

	UIWindowType GetType() override { return UIWindowType::EquipmentWindow; }

	void SetPosition(Vec2 pos)
	{
		m_position = pos;
		UpdateSlotPositions();
	}

	// Ŭ�� ���� ��ɵ� ����
	bool HandleMouseDown(Vec2 mousePos) override;
	bool HandleMouseUp(Vec2 mousePos) override;
	bool HandleDoubleClick(Vec2 mousePos) override;
	bool HandleMouseHover(Vec2 mousePos) override;
	bool HandleMouseRight(Vec2 mousePos) override;

	void ClearSlot(Wearable_part part);
	// ��� ������ ��ü. 
	void EquipItem(Item* item);
	// ��� ������ ����
	Item* UnequipItem(Wearable_part slotType);
	// �ش� Ÿ���� ��� ������ üũ.
	Item* GetEquippedItem(Wearable_part slotType) const;

	// â ��ü �׸���.
	void RenderBackground();
	void RenderTitleBar();
	void RenderSlots();
	void RenderEquippedItems();
	void RenderCloseButton();
	Wearable_part GetSlotTypeAt(Vec2 mousePos) const;

	bool CanEquipItem(Item* item, Wearable_part slotType) const;
	void UpdateSlotPositions();
	void RenderSlotIcon(Wearable_part slotType, Vec2 position);

	bool IsItemTypeMatch(Wearable_part itemType, Wearable_part slotType);
	bool IsItemTypeMatch(const std::string& itemId, Wearable_part slotType) const;

	bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

	void SendEventToComponent(const std::string& ev, Wearable_part part)
	{
		m_slots[part]->OnEvent(ev);
	}
private:
	std::unordered_map<Wearable_part, Item*> m_equippedItems;
	std::unordered_map<Wearable_part, std::shared_ptr<Object>> m_slots;
	std::unordered_map<Wearable_part, Vec2> m_slotPositions;
	std::unordered_map<Wearable_part, Vec2> m_slotSizes;

	std::unique_ptr<ItemDatabase> m_itemDatabase;
	std::unique_ptr<ItemBank> m_itemBank;
};