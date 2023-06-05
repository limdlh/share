#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct memory {
   char *response;
   size_t size;
};

#define HTTP_URL  "https://www.baidu.com/"
//#define HTTP_URL  "https://www.google.com/"

static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
 
//struct memory chunk = {0};
