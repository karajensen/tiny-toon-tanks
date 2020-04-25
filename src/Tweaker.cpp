////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker.h"
#include "common.h"
#include <assert.h>
#include <algorithm>

namespace
{
    /**
    * Data for a getter/setter callback
    */
    template <typename T> 
    struct TweakableEntry : public Tweaker::Entry
    {
        virtual void Getter(void* value) const override
        {
            *static_cast<T*>(value) = getter();
        }

        virtual void Setter(const void* value) override
        {
            setter(*static_cast<const T*>(value));
        }

        std::function<const T(void)> getter = nullptr;
        std::function<void(const T)> setter = nullptr;
    };

    /**
    * Data for a getter/setter callback for an array of values
    */
    template <typename T> 
    struct TweakableSubGroup : public Tweaker::SubGroup
    {
        virtual void Getter(void* value) const override
        {
            const std::vector<void*>& array = getter();
            for (unsigned int i = 0; i < array.size(); ++i)
            {
                static_cast<T*>(value)[i] = *static_cast<float*>(array[i]);
            }
        }

        virtual void Setter(const void* value) override
        {
            setter(value);
        }

        std::function<const std::vector<void*>&(void)> getter = nullptr;
        std::function<void(const void*)> setter = nullptr;
    };
}

Tweaker::~Tweaker() = default;

Tweaker::Tweaker(CTwBar* tweakbar)
    : m_tweakBar(tweakbar)
{
}

void Tweaker::Update()
{
    for (auto& label : m_labels)
    {
        if (label->modifiable)
        {
            const std::string value = label->getter();
            if (value != label->value)
            {
                assert(value.size() < STR_BUFFER_SIZE);
                label->value = value;
                FillBufffer(*label);
            }
        }
    }
}

std::string Tweaker::Definition(std::string label, int min, int max) const
{
    return " max=" + std::to_string(max) + 
           " min=" + std::to_string(min) + 
           Definition(label);
}

std::string Tweaker::Definition(std::string label, float step, int precision) const
{
    const int defaultPrecision = 3;
    return " step=" + std::to_string(step) + 
        " precision=" + std::to_string(precision == 0 ? defaultPrecision : precision) + 
        Definition(label);
}

std::string Tweaker::Definition(std::string label, float step, float min, float max, int precision) const
{
    const int defaultPrecision = 3;
    return " step=" + std::to_string(step) + 
        " precision=" + std::to_string(precision == 0 ? defaultPrecision : precision) + 
        " min=" + std::to_string(min) +
        " max=" + std::to_string(max) +
        Definition(label);
}

std::string Tweaker::Definition(std::string label, int precision) const
{
    return " precision=" + std::to_string(precision) + Definition(label);
}

std::string Tweaker::Definition(std::string label) const
{
    return " group='" + m_group + "'" + " label='" + label + "' ";
}

void Tweaker::SetGroup(std::string group)
{
    m_group = group;
}

void TW_CALL Tweaker::CallButton(void *clientData)
{
    static_cast<Button*>(clientData)->callback();
}

void TW_CALL Tweaker::SetCallback(const void *value, void *clientData)
{
    static_cast<Entry*>(clientData)->Setter(value);
}

void TW_CALL Tweaker::GetCallback(void *value, void *clientData)
{
    static_cast<Entry*>(clientData)->Getter(value);
}

void Tweaker::AddEntry(std::string label,
                       void* entry, 
                       TwType type, 
                       bool readonly)
{
    if (readonly)
    {
        TwAddVarRO(m_tweakBar, GetName().c_str(), type,
            entry, Definition(label).c_str());
    }
    else
    {
        TwAddVarRW(m_tweakBar, GetName().c_str(), type,
            entry, Definition(label).c_str());
    }

    LogTweakError();
}

void Tweaker::AddFltEntry(std::string label, 
                          void* entry, 
                          float step, 
                          int precision)
{
    TwAddVarRW(m_tweakBar, GetName().c_str(), TW_TYPE_FLOAT, entry, 
        Definition(label, step, precision).c_str());

    LogTweakError();
}

void Tweaker::AddFltEntry(std::string label, 
                          void* entry, 
                          float step, 
                          float min,
                          float max,
                          int precision)
{
    TwAddVarRW(m_tweakBar, GetName().c_str(), TW_TYPE_FLOAT, entry, 
        Definition(label, step, min, max, precision).c_str());

    LogTweakError();
}

void Tweaker::AddFltEntry(std::string label, 
                          void* entry,
                          int precision)
{
    TwAddVarRO(m_tweakBar, GetName().c_str(), TW_TYPE_FLOAT, entry, 
        Definition(label, precision).c_str());

    LogTweakError();
}

void Tweaker::AddFltEntry(std::string label,
                          std::function<const float(void)> getter,
                          std::function<void(const float)> setter,
                          float step)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<float>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_FLOAT, SetCallback,
        GetCallback, m_entries[index].get(), Definition(label, step, 0).c_str());

    LogTweakError();
}

void Tweaker::AddIntEntry(std::string label, 
                          void* entry, 
                          int min,
                          int max)
{
    TwAddVarRW(m_tweakBar, GetName().c_str(), TW_TYPE_INT32, entry, 
        Definition(label, min, max).c_str());

    LogTweakError();
}

void Tweaker::AddIntEntry(std::string label,
                          std::function<const int(void)> getter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_INT32, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddIntEntry(std::string label,
                          std::function<const int(void)> getter,
                          std::function<void(const int)> setter,
                          unsigned int max)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_INT32, 
        SetCallback, GetCallback, m_entries[index].get(), 
        Definition(label, 0, static_cast<int>(max)).c_str());

    LogTweakError();
}

void Tweaker::FillBufffer(Label& label)
{
    for (unsigned int i = 0; i < label.value.size(); ++i)
    {
        label.buffer[i] = label.value[i];
    }
    label.buffer[label.value.size()] = '\0';
}

void Tweaker::AddStrEntry(std::string label, 
                          std::function<const std::string(void)> getter)
{
    const auto index = m_labels.size();
    m_labels.push_back(std::make_unique<Label>());
    
    const std::string value = getter();
    m_labels[index]->getter = getter;
    m_labels[index]->modifiable = true;
    m_labels[index]->value = value;
    FillBufffer(*m_labels[index]);

    TwAddVarRO(m_tweakBar, GetName().c_str(), 
        TW_TYPE_CSSTRING(STR_BUFFER_SIZE),
        m_labels[index]->buffer, Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddStrEntry(std::string label, std::string value)
{
    const auto index = m_labels.size();
    m_labels.push_back(std::make_unique<Label>());
    m_labels[index]->modifiable = false;
    m_labels[index]->value = value;
    FillBufffer(*m_labels[index]);

   TwAddVarRO(m_tweakBar, GetName().c_str(), 
       TW_TYPE_CSSTRING(STR_BUFFER_SIZE),
       m_labels[index]->buffer, Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddButton(std::string label, 
                        std::function<void(void)> callback)
{
    const auto index = m_buttons.size();
    m_buttons.push_back(std::make_unique<Button>());
    m_buttons[index]->callback = callback;
    
    TwAddButton(m_tweakBar, GetName().c_str(),
        CallButton, m_buttons[index].get(), Definition(label).c_str());

    LogTweakError();
}

std::string Tweaker::GetName()
{
    return "Entry" + std::to_string(++m_count);
}

void Tweaker::ClearEntries()
{
    m_entries.clear();
    m_buttons.clear();
    m_group.clear();
    m_subGroups.clear();
}

template <typename T> 
TwType Tweaker::MakeSubGroup(std::vector<std::pair<std::string, T*>>& entries,
                             TwType type,
                             float step)
{
    auto entry = std::make_unique<TweakableSubGroup<T>>();
    auto* subgroup = entry.get();

    entry->getter = [subgroup]() -> const std::vector<void*>&
    {
        return subgroup->values;
    };

    entry->setter = [subgroup](const void* values)
    {
        for (unsigned int i = 0; i < subgroup->values.size(); ++i)
        {
            *static_cast<T*>(subgroup->values[i]) = static_cast<const T*>(values)[i];
        }
    };

    std::vector<TwStructMember> members;
    for (unsigned int count = 0; count < entries.size(); ++count)
    {
        auto& pair = entries[count];
        entry->values.push_back(pair.second);
    
        // Note name doesn't need to be unqiue
        // Definition doesn't support label/group
        members.emplace_back();
        members[count].Name = pair.first.c_str();
        members[count].Type = type;
        members[count].Offset = sizeof(T)*count;
        members[count].DefString = std::string(step == 0.0f ? 
            "" : "step=" + std::to_string(step)).c_str();
    }
    
    m_subGroups.push_back(std::move(entry));

    return TwDefineStruct(GetName().c_str(), &members[0], members.size(), 
        sizeof(T)*members.size(), nullptr, nullptr);
}

void Tweaker::AddSubGroup(std::string label,
                          std::vector<std::pair<std::string, float*>>& entries,
                          float step)
{
    const TwType subgroupType = MakeSubGroup(entries, TW_TYPE_FLOAT, step);
    auto& entry = m_subGroups[m_subGroups.size()-1];

    TwAddVarCB(m_tweakBar, GetName().c_str(), subgroupType, 
        SetCallback, GetCallback, entry.get(), Definition(label).c_str());

    LogTweakError();
}

void Tweaker::LogTweakError() const
{
    const char* error = TwGetLastError();
    if (error != nullptr)
    {
        LogError("Tweaker: Error for " + m_group + " - " + std::to_string(m_count));
    }
}