// common.h
#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <queue>

class Task {
private:
    std::string taskID;
    bool taskStatus;

public:
    Task(const std::string& id, bool status = false) : taskID(id), taskStatus(status) {}
    void updateStatus(const std::string& newStatus) {
        taskStatus = (newStatus == "completed");
    }
    // 其他必要的方法...
};

class Vehicle {
private:
    std::string vehicleID;
    std::string vehicleStatus;

public:
    Vehicle(const std::string& id, const std::string& status = "free") : vehicleID(id), vehicleStatus(status) {}
    void switchStatus(const std::string& newStatus) {
        vehicleStatus = newStatus;
    }
    // 其他必要的方法...
};

class BranchOffice {
private:
    std::vector<Vehicle> vehiclePool;
    std::queue<Task> taskQueue;

public:
    void acceptTask(Task task) {
        taskQueue.push(std::move(task));
    }
    void dispatchVehicle(Task task) {
        // 实现车辆分配逻辑...
    }
    void reportTaskCompletion(Task task) {
        // 实现任务完成汇报逻辑...
    }
    // 其他必要的方法...
};

#endif // COMMON_H
