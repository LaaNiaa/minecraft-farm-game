#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>
#include <vector>
#include <array>
#include "save_manager.hpp"
#include "field.hpp"

class AutosaveManager {
public:
    AutosaveManager(const std::string& savePath);
    ~AutosaveManager();

    void updateSnapshotAndMarkDirty(const std::vector<std::vector<Field>>& fields,
                                    const std::array<InventoryItem, 27>& inventory,
                                    int emeraldCount);

    void requestImmediateSave();

private:
    void workerThread(std::stop_token stoken);

    std::string saveFilePath;
    GameSnapshot latestSnapshot;
    std::atomic<bool> dirty{false};
    std::atomic<bool> immediateSaveRequested{false};

    std::mutex snapshotMutex;
    std::condition_variable_any cv;

    std::jthread worker;
};