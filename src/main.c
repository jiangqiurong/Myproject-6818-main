#include "cover.h"
#include <time.h>
// 时间子进程函数
void *Timer_Before(void *arg)
{
    while (1)
    {
        time_t tm = time(NULL);
        struct tm *lTime = localtime(&tm);
        char tbuf[1024] = {0};
        sprintf(tbuf, "%02d:%02d:%02d", lTime->tm_hour, lTime->tm_min, lTime->tm_sec);
        // 调用字库显示时间
        bitmap *map = char2rgb(tbuf, 24, 140, 30, 0x00000000, 0, 0, 0xffffffff);
        // lcd_msg *lcd = Lcd_Init();
        // Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 500, 0, tbuf, getColor(0, 0, 0, 0), 0, 0, 0, 0);

        draw_lcd_font((LcdDevice *)arg, 665, 0, map);
        destroytruetype(map);
        sleep(1);
    }
}
int main(int argc, char const *argv[])
{
    // app初始化
    app_run();

    // lcd初始化
    void *lcd = lcd_init1();
    pthread_t displaytime_lcd_thread; // 创建时间显示子线程
    pthread_create(&displaytime_lcd_thread, NULL, Timer_Before, lcd);
    while (1)
    {

        // 登录注册模块
        if (login_main() == false)
        {
            break;
        }

        // 主界面
        draw_main();
    }

    // 退出app
    app_quit();

    return 0;
}
