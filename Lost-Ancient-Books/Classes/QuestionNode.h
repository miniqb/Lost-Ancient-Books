#pragma once
#include "cocos2d.h"
#include "HanziButton.h"

class QuestionNode :
    public cocos2d::Node
{
public:
    /// <summary>
    /// ����������һ����������������������ʾ����ʾ�����ݰ����������ֺ�ȱʡ����
    /// </summary>
    /// <param name="size">ÿ���ָ�����ռ�Ĵ�С</param>
    /// <param name="path">bmp����·��</param>
    /// <param name="que">�������ӵ��ַ������飬������ÿһ���ͨ���ļ���ȡ����һ�����ֻ��߱����ţ�����ֱ�����뺺��</param>
    /// <param name="nulls">��Ŀ��Ҫȱʡ�ĺ����±꣬��0��ʼ</param>
    /// <param name="fsize">Ҫ��ʾ�������С</param>
    /// <returns>����һ������������</returns>
    static QuestionNode* create(const cocos2d::Size& size, const std::string& path, const std::vector<std::string>& que, const std::vector<int>& nulls, int fsize=80);
    bool initQuestion(const cocos2d::Size& size, const std::string& path, const std::vector<std::string>& que, const std::vector<int>& nulls, int fsize=80);
    virtual void onEnter()override;

    /// <summary>
    /// �����ֵ���߾�
    /// </summary>
    /// <param name="m">��߾࣬���ڵ���0</param>
    void setMargin(const cocos2d::Size& m) { _margin = m; }
    /// <summary>
    /// �õ��ֵ���߾�
    /// </summary>
    /// <returns>������߾�</returns>
    const cocos2d::Size& getMargin() { return _margin; }

    void setMaxWidth(float w) { _maxWidth = w; }

    const std::vector<int>& getNullsIndexs() { return _nulls; }

    /// <summary>
    /// ����һ�����֣�Ĭ�Ϸŵ������ҵ�һ����ȱλ
    /// </summary>
    /// <param name="button">Ҫ����ĺ��ְ�ť</param>
    void putButton(HanziButton* button);
    /// <summary>
    /// ����һ�����ְ�ť
    /// </summary>
    /// <param name="button">Ҫ�����ĺ��ְ�ť</param>
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

    //���ֲ��
    std::vector<HanziButton*> _hanziRabbet;
public:
    QuestionNode();
    virtual ~QuestionNode();
};

