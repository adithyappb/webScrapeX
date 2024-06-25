#pragma once
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <curl/curl.h>
#include <chrono>
#include "ThreadPool.h"

using namespace std;

class WebCrawler {
public:
    WebCrawler(size_t numThreads, int maxDepth, const string& outputFile);
    void addUrl(const string& url);
    void start();
private:
    void crawl(const string& url, int depth);
    vector<string> extractLinks(const string& html, const string& baseUrl);

    set<string> visited;
    queue<pair<string, int>> urlQueue;
    mutex queueMutex;
    condition_variable condition;
    ThreadPool pool;
    int maxDepth;
    ofstream outputFileStream;
    atomic<bool> stopFlag;

    chrono::time_point<chrono::steady_clock> startTime;

    void writeToFile(const string& url);
    bool timeExceeded() const;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

