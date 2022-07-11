#include "InfoHelper.h"

InfoHelper* InfoHelper::infoHelper = nullptr;

InfoHelper* InfoHelper::getInstance()
{
    if (!infoHelper)
    {
        infoHelper = new InfoHelper();
        infoHelper->init("word/info.plist");
    }
    return infoHelper;
}

void InfoHelper::release()
{
    CC_SAFE_DELETE(infoHelper);
}

void InfoHelper::init(std::string fileName)
{
    _infoMap = cocos2d::FileUtils::getInstance()->getValueMapFromFile(fileName);
}
