#include "QuestionHelper.h"
#include "GameConfig.h"
USING_NS_CC;
QuestionHelper* QuestionHelper::qHelper = nullptr;
QuestionHelper* QuestionHelper::getInstance()
{
    if (!qHelper)
    {
        qHelper = new QuestionHelper();
        auto config = GameConfig::getInstance();
        if (!qHelper->init(config->getQuestionFiles(), config->getAllHanziFile()))
            CC_SAFE_DELETE(qHelper);
    }
    return qHelper;
}

void QuestionHelper::release()
{
    CC_SAFE_DELETE(qHelper);
}

bool QuestionHelper::init(const std::vector<std::string>& fileNames, const std::string& randomFileName)
{
    auto fileUtils = FileUtils::getInstance();

    for (auto& fileName : fileNames)
    {
        if (fileUtils->isFileExist(fileName))
        {
            auto poms = fileUtils->getValueVectorFromFile(fileName);
            _pomList.insert(_pomList.end(), poms.begin(), poms.end());
        }
        else
            CCLOG("question path's file: %s is not find!", fileName.c_str());
    }
    _random = fileUtils->getValueVectorFromFile(randomFileName);

    std::string ss("[");
    for (auto& str : _random)
    {
        ss += '\"' + str.asString() + '\"' + ',';

    }
    ss.pop_back();
    ss += ']';
    CCLOG(ss.c_str());

    return true;
}

cocos2d::Value& QuestionHelper::getRandomPom()
{
    _nowChooseIndex = random() % _pomList.size();
    return _pomList[_nowChooseIndex];
}

cocos2d::Value& QuestionHelper::getNowChoosePom()
{
    if (_nowChooseIndex == -1)
        return getRandomPom();
    return _pomList[_nowChooseIndex];
}

std::vector<int>& QuestionHelper::getRandomIndexs(int num, int allSum, const std::vector<int>& cant)
{
    CCASSERT(allSum >= num + cant.size(), "allSum is not biger than num + cant.size()");

    std::vector<bool> cants(allSum, false);
    for (int i:cant)
    {
        if(i< allSum &&i>=0)
            cants[i] = true;
    }

    size_t length = allSum - cant.size();
    std::vector<int> chooseVec;
    chooseVec.reserve(length);

    for (size_t i = 0; i < length; ++i)
    {
        if (!cants[i])
            chooseVec.push_back(i);
    }

    std::random_shuffle(chooseVec.begin(), chooseVec.end());

    std::vector<int> tmp;
    _nulls.swap(tmp);
    _nulls.reserve(num);
    for (size_t i = 0; i < num; i++)
    {
        _nulls.push_back(chooseVec[i]);
    }

    std::sort(_nulls.begin(), _nulls.end());
    return _nulls;
}

std::string QuestionHelper::getRandomHanzi()
{
    return _random[random() % _random.size()].asString();
}
