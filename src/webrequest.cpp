#include "webrequest.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// This function will be called by libcurl to write the response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function that extracts "destination" and "display" from departures JSON
std::vector<std::vector<std::string>> parseDepartures(const json& departuresJson) {
    std::vector<std::vector<std::string>> result;

    // Check if the JSON contains a "departures" array
    if (departuresJson.contains("departures") && departuresJson["departures"].is_array()) {
        for (const auto& dep : departuresJson["departures"]) {
            std::string destination;
            std::string display;
            if (dep["line"]["id"] == 10 || dep["line"]["id"] == 11) {
                if (dep.contains("destination") && dep["destination"].is_string()) {
                    destination = dep["destination"].get<std::string>();
                }

                if (dep.contains("display") && dep["display"].is_string()) {
                    display = dep["display"].get<std::string>();
                }

                if (!destination.empty() && !display.empty()) {
                    result.push_back(std::vector<std::string>{destination, display});
                }
            }
        }
    }

    return result;
}


json getDepartures() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        std::string url = "https://transport.integration.sl.se/v1/sites/9115/departures?transport=METRO&direction=1&forecast=60";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Optional: skip SSL verification if needed (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse JSON
                json jsonData = json::parse(readBuffer);

                // You can now store it in a variable for further use
                json myData = jsonData;
                return myData;
            } catch (json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return {};
}
