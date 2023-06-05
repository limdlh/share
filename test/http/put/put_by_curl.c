#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "curl/curl.h"

struct memory {
   char *response;
   size_t size;
};
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }
	
	char *ctx = (char *) stream;
	size_t len = 0;
	len = strlen(ctx);
    if (len > size*nmemb) {
        len = size * nmemb;
    }
 
    memcpy(ptr, ctx, len);
	printf("send len=%zu, ctx: %s, ptr: %s\n", len, ctx, (char *)ptr);
 
    return len;
}

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;
 
   char *ptr = realloc(mem->response, mem->size + realsize + 1);
   if(ptr == NULL) {
	   printf("realloc fail\n");
       return 0;  /* out of memory! */
   }
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
   printf("~~~~~~~~~~~~~~~~~~~~~~mem->response: %s", mem->response);
 
   return realsize;
}

struct memory chunk = {0};

int main(int argc, char **argv) {
	CURL *curl;
	CURLcode res;
	//struct curl_slist *header = NULL;

	char *url = "http://192.168.21.16/LAPI/V1.0/Channel/0/System/Security/Login";
	char data[160]="{\"LocalAddress\":{\"AddressType\":0,\"Address\":192.168.19.133},\"ServerPort\":80,\"ServerAddress\":{\"AddressType\":0,\"Address\":192.168.21.16}}";

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
 
	if(curl){
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_READDATA, data);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)strlen(data));
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//header = curl_slist_append(header, "Expect:");
		//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	//curl_slist_free_all(header);
 
  return 0;
}