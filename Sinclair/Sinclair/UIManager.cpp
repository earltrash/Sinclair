#include "UIManager.h"

void UIManager::Initialize()
{
    try
    {
        // SettingsWindow
        //m_allWindows.emplace(UIWindowType::SettingsWindow, std::make_unique<SettingsWindow>());
        //if (auto* window = GetWindow(UIWindowType::SettingsWindow))
        //{
        //    window->SetActivate(false);
        //} 
        
        //InventoryWindow
        m_allWindows.emplace(UIWindowType::InventoryWindow, std::make_unique<Inventory>());
        if (auto* window = GetWindow(UIWindowType::InventoryWindow))
        {
            window->SetActivate(false);
        }
        // InventoryTooltip
        //m_allWindows.emplace(UIWindowType::InventoryTooltip, std::make_unique<InventoryTooltip>());
        //if (auto* window = GetWindow(UIWindowType::InventoryTooltip))
        //{
        //    window->SetActivate(false);
        //}

        // EquipmentWindow
        m_allWindows.emplace(UIWindowType::EquipmentWindow, std::make_unique<EquipmentWindow>());
        if (auto* window = GetWindow(UIWindowType::EquipmentWindow))
        {
            window->SetActivate(false);
        }

        // StatsWindow
        //m_allWindows.emplace(UIWindowType::StatsWindow, std::make_unique<StatsWindow>());
        //if (auto* window = GetWindow(UIWindowType::StatsWindow))
        //{
        //    window->SetActivate(false);
        //}

        //// EnhancementWindow
        ////m_allWindows.emplace(UIWindowType::EnhancementWindow, std::make_unique<EnhancementWindow>());
        //if (auto* window = GetWindow(UIWindowType::EnhancementWindow))
        //{
        //    window->SetActivate(false);
        //}

        //// SynthesisWindow
        ////m_allWindows.emplace(UIWindowType::SynthesisWindow, std::make_unique<SynthesisWindow>());
        //if (auto* window = GetWindow(UIWindowType::SynthesisWindow))
        //{
        //    window->SetActivate(false);
        //}

        //// StatPotionUseWindow
        ////m_allWindows.emplace(UIWindowType::StatPotionUseWindow, std::make_unique<StatPotionUseWindow>());
        //if (auto* window = GetWindow(UIWindowType::StatPotionUseWindow))
        //{
        //    window->SetActivate(false);
        //}

        OutputDebugStringA("UIManager initialized successfully\n");
    }
    // 디버그용.
    catch (const std::exception& e)
    {
        std::string errorMsg = "UIManager Initialize failed: " + std::string(e.what()) + "\n";
        OutputDebugStringA(errorMsg.c_str());
    }

}

UIManager::~UIManager() = default;