#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "Inventory.h"

class EquipmentWindow : public UIWindow
{
public:

	// ÃÊ±â À§Ä¡¿Í Å©±â ¼³Á¤
	EquipmentWindow()
		: UIWindow(UIWindowType::EquipmentWindow, { 50, 50 }, { 200, 300 }) 
	{
		m_inventory = make_unique<Inventory>();
		// Àåºñ ½½·Ô À§Ä¡ ¹× Å©±â ÃÊ±âÈ­
		m_slotPositions[Wearable_part::Glove] = { m_position.x + 222, m_position.y + 138 }; // 1¹øÄ­
		m_slotSizes[Wearable_part::Glove] = { 74, 74 };

		m_slotPositions[Wearable_part::Helmet] = { m_position.x + 222, m_position.y + 290 };// 2¹øÄ­
		m_slotSizes[Wearable_part::Helmet] = { 74, 74 };

		m_slotPositions[Wearable_part::EarRing] = { m_position.x + 222, m_position.y + 442 }; // 3¹øÄ­
		m_slotSizes[Wearable_part::EarRing] = { 74, 74 };

		m_slotPositions[Wearable_part::Upper] = { m_position.x + 222, m_position.y + 593 }; // 4¹øÄ­
		m_slotSizes[Wearable_part::Upper] = { 74, 74 };

		m_slotPositions[Wearable_part::Under] = { m_position.x + 74, m_position.y + 214 }; // 5¹øÄ­
		m_slotSizes[Wearable_part::Under] = { 74, 74 };
		
		m_slotPositions[Wearable_part::Shoes] = { m_position.x + 74, m_position.y + 365 }; // 6¹øÄ­
		m_slotSizes[Wearable_part::Shoes] = { 74, 74 };
		
		m_slotPositions[Wearable_part::Weapon] = { m_position.x + 74, m_position.y + 214 };	// 7¹øÄ­
		m_slotSizes[Wearable_part::Weapon] = { 74, 74 };

		m_slotPositions[Wearable_part::Neckless] = { m_position.x + 370, m_position.y + 214 }; // 8¹øÄ­
		m_slotSizes[Wearable_part::Neckless] = { 74, 74 };

		m_slotPositions[Wearable_part::Cape] = { m_position.x + 370, m_position.y + 365 }; // 9¹øÄ­
		m_slotSizes[Wearable_part::Cape] = { 74, 74 };
	}

		void Update() override;;
		void Render() override {};
		// Å¬¸¯ °ü·Ã ±â´Éµé ±¸Çö
		bool HandleMouseDown(Vec2 mousePos) override;
		bool HandleMouseUp(Vec2 mousePos) override;
		bool HandleDoubleClick(Vec2 mousePos) override;
		bool HandleMouseHover(Vec2 mousePos) override;

		void EquipItem(Item* item);
		Item* UnequipItem(Wearable_part slotType);
		Item* GetEquippedItem(Wearable_part slotType) const;

		void RenderBackground();
		void RenderTitleBar();
		void RenderSlots();
		void RenderEquippedItems();
		void RenderCloseButton();
		Wearable_part GetSlotTypeAt(Vec2 mousePos) const;


		bool CanEquipItem(Item* item, Wearable_part slotType) const;
		void UpdateSlotPositions();
		void RenderSlotIcon(Wearable_part slotType, Vec2 position);
private:
		bool m_isActive = false;
		UIWindowType m_windowType = UIWindowType::EquipmentWindow;

		std::unordered_map<Wearable_part, Item*> m_equippedItems;
		std::unordered_map<Wearable_part, Vec2> m_slotPositions;
		std::unordered_map<Wearable_part, Vec2> m_slotSizes;

		unique_ptr<Inventory> m_inventory;
};

