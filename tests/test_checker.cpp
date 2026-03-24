#include "checker.h"
#include <iostream>
#include <vector>

static int test_valid_case() {
    std::vector<Service> services = {
        {"ServiceA", 1, 100},
        {"ServiceB", 2, 200}
    };

    auto report = validate_services(services, "test_valid.json");

    if (!report.valid) return 1;
    if (report.error_count != 0) return 2;
    if (report.warning_count != 0) return 3;

    return 0;
}

static int test_warning_case() {
    std::vector<Service> services = {
        {"ServiceWarning", 10, 20},
        {"ServiceOK", 11, 100}
    };

    auto report = validate_services(services, "test_warning.json");

    if (!report.valid) return 1;
    if (report.error_count != 0) return 2;
    if (report.warning_count != 1) return 3;
    if (report.warnings.empty()) return 4;
    if (report.warnings[0].field != "timeout_ms") return 5;

    return 0;
}

static int test_error_case() {
    std::vector<Service> services = {
        {"", 1, 100},
        {"Duplicate", 1, -1}
    };

    auto report = validate_services(services, "test_error.json");

    if (report.valid) return 1;
    if (report.error_count != 3) return 2;
    if (report.warning_count != 0) return 3;

    return 0;
}

int main() {
    int result = 0;

    result = test_valid_case();
    if (result != 0) {
        std::cerr << "test_valid_case failed: " << result << "\n";
        return 1;
    }

    result = test_warning_case();
    if (result != 0) {
        std::cerr << "test_warning_case failed: " << result << "\n";
        return 1;
    }

    result = test_error_case();
    if (result != 0) {
        std::cerr << "test_error_case failed: " << result << "\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}
