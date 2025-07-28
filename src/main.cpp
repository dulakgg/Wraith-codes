#include <Geode/Geode.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

// Structure to hold code information
struct CodeInfo {
    std::string code;
    std::string description;
    std::string saveKey; // Key used for saving checkbox state
};

// Custom popup for displaying codes with checkboxes
class CodesPopup : public geode::Popup<> {
protected:
    // List of all Geometry Dash codes
    std::vector<CodeInfo> m_codes = {
        {"GEOMETRY", "Basic unlock", "checkbox_geometry"},
        {"DASH", "Color unlock", "checkbox_dash"},
        {"WRAITH", "Secret reward", "checkbox_wraith"},
        {"HELPER", "Bonus items", "checkbox_helper"},
        {"SECRET", "Hidden content", "checkbox_secret"},
        {"VAULT", "Extra vault access", "checkbox_vault"},
        {"TOWER", "Special tower", "checkbox_tower"},
        {"MYSTERY", "Unknown reward", "checkbox_mystery"},
        {"POWER", "Boost ability", "checkbox_power"},
        {"LEGEND", "Legendary unlock", "checkbox_legend"},
        {"SPOOKY", "Halloween content", "checkbox_spooky"},
        {"LENNY", "Special icon", "checkbox_lenny"},
        {"BLOCKBITE", "Reward", "checkbox_blockbite"},
        {"NEVERENDING", "Icon", "checkbox_neverending"},
        {"AHEAD", "More content", "checkbox_ahead"},
        {"CHALLENGE", "Special mode", "checkbox_challenge"}
    };

    ScrollLayer* m_scrollLayer = nullptr;
    CCMenu* m_contentMenu = nullptr;

    bool setup() override {
        this->setTitle("Vault Codes Tracker");
        
        // Create scroll layer for the codes - even smaller height
        auto contentSize = CCSize{320.f, 180.f}; // Reduced height further
        m_scrollLayer = ScrollLayer::create(contentSize);
        m_scrollLayer->setPosition({
            (m_size.width - contentSize.width) / 2,
            (m_size.height - contentSize.height) / 2 + 5.f
        });
        
        // Create content menu inside scroll layer
        m_contentMenu = CCMenu::create();
        m_contentMenu->setPosition({0, 0});
        m_contentMenu->setContentSize(contentSize);
        
        // Add code entries with checkboxes
        float totalHeight = m_codes.size() * 32.f + 20.f; // Optimized spacing
        float yPos = totalHeight - 20.f; // Start from actual top of content
        for (size_t i = 0; i < m_codes.size(); i++) {
            auto& codeInfo = m_codes[i];
            
            // Get saved checkbox state (default false)
            bool isChecked = Mod::get()->getSavedValue<bool>(codeInfo.saveKey, false);
            
            // Create checkbox toggle button
            auto checkedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            auto uncheckedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            checkedSprite->setScale(0.85f); // Optimized size
            uncheckedSprite->setScale(0.85f);
            
            // Create menu items from sprites
            auto uncheckedItem = CCMenuItemSprite::create(uncheckedSprite, uncheckedSprite, nullptr);
            auto checkedItem = CCMenuItemSprite::create(checkedSprite, checkedSprite, nullptr);
            
            auto checkbox = CCMenuItemToggle::createWithTarget(
                this,
                menu_selector(CodesPopup::onCheckboxToggle),
                uncheckedItem,
                checkedItem,
                nullptr
            );
            
            // Set initial state
            checkbox->setSelectedIndex(isChecked ? 1 : 0);
            checkbox->setTag(i); // Use index to identify which code this is
            checkbox->setPosition({30.f, yPos}); // Better positioning
            
            // Create code label
            auto codeLabel = CCLabelBMFont::create(codeInfo.code.c_str(), "goldFont.fnt");
            codeLabel->setScale(0.75f);
            codeLabel->setPosition({75.f, yPos + 5.f});
            codeLabel->setAnchorPoint({0, 0.5f});
            
            // Create description label
            auto descLabel = CCLabelBMFont::create(codeInfo.description.c_str(), "chatFont.fnt");
            descLabel->setScale(0.85f);
            descLabel->setPosition({75.f, yPos - 7.f});
            descLabel->setAnchorPoint({0, 0.5f});
            descLabel->setColor({180, 180, 180});
            
            // Add to menu
            m_contentMenu->addChild(checkbox);
            m_contentMenu->addChild(codeLabel);
            m_contentMenu->addChild(descLabel);
            
            yPos -= 32.f; // Optimized spacing
        }
        
        // Set content size for scrolling with proper bounds
        m_contentMenu->setContentSize({contentSize.width, totalHeight});
        m_scrollLayer->m_contentLayer->addChild(m_contentMenu);
        m_scrollLayer->m_contentLayer->setContentSize({contentSize.width, totalHeight});
        
        // Position content menu to show first items at top and enable proper scrolling
        if (totalHeight > contentSize.height) {
            // Content is larger than visible area - position to show top items first
            m_contentMenu->setPosition({0, contentSize.height - totalHeight});
        } else {
            // Content fits entirely - position normally
            m_contentMenu->setPosition({0, 0});
        }
        
        m_mainLayer->addChild(m_scrollLayer);
        
        // Add "Clear All" and "Check All" buttons with better styling
        auto clearAllBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Clear All", "goldFont.fnt", "GJ_button_02.png", 0.65f), // Red button for clear action
            this,
            menu_selector(CodesPopup::onClearAll)
        );
        clearAllBtn->setPosition({-65.f, -110.f}); // Better positioning
        
        auto checkAllBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Check All", "goldFont.fnt", "GJ_button_01.png", 0.65f), // Green button for check action
            this,
            menu_selector(CodesPopup::onCheckAll)
        );
        checkAllBtn->setPosition({65.f, -110.f}); // Better positioning
        
        m_buttonMenu->addChild(clearAllBtn);
        m_buttonMenu->addChild(checkAllBtn);
        
        return true;
    }
    
    void onCheckboxToggle(CCObject* sender) {
        auto toggle = static_cast<CCMenuItemToggle*>(sender);
        int index = toggle->getTag();
        bool isChecked = toggle->getSelectedIndex() == 1; // 1 = checked, 0 = unchecked
        
        // Save the state
        Mod::get()->setSavedValue(m_codes[index].saveKey, isChecked);
        
        log::info("Code {} {} {}", 
            m_codes[index].code, 
            isChecked ? "checked" : "unchecked",
            m_codes[index].saveKey
        );
    }
    
    void onClearAll(CCObject*) {
        // Uncheck all checkboxes and save states
        auto children = m_contentMenu->getChildren();
        for (int i = 0; i < children->count(); i++) {
            auto child = dynamic_cast<CCMenuItemToggle*>(children->objectAtIndex(i));
            if (child) {
                child->setSelectedIndex(0); // Set to unchecked
                int index = child->getTag();
                Mod::get()->setSavedValue(m_codes[index].saveKey, false);
            }
        }
        log::info("All codes cleared");
    }
    
    void onCheckAll(CCObject*) {
        // Check all checkboxes and save states
        auto children = m_contentMenu->getChildren();
        for (int i = 0; i < children->count(); i++) {
            auto child = dynamic_cast<CCMenuItemToggle*>(children->objectAtIndex(i));
            if (child) {
                child->setSelectedIndex(1); // Set to checked
                int index = child->getTag();
                Mod::get()->setSavedValue(m_codes[index].saveKey, true);
            }
        }
        log::info("All codes checked");
    }

public:
    static CodesPopup* create() {
        auto ret = new CodesPopup();
        if (ret->initAnchored(360.f, 280.f)) { // Optimized popup size
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

// Hook into SecretLayer5 (The main vault where Wraith is located)
#include <Geode/modify/SecretLayer5.hpp>
class $modify(MySecretLayer5, SecretLayer5) {
    struct Fields {
        CCMenuItemSpriteExtra* m_codesButton = nullptr;
    };

    bool init() {
        if (!SecretLayer5::init()) {
            return false;
        }

        log::info("SecretLayer5 initialized, adding Codes button!");

        // Get screen size for proper positioning
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        log::info("SecretLayer5 screen size: {}, {}", winSize.width, winSize.height);

        // Create the "Codes" button using BetterInfo's patterns
        auto codesButton = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("CODES", "goldFont.fnt", "GJ_button_04.png", 1.0f),
            this,
            menu_selector(MySecretLayer5::onCodesButton)
        );

        // Find an existing menu or create one following BetterInfo pattern
        CCMenu* menu = nullptr;
        
        // Try to find an existing menu by ID (BetterInfo style)
        if (auto existingMenu = this->getChildByID("main-menu")) {
            menu = static_cast<CCMenu*>(existingMenu);
            log::info("Found existing menu with ID 'main-menu'");
        } else if (auto existingMenu = this->getChildByID("menu")) {
            menu = static_cast<CCMenu*>(existingMenu);
            log::info("Found existing menu with ID 'menu'");
        } else {
            // Check for any CCMenu in children
            auto children = this->getChildren();
            for (int i = 0; i < children->count(); i++) {
                auto child = dynamic_cast<CCMenu*>(children->objectAtIndex(i));
                if (child) {
                    menu = child;
                    log::info("Found existing CCMenu at index {}", i);
                    break;
                }
            }
        }
        
        // If still no menu found, create one (BetterInfo style)
        if (!menu) {
            menu = CCMenu::create();
            menu->setPosition({0, 0});
            menu->setID("codes-menu"_spr);
            this->addChild(menu);
            log::info("Created new menu with ID 'codes-menu'");
        }

        // Position button in a clearly visible location in the center-right area
        // Using more conservative positioning to ensure it's visible
        codesButton->setPosition({winSize.width / 2 + 130.f, winSize.height / 2 - 170.f});
        codesButton->setID("codes-button"_spr);
        codesButton->setScale(1.0f); // Make it more visible
        codesButton->setZOrder(100); // Higher Z-order to ensure it's on top
        
        menu->addChild(codesButton);
        
        // Update layout if menu has it (BetterInfo style)
        if (menu->getChildrenCount() > 1) {
            menu->updateLayout();
        }

        // Store reference in fields
        m_fields->m_codesButton = codesButton;

        log::info("Codes button added at position ({}, {}) with scale 1.0", winSize.width / 2 + 100.f, winSize.height / 2 - 100.f);
        log::info("Button successfully added to SecretLayer5");

        return true;
    }

    void onCodesButton(CCObject*) {
        // Create and show the codes popup with checkboxes
        CodesPopup::create()->show();
        log::info("Codes button clicked in vault!");
    }
};

// Mod entry point - only SecretLayer5 hook for vault codes