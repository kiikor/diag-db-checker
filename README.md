# diag-db-checker

A small C++ diagnostic/config validation tool with CI/CD practice.

## Features

- Validate JSON config input
- Support error and warning classification
- Generate structured JSON report
- Run unit tests with CTest
- CI with GitHub Actions
- CD deploy to remote Linux server via SSH

## Project Structure

```text
diag-db-checker/
├── src/
├── include/
├── tests/
├── data/
├── .github/workflows/
├── CMakeLists.txt
└── README.md
```

## Build

```bash
cmake -B build
cmake --build build
```

## Run

Valid sample:

```bash
./build/checker data/sample_ok.json
```

Warning sample:

```bash
./build/checker data/sample_warning.json
```

Invalid sample:

```bash
./build/checker data/sample_bad.json
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Report Output

The tool generates a JSON report, for example:

```json
{
  "file": "data/sample_bad.json",
  "tool_version": "0.2.0",
  "valid": false,
  "error_count": 3,
  "warning_count": 0
}
```

Main fields:

- `valid`: whether validation passed
- `error_count`: number of blocking issues
- `warning_count`: number of non-blocking issues
- `errors`: list of blocking issues
- `warnings`: list of non-blocking issues

## CI/CD

### CI
GitHub Actions runs:

- build
- unit tests
- validation cases
- report generation

### CD
GitHub Actions deploys to a remote Linux server by:

- connecting through SSH
- uploading project source
- building on the target server
- running the checker on the server
