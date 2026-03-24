#include "checker.h"
#include <fstream>
#include <set>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static std::string current_utc_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);

    std::tm utc_tm{};
#if defined(_WIN32)
    gmtime_s(&utc_tm, &now_time);
#else
    gmtime_r(&now_time, &utc_tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

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

Report validate_services(const std::vector<Service>& services, const std::string& input_file) {
    std::set<int> seen;
    std::vector<Issue> errors;
    std::vector<Issue> warnings;

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& s = services[i];

        if (s.name.empty()) {
            errors.push_back({
                static_cast<int>(i),
                "name",
                "name is empty",
                "error"
            });
        }

        if (seen.count(s.service_id)) {
            errors.push_back({
                static_cast<int>(i),
                "service_id",
                "duplicate service_id",
                "error"
            });
        } else {
            seen.insert(s.service_id);
        }

        if (s.timeout_ms <= 0) {
            errors.push_back({
                static_cast<int>(i),
                "timeout_ms",
                "invalid timeout_ms",
                "error"
            });
        } else if (s.timeout_ms < 50) {
            warnings.push_back({
                static_cast<int>(i),
                "timeout_ms",
                "timeout_ms is low",
                "warning"
            });
        }
    }

    return {
        input_file,
        current_utc_timestamp(),
        "0.2.0",
        errors.empty(),
        static_cast<int>(errors.size()),
        static_cast<int>(warnings.size()),
        errors,
        warnings
    };
}

void save_report(const Report& report, const std::string& path) {
    std::filesystem::path output_path(path);
    std::filesystem::create_directories(output_path.parent_path());

    json j;
    j["file"] = report.file;
    j["timestamp"] = report.timestamp;
    j["tool_version"] = report.tool_version;
    j["valid"] = report.valid;
    j["error_count"] = report.error_count;
    j["warning_count"] = report.warning_count;

    j["errors"] = json::array();
    for (const auto& e : report.errors) {
        j["errors"].push_back({
            {"severity", e.severity},
            {"index", e.index},
            {"field", e.field},
            {"message", e.message}
        });
    }

    j["warnings"] = json::array();
    for (const auto& w : report.warnings) {
        j["warnings"].push_back({
            {"severity", w.severity},
            {"index", w.index},
            {"field", w.field},
            {"message", w.message}
        });
    }

    std::ofstream f(path);
    f << j.dump(2);
}
