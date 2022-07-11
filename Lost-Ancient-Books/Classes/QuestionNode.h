#pragma once
#include "cocos2d.h"
#include "HanziButton.h"

class QuestionNode :
    public cocos2d::Node
{
public:
    /// <summary>
    /// 创建并返回一个问题标题对象，用于问题显示。显示的内容包括给出的字和缺省的字
    /// </summary>
    /// <param name="size">每个字格子所占的大小</param>
    /// <param name="path">bmp字体路径</param>
    /// <param name="que">完整句子的字符串数组，数组中每一项都是通过文件读取到的一个汉字或者标点符号，不能直接输入汉字</param>
    /// <param name="nulls">题目中要缺省的汉字下标，从0开始</param>
    /// <param name="fsize">要显示的字体大小</param>
    /// <returns>返回一个问题标题对象</returns>
    static QuestionNode* create(const cocos2d::Size& size, const std::string& path, const std::vector<std::string>& que, const std::vector<int>& nulls, int fsize=80);
    bool initQuestion(const cocos2d::Size& size, const std::string& path, const std::vector<std::string>& que, const std::vector<int>& nulls, int fsize=80);
    virtual void onEnter()override;

    /// <summary>
    /// 设置字的外边距
    /// </summary>
    /// <param name="m">外边距，大于等于0</param>
    void setMargin(const cocos2d::Size& m) { _margin = m; }
    /// <summary>
    /// 得到字的外边距
    /// </summary>
    /// <returns>返回外边距</returns>
    const cocos2d::Size& getMargin() { return _margin; }

    void setMaxWidth(float w) { _maxWidth = w; }

    const std::vector<int>& getNullsIndexs() { return _nulls; }

    /// <summary>
    /// 放入一个汉字，默认放到从左到右第一个空缺位
    /// </summary>
    /// <param name="button">要放入的汉字按钮</param>
    void putButton(HanziButton* button);
    /// <summary>
    /// 返还一个汉字按钮
    /// </summary>
    /// <param name="button">要返还的汉字按钮</param>
    void returnButton(HanziButton* button);

    bool isRight();
protected:
    cocos2d::Vec2 getStartpx(size_t length, size_t height);
    cocos2d::Vec2 countStart(size_t& length);
    cocos2d::Vec2 getEx(size_t i, size_t length);
private:
    std::vector<std::string> _question;
    std::string _fontPath;
    cocos2d::Size _cellSize;
    cocos2d::Size _margin;
    int _fontSize;
    float _maxWidth;

    int _nowNullsIndex;
    std::vector<int> _nulls;

    //汉字插槽
    std::vector<HanziButton*> _hanziRabbet;
public:
    QuestionNode();
    virtual ~QuestionNode();
};

