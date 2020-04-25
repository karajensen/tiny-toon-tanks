////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "anttweakbar/AntTweakBar.h"

#include <string>
#include <functional>
#include <vector>
#include <memory>

/**
* Holds information for creating an entry in the tweak bar
*/
class Tweaker
{
public:

    /**
    * Constructor
    * @param tweakbar the tweak bar to fill in
    */
    Tweaker(CTwBar* tweakbar);

    /**
    * Destructor
    */
    ~Tweaker();

    /**
    * Updates the tweak bar
    */
    void Update();

    /**
    * Sets the group used
    * @param group The group to set
    */
    void SetGroup(std::string group);

    /**
    * Creates a new button in the tweak bar
    * @param label What the button will display
    * @param callback The function to use when pressed
    */
    void AddButton(std::string label, 
                   std::function<void(void)> callback);

    /**
    * Creates a new readonly entry for displaying a string
    * @param label What to display the entry as
    * @param getter function to retrieve the string value
    */
    void AddStrEntry(std::string label, 
                     std::function<const std::string(void)> getter);

    /**
    * Creates a new readonly entry for displaying a string
    * @param label What to display the entry as
    * @param getter function to retrieve the string value
    */
    void AddStrEntry(std::string label, std::string entry);

    /**
    * Creates a new tweakable entry for an index
    * @param label What to display the entry as
    * @param getter Callback to get the index
    * @param setter Callback for set the index
    * @param max The maximum value the entry can obtain
    */
    void AddIntEntry(std::string label, 
                     std::function<const int(void)> getter,
                     std::function<void(const int)> setter,
                     unsigned int max);

    /**
    * Creates a new tweakable entry for an int
    * @param label What to display the entry as
    * @param getter Callback to get the int
    */
    void AddIntEntry(std::string label, 
                     std::function<const int(void)> getter);

    /**
    * Creates a new tweakable entry for an int
    * @param label What to display the entry as
    * @param entry The address of the entry to add
    * @param min/max The range the value can be
    */
    void AddIntEntry(std::string label,
                     void* entry, 
                     int min,
                     int max);

    /**
    * Creates a new tweakable entry for an value
    * @param label What to display the entry as
    * @param getter Callback to get the value
    * @param setter Callback for set the value
    * @param step The differece between values
    */
    void AddFltEntry(std::string label, 
                     std::function<const float(void)> getter,
                     std::function<void(const float)> setter,
                     float step);

    /**
    * Creates a new tweakable entry
    * @param label What to display the entry as
    * @param entry The address of the entry to add
    * @param type The type of entry it is
    * @param step The differece between values
    * @param precision The amount of decimal points to display
    */
    void AddFltEntry(std::string label,
                     void* entry, 
                     float step,
                     int precision = 0);

    /**
    * Creates a new tweakable entry
    * @param label What to display the entry as
    * @param entry The address of the entry to add
    * @param type The type of entry it is
    * @param step The differece between values
    * @param min/max The range the value can be
    * @param precision The amount of decimal points to display
    */
    void AddFltEntry(std::string label,
                     void* entry, 
                     float step,
                     float min,
                     float max,
                     int precision = 0);

    /**
    * Creates a new readonly tweakable entry
    * @param label What to display the entry as
    * @param entry The address of the entry to add
    * @param precision The amount of decimal points to display
    */
    void AddFltEntry(std::string label,
                     void* entry,
                     int precision = 0);

    /**
    * Creates a new tweakable entry
    * @param label What to display the entry as
    * @param entry The address of the entry to add
    * @param type The type of entry it is
    * @param readonly Whether this entry is readonly
    */
    void AddEntry(std::string label,
                  void* entry, 
                  TwType type, 
                  bool readonly = false);

    /**
    * Creates a custom group of tweakables within the current group
    * @param label What to display the subgroup as
    * @param entries All values and their label
    * @param step The differece between values
    */
    void AddSubGroup(std::string label,
                     std::vector<std::pair<std::string, float*>>& entries,
                     float step);

    /**
    * Clears all current entries
    */
    void ClearEntries();

    /**
    * Base for a callback entry for allowing templating in cpp
    */
    struct Entry
    {
        virtual void Getter(void* value) const = 0;
        virtual void Setter(const void* value) = 0;
    };

    /**
    * Base for a subgroup callback entry for allowing templating in cpp
    */
    struct SubGroup
    {
        virtual void Getter(void* value) const = 0;
        virtual void Setter(const void* value) = 0;
        std::vector<void*> values;
    };

private:

    /**
    * Fixed size of all string entries
    */
    enum 
    { 
        STR_BUFFER_SIZE = 128 
    };

    /**
    * Data for a button
    */
    struct Button
    {
        std::function<void(void)> callback = nullptr;
    };

    /**
    * Data for a string
    * @note fixed character buffer used in place of 
    * dynamic string / TwCopyStdStringToLibrary for performance
    */
    struct Label
    {
        bool modifiable = false;
        char buffer[STR_BUFFER_SIZE];
        std::string value;
        std::function<const std::string(void)> getter = nullptr;
    };

    /**
    * Fills the internal buffer of a label
    */
    void FillBufffer(Label& label);

    /**
    * Helper function to get a new name for an entry
    */
    std::string GetName();

    /**
    * Helper function for creating a subgroup entry
    * @param subgroups Container for holding all subgroups
    * @param entries the Entries for the subgroup
    * @param definition Callback to retrieve a definition
    * @param type The type of value for each entry
    * @param step Optional step between values
    * @return the new sub group type created
    */
    template <typename T> 
    TwType MakeSubGroup(std::vector<std::pair<std::string, T*>>& entries,
                        TwType type,
                        float step = 0.0f);

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    */
    std::string Definition(std::string label) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param min The minimum value the entry can obtain
    * @param max The maximum value the entry can obtain
    */
    std::string Definition(std::string label, int min, int max) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param step The differece between values
    * @param precision The amount of decimal points to display
    */
    std::string Definition(std::string label, float step, int precision) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param step The differece between values
    * @param min/max The range of values allowed
    * @param precision The amount of decimal points to display
    */
    std::string Definition(std::string label, float step, float min, float max, int precision) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param precision The amount of decimal points to display
    */
    std::string Definition(std::string label, int precision) const;

    /**
    * Determines if there is an error and logs
    */
    void LogTweakError() const;

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @parma clientData Address of the button to call
    */
    static void TW_CALL CallButton(void *clientData);

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @param value The value to be set
    * @param clientData Address of the button to call
    */
    static void TW_CALL SetCallback(const void *value, void *clientData);

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @param value The value to be set
    * @param clientData Address of the button to call
    */
    static void TW_CALL GetCallback(void *value, void *clientData);

private:

    int m_count = 0;                                    ///< Number of entries added
    std::string m_group;                                ///< Global group currently set
    CTwBar* m_tweakBar = nullptr;                       ///< Tweak bar to fill in, not owned by this class
    std::vector<std::unique_ptr<Label>> m_labels;       ///< Data for the tweak bar strings
    std::vector<std::unique_ptr<Button>> m_buttons;     ///< Data for the tweak bar buttons
    std::vector<std::unique_ptr<Entry>> m_entries;      ///< Data for the tweak bar getter/setters
    std::vector<std::unique_ptr<SubGroup>> m_subGroups; ///< Data for the tweak bar subgroups
};