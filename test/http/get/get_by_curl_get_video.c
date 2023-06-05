#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct memory {
   char *response;
   size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;
 
   char *ptr = realloc(mem->response, mem->size + realsize + 1);
   if(ptr == NULL)
   {
	   printf("realloc fail\n");
       return 0;  /* out of memory! */
   }
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
   printf("~~~~~~~~~~~~~~~~~~~~~~mem->response: \n%s\n", mem->response);

   return realsize;
}
 
struct memory chunk = {0};

int main(void)
{
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.21.16/LAPI/V1.0/Channels/0/Media/Video/Streams/0/LiveStreamURL?TransType=0&TransProtocol=0");
	//curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
	    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}