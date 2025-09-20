#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/modify/SecretLayer5.hpp>
#include <matjson.hpp>


using namespace geode::prelude;

class MyPopup : public geode::Popup<> {
protected:
    EventListener<web::WebTask> m_listener;
    size_t m_currentPage = 0;
    static constexpr size_t kPerPage = 5; 
    CCMenu* m_contentMenu = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_progressLabel = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;
    CCLabelBMFont* m_titleLabel = nullptr;
    CCLabelBMFont* m_codeLabel = nullptr;
    std::map<std::string, std::tuple<std::string, std::string> > codes = {};
    std::vector<std::string> m_keys; 
    bool setup() override {
        
        this->setTitle("Wraith helper");

    m_contentMenu = CCMenu::create();
    m_contentMenu->setContentSize({ m_size.width - 4.f, 140.f });
        m_contentMenu->ignoreAnchorPointForPosition(false);
        m_contentMenu->setAnchorPoint({ 0.5f, 0.5f });
        m_contentMenu->setPosition({ m_size.width / 2, m_size.height / 2 - 10.f });
        m_mainLayer->addChild(m_contentMenu);

        setupPaginationControls();
        downloadCodes();
        updatePageUI();
        return true;
    }
    void updatePageUI() {
        auto totalItems = m_keys.size();
        if (totalItems == 0) {
            if (m_titleLabel) m_titleLabel->setString("Loading...");
            if (m_codeLabel) m_codeLabel->setString("Loading...");
            if (m_pageLabel) m_pageLabel->setString("Page 0/0");
            if (m_contentMenu) m_contentMenu->removeAllChildren();
            if (m_progressLabel) m_progressLabel->setString("0/0");
            return;
        }

        auto pages = totalPages();
        if (m_currentPage >= pages) m_currentPage = pages - 1;
        if (m_contentMenu) m_contentMenu->removeAllChildren();
        size_t start = m_currentPage * kPerPage;
        size_t end = std::min(start + kPerPage, totalItems);

    float containerW = m_contentMenu ? m_contentMenu->getContentSize().width : (m_size.width - 4.f);
        float yTop = (m_contentMenu ? m_contentMenu->getContentSize().height : 140.f) + 20.f;
        float lineH = 28.f;

        for (size_t i = start; i < end; ++i) {
            auto const& key = m_keys[i];
            auto const& tup = codes.at(key);
            auto const& reward = std::get<0>(tup);
            auto const& code   = std::get<1>(tup);
            bool claimed = Mod::get()->getSavedValue<bool>(key, false);

            float y = yTop - (static_cast<float>(i - start) * lineH);

            auto inputSprite = ButtonSprite::create(claimed ? "Used" : "Input", "goldFont.fnt", "GJ_button_01.png", 0.6f);
            float inputW = inputSprite->getContentSize().width * inputSprite->getScaleX();
            float leftX  = -containerW / 2.f + 180.f;
            float rightX =  containerW / 2.f - inputW / 2.f + 150.f;

            std::string line = code;
            auto* codeLabel = CCLabelBMFont::create(line.c_str(), "bigFont.fnt");
            codeLabel->setAnchorPoint({0.f, 0.5f});
            float baseScale = 0.48f;

            float baseW = codeLabel->getContentSize().width;
            float maxTextW = (rightX - leftX) - (50.f + inputW);
            if (baseW * baseScale > maxTextW && baseW > 0.f)
                baseScale = std::max(0.30f, maxTextW / baseW);

            codeLabel->setScale(baseScale);
            codeLabel->setColor(claimed ? ccColor3B{80,255,120} : ccColor3B{255,255,255});
            codeLabel->setPosition({leftX, y});
            m_contentMenu->addChild(codeLabel);

            auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            infoSprite->setScale(0.7f);
            auto infoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(MyPopup::onInfo));
            infoBtn->setTag(static_cast<int>(i));
            float infoX = leftX + codeLabel->getScaledContentSize().width + 8.f;
            infoBtn->setPosition({infoX, y});
            m_contentMenu->addChild(infoBtn);

            auto inputBtn = CCMenuItemSpriteExtra::create(inputSprite, this, menu_selector(MyPopup::onApplyCode));
            inputBtn->setTag(static_cast<int>(i));
            inputBtn->setPosition({rightX, y});
            if (claimed) {
                inputBtn->setEnabled(false);
                inputBtn->setOpacity(160);
                inputSprite->setColor(ccColor3B{150,255,170});
            }
            m_contentMenu->addChild(inputBtn);
        }
        auto const& firstKey = m_keys[start];
        auto const& firstTup = codes.at(firstKey);
        if (m_titleLabel) m_titleLabel->setString(std::get<0>(firstTup).c_str());
        if (m_codeLabel)  m_codeLabel->setString(std::get<1>(firstTup).c_str());

        if (m_pageLabel) {
            std::string s = "Page " + std::to_string(m_currentPage + 1) + "/" + std::to_string(pages);
            m_pageLabel->setString(s.c_str());
        }
        if (m_prevBtn) m_prevBtn->setEnabled(m_currentPage > 0);
        if (m_nextBtn) m_nextBtn->setEnabled(m_currentPage + 1 < pages);

        updateProgressDisplay();
    }
	void rebuildIndex() {
        m_keys.clear();
        m_keys.reserve(codes.size());
        for (auto const& [k, _] : codes) m_keys.push_back(k);
        std::sort(m_keys.begin(), m_keys.end(),
            [&](std::string const& a, std::string const& b) {
                auto const& ta = codes[a];
                auto const& tb = codes[b];
                auto const& codeA = std::get<1>(ta);
                auto const& codeB = std::get<1>(tb);
                if (codeA != codeB) return codeA < codeB; 
                return a < b;
            });
    }
    size_t totalPages() const {
        return (m_keys.size() + kPerPage - 1) / kPerPage;
    }
    void onInfo(CCObject* sender) {
        auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
        int index = button ? button->getTag() : -1;
        if (index < 0 || static_cast<size_t>(index) >= m_keys.size()) return;
        auto const& key = m_keys[static_cast<size_t>(index)];
        auto const& tup = codes.at(key);
        auto const& reward = std::get<0>(tup);
        FLAlertLayer::create("Code Info", reward.c_str(), "OK")->show();
    }
	void downloadCodes() {
        m_listener.bind([this](web::WebTask::Event* e) {
            if (auto* res = e->getValue()) {
                auto body = res->string().unwrapOr("");
                auto parsed = matjson::parse(body);
                if (!parsed) {
                    log::error("JSON failed (interesting reset your dns pls)): {}", parsed.unwrapErr());
                    return;
                }
                auto json = parsed.unwrap();
                codes.clear();
                if (json.isObject()) {
                    for (auto const& [key, val] : json) {
                        auto arrRes = val.as<std::vector<matjson::Value>>();
                        if (!arrRes) continue;
                        auto const& arr = arrRes.unwrap();
                        std::string code  = arr.size() > 0 ? arr[0].asString().unwrapOr("") : "";
                        std::string reward = arr.size() > 1 ? arr[1].asString().unwrapOr("") : "";
                        codes[key] = std::make_tuple(reward, code);
                    }
                }
                rebuildIndex();
                m_currentPage = 0;
                updatePageUI();
                updateProgressDisplay();
            }
        });

        auto req = web::WebRequest();
        bool codesSecret = Mod::get()->getSettingValue<bool>("secret");
        bool soggy = Mod::get()->getSettingValue<bool>("soggy");
        if (codesSecret) {
            m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/secretcodes.json"));
        } else if (soggy) {
            m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/soggycodes.json"));
        } else {
            m_listener.setFilter(req.get("https://raw.githubusercontent.com/dulakgg/codes/main/codes.json"));
        }
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
            menu_selector(MyPopup::onPrevButton)
        );
        
        auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        nextSprite->setScale(0.8f);
        nextSprite->setFlipX(true);
        m_nextBtn = CCMenuItemSpriteExtra::create(
            nextSprite,
            this,
            menu_selector(MyPopup::onNextButton)
        );
        
        paginationMenu->addChild(m_prevBtn);
        paginationMenu->addChild(m_nextBtn);

        m_pageLabel = CCLabelBMFont::create("Page 0/0", "bigFont.fnt");
        m_pageLabel->setScale(0.35f);
        paginationMenu->addChild(m_pageLabel);

        m_progressLabel = CCLabelBMFont::create("0/0", "goldFont.fnt");
        m_progressLabel->setScale(0.35f);
        paginationMenu->addChild(m_progressLabel);

        paginationMenu->updateLayout();
        m_mainLayer->addChild(paginationMenu);
    }
	void onNextButton(CCObject*) {
        auto pages = totalPages();
        if (pages == 0) return;
        if (m_currentPage + 1 < pages) {
            ++m_currentPage;
            updatePageUI();
        }
    }
    void onPrevButton(CCObject*) {
        auto pages = totalPages();
        if (pages == 0) return;
        if (m_currentPage > 0) {
            --m_currentPage;
            updatePageUI();
        }
    }
    void onApplyCode(CCObject* sender) {
        auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
        int index = button ? button->getTag() : -1;
        if (index < 0 || static_cast<size_t>(index) >= m_keys.size()) return;

        auto const& key = m_keys[static_cast<size_t>(index)];
        auto const& tup = codes.at(key);
        std::string code = std::get<1>(tup);

        Mod::get()->setSavedValue(key, true);
        updateProgressDisplay();
        updatePageUI();

        if (auto* scene = CCDirector::sharedDirector()->getRunningScene()) {
            if (auto* layer = scene->getChildByType<SecretLayer5>(0)) {
                if (layer->m_textInput && layer->m_wraithButton) {
                    layer->m_textInput->setString(code.c_str());
                    layer->m_wraithButton->activate();
                }
            }
        }

        utils::clipboard::write(code);

        this->onClose(nullptr);
    }
    void updateProgressDisplay() {
        if (!m_progressLabel) return;
        size_t total = m_keys.size();
        size_t claimed = 0;
        for (auto const& k : m_keys) {
            if (Mod::get()->getSavedValue<bool>(k, false)) ++claimed;
        }
        std::string s = std::to_string(claimed) + "/" + std::to_string(total);
        m_progressLabel->setString(s.c_str());
    }
public:
    static MyPopup* create() {
        auto ret = new MyPopup();
        if (ret->initAnchored(320.f, 260.f, "GJ_square07.png")) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class $modify(MySecretLayer5, SecretLayer5) {
	bool init() {
		if (!SecretLayer5::init()) {
            return false;
        }
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto buttonSprite = ButtonSprite::create("Wraith", "goldFont.fnt", "GJ_button_04.png", 1.0f);
        buttonSprite->setScale(0.8f);

        auto wraithButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MySecretLayer5::onMyButton)
        );
		CCMenu* menu = this->getChildByType<CCMenu>(0);
		wraithButton->setPosition({winSize.width - wraithButton->getContentWidth(), 0.f});
		menu->addChild(wraithButton);
		wraithButton->setID("wraithButton"_spr);
		menu->updateLayout();
		return true;
	}
	void onMyButton(CCObject*) {
		MyPopup::create()->show();
	}
};