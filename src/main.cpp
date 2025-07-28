#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CodeInfo {
    std::string code;
    std::string description;
    std::string saveKey;
};

class CodesPopup : public geode::Popup<> {
protected:
    std::vector<CodeInfo> m_codes = {
        {"putyahandsup", "UFO 71", "putyahandsup_checked"},
        {"thickofit", "1 Mana Orb", "thickofit_checked"},
        {"skibidi", "1 Mana Orb", "skibidi_checked"},
        {"fireinthehole", "1 Fire Shard", "fireinthehole_checked"},
        {"robtopisnice", "10 Mana Orbs", "robtopisnice_checked"},
        {"bussin", "69 Mana Orbs", "bussin_checked"},
        {"wellmet", "5 Diamonds", "wellmet_checked"},
        {"checksteam", "1 Demon Key", "checksteam_checked"},
        {"touchgrass", "1 Earth Shard", "touchgrass_checked"},
        {"wateronthehill", "1 Ice Shard", "wateronthehill_checked"},
        {"backondash", "Cube 409", "backondash_checked"},
        {"key", "1 Demon Key", "key_checked"},
        {"geometry", "Ball 90", "geometry_checked"},
        {"citadel", "Cube 231", "citadel_checked"},
        {"retrospective", "Ship 121", "retrospective_checked"},
        {"iaminpain", "10 Diamonds, 100 Mana Orbs", "iaminpain_checked"},
        {"ruins", "Ball 91", "ruins_checked"},
        {"spacegauntlet", "20 Diamonds, 1 Demon Key, 2 Metal Shards", "spacegauntlet_checked"},
        {"cheatcodes", "UFO 138", "cheatcodes_checked"},
        {"backstreetboy", "Robot 37", "backstreetboy_checked"},
        {"noelelectra", "Swing 17", "noelelectra_checked"},
        {"gd2025", "1 Yellow Key, 25 Diamonds, 25 Mana Orbs", "gd2025_checked"},
        {"duckstep", "Ship 146", "duckstep_checked"},
        {"skylinept2", "Cube 309", "skylinept2_checked"},
        {"boogie", "Wave 92", "boogie_checked"},
        {"buttonmasher", "UFO 63", "buttonmasher_checked"},
        {"gdawards", "Cube 485", "gdawards_checked"},
        {"ncsalbum", "Jetpack 8", "ncsalbum_checked"},
        {"gullible", "1 Mana Orb", "gullible_checked"}
    };

    static constexpr size_t CODES_PER_PAGE = 5;
    size_t m_currentPage = 0;
    CCMenu* m_contentMenu = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_progressLabel = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;

    bool setup() override {
        this->setTitle("Wraith Helper");
        
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
        
        log::info("Pagination controls created with layout - Prev: {}, Next: {}, Label: {}", 
                  m_prevBtn ? "OK" : "FAIL", 
                  m_nextBtn ? "OK" : "FAIL", 
                  m_pageLabel ? "OK" : "FAIL");
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
            checkbox->setEnabled(false);
            
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
            m_progressLabel->setColor({255, 215, 0});
        } else if (redeemedCount > m_codes.size() / 2) {
            m_progressLabel->setColor({100, 255, 100});
        } else {
            m_progressLabel->setColor({255, 255, 100});
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
        log::info("Previous page button clicked - Current page: {}", m_currentPage);
        if (m_currentPage > 0) {
            m_currentPage--;
            loadCurrentPage();
            log::info("Moved to page: {}", m_currentPage);
        }
    }
    
    void onNextPage(CCObject*) {
        size_t totalPages = (m_codes.size() + CODES_PER_PAGE - 1) / CODES_PER_PAGE;
        log::info("Next page button clicked - Current page: {}, Total pages: {}", m_currentPage, totalPages);
        if (m_currentPage < totalPages - 1) {
            m_currentPage++;
            loadCurrentPage();
            log::info("Moved to page: {}", m_currentPage);
        }
    }
    
    void onCheckboxToggle(CCObject* sender) {
        auto toggle = static_cast<CCMenuItemToggler*>(sender);
        int index = toggle->getTag();
        bool isChecked = toggle->isToggled();
        
        Mod::get()->setSavedValue(m_codes[index].saveKey, isChecked);
        updateProgressDisplay();
        
        log::info("Code {} {} {}", 
            m_codes[index].code, 
            isChecked ? "checked" : "unchecked",
            m_codes[index].saveKey
        );
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
                log::info("Auto-inputted code: {}", code);
                layer->m_wraithButton->activate();
                Mod::get()->setSavedValue(m_codes[index].saveKey, true);
                updateProgressDisplay();
            }
        }
        
        utils::clipboard::write(code);
    }

public:
    static CodesPopup* create() {
        auto ret = new CodesPopup();
        if (ret->initAnchored(340.f, 280.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

#include <Geode/modify/SecretLayer5.hpp>
class $modify(MySecretLayer5, SecretLayer5) {
    struct Fields {
        CCMenuItemSpriteExtra* m_codesButton = nullptr;
    };

    bool init() {
        if (!SecretLayer5::init()) {
            return false;
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

        codesButton->setPosition({winSize.width - (codesButton.getContentWidth() / 2.f), 0.f});
        codesButton->setID("codes-button"_spr);
        
        menu->addChild(codesButton);

        m_fields->m_codesButton = codesButton;

        log::info("Codes button added to top right corner at position ({}, {})", winSize.width - 60.f, winSize.height - 60.f);
        log::info("Wraith Helper button successfully added to SecretLayer5");

        return true;
    }

    void onCodesButton(CCObject*) {
        CodesPopup::create()->show();
    }
};