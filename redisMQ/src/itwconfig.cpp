/* 
 Read config parameter from config file.


 @Author Shan Jinlai

 03/05/2008

 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "itwconfig.h"
#include "mycommon.h"

const char *argv0="";

void ITWCONFIG::StrTrim(char *str, const char *ss) {
	int len=strlen(str)+1;
	char *buf=new char[len];
	
	char *ptr = str, *p = buf;

	if (str == NULL)
		return;

	memset(buf,0,len);

	bool found = false;
	while (ptr && *ptr) {
		const char *pp = ss;
		while (pp && *pp) {
			if (*pp++ == *ptr) {
				found = true;
				break;
			}
		}
		if (!found)
			*p++ = *ptr;
		found = false;
		ptr++;
	}

	*p = '\0';
	strcpy(str, buf);
	str[strlen(buf)] = '\0';
	delete []buf;
}
ITWCONFIG::ITWCONFIG() {
	init_cfg("billing.conf");
}

ITWCONFIG::ITWCONFIG(char *cfgFilename) {
	init_cfg(cfgFilename);
}

ITWCONFIG::ITWCONFIG(string cfgFilename) {
	init_cfg(cfgFilename.c_str());
}

ITWCONFIG::~ITWCONFIG() {
	uninit_cfg();
}

string ITWCONFIG::getValueByKey(const char *key) {
	CFG_ENTRY* entry = cfgHash[cfgHashKey(key)].link;

	while (entry) {
		if (strcmp(key, entry->key) == 0)
			return entry->value;
		else
			entry = entry->next;
	}
	return "";
}
void getRootPath(char* buff, int buflen) {
	char abuf[512];
	char buf[512];
	int count;

	strcpy(buff, "");
	if (argv0[0] == '/') {
		strcpy(abuf, argv0);
	} else {
		if(getcwd(abuf, 512)==NULL){
			fprintf(stderr, "getcwd error.\n");
		}
		strcat(abuf, "/");
		strcat(abuf, argv0);
	}

	char *p = abuf;

	char tmpBuf[512];
	while (p = strstr(abuf, "/./")) {
		memset(tmpBuf, 0, 512);
		strncpy(tmpBuf, abuf, p - abuf);
		strcat(tmpBuf, "/");
		strcat(tmpBuf, p + 3);
		strcpy(abuf, tmpBuf);
	}

	char *ptr = NULL;
	while (p = strstr(abuf, "/../")) {
		memset(tmpBuf, 0, 512);
		strncpy(tmpBuf, abuf, p - abuf);
		ptr = strrchr(tmpBuf, '/');
		*ptr = '\0';
		strcat(tmpBuf, "/");
		strcat(tmpBuf, p + 4);
		strcpy(abuf, tmpBuf);
	}

	strcpy(buf, abuf);
	ptr = strrchr(buf, '/');
	if (ptr)
		*ptr = '\0';

	/*ptr = strrchr(buf, '/');
	if (ptr)
		*ptr = '\0';
	*/
	memset(buff, 0, buflen);
	strncpy(buff, buf, buflen);
}
void ITWCONFIG::init_cfg(const char *cfgFilename) {
	char buff[1024];
	if (cfgFilename == NULL || cfgFilename[0] == '\0') {
		fprintf(stderr, "config filename is NULL.\n");
		return ;
	}
	memset(buff, 0, 1024);
	if (cfgFilename[0] != '/') {
		char *ptr = getenv("BILL_INSTALL_PATH");
		char *itworks_install_root = (ptr == NULL) ? NULL : ptr;
		if (itworks_install_root == NULL) {
			char cwdBuf[512];
			getRootPath(cwdBuf, 512);

			snprintf(buff, 1024, "%s/conf/%s", cwdBuf, cfgFilename);
		} else {
			snprintf(buff, 1024, "%s/conf/%s", itworks_install_root,
					cfgFilename);
		}
	} else
		strcpy(buff, cfgFilename);
	memset(cfgHash, 0, sizeof(CFG_HASH_ENTRY) * CFG_HASHTABLE_LEN);
	INFO("configer file path is [%s]",buff);
	FILE *fp = NULL;

	char lineBuf[256];
	char envBuf[1024];
	bool lineCont = false;

	memset(lineBuf, 0, sizeof(lineBuf));
	memset(envBuf, 0, sizeof(envBuf));

	fp = fopen(buff, "r");
	if (fp == NULL) {
		fprintf(stderr, "open config file:%s failed.\n", buff);
		return ;
	}

	while (fgets(lineBuf, sizeof(lineBuf), fp)) {
		StrTrim(lineBuf, " \n");

		char *ptr = lineBuf;

		if (!*ptr || *ptr == '#')
			continue;

		if (lineCont) {
			if (lineBuf[strlen(lineBuf) - 1] == '\\') {
				lineBuf[strlen(lineBuf) - 1] = '\0';
				strcat(envBuf, lineBuf);
				continue;
			} else {
				strcat(envBuf, lineBuf);
				lineCont = false;
			}
		} else {
			if (lineBuf[strlen(lineBuf) - 1] == '\\') {
				lineBuf[strlen(lineBuf) - 1] = '\0';
				strcpy(envBuf, lineBuf);
				lineCont = true;
				continue;
			}
			strcpy(envBuf, lineBuf);
		}

		if (envBuf[0] == '=') {
			fprintf(stderr, "Warning:incorrent configure format:%s.\n", envBuf);
			fprintf(stderr,
					"	Configuration line can't begin with a '=',omitted it.\n");
			continue;

		}

		ptr = strstr(envBuf, "=");
		if (ptr) {
			*ptr = '\0';
			CFG_ENTRY* entry = new CFG_ENTRY;
			if (entry == NULL) {
				fprintf(stderr, "No enough memory!\n");
				exit(-2);
			}
			strcpy(entry->key, envBuf);
			strcpy(entry->value, ptr + 1);
			if (entry->value[strlen(entry->value)-1] == '\r')
				entry->value[strlen(entry->value)-1] = '\0';

			entry->next = cfgHash[cfgHashKey(entry->key)].link;
			cfgHash[cfgHashKey(entry->key)].link = entry;
		}
	}

	fclose(fp);
}

void ITWCONFIG::uninit_cfg() {
	for (int i = 0; i < CFG_HASHTABLE_LEN; i++) {
		CFG_ENTRY* entry = cfgHash[i].link, *next;
		while (entry != NULL) {
			next = entry->next;
			delete entry;
			entry = next;
		}
	}

	memset(cfgHash, 0, sizeof(CFG_HASH_ENTRY) * CFG_HASHTABLE_LEN);
}

int ITWCONFIG::cfgHashKey(const char *key) {
	if (strlen(key) >= 2)
		return ((key[0] + key[1]) % CFG_HASHTABLE_LEN);

	return (key[0] % CFG_HASHTABLE_LEN);
}
