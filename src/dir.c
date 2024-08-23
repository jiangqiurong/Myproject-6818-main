#include "dir.h"

// 查找目标后缀名文件并生成链表
KernelNode_t *dir_fileToList(const char *dir, const char *suffix)
{
    KernelNode_t *list = list_init();

    // 打开目录
    DIR *dirp = opendir(dir);
    if (dirp == NULL)
    {
        log_error("open dir error");
        perror("open dir error");
        return NULL;
    }

    // 检索目标文件
    while (1)
    {
        struct dirent *dirInfo = readdir(dirp);
        if (dirInfo == NULL)
        {
            log_debug("read dir completed\n");
            break;
        }

        // 查找所有符合目标后缀名的普通文件
        if (dirInfo->d_type == DT_REG)
        {
            if (strstr(dirInfo->d_name + strlen(dirInfo->d_name) - strlen(suffix), suffix))
            {
                log_debug("%s", dirInfo->d_name);
                ElemType_t newNode;
                strcpy(newNode.fileName, dirInfo->d_name);
                list_insert_tail(list, newNode);
            }
        }
    }

    // list_show(list);

    // 关闭目录
    closedir(dirp);

    return list;
}
