#ifndef SL_TRANSPORT_H
#define SL_TRANSPORT_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
std::vector<std::vector<std::string>> parseDepartures(const json& departuresJson);
json getDepartures();

#endif
