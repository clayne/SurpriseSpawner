#pragma once
#include "Cache.h"
#include "Settings.h"
#include <ClibUtil/rng.hpp>

class Utility : public Singleton<Utility>
{
public:



    std::vector<std::string> exceptions = { "Missive Board", "Alduin", "Harkon", "Chicken", "Drawer"
    };

    bool ExceptionName(std::string exception_name){
        if (std::count(exceptions.begin(), exceptions.end(), exception_name)) {
            logger::debug("restricted name");
            return true;
            
        }
        else {
            return false;
        }
    };

    inline std::int32_t GetRandomChance(uint32_t a_min, uint32_t a_max)
    {
        static std::random_device       rd;
        static std::mt19937             gen(rd());
        std::uniform_int_distribution<> distrib(a_min, a_max);
        return distrib(gen);
    }


    // Credits: powerof3 https://github.com/powerof3/PapyrusExtenderSSE/blob/0d5d48485b444e73b641b43a99db35e7c5dcef4a/include/Papyrus/Functions/Utility.h#L10
    //inline uint32_t RandomInt(uint32_t a_min, uint32_t a_max) { return RNG().generate<std::uint32_t>(a_min, a_max); }

    void RemoveAllItems(RE::TESObjectREFR* a_refToRemoveFrom, RE::TESObjectREFR* a_refToGiveItems)
    {
        std::list<RE::ContainerObject> badItems;

        for (uint32_t i = 0; i < a_refToRemoveFrom->GetContainer()->numContainerObjects; i++) {
            auto item = a_refToRemoveFrom->GetContainer()->containerObjects[i];
            if (item && item->obj) {
                if (item->obj) {
                    badItems.push_back(*item);
                }
            }
            else {
                logger::warn("Item at index {} was nullptr", i);
            }
        }

        for (auto& badItem : badItems) {
            logger::debug("Removed {}", badItem.obj->GetName());
            if (badItem.obj->GetFormType() == RE::FormType::LeveledItem) {
                auto lvlItem    = badItem.obj->As<RE::TESLeveledList>();
                auto list_items = lvlItem->GetContainedForms();
                for (auto& list_item : list_items) {
                    a_refToRemoveFrom->GetHandle().get()->RemoveItem(list_item->As<RE::TESBoundObject>(), badItem.count, RE::ITEM_REMOVE_REASON::kRemove, nullptr,
                                                                     a_refToGiveItems);
                }
            }
            else
                a_refToRemoveFrom->GetHandle().get()->RemoveItem(badItem.obj->As<RE::TESBoundObject>(), badItem.count, RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_refToGiveItems);
        }
    }

    void PlayMeme(RE::BGSSoundDescriptorForm* sound)
    {
        auto                 settings = Settings::GetSingleton();
        RE::BSSoundHandle    handle;
        auto                 am = RE::BSAudioManager::GetSingleton();
        RE::PlayerCharacter* p  = Cache::GetPlayerSingleton();
        if (settings->toggle_meme_sound) {
            am->BuildSoundDataFromDescriptor(handle, sound->soundDescriptor);
            handle.SetVolume(1.5f);
            handle.SetObjectToFollow(p->Get3D());
            handle.Play();
        }
        else
            return;
    }

    bool LocationCheck(std::string_view locKeyword)
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

        if (player->GetCurrentLocation() != nullptr) {
            return player->GetCurrentLocation()->HasKeywordString(locKeyword);
            // logger::debug("current location is: {}", player->GetCurrentLocation()->GetName());
        }
        else {
            logger::debug("no location found");
            return false;
        }
    }

    bool LocPlayerOwned()
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

        if (player->GetCurrentLocation() != nullptr) {
            if (player->GetCurrentLocation()->HasKeywordString("LocTypePlayerHouse")) {
                // logger::debug("current location is: {}", player->GetCurrentLocation()->GetName());
                return true;
            }
            else
                return false;
        }
        else {
            // logger::debug("current location is: {}", player->GetCurrentLocation()->GetName());
            return false;
        }
    }

    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType()) {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction: {
            return true;
        }
        default: {
            return false;
        }
        }
    }

    inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (IsPermanent(spell)) {
            target->AddSpell(spell);
        }
        else {
            caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
        }
    }

    inline static void ApplyStress(RE::Actor* target)
    {
        RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();
        auto                 settings = Settings::GetSingleton();
        Utility::ApplySpell(player, target, settings->StressSpell);
        logger::debug("applied {} to {}", settings->StressSpell->GetName(), target->AsReference()->GetName());
    }


};
