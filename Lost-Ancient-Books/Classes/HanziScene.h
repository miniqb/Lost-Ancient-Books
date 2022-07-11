#pragma once
#include "cocos2d.h"
#include "QuestionNode.h"
class HanziScene :
    public cocos2d::Scene
{
public:
    bool initHanziScene(cocos2d::RenderTexture* texture);
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    void onMouseDown(cocos2d::EventMouse* event);
    // implement the "static create()" method manually
    Node* creatHanziPackageNode(float maxWidth, const cocos2d::Size& margin, const cocos2d::Vector<HanziButton*>& buttons, float fontSize);

    Node* createWardNode(const cocos2d::Vec2& pos, const cocos2d::Size& size, int shellSum, int pieceSum);

    void outScene(bool res);

    void update(float dt)override;
    void onEnter()override;
    void onExit()override;
public:
    static HanziScene* create(cocos2d::RenderTexture* texture);
protected:
    bool showResult();
    void answerCallback();
    void resetHanziButtons();
private:
    QuestionNode* _questionNode;
    cocos2d::ui::LoadingBar* _loadingBar;
    float _answerTime = 20.f;
    cocos2d::Label* _tipLabel;
    cocos2d::ui::Button* _okButton;
    cocos2d::ui::Button* _cancleButton;
    cocos2d::Sprite* _background;
    
    int _shellSum;
    int _pieceSum;
};

