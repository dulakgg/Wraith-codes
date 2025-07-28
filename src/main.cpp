#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Structure to hold code information
struct CodeInfo {
    std::string code;
    std::string description;
    std::string saveKey; // Key used for saving checkbox state
};

// Custom popup for displaying codes with checkboxes and pagination
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

    static constexpr size_t CODES_PER_PAGE = 5;
    size_t m_currentPage = 0;
    CCMenu* m_contentMenu = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;

    bool setup() override {
        this->setTitle("Vault Codes Tracker");
        
        // Create content menu with proper layout
        m_contentMenu = CCMenu::create();
        m_contentMenu->setContentSize({280.f, 150.f}); // Much smaller for compact popup
        m_contentMenu->ignoreAnchorPointForPosition(false); // Use proper anchor points
        m_contentMenu->setAnchorPoint({0.5f, 0.5f}); // Center anchor
        // Position relative to popup center
        m_contentMenu->setPosition({m_size.width / 2, m_size.height / 2 + 5.f}); 
        
        // Set up column layout for content
        m_contentMenu->setLayout(ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(2.f) // Smaller gap for compact layout
        );
        
        // Create pagination controls
        setupPaginationControls();
        
        // Load the first page
        loadCurrentPage();
        
        m_mainLayer->addChild(m_contentMenu);
        
        // Add "Clear All" and "Check All" buttons using proper layout
        auto actionMenu = CCMenu::create();
        actionMenu->setContentSize({200.f, 40.f});
        actionMenu->ignoreAnchorPointForPosition(false);
        actionMenu->setAnchorPoint({0.5f, 0.5f});
        actionMenu->setPosition({m_size.width / 2, 15.f}); // Bottom of popup
        
        // Set up row layout for action buttons
        actionMenu->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setGap(30.f)
        );
        
        auto clearAllBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Clear All", "goldFont.fnt", "GJ_button_02.png", 0.65f),
            this,
            menu_selector(CodesPopup::onClearAll)
        );
        
        auto checkAllBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Check All", "goldFont.fnt", "GJ_button_01.png", 0.65f),
            this,
            menu_selector(CodesPopup::onCheckAll)
        );
        
        actionMenu->addChild(clearAllBtn);
        actionMenu->addChild(checkAllBtn);
        actionMenu->updateLayout();
        
        m_mainLayer->addChild(actionMenu);

        return true;
    }
    
    void setupPaginationControls() {
        // Create a separate menu for pagination controls
        auto paginationMenu = CCMenu::create();
        paginationMenu->setContentSize({300.f, 40.f});
        paginationMenu->ignoreAnchorPointForPosition(false);
        paginationMenu->setAnchorPoint({0.5f, 0.5f});
        // Position at bottom of popup
        paginationMenu->setPosition({m_size.width / 2, 40.f});
        
        // Set up row layout for pagination
        paginationMenu->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setGap(20.f)
        );
        
        // Create Previous button (left arrow)
        auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        prevSprite->setScale(0.8f);
        prevSprite->setFlipX(false); // Don't flip for left
        m_prevBtn = CCMenuItemSpriteExtra::create(
            prevSprite,
            this,
            menu_selector(CodesPopup::onPrevPage)
        );
        
        // Create Next button (right arrow)
        auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        nextSprite->setScale(0.8f);
        nextSprite->setFlipX(true); // Flip for right
        m_nextBtn = CCMenuItemSpriteExtra::create(
            nextSprite,
            this,
            menu_selector(CodesPopup::onNextPage)
        );
        
        // Create page label
        m_pageLabel = CCLabelBMFont::create("Page 1/4", "goldFont.fnt");
        m_pageLabel->setScale(0.7f);
        
        // Add to pagination menu
        paginationMenu->addChild(m_prevBtn);
        paginationMenu->addChild(m_pageLabel);
        paginationMenu->addChild(m_nextBtn);
        
        // Update layout
        paginationMenu->updateLayout();
        
        // Add pagination menu to main layer
        m_mainLayer->addChild(paginationMenu);
        
        // Update button states
        updatePaginationControls();
        
        log::info("Pagination controls created with layout - Prev: {}, Next: {}, Label: {}", 
                  m_prevBtn ? "OK" : "FAIL", 
                  m_nextBtn ? "OK" : "FAIL", 
                  m_pageLabel ? "OK" : "FAIL");
    }
    
    void loadCurrentPage() {
        // Clear current content
        m_contentMenu->removeAllChildren();
        
        // Calculate page bounds
        size_t startIndex = m_currentPage * CODES_PER_PAGE;
        size_t endIndex = std::min(startIndex + CODES_PER_PAGE, m_codes.size());
        
        // Add code entries for current page
        for (size_t i = startIndex; i < endIndex; i++) {
            auto& codeInfo = m_codes[i];
            
            // Create a container for each code entry
            auto entryContainer = CCMenu::create();
            entryContainer->setContentSize({260.f, 25.f}); // Even smaller for very compact popup
            entryContainer->ignoreAnchorPointForPosition(false);
            entryContainer->setAnchorPoint({0.5f, 0.5f});
            
            // Set up row layout for the entry
            entryContainer->setLayout(RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
                ->setGap(10.f)
            );
            
            // Get saved checkbox state
            bool isChecked = Mod::get()->getSavedValue<bool>(codeInfo.saveKey, false);
            
            // Create checkbox toggle button
            auto checkedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            auto uncheckedSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            checkedSprite->setScale(0.85f);
            uncheckedSprite->setScale(0.85f);
            
            auto uncheckedItem = CCMenuItemSprite::create(uncheckedSprite, uncheckedSprite, nullptr);
            auto checkedItem = CCMenuItemSprite::create(checkedSprite, checkedSprite, nullptr);
            
            auto checkbox = CCMenuItemToggle::createWithTarget(
                this,
                menu_selector(CodesPopup::onCheckboxToggle),
                uncheckedItem,
                checkedItem,
                nullptr
            );
            
            checkbox->setSelectedIndex(isChecked ? 1 : 0);
            checkbox->setTag(i); // Use original index
            
            // Create text container for code and description
            auto textContainer = CCNode::create();
            textContainer->setContentSize({200.f, 25.f}); // Smaller for compact layout
            textContainer->setAnchorPoint({0.f, 0.5f});
            
            // Create code label
            auto codeLabel = CCLabelBMFont::create(codeInfo.code.c_str(), "goldFont.fnt");
            codeLabel->setScale(0.7f); // Slightly smaller
            codeLabel->setAnchorPoint({0, 0.5f});
            codeLabel->setPosition({0.f, 15.f}); // Adjusted for smaller container
            
            // Create description label
            auto descLabel = CCLabelBMFont::create(codeInfo.description.c_str(), "chatFont.fnt");
            descLabel->setScale(0.8f); // Slightly smaller
            descLabel->setAnchorPoint({0, 0.5f});
            descLabel->setPosition({0.f, 3.f}); // Adjusted for smaller container
            descLabel->setColor({180, 180, 180});
            
            // Add labels to text container
            textContainer->addChild(codeLabel);
            textContainer->addChild(descLabel);
            
            // Add checkbox and text to entry container
            entryContainer->addChild(checkbox);
            entryContainer->addChild(textContainer);
            
            // Update entry layout
            entryContainer->updateLayout();
            
            // Add entry to main content menu
            m_contentMenu->addChild(entryContainer);
        }
        
        // Update main content layout
        m_contentMenu->updateLayout();
        
        // Update pagination controls
        updatePaginationControls();
    }
    
    void updatePaginationControls() {
        size_t totalPages = (m_codes.size() + CODES_PER_PAGE - 1) / CODES_PER_PAGE;
        
        // Update page label
        std::string pageText = fmt::format("Page {}/{}", m_currentPage + 1, totalPages);
        m_pageLabel->setString(pageText.c_str());
        
        // Update button states
        m_prevBtn->setEnabled(m_currentPage > 0);
        m_nextBtn->setEnabled(m_currentPage < totalPages - 1);
        
        // Update button opacity for visual feedback
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
        // Clear all codes (not just current page)
        for (size_t i = 0; i < m_codes.size(); i++) {
            Mod::get()->setSavedValue(m_codes[i].saveKey, false);
        }
        // Reload current page to reflect changes
        loadCurrentPage();
        log::info("All codes cleared");
    }
    
    void onCheckAll(CCObject*) {
        // Check all codes (not just current page)
        for (size_t i = 0; i < m_codes.size(); i++) {
            Mod::get()->setSavedValue(m_codes[i].saveKey, true);
        }
        // Reload current page to reflect changes
        loadCurrentPage();
        log::info("All codes checked");
    }

public:
    static CodesPopup* create() {
        auto ret = new CodesPopup();
        if (ret->initAnchored(340.f, 280.f)) { // Much smaller and more compact
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