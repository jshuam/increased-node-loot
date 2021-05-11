/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

bool IncreasedNodeLootEnabled;
bool IncreasedNodeLootAnnounceModule;
uint32 IncreasedNodeLootMultiplier;

class IncreasedNodeLootConf : public WorldScript
{
public:
    IncreasedNodeLootConf() : WorldScript("IncreasedNodeLootConf") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        IncreasedNodeLootEnabled = sConfigMgr->GetBoolDefault("IncreasedNodeLoot.Enabled", true);
        IncreasedNodeLootAnnounceModule = sConfigMgr->GetBoolDefault("IncreasedNodeLoot.Announce", true);
        IncreasedNodeLootMultiplier = sConfigMgr->GetIntDefault("IncreasedNodeLoot.Multiplier", 5);
    }
};

class IncreasedNodeLootAnnounce : public PlayerScript
{
public:
    IncreasedNodeLootAnnounce() : PlayerScript("IncreasedNodeLootAnnounce") { }

    void OnLogin(Player* player) override
    {
        if (IncreasedNodeLootEnabled && IncreasedNodeLootAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is using |cff00AAFFIncreased Node Loot|r by |cffFF6F00Jyger|r");
        }
    }
};

class IncreasedNodeLoot : public MiscScript
{
public:
    IncreasedNodeLoot() : MiscScript("IncreasedNodeLoot") { }

    void OnAfterLootTemplateProcess(Loot* loot, LootTemplate const* /*tab*/, LootStore const& /*store*/, Player* /*lootOwner*/, bool /*personal*/, bool /*noEmptyError*/, uint16 /*lootMode*/) override
    {
        if (IncreasedNodeLootEnabled)
        {
            LootItemList& items = loot->items;

            for (LootItem& item : items)
            {
                const ItemTemplate* itemTemplate = sObjectMgr->GetItemTemplate(item.itemid);

                if (itemTemplate->Class == 7 && (itemTemplate->SubClass == 7 || itemTemplate->SubClass == 9))
                {
                    item.count *= IncreasedNodeLootMultiplier;
                }
            }
        }
    }
};

void AddIncreasedNodeLootScripts()
{
    new IncreasedNodeLootConf();
    new IncreasedNodeLootAnnounce();
    new IncreasedNodeLoot();
}
