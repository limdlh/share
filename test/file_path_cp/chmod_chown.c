#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>

int Webconfigchown(const char * dirname, const char * username, const char * groupname)
{
    uid_t uid = 99;
    gid_t gid = 99;
    struct passwd *pwd;
    struct group  *grp;

    if(!dirname || !username || !groupname)
    {
        printf("Webconfig input dirname or username or groupname NULL\n");
        return -1;
    }

    pwd = getpwnam(username);
    if (pwd == NULL) 
    {
        printf("Webconfig get user nobody id failed errno, errmsg \n");
        return -1;
    }
    uid = pwd->pw_uid;
	printf("uid: %d\n", uid);
    
    grp = getgrnam(groupname);
    if (grp == NULL) 
    {
        printf("Webconfig get group nogroup id failed errno, errmsg \n");
        return -1;
    }
    gid = grp->gr_gid;
	printf("gid: %d\n", gid);

    if (chown(dirname, uid, gid) == -1) 
    {
        printf("Webconfig chown %s id failed errno, errmsg\n", dirname);
        return -1;
    }

    return 1;
}

int main(int argc, const char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "%s <filename> <mode>\n", argv[0]);
        exit(1);
    }

    //int mode = strtol(argv[2], NULL, 8);
	int mode = strtol("715", NULL, 8);
	//int mode = 777;
    int ret = chmod(argv[1], mode);
    if(ret == -1)
    {
        perror("chmod");
        exit(1);
    }

    //ret = chown(argv[1], "nobody", "nogroup");
    //ret = chown(argv[1], 65534, 65534);
    ret = chown(argv[1], 99, 99);
    if(ret == -1)
    {
        perror("chown");
        exit(1);
    }
	
	Webconfigchown("./dir/", "nobody", "nogroup");

    return 0;
}


