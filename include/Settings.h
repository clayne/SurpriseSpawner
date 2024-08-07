#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Settings : public Singleton<Settings>
{
public:
    void   LoadSettings() noexcept;
    void   LoadForms() noexcept;
    void   LoadExceptionJSON(const wchar_t* a_path);
    double GetRandomDouble(double a_min, double a_max);

    json JSONSettings;

    std::string FileName;
    int         minNumber;
    int         maxNumber;
    int         compareValue;

    RE::FormID SpawnFormID;
    RE::FormID SpawnExplosionFormID;
    RE::FormID SpawnUrnExplosionFormID;
    RE::FormID DraugrEnemyFormID;
    RE::FormID DwarvenSpawnFormID;
    RE::FormID ShadeSpawnFormID;
    RE::FormID MemeSoundFormID;
    RE::FormID WerewolfSpawnFormID;
    RE::FormID MimicSpawnFormID;
    RE::FormID StressSpellFormID;

    RE::BGSExplosion*             SpawnExplosion;
    RE::BGSExplosion*             UrnExplosion;
    RE::TESNPC*                   SpawnEnemy; // had to be TESNPC* cause Actor* doesn't work
    RE::TESNPC*                   DraugrEnemy;
    RE::TESNPC*                   DwarvenEnemy;
    RE::TESNPC*                   ShadeEnemy;
    RE::TESNPC*                   WerewolfEnemy;
    RE::TESNPC*                   MimicEnemy;
    RE::TESFaction*               WerewolfFaction;
    RE::SpellItem*                StressSpell;
    std::chrono::duration<double> thread_delay;
    RE::BGSSoundDescriptorForm*   MemeSound;
    RE::TESGlobal*                MinChanceGlobal;
    RE::TESGlobal*                MaxChanceGlobal;

    static RE::FormID ParseFormID(const std::string& str);

    inline static bool debug_logging{};

    inline static bool   npc_event_active{ true };
    inline static bool   draugr_container_event_active{ true };
    inline static bool   dwarven_container_event_active{ true };
    inline static bool   shade_container_event_active{ true };
    inline static bool   generic_container_event_active{ true };
    inline static bool   urn_explosion_event_active{ true };
    inline static bool   toggle_meme_sound{ false };
    inline static bool   toggle_visual_explosion{ true };
    inline static bool   delayed_explosion{ false };
    inline static double delay_timer{ 2.5 };
    inline static bool   useDelayRange{ true };
    inline static double minTime;
    inline static double maxTime;
    inline static bool   useChanceGlobal{ false };
};
