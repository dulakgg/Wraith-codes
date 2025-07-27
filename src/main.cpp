#include <Geode/Geode.hpp>

using namespace geode::prelude;

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
        // Create a comprehensive list of Geometry Dash codes
        std::string codeList = "Geometry Dash Vault Codes:\n\n";
        codeList += "GEOMETRY - Basic unlock\n";
        codeList += "DASH - Color unlock\n"; 
        codeList += "WRAITH - Secret reward\n";
        codeList += "HELPER - Bonus items\n";
        codeList += "SECRET - Hidden content\n";
        codeList += "VAULT - Extra vault access\n";
        codeList += "TOWER - Special tower\n";
        codeList += "MYSTERY - Unknown reward\n";
        codeList += "POWER - Boost ability\n";
        codeList += "LEGEND - Legendary unlock\n";
        codeList += "SPOOKY - Halloween content\n";
        codeList += "LENNY - Special icon\n";
        codeList += "BLOCKBITE - Reward\n";
        codeList += "NEVERENDING - Icon\n";
        codeList += "AHEAD - More content\n";
        codeList += "CHALLENGE - Special mode\n";
        
        FLAlertLayer::create("Vault Codes", codeList, "OK")->show();
        log::info("Codes button clicked in vault!");
    }
};

// Mod entry point - only SecretLayer5 hook for vault codes