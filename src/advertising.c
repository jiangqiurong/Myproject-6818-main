#include "draw.h"
#include "advertising.h"

bool advertising()
{

    int next = 0;
    // 显示图片界面
    bmp_show("./res/ad/photo4.bmp", 0, 0);

    int x, y;

    while (1)
    {
        touch_click(&x, &y); // 获取触摸屏的坐标

        if (x > 0 && x < 267 && y > 0 && y < 240) // 手动下一张
        {
            next++;
            next = next % 4;
            printf("下一张！\n");
            if (next == 2)
            {
                bmp_show("./res/ad/photo2.bmp", 0, 0);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 3);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 3)
            {
                bmp_show("./res/ad/photo3.bmp", 0, 0);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 4);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 1)
            {
                bmp_show("./res/ad/photo1.bmp", 0, 0);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 2);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 0)
            {
                bmp_show("./res/ad/photo4.bmp", 0, 0);
            }
        }

        if (x > 267 && x < 533 && y > 0 && y < 240) // 自动下一张
        {
            for (int photosize = 0; photosize < 4; photosize++)
            {
                next++;
                next = next % 4;
                printf("下一张！\n");
                if (next == 2)
                {
                    bmp_show("./res/ad/photo2.bmp", 0, 0);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);

                    sleep(2);
                }
                else if (next == 3)
                {
                    bmp_show("./res/ad/photo3.bmp", 0, 0);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);

                    sleep(2);
                }
                else if (next == 1)
                {
                    bmp_show("./res/ad/photo1.bmp", 0, 0);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);

                    sleep(2);
                }
                else if (next == 0)
                {
                    bmp_show("./res/ad/photo4.bmp", 0, 0);
                    sleep(2);
                }
            }
            // ShowBmp(0, 0, "./rec/photo4.bmp");
        }

        if (x > 0 && x < 267 && y > 240 && y < 480) // 手动下一张渐进
        {
            next++;
            next = next % 4;
            printf("下一张！\n");
            if (next == 2)
            {

                bmp_show_gradually("./res/ad/photo2.bmp", 0, 0, 1);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 3)
            {

                bmp_show_gradually("./res/ad/photo3.bmp", 0, 0, 1);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 1)
            {

                bmp_show_gradually("./res/ad/photo1.bmp", 0, 0, 1);
                lcd_msg *lcd = Lcd_Init();
                Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
            }

            else if (next == 0)
            {

                bmp_show_gradually("./res/ad/photo4.bmp", 0, 0, 1);
            }
        }

        if (x > 267 && x < 533 && y > 240 && y < 480) // 自动下一张渐进
        {
            for (int photosize = 0; photosize < 4; photosize++)
            {
                next++;
                next = next % 4;
                printf("下一张！\n");
                if (next == 2)
                {

                    bmp_show_gradually("./res/ad/photo2.bmp", 0, 0, 1);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                }

                else if (next == 3)
                {

                    bmp_show_gradually("./res/ad/photo3.bmp", 0, 0, 1);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                }

                else if (next == 1)
                {

                    bmp_show_gradually("./res/ad/photo1.bmp", 0, 0, 1);
                    lcd_msg *lcd = Lcd_Init();
                    Lcd_ShowFont(lcd, 20, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/23", getColor(0, 0, 0, 0), 0, 0, 0, 0);
                }
                sleep(2);
            }
            bmp_show_gradually("./res/ad/photo4.bmp", 0, 0, 1);
        }

        if (x > 533 && x < 800 && y > 0 && y < 480) // 退出按钮
        {
            log_debug("退出按键");
            return false;
        }
    }
}