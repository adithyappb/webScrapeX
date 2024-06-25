#include "WebCrawler.h"
#include <regex>
#include <iostream>

using namespace std;

WebCrawler::WebCrawler(size_t numThreads, int maxDepth, const string& outputFile)
    : pool(numThreads), maxDepth(maxDepth), stopFlag(false) {
    outputFileStream.open(outputFile);
    if (!outputFileStream.is_open()) {
        throw runtime_error("Failed to open output file");
    }
    startTime = chrono::steady_clock::now();
}

void WebCrawler::addUrl(const string& url) {
    unique_lock<mutex> lock(queueMutex);
    urlQueue.push({url, 0});
    condition.notify_one();
}

void WebCrawler::start() {
    while (true) {
        pair<string, int> urlTask;
        {
            unique_lock<mutex> lock(queueMutex);
            if (urlQueue.empty() && stopFlag) break;
            if (timeExceeded()) {
                stopFlag = true;
                break;
            }
            condition.wait_for(lock, chrono::milliseconds(100), [this]{ return !urlQueue.empty(); });
            if (urlQueue.empty()) continue;
            urlTask = urlQueue.front();
            urlQueue.pop();
        }

        pool.enqueue([this, urlTask] { crawl(urlTask.first, urlTask.second); });
    }
    outputFileStream.close();
}

void WebCrawler::crawl(const string& url, int depth) {
    if (depth > maxDepth || timeExceeded()) return;

    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    vector<string> links = extractLinks(readBuffer, url);

    unique_lock<mutex> lock(queueMutex);
    for (const auto& link : links) {
        if (visited.find(link) == visited.end()) {
            visited.insert(link);
            urlQueue.push({link, depth + 1});
            writeToFile(link);
            condition.notify_one();
        }
    }
}

vector<string> WebCrawler::extractLinks(const string& html, const string& baseUrl) {
    regex urlRegex("<a\\s+href=[\"']([^\"']*)[\"']", regex::icase);
    sregex_iterator it(html.begin(), html.end(), urlRegex);
    sregex_iterator end;
    vector<string> urls;

    while (it != end) {
        smatch match = *it;
        string url = match[1].str();
        if (url.find("http") == string::npos) {
            url = baseUrl + url;
        }
        urls.push_back(url);
        ++it;
    }

    return urls;
}

void WebCrawler::writeToFile(const string& url) {
    if (outputFileStream.is_open()) {
        outputFileStream << url << endl;
    }
}

bool WebCrawler::timeExceeded() const {
    return chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - startTime).count() > 30;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


