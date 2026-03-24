#include "checker.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./checker <input.json> [output.json]\n";
        return 2;
    }

    std::string input = argv[1];
    std::string output = (argc >= 3) ? argv[2] : "reports/report.json";

    auto services = load_config(input);
    auto report = validate_services(services);
    save_report(report, output);

    std::cout << "Validation finished.\n";
    std::cout << "Valid: " << (report.valid ? "true" : "false") << "\n";
    std::cout << "Errors: " << report.error_count << "\n";

    return report.valid ? 0 : 1;
}
