#include <stdio.h>
//#include "include/curl/curl.h"
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;
  FILE *headerfile;
  const char *pPassphrase = NULL;    //password for private key
 
  static const char *pCertFile = "./clientcrt.pem"; //client certificate
  static const char *pCACertFile = "cacrt.pem"; //CA root certificat
  static const char *pHeaderFile = "dumpit";
 
  const char *pKeyName = "clientkey.pem";        // private.key
  const char *pKeyType = "PEM";
  const char *pEngine = NULL;

  headerfile = fopen(pHeaderFile, "wb");
  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
   if(curl) {
    /* what call to write: */
    curl_easy_setopt(curl, CURLOPT_URL, "HTTPS://192.168.31.14:8124");
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerfile);
 
    do { /* dummy loop, just to break out from */
      if(pEngine) {
        /* use crypto engine */
        if(curl_easy_setopt(curl, CURLOPT_SSLENGINE, pEngine) != CURLE_OK) {
          /* load the crypto engine */
          fprintf(stderr, "can't set crypto engine\n");
           break;
        }
        if(curl_easy_setopt(curl, CURLOPT_SSLENGINE_DEFAULT, 1L) != CURLE_OK) {
          /* set the crypto engine as default */
          /* only needed for the first time you load a engine in a curl object... */
          fprintf(stderr, "can't set crypto engine as default\n");
          break;
        }
      }
      /* cert is stored PEM coded in file...   since PEM is default, we needn't set it for PEM */
      curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
 
      /* set the cert for client authentication */
      curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);
 
      /* sorry, for engine we must set the passphrase (if the key has one...) */
      if(pPassphrase){
        curl_easy_setopt(curl, CURLOPT_KEYPASSWD, pPassphrase);
	  }
      /* if we use a key stored in a crypto engine, we must set the key type to "ENG" */
      curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, pKeyType);
 
      /* set the private key (file or ID in engine) */
      curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);
 
      /* set the file with the certs vaildating the server */
      curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);
 
      /* disconnect if we can't validate server's cert */
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
 
      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if(res != CURLE_OK){
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	  }
      /* we are done... */
    } while(0);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
 
  curl_global_cleanup();
 
  return 0;
}
 