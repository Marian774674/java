#pragma once
#include <string>
#include <vector>

// NPC struct - friendly characters with dialogue, quests, and optional rewards
struct NPC {
    std::string name;
    char icon;                       // displayed on map
    int x = 0, y = 0;

    // Multi-stage dialogue
    std::vector<std::string> dialogueGreeting;    // First meeting → gives quest
    std::vector<std::string> dialogueInProgress;  // Quest active, not yet done
    std::vector<std::string> dialogueComplete;    // Quest completed (turn-in)
    std::vector<std::string> dialoguePostQuest;   // After quest (repeatable hints)

    // Reward (fragment)
    std::string rewardFragmentKeyword; // empty = no fragment reward
    int rewardCount = 1;

    // Quest tracking
    int questId = 0;       // 0=none, 1=Sage, 2=Scholar, 3=Keeper, 4=Ghost
    int questStage = 0;    // 0=greeting, 1=in-progress, 2=completed

    // Visibility
    bool isHidden = false; // invisible on map until interacted with
    bool rewardGiven = false;
};
