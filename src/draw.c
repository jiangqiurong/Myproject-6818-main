#include "cover.h"

void draw_main()
{

drawMain:

    // 读取前清空上一次读取到的信息
    memset(&touchInfo, 0, sizeof(touchInfo));

    bmp_show("./res/bg.bmp", 0, 0);
    bmp_showScale("./res/ad/adbtn.bmp", 200, 150, 2);
    bmp_showScale("./res/draw/drawbtn.bmp", 300, 150, 2);
    bmp_showScale("./res/draw/out.bmp", 350, 350, 2);
    bmp_showScale("./res/sport/sportbtn.bmp", 500, 150, 2);

    strcpy(drawCanvasName, "./res/draw/whitecanvas.bmp");

    lcd_msg *lcd = Lcd_Init();
    Lcd_ShowFont(lcd, 30, 800, 100, getColor(255, 255, 255, 255), 100, 0, "天气:微雨  空气质量:优  时间:2024/8/22\n作者:@2427 江秋荣", getColor(0, 0, 0, 0), 0, 0, 0, 0);

    Lcd_ShowFont(lcd, 25, 800, 480, getColor(255, 255, 255, 255), 10, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 255, 0, 0), 0, 0, 0, 0);
    Lcd_ShowFont(lcd, 25, 800, 480, getColor(255, 255, 255, 255), 610, 90, "公告模块：\n消费者如对所购买的\n保健食品质量安全有\n质疑，或发现存在虚\n假宣传等违法行为，\n请及时向当地市场监\n管部门举报，也可拨\n打投诉举报电话：\n12315。", getColor(0, 255, 0, 0), 0, 0, 0, 0);

    while (1)
    {
        int x, y;
        touch_click(&x, &y);

        // 广告模块
        if (x >= 200 && x <= 329 && y >= 170 && y <= 270)
        {
            // drawCanvas:
            log_debug("广告");
            if (advertising() == false)
            {
                log_debug("返回主界面");
                goto drawMain;
            }
        }

        // 画图模块
        if (x >= 300 && x <= 390 && y >= 170 && y <= 270)
        {
        drawCanvas:
            log_debug("画图");
            if (draw_layout() == false)
            {
                log_debug("返回主界面");
                goto drawMain;
            }
        }
        // 2048模块
        else if (x >= 420 && x <= 500 && y >= 170 && y <= 270)
        {
            log_debug("2048");
            if (game() == false)
            {
                log_debug("返回主界面");
                goto drawMain;
            }
            /*
            log_debug("查看");
            if (draw_check() == false)
            {
                log_debug("返回主界面");
                goto drawMain;
            }
            else
            {
                goto drawCanvas;
            }
            */
        }
        // 体育
        if (x >= 501 && x <= 580 && y >= 170 && y <= 270)
        {

            log_debug("体育");
            if (sport() == false)
            {
                log_debug("返回主界面");
                goto drawMain;
            }
        }
        // 退出登录
        else if (x >= 360 && x <= 450 && y >= 360 && y <= 478)
        {
            log_debug("退出登录");
            return;
        }
    }
}

// 查看模块
/*
bool draw_check()
{
    char filePath[256] = {0};
    sprintf(filePath, "./res/users/%s/", curUser);

    KernelNode_t *bmpList = dir_fileToList(filePath, "%s.bmp");

    // 不存在用户目录即还没保存过图片
    if (bmpList == NULL)
    {
        bmp_show("./res/draw/nocanvas.bmp", 250, 150);
        sleep(1);

        return false;
    }

    // 存在用户目录但目录里没有文件
    if (list_empty(&bmpList->list))
    {
        bmp_show("./res/draw/nocanvas.bmp", 250, 150);
        sleep(1);

        return false;
    }

    bmp_show("./res/draw/checklayout.bmp", 0, 0);

    KernelNode_t *p = list_entry(bmpList->list.next, KernelNode_t, list);
    while (1)
    {
        int x, y;
        char fileName[256] = {0};

        sprintf(fileName, "%s/%s", filePath, p->data.fileName);

        bmp_show(fileName, 100, 50);

        touch_click(&x, &y);

        // 下一张画布
        if (x > 730 && x <= 780 && y > 220 && y <= 270)
        {
            log_debug("下一张");
            p = list_entry(p->list.next, KernelNode_t, list);
            // 下一张如果是没有存图像数据的头节点，直接跳到头节点的下一张
            if (p == bmpList)
            {
                p = list_entry(p->list.next, KernelNode_t, list);
            }
        }
        // 上一张画布
        else if (x > 20 && x <= 70 && y > 220 && y <= 270)
        {
            log_debug("上一张");
            p = list_entry(p->list.prev, KernelNode_t, list);
            // 上一张如果是没有存图像数据的头节点，直接跳到头节点的上一张
            if (p == bmpList)
            {
                p = list_entry(p->list.prev, KernelNode_t, list);
            }
        }
        // 退出查看模块
        else if (x > 730 && x <= 780 && y > 50 && y <= 80)
        {
            log_debug("退出查看模块");
            list_destroy(bmpList);
            return false;
        }
        // 点击图片，进入画图模块对图片进行编辑
        else if (x > 120 && x <= 680 && y > 70 && y <= 410)
        {
            log_debug("进入画图模块进行编辑");
            strcpy(drawCanvasName, fileName);
            return true;
        }
        // 删除
        else if (x > 740 && x <= 780 && y > 410 && y <= 470)
        {
            log_debug("删除");
            log_debug("当前.bmp文件：%s", fileName);
            // 弹出删除确认框前记录lcd状态
            lcd_from_prev_to(lcdBuf, lcdTempBuf);
            // 弹出删除确认框
            bmp_show("./res/draw/delete.bmp", 250, 150);
            while (1)
            {
                touch_click(&x, &y);

                // 取消
                if (x > 470 && x <= 520 && y > 250 && y <= 300)
                {
                    // 恢复lcd之前的状态
                    lcd_from_prev_to(lcdTempBuf, lcdBuf);
                    break;
                }
                // 确认
                else if (x > 300 && x <= 350 && y > 250 && y <= 300)
                {
                    KernelNode_t *delNode = p;
                    p = list_entry(p->list.next, KernelNode_t, list);
                    list_del(&delNode->list);

                    // 删除的是链表里最后一张图片
                    if (p == bmpList)
                    {
                        // 删完了
                        if (list_empty(&bmpList->list))
                        {
                            remove(fileName);

                            list_destroy(bmpList);

                            bmp_show("./res/draw/checklayout.bmp", 0, 0);
                            bmp_show("./res/draw/nocanvas.bmp", 250, 150);
                            sleep(1);

                            return false;
                        }

                        // 没删完，指向链表里第一张图片
                        p = list_entry(p->list.next, KernelNode_t, list);
                    }

                    // 删除本地文件
                    remove(fileName);

                    // 恢复lcd之前的状态
                    lcd_from_prev_to(lcdTempBuf, lcdBuf);

                    break;
                }
            }
        }
    }
}
*/
// 绘画工作区
bool draw_layout()
{
    bmp_show("./res/draw/drawlayout.bmp", 0, 0);
    bmp_show(drawCanvasName, 100, 0);
    bmp_showScale("./res/draw/select.bmp", 720, 300, 3);
    bmp_showScale("./res/draw/black.bmp", 730, 420, 2);
    bmp_showScale("./res/draw/middlepen.bmp", 25, 420, 2);

    // 初始化画笔属性
    draw_set_draw_pen(PEN_MODE_BRUSH,
                      PEN_COLOR_BLACK,
                      PEN_STRETCH_SHAPE_CIRCLE,
                      PEN_PIXEL_MIDDLE);

    // 读取前清空上一次读取到的信息
    memset(&touchInfo, 0, sizeof(touchInfo));

    int x, y;

    while (1)
    {
        touch_click(&x, &y);

        // 画布区域
        if (DRAW_DRAWABLE_AREA)
        {
        drawableArea:
            log_debug("画布区域");
            bmp_showScale("./res/draw/drawable.bmp", 720, 300, 3);

            // 在画画模式点击了画布外的区域，退出画画模式，进入选择模式
            if (draw_drawableArea() == false)
            {
                goto nonDrawableArea;
            }
        }
        // 非画布区域
        else if (DRAW_NON_DRAWABLE_AREA)
        {
        nonDrawableArea:
            log_debug("非画布区域");
            bmp_showScale("./res/draw/nondrawable.bmp", 720, 300, 3);

            Btn_NonDrawableArea_t btnValue = draw_nonDrawableArea();

            // 点击了退出按钮，退出绘画工作区
            if (btnValue == BTN_EXIT)
            {
                return false;
            }
            // 在选择模式点击了画布区域，退出选择模式，进入画画模式
            else if (btnValue == BTN_DRAWABLE_AREA)
            {
                goto drawableArea;
            }
        }
    }
}

// 画布区域
bool draw_drawableArea()
{
    // 读取前清空上一次读取到的信息
    memset(&touchInfo, 0, sizeof(touchInfo));

    switch (drawPen.drawMode)
    {
    // 笔刷模式
    case PEN_MODE_BRUSH:
    {
        log_debug("brush mode");
        int x, y;

        while (1)
        {
            read(touchFd, &touchInfo, sizeof(touchInfo));

            // 触摸屏发生坐标事件就记录坐标
            touch_get_coordinate(x, y)

                // 笔刷画图
                if (touchInfo.type == EV_SYN && touchInfo.code == SYN_REPORT)
            {
                int radius = drawPen.drawPixel / 2;
                draw_create_circle(x, y, radius);
            }

            // 按下画布以外的区域返回false
            if (touchInfo.type == EV_KEY && touchInfo.code == BTN_TOUCH && touchInfo.value > 0)
            {
                if (DRAW_NON_DRAWABLE_AREA)
                {
                    return false;
                }
            }
        }
    }
    // 拉伸模式
    case PEN_MODE_STRETCH:
    {
        log_debug("stretch mode");
        int x, y, startX, startY, endX, endY;

        while (1)
        {
            read(touchFd, &touchInfo, sizeof(touchInfo));

            touch_get_coordinate(x, y);

            // 记录按下的坐标
            if (touchInfo.type == EV_KEY && touchInfo.code == BTN_TOUCH && touchInfo.value > 0)
            {
                startX = x;
                startY = y;
                log_debug("按下(%d,%d)", startX, startY);
            }

            // 记录松开的坐标
            if (touchInfo.type == EV_KEY && touchInfo.code == BTN_TOUCH && touchInfo.value == 0)
            {
                endX = x;
                endY = y;
                log_debug("松开(%d,%d)", endX, endY);

                // 计算出拉伸区域的宽高
                unsigned int drawWidth, drawHeight, offsetX, offsetY;
                drawWidth = (endX > startX) ? (endX - startX) : (startX - endX);
                drawHeight = (endY > startY) ? (endY - startY) : (startY - endY);
                offsetX = (endX > startX) ? startX : endX;
                offsetY = (endY > startY) ? startY : endY;
                log_debug("width:%d height:%d", drawWidth, drawHeight);

                // 在拉伸区域内形成形状
                if (drawWidth != 0 && drawHeight != 0)
                {
                    switch (drawPen.drawStretchShape)
                    {
                    // 圆形
                    case PEN_STRETCH_SHAPE_CIRCLE:
                    {
                        // 计算圆心坐标和圆半径
                        int posX = drawWidth / 2 + offsetX;
                        int posY = drawHeight / 2 + offsetY;
                        int radius = (drawWidth > drawHeight) ? drawWidth / 2 : drawHeight / 2;
                        log_debug("x:%d y:%d r:%d", posX, posY, radius);

                        // 形成圆形
                        draw_create_circle(posX, posY, radius);

                        break;
                    }
                    // 矩形
                    case PEN_STRETCH_SHAPE_RECTANGLE:
                    {
                        // 计算矩形中心点和矩形宽和高
                        int posX = drawWidth / 2 + offsetX;
                        int posY = drawHeight / 2 + offsetY;
                        int width = drawWidth;
                        int height = drawHeight;
                        log_debug("x:%d y:%d width:%d height:%d", posX, posY, width, height);

                        // 形成矩形
                        draw_create_rectangle(posX, posY, width, height);

                        break;
                    }
                    // 三角形--算法有缺陷
                    case PEN_STRETCH_SHAPE_TRIANGLE:
                    {
                        // 计算三角形底边中点坐标和三角形底和高
                        int posX = drawWidth / 2 + offsetX;
                        int posY = drawHeight / 2 + offsetY;
                        int width = drawWidth;
                        int height = drawHeight;
                        log_debug("x:%d y:%d width:%d height:%d", posX, posY, width, height);

                        for (int drawY = posY - 1, i = 0; drawY >= posY - height; drawY--, i++)
                        {
                            for (int drawX = posX - 1; drawX >= offsetX + i; drawX--)
                            {
                                if (drawX < 100 || drawX >= 700 || drawY > 380)
                                {
                                    continue;
                                }
                                lcdBuf[drawY][drawX] = drawPen.drawColor;
                            }

                            for (int drawX = posX; drawX < width + offsetX - i; drawX++)
                            {
                                if (drawX < 100 || drawX >= 700 || drawY > 380)
                                {
                                    continue;
                                }
                                lcdBuf[drawY][drawX] = drawPen.drawColor;
                            }
                        }

                        break;
                    }
                    }
                }
            }

            // 按下画布以外的区域返回false
            if (touchInfo.type == EV_KEY && touchInfo.code == BTN_TOUCH && touchInfo.value > 0)
            {
                if (DRAW_NON_DRAWABLE_AREA)
                {
                    return false;
                }
            }
        }
    }
    }
}

// 非画布区域
Btn_NonDrawableArea_t draw_nonDrawableArea()
{
    // 读取前清空上一次读取到的信息
    memset(&touchInfo, 0, sizeof(touchInfo));

    int x, y;

    while (1)
    {
        touch_click(&x, &y);
        Btn_NonDrawableArea_t btnValue = draw_nonDrawableAreaBtn(&x, &y);
        if (btnValue == BTN_EXIT)
        {
            return BTN_EXIT;
        }
        else if (btnValue == BTN_DRAWABLE_AREA)
        {
            return BTN_DRAWABLE_AREA;
        }
    }
}

Btn_NonDrawableArea_t draw_nonDrawableAreaBtn(int *x, int *y)
{
    // 退出按键
    if (*x >= 701 && *x < 800 && *y >= 0 && *y < 100)
    {
        log_debug("退出按键");
        return BTN_EXIT;
    }
    // 保存按键
    else if (*x >= 701 && *x < 800 && *y >= 101 && *y < 265)
    {
        log_debug("保存按键");
        draw_saveToBmp();
        return BTN_SAVE;
    }
    // 蓝色按键
    else if (*x >= 626 && *x < 700 && *y >= 381 && *y < 480)
    {
        log_debug("蓝色按键");
        bmp_showScale("./res/draw/blue.bmp", 730, 420, 2);
        drawPen.drawColor = PEN_COLOR_BLUE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_BLUE;
    }
    // 绿色按键
    else if (*x >= 550 && *x < 625 && *y >= 381 && *y < 480)
    {
        log_debug("绿色按键");
        bmp_showScale("./res/draw/green.bmp", 730, 420, 2);
        drawPen.drawColor = PEN_COLOR_GREEN;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_GREEN;
    }
    // 红色按键
    else if (*x >= 475 && *x < 550 && *y >= 381 && *y < 480)
    {
        log_debug("红色按键");
        bmp_showScale("./res/draw/red.bmp", 730, 420, 2);
        drawPen.drawColor = PEN_COLOR_RED;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_RED;
    }
    // 黑色按键
    else if (*x >= 380 && *x < 475 && *y >= 381 && *y < 480)
    {
        log_debug("黑色按键");
        bmp_showScale("./res/draw/black.bmp", 730, 420, 2);
        drawPen.drawColor = PEN_COLOR_BLACK;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_BLACK;
    }
    // 橡皮擦按键
    else if (*x >= 290 && *x < 380 && *y >= 381 && *y < 480)
    {
        log_debug("橡皮擦按键");
        bmp_showScale("./res/draw/eraser.bmp", 730, 420, 2);
        drawPen.drawMode = PEN_MODE_BRUSH;
        drawPen.drawColor = PEN_COLOR_WHITE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_ERASER;
    }
    // 大画笔按键
    else if (*x >= 220 && *x < 290 && *y >= 381 && *y < 480)
    {
        log_debug("大画笔按键");
        bmp_showScale("./res/draw/bigpen.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_BRUSH;
        drawPen.drawPixel = PEN_PIXEL_BIG;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_BIG_PEN;
    }
    // 中画笔按键
    else if (*x >= 160 && *x < 220 && *y >= 381 && *y < 480)
    {
        log_debug("中画笔按键");
        bmp_showScale("./res/draw/middlepen.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_BRUSH;
        drawPen.drawPixel = PEN_PIXEL_MIDDLE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_MIDDLE_PEN;
    }
    // 小画笔按键
    else if (*x >= 100 && *x < 160 && *y >= 381 && *y < 480)
    {
        log_debug("小画笔按键");
        bmp_showScale("./res/draw/smallpen.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_BRUSH;
        drawPen.drawPixel = PEN_PIXEL_SMALL;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_LITTLE_PEN;
    }
    // 点击画布区域
    else if (*x >= 100 && *x < 700 && *y >= 0 && *y < 380)
    {
        log_debug("画布区域");
        return BTN_DRAWABLE_AREA;
    }
    // 点击圆形
    else if (*x >= 0 && *x < 100 && *y >= 0 && *y < 100)
    {
        log_debug("圆形按钮");
        bmp_showScale("./res/draw/circle.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_STRETCH;
        drawPen.drawStretchShape = PEN_STRETCH_SHAPE_CIRCLE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_CIRCLE;
    }
    // 点击矩形
    else if (*x >= 0 && *x < 100 && *y >= 100 && *y < 265)
    {
        log_debug("矩形按钮");
        bmp_showScale("./res/draw/rectangle.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_STRETCH;
        drawPen.drawStretchShape = PEN_STRETCH_SHAPE_RECTANGLE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_RECTANGLE;
    }
    // 点击三角形
    else if (*x >= 0 && *x < 100 && *y >= 265 && *y < 380)
    {
        log_debug("三角形按钮");
        bmp_showScale("./res/draw/triangle.bmp", 25, 420, 2);
        drawPen.drawMode = PEN_MODE_STRETCH;
        drawPen.drawStretchShape = PEN_STRETCH_SHAPE_TRIANGLE;
        draw_set_draw_pen(drawPen.drawMode,
                          drawPen.drawColor,
                          drawPen.drawStretchShape,
                          drawPen.drawPixel);
        return BTN_TRIANGLE;
    }
}

// 保存图片模块
int draw_saveToBmp()
{
    int drawAreaWidth = 600;
    int drawAreaHeight = 380;

    char filePath[256] = {0};
    char fileName[256] = {0};
    char saveName[10] = {0};

    // 取消保存
    if (touch_keyboardGetNum(saveName, KEYBOARD_FILENAME_IN) == -1)
    {
        lcd_from_prev_to(lcdTempBuf, lcdBuf);
        return -1;
    }

    lcd_from_prev_to(lcdTempBuf, lcdBuf);

    mkdir("./res/users", S_IRWXU | S_IRWXG | S_IRWXO);

    sprintf(filePath, "./res/users/%s/", curUser);

    mkdir(filePath, S_IRWXU | S_IRWXG | S_IRWXO);

    sprintf(fileName, "%s%s.bmp", filePath, saveName);

    FILE *fp = fopen(fileName, "wb");
    if (fp == NULL)
    {
        perror("fopen test.bmp error");
        log_error("fopen test.bmp error");
        return -1;
    }

    // 文件头
    struct tagBITMAPFILEHEADER bfh;
    memset(&bfh, 0, sizeof(bfh));

    bfh.bfType = 0x4d42;
    bfh.bfSize = 54 + drawAreaWidth * drawAreaHeight * 4;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = 54;

    // 文件信息头
    struct tagBITMAPINFOHEADER bih;
    memset(&bih, 0, sizeof(bih));

    bih.biSize = sizeof(bih);
    bih.biWidth = drawAreaWidth;
    bih.biHeight = drawAreaHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = drawAreaWidth * drawAreaHeight * 4;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    // 先写入头信息数据
    fwrite(&bfh, sizeof(bfh), 1, fp);
    fwrite(&bih, sizeof(bih), 1, fp);

    // 再写入颜色数据
    // argb转成bgr，从图片底部开始写入数据
    for (int y = drawAreaHeight - 1; y >= 0; y--)
    {
        for (int x = 100; x < drawAreaWidth + 100; x++)
        {
            unsigned char b = lcdBuf[y][x] & 0xff;
            unsigned char g = (lcdBuf[y][x] & 0xff00) >> 8;
            unsigned char r = (lcdBuf[y][x] & 0xff0000) >> 16;
            unsigned char pixelData[3] = {b, g, r};
            fwrite(&pixelData, sizeof(pixelData), 1, fp);
        }
    }

    bmp_show("./res/draw/savesuccess.bmp", 280, 200);
    sleep(1);

    lcd_from_prev_to(lcdTempBuf, lcdBuf);

    fclose(fp);
}
