#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

// 递归删除文件夹
bool removeDirectory(const char *path) {
    if (path == NULL || strlen(path) <= 0)
    {
        printf("path is NULL or empty\n");
        return -2;
    }

    DIR *dir = opendir(path);
    if (dir == nullptr) {
        // 打开目录失败
        return false;
    }

    dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // 跳过当前目录和父目录
            continue;
        }

        // 构造文件/文件夹的完整路径
        std::string fullPath = std::string(path) + "/" + entry->d_name;

        if (entry->d_type == DT_DIR) {
            // 如果是文件夹，递归删除
            if (!removeDirectory(fullPath.c_str())) {
                closedir(dir);
                return false;
            }
        } else {
            // 如果是文件，直接删除
            if (remove(fullPath.c_str()) != 0) {
                closedir(dir);
                return false;
            }
        }
    }

    // 关闭目录流
    closedir(dir);

    // 删除当前目录
    if (rmdir(path) != 0) {
        return false;
    }

    return true;
}

int main() {
    const char *path = "./dir/";
	//const char *path = "";
    if (removeDirectory(path)) {
        std::cout << "成功删除文件夹：" << path << std::endl;
    } else {
        std::cout << "删除文件夹失败：" << path << std::endl;
    }
    return 0;
}

