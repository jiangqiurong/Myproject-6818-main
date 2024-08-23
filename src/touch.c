#include "touch.h"

// 触摸屏初始化
void touch_init()
{
    touchFd = open("/dev/input/event0", O_RDONLY);
    if (touchFd == -1)
    {
        log_error("open touch error");
        return;
    }

    log_debug("touch init completed");
}

// 触摸屏销毁
void touch_destroy()
{
    close(touchFd);

    log_debug("touch destroy completed");
}

// 返回点击坐标
void touch_click(int *posX, int *posY)
{
    while (1)
    {
        read(touchFd, &touchInfo, sizeof(touchInfo));

        touch_get_coordinate(*posX, *posY);

        if (touchInfo.type == EV_KEY &&
            touchInfo.code == BTN_TOUCH &&
            touchInfo.value == 0)
        {
            break;
        }
    }
}

// 获取键盘数字
int touch_keyboardGetNum(char *str, Keyboard_Flag_t flag)
{
    int numY, numCnt;
    numCnt = 0;
    numY = 105;

    // 画板
    lcd_from_prev_to(lcdBuf, lcdTempBuf);

keyboardMain:
    lcd_from_prev_to(lcdTempBuf, lcdBuf);
    bmp_showScale("./res/touch/keyboard.bmp", 250, 68, 1);

    switch (flag)
    {
    case KEYBOARD_ACCCOUNT_IN:
        bmp_showScale("./res/touch/accin.bmp", 292, 25, 1);
        break;
    case KEYBOARD_PASSWORD_IN:
        bmp_showScale("./res/touch/pwdin.bmp", 292, 25, 1);
        break;
    case KEYBOARD_FILENAME_IN:
        bmp_showScale("./res/touch/filenamein.bmp", 292, 25, 1);
        break;
    }

    // 打开文字库
    font *f = font_init();
    while (1)
    {
        // 设置文字属性
        Font_t fontp = {
            .bgColor = FONT_WHITE,
            .bgHeight = 30,
            .bgWidth = 30,
            .fontColor = FONT_BLACK,
            .fontSize = 30,
            .offsetX = 7,
            .offsetY = 0,
            .str = "*"};
        bitmap *bm = font_set(f, fontp);

        int x, y;

        touch_click(&x, &y);

        // 输入密码显示的位置
        int numX[6] = {300, 335, 370, 405, 440, 475};

        // 判断按下的数字，将其刷在输入框，密码模式显示为*
        // 按下1
        if (x >= 251 && x <= 349 && y >= 286 && y <= 353)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "1");
                bm = font_set(f, fontp);
            }
            log_debug("1");
            char num[2];
            sprintf(num, "%d", 1);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下2
        else if (x >= 351 && x <= 449 && y >= 286 && y <= 353)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "2");
                bm = font_set(f, fontp);
            }
            log_debug("2");
            char num[2];
            sprintf(num, "%d", 2);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下3
        else if (x >= 451 && x <= 549 && y >= 286 && y <= 353)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "3");
                bm = font_set(f, fontp);
            }
            log_debug("3");
            char num[2];
            sprintf(num, "%d", 3);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下4
        else if (x >= 251 && x <= 349 && y >= 219 && y <= 285)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "4");
                bm = font_set(f, fontp);
            }
            log_debug("4");
            char num[2];
            sprintf(num, "%d", 4);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下5
        else if (x >= 351 && x <= 449 && y >= 219 && y <= 285)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "5");
                bm = font_set(f, fontp);
            }
            log_debug("5");
            char num[2];
            sprintf(num, "%d", 5);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下6
        else if (x >= 451 && x <= 549 && y >= 219 && y <= 285)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "6");
                bm = font_set(f, fontp);
            }
            log_debug("6");
            char num[2];
            sprintf(num, "%d", 6);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下7
        else if (x >= 251 && x <= 349 && y >= 151 && y <= 217)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "7");
                bm = font_set(f, fontp);
            }
            log_debug("7");
            char num[2];
            sprintf(num, "%d", 7);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下8
        else if (x >= 351 && x <= 449 && y >= 151 && y <= 217)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "8");
                bm = font_set(f, fontp);
            }
            log_debug("8");
            char num[2];
            sprintf(num, "%d", 8);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下9
        else if (x >= 451 && x <= 549 && y >= 151 && y <= 217)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "9");
                bm = font_set(f, fontp);
            }
            log_debug("9");
            char num[2];
            sprintf(num, "%d", 9);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下回退键
        else if (x >= 251 && x <= 349 && y >= 355 && y <= 421)
        {

            log_debug("return");
            if (strlen(str) > 0)
            {
                numCnt--;
                strcpy(fontp.str, " ");
                bm = font_set(f, fontp);
                font_show(bm, numX[numCnt], numY);
                str[strlen(str) - 1] = '\0';
            }
        }
        // 按下0
        else if (x >= 351 && x <= 449 && y >= 355 && y <= 421)
        {
            if (numCnt > 5)
            {
                continue;
            }
            if (strlen(str) == 6)
            {
                continue;
            }
            if (INPUT_VISIBLE(flag))
            {
                strcpy(fontp.str, "0");
                bm = font_set(f, fontp);
            }
            log_debug("0");
            char num[2];
            sprintf(num, "%d", 0);
            strcat(str, num);
            font_show(bm, numX[numCnt], numY);
            numCnt++;
        }
        // 按下确认键
        else if (x >= 451 && x <= 549 && y >= 355 && y <= 421)
        {
            log_debug("confirm");
            if (strlen(str) == 0)
            {
                switch (flag)
                {
                case KEYBOARD_ACCCOUNT_IN:
                    bmp_show("./res/touch/accnull.bmp", 230, 200);
                    sleep(1);
                    break;
                case KEYBOARD_PASSWORD_IN:
                    bmp_show("./res/touch/pwdnull.bmp", 230, 200);
                    sleep(1);
                    break;
                case KEYBOARD_FILENAME_IN:
                    bmp_show("./res/touch/filenamenull.bmp", 230, 200);
                    sleep(1);
                    break;
                }
                goto keyboardMain;
            }
            free(bm);
            fontUnload(f);
            break;
        }
        // 键盘区域外
        else if ((x >= 0 && x < 240) || (x >= 560 && x < 800))
        {
            log_debug("关闭键盘");
            return -1;
        }
    }
}
