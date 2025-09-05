#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>
#include <vector>
#include <string>
#include <Geode/modify/SecretLayer5.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <regex>
#include <functional>

using namespace geode::prelude;

struct CodeInfo {
    std::string code;
    std::string description;
    std::string saveKey;
};
class CodesPopup : public geode::Popup<> {
protected:
    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -502, true);
    }
    std::vector<CodeInfo> m_codes = {};

    static constexpr size_t CODES_PER_PAGE = 5;
    size_t m_currentPage = 0;
    CCMenu* m_contentMenu = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_progressLabel = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;

    bool setup() override {
        this->setTitle("Codes");
        this->setTouchPriority(-502);
        
        m_contentMenu = CCMenu::create();
        m_contentMenu->setContentSize({320.f, 180.f});
        m_contentMenu->ignoreAnchorPointForPosition(false);
        m_contentMenu->setAnchorPoint({0.5f, 0.5f});
        m_contentMenu->setPosition({m_size.width / 2, m_size.height / 2 + 20.f}); 
        
        m_contentMenu->setLayout(ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(8.f)
        );
        
        setupPaginationControls();
        setupProgressDisplay();
        loadCurrentPage();
        
        m_mainLayer->addChild(m_contentMenu);
        
        return true;
    }
    
    void setupPaginationControls() {
        auto paginationMenu = CCMenu::create();
        paginationMenu->setContentSize({300.f, 40.f});
        paginationMenu->ignoreAnchorPointForPosition(false);
        paginationMenu->setAnchorPoint({0.5f, 0.5f});
        paginationMenu->setPosition({m_size.width / 2, 40.f});
        
        paginationMenu->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setGap(20.f)
        );
        
        auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        prevSprite->setScale(0.8f);
        prevSprite->setFlipX(false);
        m_prevBtn = CCMenuItemSpriteExtra::create(
            prevSprite,
            this,
            menu_selector(CodesPopup::onPrevPage)
        );
        
        auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        nextSprite->setScale(0.8f);
        nextSprite->setFlipX(true);
        m_nextBtn = CCMenuItemSpriteExtra::create(
            nextSprite,
            this,
            menu_selector(CodesPopup::onNextPage)
        );
        
        m_pageLabel = CCLabelBMFont::create("Page 1/4", "goldFont.fnt");
        m_pageLabel->setScale(0.7f);
        
        paginationMenu->addChild(m_prevBtn);
        paginationMenu->addChild(m_pageLabel);
        paginationMenu->addChild(m_nextBtn);
        
        paginationMenu->updateLayout();
        
        m_mainLayer->addChild(paginationMenu);
        
        updatePaginationControls();
    }
    
    void setupProgressDisplay() {
        m_progressLabel = CCLabelBMFont::create("0/29", "bigFont.fnt");
        m_progressLabel->setScale(0.5f);
        m_progressLabel->setAnchorPoint({1.0f, 1.0f});
        m_progressLabel->setPosition({m_size.width - 20.f, m_size.height - 20.f});
        m_progressLabel->setColor({100, 255, 100});
        
        m_mainLayer->addChild(m_progressLabel);
        
        updateProgressDisplay();
    }
    
    void loadCurrentPage() {
        m_contentMenu->removeAllChildren();
        
        size_t startIndex = m_currentPage * CODES_PER_PAGE;
        size_t endIndex = std::min(startIndex + CODES_PER_PAGE, m_codes.size());
        
        for (size_t i = startIndex; i < endIndex; i++) {
            auto& codeInfo = m_codes[i];
            
            auto entryContainer = CCMenu::create();
            entryContainer->setContentSize({300.f, 30.f});
            entryContainer->ignoreAnchorPointForPosition(false);
            entryContainer->setAnchorPoint({0.5f, 0.5f});
            
            bool isChecked = Mod::get()->getSavedValue<bool>(codeInfo.saveKey, false);
            
            auto checkedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            auto uncheckedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            checkedSprite->setScale(0.8f);
            uncheckedSprite->setScale(0.8f);
            
            auto checkbox = CCMenuItemToggler::create(
                uncheckedSprite,
                checkedSprite,
                this,
                menu_selector(CodesPopup::onCheckboxToggle)
            );
            
            checkbox->setTag(i);
            checkbox->toggle(isChecked ? true : false);
            checkbox->setAnchorPoint({0.5f, 0.5f});
            checkbox->setPosition({25.f, 15.f});
            
            auto codeLabel = CCLabelBMFont::create(codeInfo.code.c_str(), "goldFont.fnt");
            codeLabel->setScale(0.65f);
            codeLabel->setAnchorPoint({0.f, 0.5f});
            
            auto codeButton = CCMenuItemSpriteExtra::create(
                codeLabel,
                this,
                menu_selector(CodesPopup::onCodeClick)
            );
            codeButton->setTag(i);
            codeButton->setAnchorPoint({0.f, 0.5f});
            codeButton->setPosition({50.f, 20.f});
            
            auto descLabel = CCLabelBMFont::create(codeInfo.description.c_str(), "chatFont.fnt");
            descLabel->setScale(0.7f);
            descLabel->setAnchorPoint({0.f, 0.5f});
            descLabel->setPosition({50.f, 8.f});
            descLabel->setColor({180, 180, 180});
            
            entryContainer->addChild(checkbox);
            entryContainer->addChild(codeButton);
            entryContainer->addChild(descLabel);
            
            m_contentMenu->addChild(entryContainer);
        }
        
        m_contentMenu->updateLayout();
        updatePaginationControls();
    }
    
    void updateProgressDisplay() {
        size_t redeemedCount = 0;
        
        for (const auto& code : m_codes) {
            if (Mod::get()->getSavedValue<bool>(code.saveKey, false)) {
                redeemedCount++;
            }
        }
        
        std::string progressText = fmt::format("{}/{}", redeemedCount, m_codes.size());
        m_progressLabel->setString(progressText.c_str());
        
        if (redeemedCount == m_codes.size()) {
            m_progressLabel->setColor({ 255, 255, 255 });
        } else if (redeemedCount > m_codes.size() / 2) {
            m_progressLabel->setColor({100, 255, 100});
        } else {
            m_progressLabel->setColor({ 255, 255, 255 });
        }
    }
    
    void updatePaginationControls() {
        size_t totalPages = (m_codes.size() + CODES_PER_PAGE - 1) / CODES_PER_PAGE;
        
        std::string pageText = fmt::format("Page {}/{}", m_currentPage + 1, totalPages);
        m_pageLabel->setString(pageText.c_str());
        
        m_prevBtn->setEnabled(m_currentPage > 0);
        m_nextBtn->setEnabled(m_currentPage < totalPages - 1);
        
        m_prevBtn->setOpacity(m_currentPage > 0 ? 255 : 100);
        m_nextBtn->setOpacity(m_currentPage < totalPages - 1 ? 255 : 100);
    }
    
    void onPrevPage(CCObject*) {
        if (m_currentPage > 0) {
            m_currentPage--;
            loadCurrentPage();
        }
    }
    
    void onNextPage(CCObject*) {
        size_t totalPages = (m_codes.size() + CODES_PER_PAGE - 1) / CODES_PER_PAGE;
        if (m_currentPage < totalPages - 1) {
            m_currentPage++;
            loadCurrentPage();
        }
    }
    
    void onCheckboxToggle(CCObject* sender) {
        auto toggle = static_cast<CCMenuItemToggler*>(sender);
        int index = toggle->getTag();
        bool isChecked = toggle->isToggled();
        bool originalValue = Mod::get()->getSavedValue<bool>(m_codes[index].saveKey, false);
        
        Mod::get()->setSavedValue(m_codes[index].saveKey, !originalValue);
        updateProgressDisplay();
    }
    
    void onCodeClick(CCObject* sender) {
        auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
        int index = button->getTag();
        std::string code = m_codes[index].code;
        
        this->onClose(nullptr);
        
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (auto layer = scene->getChildByType<SecretLayer5>(0)) {
            if (layer->m_textInput && layer->m_wraithButton) {
                layer->m_textInput->setString(code.c_str());
                layer->m_wraithButton->activate();
                Mod::get()->setSavedValue(m_codes[index].saveKey, true);
                updateProgressDisplay();
            }
        }
        
        utils::clipboard::write(code);
    }

public:
    static CodesPopup* create(std::vector<CodeInfo> externalCodes = {}) {
        auto ret = new CodesPopup();
        if (!externalCodes.empty()) {
            ret->m_codes = std::move(externalCodes);
        }
        if (ret->initAnchored(340.f, 280.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class InfoOKDelegate : public CCNode, public FLAlertLayerProtocol {
public:
    std::function<void()> onOK;
    static InfoOKDelegate* create(std::function<void()> cb) {
        auto p = new InfoOKDelegate();
        if (p && p->init()) {
            p->onOK = std::move(cb);
            p->autorelease();
            return p;
        }
        delete p;
        return nullptr;
    }
    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override {
        if (!btn2 && onOK) onOK();
        this->removeFromParent();
    }
};

class $modify(MySecretLayer5, SecretLayer5) {
    struct Fields {
        CCMenuItemSpriteExtra* m_codesButton = nullptr;
        EventListener<web::WebTask> m_listener;
        std::vector<CodeInfo> m_codes;
        bool m_loaded = false;
    };

    bool init() {
        if (!SecretLayer5::init()) {
            return false;
        }
        bool codesSecret = Mod::get()->getSettingValue<bool>("secret");
        bool soggy = Mod::get()->getSettingValue<bool>("soggy");
        m_fields->m_listener.bind([this] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                auto body = res->string().unwrapOr("");
                try {
                    std::regex item(R"(\{\s*\"([^\"]*)\"\s*,\s*\"([^\"]*)\"\s*,\s*\"([^\"]*)\"\s*\})");
                    std::sregex_iterator it(body.begin(), body.end(), item);
                    std::sregex_iterator end;
                    std::vector<CodeInfo> parsed;
                    for (; it != end; ++it) {
                        CodeInfo ci;
                        ci.code = (*it)[1].str();
                        ci.description = (*it)[2].str();
                        ci.saveKey = (*it)[3].str();
                        if (ci.saveKey.empty() && !ci.code.empty()) ci.saveKey = ci.code + "_checked";
                        if (!ci.code.empty()) parsed.push_back(std::move(ci));
                    }
                    m_fields->m_codes = std::move(parsed);
                    m_fields->m_loaded = true;
                } catch (std::exception const& ex) {
                    m_fields->m_loaded = false;
                }
            } 
        });

    auto req = web::WebRequest();
    if (codesSecret == true) {
        m_fields->m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/secretcodes.json"));
    } else if (soggy) {
        m_fields->m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/soggycodes.json"));
    } else {
        m_fields->m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/codes.json"));
    }
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto buttonSprite = ButtonSprite::create("Codes", "goldFont.fnt", "GJ_button_04.png", 1.0f);
        buttonSprite->setScale(0.8f);

        auto codesButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MySecretLayer5::onCodesButton)
        );

        CCMenu* menu = this->getChildByType<CCMenu>(0);
        if (!menu) return true;

        codesButton->setPosition({winSize.width - codesButton->getContentWidth(), 0.f});
        codesButton->setID("codes-button"_spr);
        
        menu->addChild(codesButton);

        m_fields->m_codesButton = codesButton;
        return true;
    }

    void onCodesButton(CCObject*) {
        if (m_fields->m_codes.empty()) {
            log::error("No codes loaded from remote; not showing popup.");
            return;
        }

    bool isNew = Mod::get()->getSettingValue<bool>("new-p");
    if (isNew) {
            auto delegate = InfoOKDelegate::create([this]() {
                Mod::get()->setSettingValue<bool>("new-p", false);
                CodesPopup::create(m_fields->m_codes)->show();
            });
            this->addChild(delegate);
            auto info = FLAlertLayer::create(delegate, "Info", "You can click the codes to auto input them!", "OK", nullptr);
            info->show();
        } else {
            CodesPopup::create(m_fields->m_codes)->show();
        }
    }
};