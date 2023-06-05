#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


#define POSTURL    "https://midtest.lock.hongyanyun.cn:8088/v1/lock/zkpKey"  //用于测试阶段
#define POSTFIELDS "{\"appUserId\":\"1569502704483377154\",\"deviceName\":\"H000059200400029\",\"productKey\":\"a164msJWX5c\"}"

struct memory {
   char *response;
   size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
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

int main(int argc,char *argv[])
{
	static const char *pHeaderFile = "dumpit";
	static const char *pCertFile = "./8166028_midtest.lock.hongyanyun.cn.pem"; //client certificate
	//static const char *pCACertFile = "cacrt.pem"; //CA root certificat
	const char *pKeyName = "./8166028_midtest.lock.hongyanyun.cn.key";        // private.key
 
	struct curl_slist *http_header = NULL;
    FILE *headerfile = fopen(pHeaderFile, "wb");
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl;
	curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return -1;
	}
 
    printf("POSTURL:%s\n", POSTURL);
	printf("POSTFIELDS:%s\n", POSTFIELDS);
	
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //url地址
	//curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerfile);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);  //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
    curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);/* set the cert for client authentication */
	//curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);/* set the file with the certs vaildating the server */
	curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);/* set the private key (file or ID in engine) */
    curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");/* if we use a key stored in a crypto engine, we must set the key type to "ENG" */
	//curl_easy_setopt(curl, CURLOPT_CAINFO,  "./hongyan_ssl_test/8166028_midtest.lock.hongyanyun.cn.pem");//cacert.pem为curl官网下载的根证书文件
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,POSTFIELDS); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //打印调试信息

	http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	
    CURLcode res;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK){
		switch(res){
			case CURLE_UNSUPPORTED_PROTOCOL:
				fprintf(stderr,"-----------> 不支持的协议,由URL的头部指定\n");
			case CURLE_COULDNT_CONNECT:
				fprintf(stderr,"-----------> 不能连接到remote主机或者代理\n");
			case CURLE_HTTP_RETURNED_ERROR:
				fprintf(stderr,"-----------> http返回错误\n");
			case CURLE_READ_ERROR:
				fprintf(stderr,"-----------> 读本地文件错误\n");
			default:
				fprintf(stderr,"-----------> 返回值:%d\n",res);
		}
		return -1;
	} else {
		printf("Success -----------> 返回值:%d\n",res);
	}

    fclose(headerfile);
	curl_easy_cleanup(curl);
    //printf("end  limd -----------> \n");
    return 0;
}
