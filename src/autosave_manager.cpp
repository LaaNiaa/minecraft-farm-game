#include "../include/autosave_manager.hpp"
#include <iostream>

AutosaveManager::AutosaveManager(const std::string& savePath)
    : saveFilePath(savePath) {
    worker = std::jthread([this](std::stop_token stoken) {
        workerThread(std::move(stoken));
    });
}

AutosaveManager::~AutosaveManager() {
    if (dirty) {
        immediateSaveRequested = true;
        cv.notify_one();
    }
}

void AutosaveManager::updateSnapshotAndMarkDirty(const std::vector<std::vector<Field>>& fields,
                                                 const std::array<InventoryItem, 27>& inventory,
                                                 int emeraldCount) {
    std::lock_guard<std::mutex> lock(snapshotMutex);

    latestSnapshot.emeraldCount = emeraldCount;
    latestSnapshot.inventoryItems = inventory;
    latestSnapshot.fields.resize(fields.size());

    for (size_t y = 0; y < fields.size(); ++y) {
        latestSnapshot.fields[y].resize(fields[y].size());
        for (size_t x = 0; x < fields[y].size(); ++x) {
            latestSnapshot.fields[y][x].blockType = fields[y][x].getBlockType();
            latestSnapshot.fields[y][x].cropState = fields[y][x].getCropState();
            latestSnapshot.fields[y][x].cropType = fields[y][x].getCropType();
            latestSnapshot.fields[y][x].cropAge = fields[y][x].cropAge;
        }
    }
    dirty = true;
}

void AutosaveManager::requestImmediateSave() {
    if (dirty) {
        immediateSaveRequested = true;
        cv.notify_one();
    }
}

void AutosaveManager::workerThread(std::stop_token stoken) {
    while (!stoken.stop_requested()) {
        std::unique_lock<std::mutex> lock(snapshotMutex);

        cv.wait_for(lock, stoken, std::chrono::seconds(30), [this]() {
            return immediateSaveRequested.load();
        });

        immediateSaveRequested = false;

        if (dirty) {
            GameSnapshot localSnapshot = latestSnapshot;
            dirty = false;

            lock.unlock();

            try {
                if (SaveManager::saveSnapshot(localSnapshot, saveFilePath)) {
                    std::cout << "[Autosave] Gra zostala zapisana w tle." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "[Autosave] Blad zapisu: " << e.what() << std::endl;
            }
        }
    }

    std::unique_lock<std::mutex> lock(snapshotMutex);
    if (dirty) {
        GameSnapshot localSnapshot = latestSnapshot;
        dirty = false;
        lock.unlock();
        try {
            if (SaveManager::saveSnapshot(localSnapshot, saveFilePath)) {
                std::cout << "[Autosave] Zapisano stan gry podczas wychodzenia." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[Autosave] Blad awaryjnego zapisu: " << e.what() << std::endl;
        }
    }
}