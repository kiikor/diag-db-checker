#pragma once
#include <string>
#include <vector>

struct Service {
    std::string name;
    int service_id;
    int timeout_ms;
};

struct Error {
    int index;
    std::string field;
    std::string message;
};

struct Report {
    bool valid;
    int error_count;
    std::vector<Error> errors;
};

std::vector<Service> load_config(const std::string& path);
Report validate_services(const std::vector<Service>& services);
void save_report(const Report& report, const std::string& path);
