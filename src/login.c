#include "login.h"

// 登录注册模块
bool login_main()
{
    // 获取用户信息 存到用户链表
    KernelNode_t *userList = login_userToList();
    // list_show(userList);

loginOrRegister:
    bmp_show("./res/bg.bmp", 0, 0);
    bmp_showScale("./res/login/loginbtn.bmp", 300, 150, 2);
    bmp_showScale("./res/login/out.bmp", 350, 350, 2);

    // 判断登录还是注册
    while (1)
    {
        int x, y;
        touch_click(&x, &y);

        // 登录
        if (x >= 310 && x <= 380 && y >= 170 && y <= 270)
        {
            if (!login_signIn(userList))
            {
                // 登录信息错误
                goto loginOrRegister;
            }
            // 登录信息正确
            list_destroy(userList);
            return true;
        }
        // 注册
        else if (x >= 420 && x <= 490 && y >= 170 && y <= 270)
        {
            // 无论注册是否成功都回到登录注册选项界面
            login_register(userList);
            goto loginOrRegister;
        }
        // 退出软件
        else if (x >= 360 && x <= 450 && y >= 360 && y <= 478)
        {
            bmp_show("./res/login/exit.bmp", 0, 0);
            return false;
        }
    }
}

// 获取用户信息 存到用户链表
KernelNode_t *login_userToList()
{
    // 打开用户信息文件
    FILE *fp = fopen("./lib/user.txt", "a+b");
    if (fp == NULL)
    {
        log_error("fopen user.txt error");
        perror("open user.txt error");
        return NULL;
    }

    // 新建用于存放用户信息的链表
    KernelNode_t *userList = list_init();

    // 读取文件内用户信息，存到链表中
    while (1)
    {
        char user[10] = {0};
        char password[10] = {0};
        int ret = fscanf(fp, "user:%s password:%s\n", user, password);
        if (ret == 0)
        {
            return userList;
        }

        ElemType_t data;
        memset(&data, 0, sizeof(data));
        strcpy(data.user, user);
        strcpy(data.password, password);

        list_insert_tail(userList, data);

        // 读到末尾，关闭文件，将用户链表返回
        if (feof(fp))
        {
            fclose(fp);
            return userList;
        }
    }
}

bool login_signIn(KernelNode_t *userList)
{
    // 获取登录账号
    char accIn[10] = {0};
    if (touch_keyboardGetNum(accIn, KEYBOARD_ACCCOUNT_IN) == -1)
    {
        return false;
    }

    // 获取登录密码
    char pwdIn[10] = {0};
    if (touch_keyboardGetNum(pwdIn, KEYBOARD_PASSWORD_IN) == -1)
    {
        return false;
    }

    // 遍历用户信息，判断登录信息是否正确
    KernelNode_t *p = list_entry(userList->list.next, KernelNode_t, list);
    while (1)
    {
        if (!strcmp(accIn, p->data.user) && !strcmp(pwdIn, p->data.password))
        {
            bmp_show("./res/login/success.bmp", 280, 200);
            strcpy(curUser, accIn);
            sleep(1);
            return true;
        }

        if (p->list.next == &userList->list)
        {
            bmp_show("./res/login/pwdwrong.bmp", 280, 200);
            sleep(1);
            return false;
        }
        p = list_entry(p->list.next, KernelNode_t, list);
    }
}

bool login_register(KernelNode_t *userList)
{
    // 获取注册账号
    char accIn[10] = {0};
    if (touch_keyboardGetNum(accIn, KEYBOARD_ACCCOUNT_IN) == -1)
    {
        return false;
    }
    if (strcmp(accIn, "") == 0)
    {
        bmp_show("./res/login/accnull.bmp", 230, 200);
        sleep(1);
        return false;
    }

    // 遍历用户信息，判断账号是否已存在
    KernelNode_t *p = list_entry(userList->list.next, KernelNode_t, list);
    while (1)
    {
        if (!strcmp(accIn, p->data.user))
        {
            bmp_show("./res/login/accexist.bmp", 260, 200);
            sleep(1);
            return false;
        }

        if (p->list.next == &userList->list)
        {
            break;
        }
        p = list_entry(p->list.next, KernelNode_t, list);
    }

    // 获取注册密码
    char pwdIn[10] = {0};
    if (touch_keyboardGetNum(pwdIn, KEYBOARD_PASSWORD_IN) == -1)
    {
        return false;
    }
    if (strcmp(pwdIn, "") == 0)
    {
        bmp_show("./res/login/pwdnull.bmp", 230, 200);
        sleep(1);
        return false;
    }

    // 将注册的信息存到用户链表
    ElemType_t data;
    strcpy(data.user, accIn);
    strcpy(data.password, pwdIn);

    if (strlen(data.user) == 0 || strlen(data.password) == 0)
    {
        printf("空信息\n");
        while (1)
            ;
    }

    list_insert_tail(userList, data);

    // 将注册信息写入user.txt文件
    FILE *fp = fopen("./lib/user.txt", "a+b");
    if (fp == NULL)
    {
        log_error("fopen user.txt error");
        perror("open user.txt error");
        return false;
    }
    fprintf(fp, "user:%s password:%s\n", accIn, pwdIn);

    fclose(fp);

    // 注册成功
    bmp_show("./res/login/accsuccess.bmp", 280, 200);
    sleep(1);

    return true;
}
