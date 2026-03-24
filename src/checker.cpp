#include "checker.h"
#include <fstream>
#include <set>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<Service> load_config(const std::string& path) {
    std::ifstream f(path);
    json j;
    f >> j;

    std::vector<Service> services;

    for (auto& item : j) {
        services.push_back({
            item.value("name", ""),
            item.value("service_id", -1),
            item.value("timeout_ms", -1)
        });
    }

    return services;
}

Report validate_services(const std::vector<Service>& services) {
    std::set<int> seen;
    std::vector<Error> errors;

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& s = services[i];

        if (s.name.empty()) {
            errors.push_back({static_cast<int>(i), "name", "name is empty"});
        }

        if (seen.count(s.service_id)) {
            errors.push_back({static_cast<int>(i), "service_id", "duplicate service_id"});
        } else {
            seen.insert(s.service_id);
        }

        if (s.timeout_ms <= 0) {
            errors.push_back({static_cast<int>(i), "timeout_ms", "invalid timeout_ms"});
        }
    }

    return {
        errors.empty(),
        static_cast<int>(errors.size()),
        errors
    };
}

void save_report(const Report& report, const std::string& path) {
    std::filesystem::path output_path(path);
    std::filesystem::create_directories(output_path.parent_path());

    json j;
    j["valid"] = report.valid;
    j["error_count"] = report.error_count;
    j["errors"] = json::array();

    for (const auto& e : report.errors) {
        j["errors"].push_back({
            {"index", e.index},
            {"field", e.field},
            {"message", e.message}
        });
    }

    std::ofstream f(path);
    f << j.dump(2);
}
