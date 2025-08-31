#include "SettingWindow.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "SoundManager.h" // SoundManager와 통신하기 위해 추가

SettingWindow::SettingWindow() : UIWindow(UIWindowType::SettingsWindow, { 431 , 270 }, { 1059, 540 })
{
    m_minValue = 0.0f;
    m_maxValue = 1.0f;

    // SoundManager에서 현재 마스터 볼륨 값을 가져와 초기화합니다.
    m_bgmCurrentValue = SoundManager::Instance().GetMasterBGMVolume();
    m_sfxCurrentValue = SoundManager::Instance().GetMasterSFXVolume();

    m_uiRenderer = std::make_unique<UI_Renderer>();
    if (m_uiRenderer)
    {
        m_uiRenderer->SetBitmap("settingwindow", ResourceManager::Get().GetTexture("SettingWindow"));
        m_uiRenderer->SetBitmap("settinghandle", ResourceManager::Get().GetTexture("SettingHandle"));
        m_uiRenderer->SetBitmap("settingbar", ResourceManager::Get().GetTexture("Setting"));
        m_uiRenderer->SetBitmap("close_button", ResourceManager::Get().GetTexture("CloseButton"));
        m_uiRenderer->SetBitmap("close_button_hover", ResourceManager::Get().GetTexture("CloseButton"));
    }

    CreateSliderHandle();
}

void SettingWindow::Update()
{
    Vec2 mousePos = InputManager::Get().GetMousePosition();
    if (m_draggingBGM && m_bgmHandleComponent)
    {
        m_bgmHandleComponent->UpdateValueFromMousePos(mousePos);
    }

    if (m_draggingSFX && m_sfxHandleComponent)
    {
        m_sfxHandleComponent->UpdateValueFromMousePos(mousePos);
    }

    if (m_bgmHandleObject) m_bgmHandleObject->Update();
    if (m_sfxHandleObject) m_sfxHandleObject->Update();
}

void SettingWindow::Render()
{
    if (!m_isActive || !m_uiRenderer) return;

    RenderSliderBackground();
    RenderCloseButton();
    RenderSliderTrack();
    if (m_bgmHandleObject)
    {
        m_bgmHandleComponent->UpdateDestRect();
        D2DRenderer::Get().DrawBitmap(m_bgmHandleObject->GetRenderInfo()->GetRenderInfo());
    }
    if (m_sfxHandleObject)
    {
        m_sfxHandleComponent->UpdateDestRect();
        D2DRenderer::Get().DrawBitmap(m_sfxHandleObject->GetRenderInfo()->GetRenderInfo());
    }
    RenderValueText();
}

bool SettingWindow::HandleMouseHover(Vec2 mousePos)
{
    if (m_draggingBGM || m_draggingSFX || m_draggingWindow) return true;
    if (m_bgmHandleComponent && m_bgmHandleComponent->IsPointInHandle(mousePos)) {
        m_bgmHandleComponent->SetState(SliderHandleComponent::ButtonState::Hover);
        return true;
    }
    else if (m_sfxHandleComponent && m_sfxHandleComponent->IsPointInHandle(mousePos)) {
        m_sfxHandleComponent->SetState(SliderHandleComponent::ButtonState::Hover);
        return true;
    }
    else {
        if (m_bgmHandleComponent) m_bgmHandleComponent->SetState(SliderHandleComponent::ButtonState::Normal);
        if (m_sfxHandleComponent) m_sfxHandleComponent->SetState(SliderHandleComponent::ButtonState::Normal);
    }

    return false;
}

bool SettingWindow::HandleMouseDown(Vec2 mousePos)
{
    if (m_bgmHandleComponent && m_bgmHandleComponent->IsPointInHandle(mousePos)) {
        m_draggingBGM = true;
        m_bgmHandleComponent->SetState(SliderHandleComponent::ButtonState::Pressed);
        return true;
    }
    if (m_sfxHandleComponent && m_sfxHandleComponent->IsPointInHandle(mousePos)) {
        m_draggingSFX = true;
        m_sfxHandleComponent->SetState(SliderHandleComponent::ButtonState::Pressed);
        return true;
    }
    return false;
}

bool SettingWindow::HandleMouseUp(Vec2 mousePos)
{
    if (m_draggingBGM) {
        m_draggingBGM = false;
        if (m_bgmHandleComponent) {
            m_bgmHandleComponent->SetState(SliderHandleComponent::ButtonState::Normal);
        }
        return true;
    }
    if (m_draggingSFX) {
        m_draggingSFX = false;
        if (m_sfxHandleComponent) {
            m_sfxHandleComponent->SetState(SliderHandleComponent::ButtonState::Normal);
        }
        return true;
    }
    return false;
}

bool SettingWindow::HandleDoubleClick(Vec2 mousePos) { return true; }
bool SettingWindow::HandleMouseRight(Vec2 mousePos) { return true; }
bool SettingWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) { return true; }

UIWindowType SettingWindow::GetType() { return UIWindowType::SettingsWindow; }

void SettingWindow::SetBGMValue(float value)
{
    float normalizedValue = (value - m_minValue) / (m_maxValue - m_minValue);
    normalizedValue = std::clamp(normalizedValue, 0.0f, 1.0f);
    m_bgmCurrentValue = m_minValue + normalizedValue * (m_maxValue - m_minValue);
    if (m_bgmHandleComponent)
    {
        m_bgmHandleComponent->SetValue(normalizedValue);
    }
}

float SettingWindow::GetBGMValue() const { return m_bgmCurrentValue; }

void SettingWindow::SetSFXValue(float value)
{
    float normalizedValue = (value - m_minValue) / (m_maxValue - m_minValue);
    normalizedValue = std::clamp(normalizedValue, 0.0f, 1.0f);
    m_sfxCurrentValue = m_minValue + normalizedValue * (m_maxValue - m_minValue);
    if (m_sfxHandleComponent)
    {
        m_sfxHandleComponent->SetValue(normalizedValue);
    }
}

float SettingWindow::GetSFXValue() const { return m_sfxCurrentValue; }
void SettingWindow::SetOnBGMValueChanged(std::function<void(float)> callback) { m_onBGMValueChanged = callback; }
void SettingWindow::SetOnSFXValueChanged(std::function<void(float)> callback) { m_onSFXValueChanged = callback; }

void SettingWindow::CreateSliderHandle()
{
    const float sliderTrackWidth = 630.0f;
    const float sliderTrackHeight = 32.0f;
    const float handleWidth = 19.0f;
    const float handleHeight = 38.0f;

    m_bgmMinX = m_position.x + 298.0f;
    m_bgmMaxX = m_bgmMinX + sliderTrackWidth;
    m_sfxMinX = m_position.x + 298.0f;
    m_sfxMaxX = m_sfxMinX + sliderTrackWidth;

    m_bgmHandleObject = std::make_unique<Object>();
    float bgmHandleY = m_position.y + 228.0f + (sliderTrackHeight - handleHeight) / 2.0f;
    float initialBGM_X = m_position.x + 298.0f + (sliderTrackWidth * ((m_bgmCurrentValue - m_minValue) / (m_maxValue - m_minValue)));
    m_bgmHandleObject->SetPosition(Vec2(initialBGM_X, bgmHandleY));
    RenderInfo* bgmRenderInfo = m_bgmHandleObject->GetRenderInfo();
    auto handleImg = m_uiRenderer->GetBitmap("settinghandle");
    bgmRenderInfo->SetBitmap(handleImg.Get());
    m_bgmHandleComponent = m_bgmHandleObject->AddComponent<SliderHandleComponent>(bgmRenderInfo, m_bgmMinX, m_bgmMaxX, m_bgmCurrentValue);
    m_bgmHandleComponent->SetWidth(handleWidth);
    m_bgmHandleComponent->SetHeight(handleHeight);

    // 슬라이더 값이 변경될 때 SoundManager의 마스터 볼륨을 업데이트합니다.
    m_bgmHandleComponent->SetOnValueChanged([this](float normalizedValue) {
        m_bgmCurrentValue = m_minValue + normalizedValue * (m_maxValue - m_minValue);
        SoundManager::Instance().SetMasterBGMVolume(m_bgmCurrentValue);
        if (m_onBGMValueChanged) { m_onBGMValueChanged(m_bgmCurrentValue); }
        });

    m_sfxHandleObject = std::make_unique<Object>();
    float sfxHandleY = m_position.y + 371.0f + (sliderTrackHeight - handleHeight) / 2.0f;
    float initialSFX_X = m_position.x + 298.0f + (sliderTrackWidth * ((m_sfxCurrentValue - m_minValue) / (m_maxValue - m_minValue)));
    m_sfxHandleObject->SetPosition(Vec2(initialSFX_X, sfxHandleY));
    RenderInfo* sfxRenderInfo = m_sfxHandleObject->GetRenderInfo();
    sfxRenderInfo->SetBitmap(handleImg.Get());
    m_sfxHandleComponent = m_sfxHandleObject->AddComponent<SliderHandleComponent>(sfxRenderInfo, m_sfxMinX, m_sfxMaxX, m_sfxCurrentValue);
    m_sfxHandleComponent->SetWidth(handleWidth);
    m_sfxHandleComponent->SetHeight(handleHeight);

    // 슬라이더 값이 변경될 때 SoundManager의 마스터 볼륨을 업데이트합니다.
    m_sfxHandleComponent->SetOnValueChanged([this](float normalizedValue) {
        m_sfxCurrentValue = m_minValue + normalizedValue * (m_maxValue - m_minValue);
        SoundManager::Instance().SetMasterSFXVolume(m_sfxCurrentValue);
        if (m_onSFXValueChanged) { m_onSFXValueChanged(m_sfxCurrentValue); }
        });
}

void SettingWindow::RenderSliderBackground()
{
    if (m_uiRenderer)
    {
        ID2D1Bitmap1* bg = m_uiRenderer->GetBitmap("settingwindow").Get();
        D2D1_RECT_F dest = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
        D2DRenderer::Get().DrawBitmap(bg, dest);
    }
}

void SettingWindow::RenderSliderTrack()
{
    if (m_uiRenderer)
    {
        ID2D1Bitmap1* bg = m_uiRenderer->GetBitmap("settingbar").Get();
        float bgWidth = 650.0f;
        float bgHeight = 32.0f;

        D2D1_RECT_F dest1 = {
          m_position.x + 298.0f,
          m_position.y + 228.0f,
          m_position.x + 298.0f + bgWidth,
          m_position.y + 228.0f + bgHeight
        };
        D2DRenderer::Get().DrawBitmap(bg, dest1);

        D2D1_RECT_F dest2 = {
          m_position.x + 298.0f,
          m_position.y + 371.0f,
          m_position.x + 298.0f + bgWidth,
          m_position.y + 371.0f + bgHeight
        };
        D2DRenderer::Get().DrawBitmap(bg, dest2);
    }
}

void SettingWindow::RenderValueText()
{
    if (m_uiRenderer)
    {
        float titleX = m_position.x + 97.77f;
        float titleY = m_position.y + 73.0f;
        D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
        D2DRenderer::Get().DrawMessage(L"설정", titleX, titleY, 100, 25.0f, D2D1::ColorF(0xffe6d8));
        float bgmTextX = m_position.x + 120;
        float bgmTextY = m_position.y + 228.77f;
        D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
        D2DRenderer::Get().DrawMessage(L"배경음", bgmTextX, bgmTextY, 100, 30.0f, D2D1::ColorF(0xffe6d8));

        float sfxTextX = m_position.x + 120;
        float sfxTextY = m_position.y + 371.77f;
        D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
        D2DRenderer::Get().DrawMessage(L"효과음", sfxTextX, sfxTextY, 100, 30.0f, D2D1::ColorF(0xffe6d8));
    }
}

void SettingWindow::RenderCloseButton()
{
    float rightMargin = 85.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35.0f };
    Vec2 closeButtonSize = { 35.0f, 35.0f };
    D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };
    if (m_uiRenderer)
    {
        ID2D1Bitmap1* cb = m_uiRenderer->GetBitmap("close_button").Get();
        if (cb) D2DRenderer::Get().DrawBitmap(cb, destRect);
    }
}

void SettingWindow::MoveHandles(Vec2 delta)
{
    if (m_bgmHandleObject)
    {
        Vec2 currentPos = m_bgmHandleObject->GetTransform().GetPosition();
        m_bgmHandleObject->SetPosition(currentPos + delta);
    }
    if (m_sfxHandleObject)
    {
        Vec2 currentPos = m_sfxHandleObject->GetTransform().GetPosition();
        m_sfxHandleObject->SetPosition(currentPos + delta);
    }

    m_bgmMinX += delta.x;
    m_bgmMaxX += delta.x;
    m_sfxMinX += delta.x;
    m_sfxMaxX += delta.x;

    if (m_bgmHandleComponent)
    {
        m_bgmHandleComponent->UpdateRange(m_bgmMinX, m_bgmMaxX);
        Vec2 bgmPos = m_bgmHandleObject->GetTransform().GetPosition();
        float normalizedBGM = (bgmPos.x - m_bgmMinX) / (m_bgmMaxX - m_bgmMinX);
        normalizedBGM = std::clamp(normalizedBGM, 0.0f, 1.0f);
        m_bgmCurrentValue = m_minValue + normalizedBGM * (m_maxValue - m_minValue);
        m_bgmHandleComponent->UpdateValueOnly(normalizedBGM);
    }
    if (m_sfxHandleComponent)
    {
        m_sfxHandleComponent->UpdateRange(m_sfxMinX, m_sfxMaxX);
        Vec2 sfxPos = m_sfxHandleObject->GetTransform().GetPosition();
        float normalizedSFX = (sfxPos.x - m_sfxMinX) / (m_sfxMaxX - m_sfxMinX);
        normalizedSFX = std::clamp(normalizedSFX, 0.0f, 1.0f);
        m_sfxCurrentValue = m_minValue + normalizedSFX * (m_maxValue - m_minValue);
        m_sfxHandleComponent->UpdateValueOnly(normalizedSFX);
    }
}