#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "curl/curl.h"
 
typedef struct {
    char *data;
    char *pos;
    char *last;
} drp_upload_ctx;
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    drp_upload_ctx *ctx = (drp_upload_ctx *) stream;
    size_t len = 0;
 
    if (ctx->pos >= ctx->last) {
        return 0;
    }
 
    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }
 
    len = ctx->last - ctx->pos;
    if (len > size*nmemb) {
        len = size * nmemb;
    }
 
    memcpy(ptr, ctx->pos, len);
    ctx->pos += len;
 
    printf("send len=%zu\n", len);
 
    return len;
}
 
int main(int argc, char **argv)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist   *header = NULL;
 
  char *url = argv[1];
  char data[16]="abcdefghijklmnop";
 
  drp_upload_ctx *upload_ctx = (drp_upload_ctx *) malloc(sizeof(drp_upload_ctx));
  if (upload_ctx == NULL) {
    return 1;
  }
 
  upload_ctx->data = data;
  upload_ctx->pos = data;
  upload_ctx->last = upload_ctx->pos + 16;
 
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
 
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_READDATA, upload_ctx);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)(upload_ctx->last - upload_ctx->pos));
 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    //header = curl_slist_append(header, "Expect:");
    //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
 
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
       fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
       curl_easy_cleanup(curl);
  }
 
  curl_global_cleanup();
  //curl_slist_free_all(header);
  free(upload_ctx);
 
  return 0;
}