#pragma once
#include "UIWindow.h"
#include "Item.h"

#define SlotSize 74.0f
#define ButX 186.0f
#define ButY 47.0f

enum class SynButton
{
	Syn,
	Cancle,
	Nothing
};

enum class SynSlot
{
	Result,
	Slot1,
	Slot2,
	Nothing
};


class SynthesisWin : public UIWindow
{



public:
	SynthesisWin();
	~SynthesisWin();

	virtual bool HandleMouseHover(Vec2 mousePos) override;
	virtual bool HandleMouseDown(Vec2 mousePos)  override;
	virtual bool HandleMouseUp(Vec2 mousePos)  override;
	virtual bool HandleDoubleClick(Vec2 mousePos) override;
	virtual void SetActivate(bool active) { m_isActive = active; }


	void Update() override;
	void Render() override;

	// Ÿ�� üũ��
	virtual UIWindowType GetType() override;

	void UpdatePosition();
	void MemBitmapLoad();

	void MemInit();
	void MemRender();
	SynButton ButtonInit(Vec2 pos);
	SynSlot SlotInit(Vec2 pos);

	bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

	// �ռ���.
	void PerformSynthesis();

private:

	// size?, bound�� ���⿡ �� �ֱ� ��. 


	//�迭�� �ص� �Ǳ� �ϴٸ� ������ 
	std::map<SynSlot, Vec2>m_slot;
	std::map<SynSlot, Item*>m_slot_Item;

	std::map<SynButton, Vec2>m_but;

	//��� ����� �ҷ��� �� �� 


};

