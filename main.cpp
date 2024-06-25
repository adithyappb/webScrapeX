#include "WebCrawler.h"

int main() {
    WebCrawler crawler(4, 2, "crawled_urls.txt");
    crawler.addUrl("https://www.softwareag.com/");
    crawler.start();
    return 0;
}

