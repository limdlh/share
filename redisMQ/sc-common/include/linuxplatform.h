#ifndef _LINUX_PLAT_FORM_H
#define _LINUX_PLAT_FORM_H
#ifndef PRIz
#define PRIz "z"
#endif /* PRIz */

#define __STDC_FORMAT_MACROS

#include <inttypes.h>
#include <algorithm>
#include <arpa/inet.h>
#include <assert.h>
#include <cctype>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <list>
#include <map>
#include <set>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <vector>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <iconv.h>
#include <pthread.h>

using namespace std;

#define SOCKET int32_t
#define PutEnv putenv
#define TzSet tzset
#define PATH_SEPARATOR '/'
#define CLOSE_SOCKET(fd) if((fd)>=0) close((fd))
#define EHTONS(x) htons(x)







#define Timestamp_init {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}


#define STR(x) ((x).c_str())
#define MAP_HAS1(m,k) ((bool)((m).find((k))!=(m).end()))
#define MAP_HAS2(m,k1,k2) ((MAP_HAS1((m),(k1))==true)?MAP_HAS1((m)[(k1)],(k2)):false)
#define MAP_HAS3(m,k1,k2,k3) ((MAP_HAS1((m),(k1)))?MAP_HAS2((m)[(k1)],(k2),(k3)):false)
#define FOR_MAP(m,k,v,i) for(map< k , v >::iterator i=(m).begin();i!=(m).end();i++)
#define MAP_KEY(i) ((i)->first)
#define MAP_VAL(i) ((i)->second)
#define MAP_ERASE1(m,k) if(MAP_HAS1((m),(k))) (m).erase((k));
#define MAP_ERASE2(m,k1,k2) \
if(MAP_HAS1((m),(k1))){ \
    MAP_ERASE1((m)[(k1)],(k2)); \
    if((m)[(k1)].size()==0) \
        MAP_ERASE1((m),(k1)); \
}
#define MAP_ERASE3(m,k1,k2,k3) \
if(MAP_HAS1((m),(k1))){ \
    MAP_ERASE2((m)[(k1)],(k2),(k3)); \
    if((m)[(k1)].size()==0) \
        MAP_ERASE1((m),(k1)); \
}


#define FOR_VECTOR(v,i) for(uint32_t i=0;i<(v).size();i++)
#define FOR_VECTOR_ITERATOR(e,v,i) for(vector<e>::iterator i=(v).begin();i!=(v).end();i++)
#define FOR_VECTOR_WITH_START(v,i,s) for(uint32_t i=s;i<(v).size();i++)
#define ADD_VECTOR_END(v,i) (v).push_back((i))
#define ADD_VECTOR_BEGIN(v,i) (v).insert((v).begin(),(i))
#define VECTOR_VAL(i) (*(i))


typedef void (*SignalFnc)(void);


string GetEnvVariable(const char *pEnvVarName);
void replace(string &target, string search, string replacement);
bool fileExists(string path);
int fileSize(string path);
string lowerCase(string value);
string upperCase(string value);
string changeCase(string &value, bool lowerCase);
string tagToString(uint64_t tag);
bool setMaxFdCount(uint32_t &current, uint32_t &max);
bool enableCoreDumps();
bool setFdJoinMulticast(SOCKET sock, string bindIp, uint16_t bindPort, string ssmIp);
bool setFdCloseOnExec(int fd);
bool setFdNonBlock(SOCKET fd);
bool setFdNoSIGPIPE(SOCKET fd);
bool setFdKeepAlive(SOCKET fd, bool isUdp);
bool setFdNoNagle(SOCKET fd, bool isUdp);
bool setFdReuseAddress(SOCKET fd);
bool setFdTTL(SOCKET fd, uint8_t ttl);
bool setFdMulticastTTL(SOCKET fd, uint8_t ttl);
bool setFdTOS(SOCKET fd, uint8_t tos);
bool setFdMaxSndRcvBuff(SOCKET fd);
bool setFdOptions(SOCKET fd, bool isUdp);
void killProcess(pid_t pid);
bool deleteFile(string path);
bool deleteFolder(string path, bool force);
bool createFolder(string path, bool recursive);
string getHostByName(string name);
bool isNumeric(string value);
void split(string str, string separator, vector<string> &result);
uint64_t getTagMask(uint64_t tag);
string generateRandomString(uint32_t length);
void lTrim(string &value);
void rTrim(string &value);
void trim(string &value);
int8_t getCPUCount();
map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings);
void splitFileName(string fileName, string &name, string &extension, char separator = '.');
double getFileModificationDate(string path);
string normalizePath(string base, string file);
bool listFolder(string path, vector<string> &result,
		bool normalizeAllPaths = true, bool includeFolders = false,
		bool recursive = true);
/*resultType 0 only file, 1 only dir,2 all*/
bool listFolder1(string path, vector<string> &result,
		bool normalizeAllPaths = true, int resultType = 0,
		bool recursive = true);
bool moveFile(string src, string dst);
bool isAbsolutePath(string &path);
void installSignal(int sig, SignalFnc pSignalFnc);
void installQuitSignal(SignalFnc pQuitSignalFnc);
void installConfRereadSignal(SignalFnc pConfRereadSignalFnc);
#define getutctime() time(NULL)
time_t getlocaltime();
time_t gettimeoffset();
void GetFinishedProcesses(vector<pid_t> &pids, bool &noMorePids);
bool LaunchProcess(string fullBinaryPath, vector<string> &arguments, vector<string> &envVars, pid_t &pid);
int u2g(const char *inbuf, size_t inlen, char *outbuf, size_t outlen) ;
int g2u(const char *inbuf, size_t inlen, char *outbuf, size_t outlen);

string u2g(string src);
string g2u(string src);
string create_uuid();
string from_unix_time(time_t time,const char* format="%Y-%m-%d %H:%M:%S");
//ªÒ»°Œ¢√Î  1/1000000
long get_utime();



#endif
