#include "sport.h"
#include "cover.h"
#include "time.h"

// 广告进程
void *advertising1(void *arg)
{

    lcd_msg *lcd = Lcd_Init();
    int i = 0;

    while (1)
    {

        if (i == 1700)
        {
            i = 0;
        }
        Lcd_ShowFont_bmp(lcd, 35, "./res/sport/ad.bmp", 800 - i, 0, "消费者如对所购买的保健食品质量安全有质疑，或发现存在虚假宣传等违法行为，请及时向当地市场监管部门举报，也可拨打投诉举报电话：12315。", getColor(0, 255, 0, 0), 0, 0, 440, 0);
        usleep(1);
        i++;
    }
}

bool sport()
{
    int nbastat = 0;
    void *lcd_mbuf = lcd_init1();
    // 显示图片界面
    bmp_show("./res/sport/bg.bmp", 0, 0);
    lcd_msg *lcd = Lcd_Init();

    Lcd_ShowFont(lcd, 30, 800, 100, getColor(0, 0, 0, 0), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/22", getColor(255, 255, 255, 255), 0, 0, 0, 0);

    Lcd_ShowFont(lcd, 30, 50, 50, getColor(0, 0, 0, 0), 0, 0, "退出", getColor(255, 255, 255, 255), 0, 700, 31, 0);
    Lcd_ShowFont(lcd, 70, 400, 130, getColor(0, 0, 0, 0), 0, 0, "2024 巴黎奥运会\n中国队   瑞典队", getColor(255, 255, 255, 255), 0, 215, 95, 0);

    pthread_t displaytime_lcd_thread; // 创建广告显示子线程
    pthread_create(&displaytime_lcd_thread, NULL, advertising1, lcd);

    Lcd_ShowFont(lcd, 45, 45, 45, getColor(0, 0, 0, 0), 0, 0, "0", getColor(0, 255, 0, 0), 0, 395, 275, 0); // 场次
    Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, "0", getColor(0, 255, 0, 0), 0, 250, 230, 0); // 左得分
    Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, "0", getColor(0, 255, 0, 0), 0, 480, 230, 0); // 右得分
    Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, "0", getColor(0, 255, 0, 0), 0, 230, 310, 0); // 左犯规
    Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, "0", getColor(0, 255, 0, 0), 0, 530, 310, 0); // 右犯规

    struct TriggerRegion
    {
        int x_min;
        int x_max;
        int y_min;
        int y_max;
    };

    // 触发区域的定义
    // 参数说明（按钮起始x，按钮x范围加上按钮大小范围x，按钮其实y，按钮y范围加上按钮大小范围y）

    /******左得分模块*************/
    // struct TriggerRegion left_button_add1 = {289, 289 + 40, 307, 307 + 25};
    struct TriggerRegion left_button_add1 = {100, 160, 135, 165};

    /******右得分模块*************/
    // struct TriggerRegion right_button_add1 = {469, 469 + 40, 307, 307 + 25};
    struct TriggerRegion right_button_add1 = {645, 705, 125, 155};

    struct TriggerRegion left_foul = {215, 295, 375, 420}; // 犯规

    /******右暂停/犯规模块*************/
    // struct TriggerRegion right_pause = {640, 64660,0 + 50, 198, 198 + 25};
    struct TriggerRegion right_foul = {510, 600, 370, 415};

    /******场次模块*************/
    struct TriggerRegion bar = {379, 379 + 40, 399, 399 + 40};

    /******比赛结束模块*************/
    struct TriggerRegion endnba = {350, 350 + 100, 341, 341 + 45};

    /******开始/暂停模块*************/
    struct TriggerRegion nbabegin = {340, 340 + 120, 198, 198 + 50};

    int touch_fd = open("/dev/input/event0", O_RDONLY);
    if (touch_fd == -1)
    {
        perror("touch event open failed !\n");
    }

    struct input_event touch_buf;
    int x = -1;
    int y = -1;

    // 各按钮标志位
    int lpause = 1;
    int lfoul = 1;
    int rpause = 1;
    int rfoul = 1;

    int lall = 0;
    int rall = 0;

    int zxjbar = 1;

    while (1)
    {
        int next = 0;
        int x, y;
        touch_click(&x, &y);

        printf("x: %d y: %d\n", x, y);
        // 清空本次点击坐标,保证每次数据都是最新值
        // 340, 340 + 120, 198, 198 + 50
        if (x >= nbabegin.x_min && x <= nbabegin.x_max &&
            y >= nbabegin.y_min && y <= nbabegin.y_max)
        {
            printf("比赛开始/暂停！ \n");

            if (nbastat == 0)
            {
                nbastat = 1;
            }
            else
            {
                nbastat = 0;
            }
        }
        // 379, 379 + 40, 399, 399 + 40,场次
        else if ((x >= bar.x_min && x <= bar.x_max &&
                  y >= bar.y_min && y <= bar.y_max))
        {

            char str[1];
            sprintf(str, "%d", zxjbar);
            Lcd_ShowFont(lcd, 45, 45, 45, getColor(0, 0, 0, 0), 0, 0, " ", getColor(0, 255, 0, 0), 0, 395, 275, 0);

            // Lcd_ShowFont(lcd, 45, 800, 480, getColor(255, 255, 255, 255), 395, 305, "", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            Lcd_ShowFont(lcd, 45, 45, 45, getColor(0, 0, 0, 0), 0, 0, str, getColor(0, 255, 0, 0), 0, 395, 275, 0);
            // Lcd_ShowFont(lcd, 50, 800, 200, getColor(255, 255, 255, 255), 700, 30, "下滑\n退出", getColor(0, 0, 0, 0), 0, 0, 0, 0);

            zxjbar++;
        }
        // 100, 160, 135, 165
        else if (x >= left_button_add1.x_min && x <= left_button_add1.x_max &&
                 y >= left_button_add1.y_min && y <= left_button_add1.y_max)
        {

            lall += 1;
            // printf("左队得分1分！ \n");
            printf("lall =  %d \n", lall);
            // if (lall != 0)
            // {
            //     int ret = bmp_show("./res/sport/black.bmp", 394, 280);
            // }
            char str[1];
            sprintf(str, "%d", lall);
            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, " ", getColor(0, 255, 0, 0), 0, 250, 230, 0);

            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, str, getColor(0, 255, 0, 0), 0, 250, 230, 0);
        }
        // 645, 705, 125, 155
        else if (x >= right_button_add1.x_min && x <= right_button_add1.x_max &&
                 y >= right_button_add1.y_min && y <= right_button_add1.y_max)
        {
            // printf("右队得分1分！ \n");
            printf("rall =  %d \n", rall);
            rall += 1;
            // if (rall != 0)
            // {
            //     int ret = mmap_draw(lcd_mbuf, "bmp/rfen.bmp", 640, 132);
            // }
            char str[1];
            sprintf(str, "%d", rall);
            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, " ", getColor(0, 255, 0, 0), 0, 480, 230, 0);

            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, str, getColor(0, 255, 0, 0), 0, 480, 230, 0); // 右得分
        }
        // 215, 295, 375, 420
        else if (x >= left_foul.x_min && x <= left_foul.x_max &&
                 y >= left_foul.y_min && y <= left_foul.y_max)
        {
            // printf("左犯规！ \n\n");
            // if (lfoul != 0)
            // {
            // int ret = mmap_draw(lcd_mbuf, "bmp/all.bmp", 115, 235);
            // }
            char str[1];
            sprintf(str, "%d", lfoul);
            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, " ", getColor(0, 255, 0, 0), 0, 230, 310, 0);
            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, str, getColor(0, 255, 0, 0), 0, 230, 310, 0);
            nbastat = 0;
            lfoul++;
        }
        // 510, 600, 370, 415
        else if (x >= right_foul.x_min && x <= right_foul.x_max &&
                 y >= right_foul.y_min && y <= right_foul.y_max)
        {
            // printf("右犯规！ \n\n");
            // if (rfoul != 0)
            // {
            // int ret = mmap_draw(lcd_mbuf, "bmp/rfoul.bmp", 715, 235);
            // }
            char str[1];
            sprintf(str, "%d", rfoul);
            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, " ", getColor(0, 255, 0, 0), 0, 530, 310, 0);

            Lcd_ShowFont(lcd, 60, 60, 60, getColor(0, 0, 0, 0), 0, 0, str, getColor(0, 255, 0, 0), 0, 530, 310, 0);
            nbastat = 0;
            rfoul++;
        }

        // 退出按键
        if (x >= 701 && x <= 800 && y >= 0 && y <= 100)
        {
            log_debug("退出按键");
            int ret = pthread_cancel(displaytime_lcd_thread);
            if (ret != 0)
            {
                perror("pthread_cancel error");
                return -1;
            }

            void *result;
            ret = pthread_join(displaytime_lcd_thread, &result);
            if (ret != 0)
            {
                perror("pthread_join error");
                return -1;
            }
            return false;
        }
    }
    return false;
}
