#pragma once
#include <string>
#include <vector>

struct Service {
    std::string name;
    int service_id;
    int timeout_ms;
};

struct Issue {
    int index;
    std::string field;
    std::string message;
    std::string severity;
};

struct Report {
    std::string file;
    std::string timestamp;
    std::string tool_version;
    bool valid;
    int error_count;
    int warning_count;
    std::vector<Issue> errors;
    std::vector<Issue> warnings;
};

std::vector<Service> load_config(const std::string& path);
Report validate_services(const std::vector<Service>& services, const std::string& input_file);
void save_report(const Report& report, const std::string& path);
