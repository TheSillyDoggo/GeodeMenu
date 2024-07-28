#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Event.hpp>
#include <cocos2d.h>
#include <unordered_map>
#include <unordered_set>

#ifdef GEODE_IS_WINDOWS
    #ifdef HJFOD_CUSTOM_KEYBINDS_EXPORTING
        #define CUSTOM_KEYBINDS_DLL __declspec(dllexport)
    #else
        #define CUSTOM_KEYBINDS_DLL __declspec(dllimport)
    #endif
#else
    #define CUSTOM_KEYBINDS_DLL
#endif

struct BindSaveData;

namespace keybinds {
    class Bind;
    class BindManager;

    using DeviceID = std::string;

    /**
     * Base class for implementing bindings for different input devices
     */
    class CUSTOM_KEYBINDS_DLL Bind : public cocos2d::CCObject {
    protected:
        friend class BindManager;

    public:
        /**
         * Get the hash for this bind
         */
        virtual size_t getHash() const = 0;
        /**
         * Check if this bind is equal to another. By default compares hashes
         */
        virtual bool isEqual(Bind* other) const;
        /**
         * Get the bind's representation as a human-readable string
         */
        virtual std::string toString() const = 0;
        virtual cocos2d::CCNode* createLabel() const;
        virtual DeviceID getDeviceID() const = 0;
        virtual matjson::Value save() const = 0;

        cocos2d::CCNodeRGBA* createBindSprite() const;
    };

    enum class Modifier : unsigned int {
        None        = 0b0000,
        Control     = 0b0001,
        Shift       = 0b0010,
        Alt         = 0b0100,
        Command     = 0b1000,
    #ifdef GEODE_IS_MACOS
        PlatformControl = Command,
    #else
        PlatformControl = Control,
    #endif
    };
    CUSTOM_KEYBINDS_DLL Modifier operator|(Modifier const& a, Modifier const& b);
    CUSTOM_KEYBINDS_DLL Modifier operator|=(Modifier& a, Modifier const& b);
    CUSTOM_KEYBINDS_DLL bool operator&(Modifier const& a, Modifier const& b);

    CUSTOM_KEYBINDS_DLL std::string keyToString(cocos2d::enumKeyCodes key);
    CUSTOM_KEYBINDS_DLL bool keyIsModifier(cocos2d::enumKeyCodes key);
    CUSTOM_KEYBINDS_DLL bool keyIsController(cocos2d::enumKeyCodes key);

    enum class MouseButton {
        Button4 = 0,
        Button5 = 1,
    };

    CUSTOM_KEYBINDS_DLL std::string mouseToString(MouseButton button);

    class CUSTOM_KEYBINDS_DLL Keybind final : public Bind {
    protected:
        cocos2d::enumKeyCodes m_key;
        Modifier m_modifiers;

    public:
        static Keybind* create(cocos2d::enumKeyCodes key, Modifier modifiers = Modifier::None) { return nullptr; }
        static Keybind* parse(matjson::Value const&);

        cocos2d::enumKeyCodes getKey() const;
        Modifier getModifiers() const;

        size_t getHash() const override;
        bool isEqual(Bind* other) const override;
        std::string toString() const override;
        DeviceID getDeviceID() const override;
        matjson::Value save() const override;
    };

    class CUSTOM_KEYBINDS_DLL MouseBind final : public Bind {
    protected:
        MouseButton m_button;
        Modifier m_modifiers;

    public:
        static MouseBind* create(MouseButton button, Modifier modifiers = Modifier::None);
        static MouseBind* parse(matjson::Value const&);

        MouseButton getButton() const;
        Modifier getModifiers() const;

        size_t getHash() const override;
        bool isEqual(Bind* other) const override;
        std::string toString() const override;
        DeviceID getDeviceID() const override;
        matjson::Value save() const override;
    };

    class CUSTOM_KEYBINDS_DLL ControllerBind final : public Bind {
    protected:
        cocos2d::enumKeyCodes m_button;

    public:
        static ControllerBind* create(cocos2d::enumKeyCodes button);
        static ControllerBind* parse(matjson::Value const&);

        cocos2d::enumKeyCodes getButton() const;

        size_t getHash() const override;
        bool isEqual(Bind* other) const override;
        std::string toString() const override;
        cocos2d::CCNode* createLabel() const override;
        DeviceID getDeviceID() const override;
        matjson::Value save() const override;
    };

    struct CUSTOM_KEYBINDS_DLL BindHash {
        geode::Ref<Bind> bind;
        BindHash(Bind* bind);
        bool operator==(BindHash const& other) const;
    };
}

namespace std {
    template <>
    struct hash<keybinds::BindHash> {
        CUSTOM_KEYBINDS_DLL std::size_t operator()(keybinds::BindHash const&) const;
    };
}

namespace keybinds {
    class BindManager;
    class InvokeBindFilter;

    using ActionID = std::string;
    class CUSTOM_KEYBINDS_DLL Category final {
        std::string m_value;

    public:
        Category()
        {
            
        }
        Category(const char* path)
        {
            m_value = path;
        }
        Category(std::string const& path)
        {
            m_value = path;
        }
        std::vector<std::string> getPath() const;
        std::optional<Category> getParent() const;
        bool hasParent(Category const& parent) const { return false; }
        std::string toString() const { return m_value; }

        bool operator==(Category const&) const { return false; }

        static constexpr auto PLAY { "Play" };
        static constexpr auto PLAY_PAUSE { "Play/Pause" };
        static constexpr auto EDITOR { "Editor" };
        static constexpr auto GLOBAL { "Global" };
        static constexpr auto EDITOR_UI { "Editor/UI" };
        static constexpr auto EDITOR_MODIFY { "Editor/Modify" };
        static constexpr auto EDITOR_MOVE { "Editor/Move" };
    };

    class CUSTOM_KEYBINDS_DLL BindableAction {
    protected:
        ActionID m_id;
        std::string m_name;
        std::string m_description;
        geode::Mod* m_owner;
        std::vector<geode::Ref<Bind>> m_defaults;
        Category m_category;
        bool m_repeatable;

    public:
        ActionID getID() const;
        std::string getName() const;
        std::string getDescription() const;
        geode::Mod* getMod() const;
        std::vector<geode::Ref<Bind>> getDefaults() const;
        Category getCategory() const;
        bool isRepeatable() const;

        BindableAction() = default;
        BindableAction(ActionID const& id, std::string const& name, std::string const& description, std::vector<geode::Ref<Bind>> const& defaults, Category const& category, bool repeatable = true, geode::Mod* owner = geode::Mod::get())
        {
            m_id = id;
            m_name = name;
            m_description = description;
            m_defaults = defaults;
            m_category = category;
            m_repeatable = repeatable;
            m_owner = owner;
        }
    };

    class CUSTOM_KEYBINDS_DLL InvokeBindEvent : public geode::Event {
    protected:
        ActionID m_id;
        bool m_down;

        friend class BindManager;
        friend class InvokeBindFilter;

    public:
        InvokeBindEvent(ActionID const& id, bool down) 
        {
            m_id = id;
            m_down = down;
        }
        ActionID getID() const { return m_id; }
        bool isDown() const { return false; }
    };

    class CUSTOM_KEYBINDS_DLL InvokeBindFilter : public geode::EventFilter<InvokeBindEvent> {
    protected:
        cocos2d::CCNode* m_target;
        ActionID m_id;

    public:
        using Callback = geode::ListenerResult(InvokeBindEvent*);

        geode::ListenerResult handle(geode::utils::MiniFunction<Callback> fn, InvokeBindEvent* event) { return geode::ListenerResult::Propagate; }
        InvokeBindFilter(cocos2d::CCNode* target, ActionID const& id) { }
    };

    class CUSTOM_KEYBINDS_DLL PressBindEvent : public geode::Event {
    protected:
        Bind* m_bind;
        bool m_down;

    public:
        PressBindEvent(Bind* bind, bool down)
        {
            m_bind = bind;
            m_down = down;
        }
        Bind* getBind() const { return m_bind; }
        bool isDown() const { return false; }
    };

    class CUSTOM_KEYBINDS_DLL PressBindFilter : public geode::EventFilter<PressBindEvent> {
    public:
        using Callback = geode::ListenerResult(PressBindEvent*);

        geode::ListenerResult handle(geode::utils::MiniFunction<Callback> fn, PressBindEvent* event) { return geode::ListenerResult::Propagate; }
        PressBindFilter()
        {

        }
    };

    class CUSTOM_KEYBINDS_DLL DeviceEvent : public geode::Event {
    protected:
        DeviceID m_id;
        bool m_attached;

    public:
        DeviceEvent(DeviceID const& id, bool attached)
        {
            m_id = id;
            m_attached = attached;
        }
        DeviceID getID() const { return m_id; }
        bool wasAttached() const { return false; }
        bool wasDetached() const { return false; }
    };

    class CUSTOM_KEYBINDS_DLL DeviceFilter : public geode::EventFilter<DeviceEvent> {
    protected:
        std::optional<DeviceID> m_id;

    public:
        using Callback = void(DeviceEvent*);

        geode::ListenerResult handle(geode::utils::MiniFunction<Callback> fn, DeviceEvent* event) { return geode::ListenerResult::Propagate; }
        DeviceFilter(std::optional<DeviceID> id = std::nullopt) {
            m_id = id;
        }
    };

    struct CUSTOM_KEYBINDS_DLL RepeatOptions {
        bool enabled = true;
        size_t rate = 50;
        size_t delay = 500;
    };

    using BindParser = std::function<Bind*(matjson::Value const&)>;

    class CUSTOM_KEYBINDS_DLL BindManager : public cocos2d::CCObject {
    // has to inherit from CCObject for scheduler
    public:
        using DevicelessActions = std::unordered_map<ActionID, std::set<matjson::Value>>;

    protected:
        struct ActionData {
            BindableAction definition;
            RepeatOptions repeat;
        };

        std::unordered_map<BindHash, std::vector<ActionID>> m_binds;
        std::unordered_map<DeviceID, DevicelessActions> m_devicelessBinds;
        std::unordered_map<DeviceID, BindParser> m_devices;
        std::vector<std::pair<ActionID, ActionData>> m_actions;
        std::vector<Category> m_categories;
        geode::EventListener<PressBindFilter> m_listener =
            geode::EventListener<PressBindFilter>(this, &BindManager::onDispatch);
        std::unordered_map<ActionID, float> m_repeating;
        std::unordered_map<ActionID, std::unordered_set<size_t>> m_downActionBinds;

        BindManager()
        {

        }

        geode::ListenerResult onDispatch(PressBindEvent* event);
        void onRepeat(float dt) { }
        void repeat(ActionID const& action) { }
        void unrepeat(ActionID const& action) { }

        bool loadActionBinds(ActionID const& action) { return false; }
        void saveActionBinds(ActionID const& action) { }

        /// Checks if a specific Bind + Action combo is being held.
        /// @param action the action ID we are checking
        /// @param bind the bind we are checking
        /// @returns true if the action bind is being held, false otherwise
        bool isActionBindHeld(ActionID const& action, Bind* bind) { return false; }

        /// Marks a bind that belongs to an action as held. No changes will be made if the action doesn't use the bind.
        /// @param action the action ID
        /// @param bind the bind we are marking as held, has to belong to the action
        void markActionBindHeld(ActionID const& action, Bind* bind) { }

        /// Marks a bind that belongs to an action as released. No changes will be made if the action doesn't use the bind.
        /// @param action the action ID
        /// @param bind the bind we are marking as released, has to belong to the action
        void unmarkActionBindHeld(ActionID const& action, Bind* bind) { }

        friend class InvokeBindFilter;
        friend struct matjson::Serialize<BindSaveData>;

    public:
        static BindManager* get() { return nullptr; }
        void save() { }

        void attachDevice(DeviceID const& device, BindParser parser) { }
        void detachDevice(DeviceID const& device) { }

        matjson::Value saveBind(Bind* bind) const;
        Bind* loadBind(matjson::Value const& json) const;

        bool registerBindable(BindableAction const& action, ActionID const& after = "") { return true; }
        void removeBindable(ActionID const& action) { }
        std::optional<BindableAction> getBindable(ActionID const& action) const;
        std::vector<BindableAction> getAllBindables() const;
        std::vector<BindableAction> getBindablesIn(Category const& category, bool sub = false) const;
        std::vector<BindableAction> getBindablesFor(Bind* bind) const;
        std::vector<Category> getAllCategories() const;
        /**
         * Add a new bindable category. If the category is a subcategory (its
         * ID has a slash, like "Editor/Modify"), then all its parent
         * categories are inserted aswell, and the subcategory is added after
         * its parent's last subcategory
         * @param category The category to add. Specify a subcategory by
         * including a slash in the name (like "Editor/Modify")
         */
        void addCategory(Category const& category) { }
        /**
         * @note Also removes all the bindables in this category
         */
        void removeCategory(Category const& category) { }

        void addBindTo(ActionID const& action, Bind* bind) { }
        void removeBindFrom(ActionID const& action, Bind* bind) { }
        void removeAllBindsFrom(ActionID const& action) { }
        void resetBindsToDefault(ActionID const& action) { }
        bool hasDefaultBinds(ActionID const& action) const { return false; }
        std::vector<geode::Ref<Bind>> getBindsFor(ActionID const& action) const { return {}; }

        std::optional<RepeatOptions> getRepeatOptionsFor(ActionID const& action);
        void setRepeatOptionsFor(ActionID const& action, RepeatOptions const& options) { }
        void stopAllRepeats() { }
    };
}