#include "lcd.h"

// lcd初始化
LcdDevice *lcd_init1()
{
    LcdDevice *lcd = malloc(sizeof(LcdDevice));
    // 1、打开液晶屏
    lcd->fd = open("/dev/fb0", O_RDWR | O_EXCL);
    if (lcd->fd == -1)
    {
        perror("open lcd error");
        return NULL;
    }
    // 2、获取液晶屏设备的硬件参数
    if (ioctl(lcd->fd, FBIOGET_VSCREENINFO, &(lcd->varinfo)) != 0)
    {
        perror("获取LCD设备可变属性信息失败");
        return NULL;
    }
    // 800   480
    // printf("[可见区分辨率]: %d×%d\n", lcd->varinfo.xres, lcd->varinfo.yres);
    // printf("[虚拟区分辨率]: %d×%d\n", lcd->varinfo.xres_virtual, lcd->varinfo.yres_virtual);
    // printf("[色深]: %d bits\n", lcd->varinfo.bits_per_pixel); // 32位   转成4个字节 32/8
    // 获得当前显卡所支持的虚拟区显存大小
    lcd->lw = lcd->varinfo.xres;
    lcd->lh = lcd->varinfo.yres;
    lcd->pertype = lcd->varinfo.bits_per_pixel;
    lcd->area = 0;
    unsigned long screen_size = lcd->lw * lcd->lh * lcd->pertype / 8;
    // 3、进行内存映射
    lcd->lcd_p = mmap(NULL, 2 * screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd->fd, 0);
    bzero(lcd->lcd_p, 2 * screen_size);

    if (lcd->lcd_p == MAP_FAILED)
    {
        perror("mmap error");
        return NULL;
    }

    return lcd;
}
// 初始化液晶屏
void lcd_init()
{
    lcdFd = open("/dev/fb0", O_RDWR);
    if (lcdFd == -1)
    {
        log_error("open lcd error");
        return;
    }

    if (ioctl(lcdFd, FBIOGET_VSCREENINFO, &lcdVarInfo) != 0)
    {
        perror("获取lcd信息失败");
        log_error("get lcd info failed");
        close(lcdFd);
        return;
    }

    lcdWidth = lcdVarInfo.xres;
    lcdHeight = lcdVarInfo.yres;

    lcdBuf = (int(*)[lcdWidth])mmap(NULL,
                                    lcdWidth * lcdHeight * sizeof(int),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    lcdFd,
                                    0);
    if (lcdBuf == MAP_FAILED)
    {
        log_error("mmap lcd failed");
        return;
    }

    lcdTempBuf = (int(*)[lcdWidth])malloc(lcdWidth * lcdHeight * sizeof(int));
    if (lcdTempBuf == NULL)
    {
        perror("tempBuf malloc error");
        log_error("tempBuf malloc error");
        return;
    }

    lcdOverBuf = (int(*)[lcdWidth])malloc(lcdWidth * lcdHeight * sizeof(int));
    if (lcdOverBuf == NULL)
    {
        perror("tempBuf malloc error");
        log_error("tempBuf malloc error");
        return;
    }

    log_debug("lcd init completed");
}

// 液晶屏销毁
void lcd_destroy()
{
    munmap(lcdBuf, lcdWidth * lcdHeight * sizeof(int));
    free(lcdTempBuf);
    free(lcdOverBuf);
    close(lcdFd);

    log_debug("lcd destroy completed");
}

/**
  * @brief  显示字到开发板屏幕上
  * @note   None
  * @param  lcd：			显示屏的结构体指针
            f_s：			字体大小
            bm_w：			画板的宽
            bm_h：			画板的高
            bm_c：			画板的颜色
            f_x：			字体在画板的x轴位置
            f_y： 			字体在画板的y轴位置
            buf：			需要显示的字
            f_c： 			字体的颜色
            f_lw：			字体显示在行的宽度，超过则换行，一般写0，默认不换行
            lcd_x：			画板和字形成的bmp图片，在显示屏的x轴的显示位置
            lcd_y：			画板和字形成的bmp图片，在显示屏的y轴的显示位置
            lcd_effect：	画板和字形成的bmp图片，在显示屏的显示效果
  * @retval None
 */

void Lcd_ShowFont(lcd_msg *lcd, int f_s, int bm_w, int bm_h, color bm_c, int f_x, int f_y, char *font_text, color f_c, int f_lw, int lcd_x, int lcd_y, int lcd_effect)
{
    // 1、打开字体
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

    // 2、字体大小的设置
    fontSetSize(f, f_s);

    // 3、创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit(bm_w, bm_h, 4, bm_c);
    // A, R, G, B，画板的颜色
    // 4、将字写到画板(点阵图)上，形成一张bmp图片
    fontPrint(f, bm, f_x, f_y, font_text, f_c, f_lw);
    // A, R, G, B，字的颜色
    // 5、将形成的bmp图片，显示到开发板lcd设备上
    Lcd_ShowFontBmpToLcd(lcd, bm, lcd_x, lcd_y, lcd_effect); // 此处的特效，需要适配

    // 6、关闭字库、画板
    fontUnload(f);
    destroyBitmap(bm);
}
/**
  * @brief  显示字到开发板屏幕上
  * @note   None
  * @param  lcd：			显示屏的结构体指针
            f_s：			字体大小
            bmp_buf：		bmp图片路径
            f_x：			字体在bmp的x轴位置
            f_y： 			字体在bmp的y轴位置
            font_text：		需要显示的字
            f_c： 			字体的颜色
            f_lw：			字体显示在行的宽度，超过则换行，一般写0，默认不换行
            lcd_x：			画板和字形成的bmp图片，在显示屏的x轴的显示位置
            lcd_y：			画板和字形成的bmp图片，在显示屏的y轴的显示位置
            lcd_effect：	画板和字形成的bmp图片，在显示屏的显示效果
  * @retval None
 */
void Lcd_ShowFont_bmp(lcd_msg *lcd, int f_s, char *bmp_path, int f_x, int f_y, char *font_text, color f_c, int f_lw, int lcd_x, int lcd_y, int lcd_effect)
{
    // 1、打开字体
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

    // 2、字体大小的设置
    fontSetSize(f, f_s);

    // 3、创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit_bmp(bmp_path);

    // A, R, G, B，画板的颜色
    // 4、将字写到画板(点阵图)上，形成一张bmp图片
    fontPrint(f, bm, f_x, f_y, font_text, f_c, f_lw);
    // A, R, G, B，字的颜色
    // 5、将形成的bmp图片，显示到开发板lcd设备上
    Lcd_ShowFontBmpToLcd(lcd, bm, lcd_x, lcd_y, lcd_effect); // 此处的特效，需要适配

    // 6、关闭字库、画板
    fontUnload(f);
    destroyBitmap(bm);
}

/**
 * @brief  初始化显示屏文件
 * @note   None
 * @param  None
 * @retval 返回lcd信息结构体指针
 */
lcd_msg *Lcd_Init(void)
{
    // 1、初始化信息
    lcd_msg *lcd = malloc(sizeof(lcd_msg));
    if (lcd != NULL)
    {
        // 2、填写显示屏结构体初始化信息
        lcd->devpath = LCD_DEVPATH;
        lcd->fd = -1;
        lcd->p = NULL;
        lcd->tmp_p = NULL;

        // 3、打开文件(lcd文件)(以读写的形式打开)
        lcd->fd = open(lcd->devpath, O_RDWR);
        if (lcd->fd == -1)
        {
            perror("打开lcd文件失败！\n");

            return (lcd_msg *)-1;
        }

        // 4、申请一块大小为800*480*4字节的映射内存，并将之与文件lcd_fd相关联
        lcd->p = mmap(
            NULL,                   // 默认写NULL，内核会自动帮你获取合适的虚拟内存起始地址
            800 * 480 * 4,          // 开发板lcd的虚拟内存的大小(800*480*4个字节)
            PROT_READ | PROT_WRITE, // 此虚拟内存空间(可读可写，注意你打开的文件权限也必须是可读可写)
            MAP_SHARED,             // 允许其他进程访问此空间(也可以直接作用于实际的lcd硬件中)
            lcd->fd,                // 打开的文件描述符
            0                       // 申请的虚拟空间的起始位置
        );

        if (lcd->p == (void *)-1)
        {
            perror("映射lcd设备失败！\n");
            return (lcd_msg *)-2;
        }
    }

    // 5、成功返回此结构体指针
    return lcd;
}

/**
 * @brief  显示颜色块
 * @note   None
 * @param  lcd：		显示屏的结构体指针
 *        	color：     显示的颜色(0x00000000)ARGB
 * 		lcd_x：		颜色块在lcd屏幕上的x轴显示位置
 * 		lcd_y：		颜色块在lcd屏幕上的y轴显示位置
 *	 		color_w：	颜色块宽度
 * 		color_h：	颜色块长度
 * 		mode：		颜色块的显示特效
 * @retval None
 */
void Lcd_ShowColor(lcd_msg *lcd, int color, int lcd_x, int lcd_y, int color_w, int color_h, int mode)
{
    // 1、记住指针指向这个虚拟内存初始的位置
    lcd->tmp_p = lcd->p;

    // 2、指定颜色块在lcd屏幕的哪个位置上显示
    lcd->p = lcd->p + lcd_x;       // x轴
    lcd->p = lcd->p + lcd_y * 800; // y轴

    // 3、操作指针lcd->p，来对lcd屏幕(虚拟内存),写入颜色数据
    int i, j;

    switch (mode)
    {
    case 0: // 正常显示色块
        for (i = 0; i < color_w; i++)
        {
            for (j = 0; j < color_h; j++)
            {
                *(lcd->p + 800 * j + i) = color; // 将其写入的颜色，局限在方块中
            }
        }
        break;

    case 1: // 从左到右显示色块
        for (i = 0; i < color_w; i++)
        {
            usleep(3000);
            for (j = 0; j < color_h; j++)
            {
                *(lcd->p + 800 * j + i) = color; // 将其写入的颜色，局限在方块中
            }
        }
        break;
    }
    // 4、将原先指向这个虚拟内存初始位置的指针，重新赋值回来
    lcd->p = lcd->tmp_p;
}

/**
 * @brief  显示bmp图片数据
 * @note   专门给字库用的
 * @param  lcd：		显示屏的结构体指针
 *        	bm          字库位图(底板)
 * 		lcd_x：		颜色块在lcd屏幕上的x轴显示位置
 * 		lcd_y：		颜色块在lcd屏幕上的y轴显示位置
 * 		mode：		颜色块的显示特效
 * @retval 成功：返回0
 *         失败：返回负整数
 */
int Lcd_ShowFontBmpToLcd(lcd_msg *lcd, bitmap *bm, int lcd_x, int lcd_y, int mode)
{
    // 0、记住最初的位置
    int(*p)[800] = (int(*)[800])lcd->p;

    // 1、判断要刷的图片是否超过了屏幕范围
    int w = bm->width;
    int h = bm->height;
    int n = bm->byteperpixel;
    unsigned char *bmp_buf = bm->map;

    if (w + lcd_x > 800 || h + lcd_y > 480)
    {
        printf("bmp图片越界!\n");
        return -1;
    }
    // printf("w = %d\n", w);
    // printf("h = %d\n", h);

    // 2、将获取的BGR数据(bmp的颜色数据)，转换成ARGB数据(lcd数据)
    unsigned int tmp_buf[w * h]; // ARGB数据(lcd数据)存放地(变长数组，不能赋值)
    unsigned int lcd_buf[w * h]; // ARGB数据(lcd数据)存放地(变长数组，不能赋值)

    int i, j, k, x, y;

    // 正的的lcd数据
    for (i = 0; i < w * h; i++)
    {
        tmp_buf[i] = (bmp_buf[i * 4 + 0] << 16) | (bmp_buf[i * 4 + 1] << 8) | (bmp_buf[i * 4 + 2] << 0) | (bmp_buf[i * 4 + 3] << 24);
        // ARGB            R                     G                      B                 A
    }

    // 上下翻转颠倒的lcd数据
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            lcd_buf[y * w + x] = tmp_buf[(h - 1 - y) * w + x]; // (二维数组)
        }
    }

    // 3、显示bmp图片到lcd设备上
    switch (mode)
    {
    case 0: // 没有显示效果，直接显示
        for (y = 0; y < h; y++)
        {
            for (x = 0; x < w; x++)
            {
                unsigned int alpha = lcd_buf[(h - 1 - y) * w + x] & 0xff000000;

                if (alpha == 0xff000000) // 透明度为 0 不渲染
                {
                    continue;
                }
                //*(p+800*(y+lcd_y)+x+lcd_x)= lcd_buf[(h-1-y)*w+x];//把图片像素填充到显存中去，让它在LCD屏幕上显示出来
                p[y + lcd_y][x + lcd_x] = lcd_buf[(h - 1 - y) * w + x];
            }
        }
        break;

    case 1: // 从上到下的显示效果

        for (y = 0; y < h; y++)
        {
            for (x = 0; x < w; x++)
            {
                unsigned int alpha = lcd_buf[(h - 1 - y) * w + x] & 0xff000000;

                if (alpha == 0xff000000) // 透明度为 0 不渲染
                {
                    continue;
                }

                // *(fbp+800*(y+lcd_y)+x+lcd_x)= lcd_buf[(h-1-y)*w+x];//把图片像素填充到显存中去，让它在LCD屏幕上显示出来
                p[y + lcd_y][x + lcd_x] = lcd_buf[(h - 1 - y) * w + x];
            }
            usleep(3000); // 停顿3毫秒，让我们能够观察到图片是如何在LCD上面显示出来的
        }
        break;

    case 2: // 从左往右的显示效果

        for (x = 0; x < w; x++)
        {
            for (y = 0; y < h; y++)
            {
                unsigned int alpha = lcd_buf[(h - 1 - y) * w + x] & 0xff000000;

                if (alpha == 0xff000000) // 透明度为 0 不渲染
                {
                    continue;
                }
                // *(fbp+800*(y+lcd_y)+x+lcd_x)= lcd_buf[(h-1-y)*w+x];//把图片像素填充到显存中去，让它在LCD屏幕上显示出来
                p[y + lcd_y][x + lcd_x] = lcd_buf[(h - 1 - y) * w + x];
            }
            usleep(2000); // 停顿2毫秒，让我们能够观察到图片是如何在LCD上面显示出来的
        }
        break;

    case 3: // 从下到上：
        for (y = 0; y < h; y++)
        {
            for (x = 0; x < w; x++)
            {
                unsigned int alpha = lcd_buf[y * w + x] & 0xff000000;

                if (alpha == 0xff000000) // 透明度为 0 不渲染
                {
                    continue;
                }
                // *(fbp + (h-y-1+lcd_y)*800 + lcd_x + x) = lcd_buf[y*w+x];//把图片像素填充到显存中去，让它在LCD屏幕上显示出来
                p[h - y - 1 + lcd_y][x + lcd_x] = lcd_buf[y * w + x];
            }
            usleep(3000); // 停顿3毫秒，让我们能够观察到图片是如何在LCD上面显示出来的
        }
        break;

    case 4: // 从右到左：

        for (x = 0; x < w; x++)
        {
            for (y = 0; y < h; y++)
            {
                unsigned int alpha = lcd_buf[(h - 1 - y) * w + w - x - 1] & 0xff000000;

                if (alpha == 0xff000000) // 透明度为 0 不渲染
                {
                    continue;
                }
                // *(fbp + (y+lcd_y)*800 + lcd_x + w - x - 1) = lcd_buf[(h-1-y)*w+w - x - 1];
                p[y + lcd_y][lcd_x + w - x - 1] = lcd_buf[(h - 1 - y) * w + w - x - 1];
            }
            usleep(2000); // 时间停顿，显示刷的效果
        }
        break;

    case 5: // 横百叶窗

        // int k;
        for (x = 0; x < (w / 8); x++)
        {
            for (y = 0; y < h; y++)
            {
                for (k = 0; k < 8; k++)
                {
                    unsigned int alpha = lcd_buf[(h - 1 - y) * w + x + k * w / 8] & 0xff000000;

                    if (alpha == 0xff000000) // 透明度为 0 不渲染
                    {
                        continue;
                    }
                    // *(fbp+800*(y+lcd_y)+x+lcd_x+k*w/8)= lcd_buf[(h-1-y)*w+x+k*w/8];
                    p[y + lcd_y][x + lcd_x + k * w / 8] = lcd_buf[(h - 1 - y) * w + x + k * w / 8];
                }
            }
            usleep(4000);
        }
        break;

    case 6: // 竖百叶窗
        // int k;
        for (y = 0; y < (h / 8); y++)
        {
            for (x = 0; x < w; x++)
            {
                for (k = 0; k < 8; k++)
                {
                    unsigned int alpha = lcd_buf[(h - 1 - y - k * h / 8) * w + x] & 0xff000000;

                    if (alpha == 0xff000000) // 透明度为 0 不渲染
                    {
                        continue;
                    }
                    // *(fbp+800*(y+lcd_y+k*h/8)+x+lcd_x)= lcd_buf[(h-1-y-k*h/8)*w+x];
                    p[y + lcd_y + k * h / 8][x + lcd_x] = lcd_buf[(h - 1 - y - k * h / 8) * w + x];
                }
            }
            usleep(3000);
        }
        break;

    case 7: // 圆形收缩
        // int k;
        for (k = h * 975 / 1000; k >= 0; k -= 3)
        {
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    if ((x - w / 2) * (x - w / 2) + (y - h / 2) * (y - h / 2) >= k * k)
                    {
                        unsigned int alpha = lcd_buf[(h - 1 - y) * w + x] & 0xff000000;

                        if (alpha == 0xff000000) // 透明度为 0 不渲染
                        {
                            continue;
                        }
                        // *(fbp+800*(y+lcd_y)+x+lcd_x)= lcd_buf[(h-1-y)*w+x];
                        p[y + lcd_y][x + lcd_x] = lcd_buf[(h - 1 - y) * w + x];
                    }
                }
            }
            usleep(500);
        }
        break;

    case 8: // 圆形扩散
        // int k;
        for (k = 0; k < (h * 975 / 1000); k += 3)
        {
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    if ((x - w / 2) * (x - w / 2) + (y - h / 2) * (y - h / 2) <= k * k)
                    {
                        unsigned int alpha = lcd_buf[(h - 1 - y) * w + x] & 0xff000000;

                        if (alpha == 0xff000000) // 透明度为 0 不渲染
                        {
                            continue;
                        }
                        // *(fbp+800*(y+lcd_y)+x+lcd_x)= lcd_buf[(h-1-y)*w+x];
                        p[y + lcd_y][x + lcd_x] = lcd_buf[(h - 1 - y) * w + x];
                    }
                }
            }
            usleep(500);
        }

        break;
    }

    return 0;
}

/**
 * @brief  释放显示屏文件
 * @note   None
 * @param  lcd：		显示屏的结构体指针
 * @retval None
 */
void Lcd_Release(lcd_msg *lcd)
{
    // 解除映射
    munmap(lcd->p, 800 * 480 * 4);

    // 关闭触摸屏文件
    close(lcd->fd);

    // 释放申请的触摸屏信息的内存
    free(lcd);
}

int lcd_fd = -1;
// 帧缓冲首地址
int *plcd = NULL;

char w_digit[][58] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x1E, 0xF0,
     0x3C, 0x78, 0x38, 0x38, 0x78, 0x3C, 0x78, 0x3C, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x1C,
     0x70, 0x1C, 0x70, 0x1C, 0x70, 0x3C, 0x78, 0x3C, 0x78, 0x38, 0x38, 0x38, 0x3C, 0x70, 0x1E, 0xF0,
     0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80,
     0x1F, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80,
     0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0xC0,
     0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x1C, 0x70,
     0x38, 0x38, 0x78, 0x3C, 0x78, 0x3C, 0x78, 0x3C, 0x38, 0x38, 0x00, 0x78, 0x00, 0x70, 0x00, 0xE0,
     0x01, 0xC0, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00, 0x1C, 0x0C, 0x38, 0x1C, 0x70, 0x3C, 0x7F, 0xF8,
     0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x38, 0xF0,
     0x38, 0x78, 0x78, 0x78, 0x38, 0x78, 0x00, 0x78, 0x00, 0x70, 0x00, 0xE0, 0x07, 0xC0, 0x00, 0xF0,
     0x00, 0x78, 0x00, 0x38, 0x00, 0x3C, 0x00, 0x3C, 0x78, 0x3C, 0x78, 0x38, 0x78, 0x38, 0x38, 0xF0,
     0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE0, 0x01, 0xE0,
     0x03, 0xE0, 0x03, 0xE0, 0x07, 0xE0, 0x0E, 0xE0, 0x0C, 0xE0, 0x1C, 0xE0, 0x18, 0xE0, 0x30, 0xE0,
     0x70, 0xE0, 0x60, 0xE0, 0xFF, 0xFC, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE0, 0x01, 0xF0,
     0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x3F, 0xF8,
     0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x30, 0x00, 0x37, 0xE0, 0x3C, 0x70, 0x38, 0x38,
     0x10, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x78, 0x3C, 0x78, 0x3C, 0x78, 0x38, 0x38, 0x78, 0x38, 0xF0,
     0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x0E, 0x78,
     0x1C, 0x78, 0x38, 0x78, 0x38, 0x00, 0x78, 0x00, 0x70, 0x00, 0x77, 0xE0, 0x7E, 0x78, 0x78, 0x38,
     0x78, 0x3C, 0x70, 0x3C, 0x70, 0x1C, 0x70, 0x1C, 0x78, 0x3C, 0x38, 0x3C, 0x3C, 0x38, 0x1E, 0x70,
     0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x3F, 0xF8,
     0x78, 0x38, 0x70, 0x30, 0x60, 0x60, 0x00, 0x60, 0x00, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x03, 0x80,
     0x03, 0x80, 0x03, 0x80, 0x07, 0x80, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x0F, 0x00, 0x0F, 0x00,
     0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x3C, 0x70,
     0x38, 0x38, 0x70, 0x3C, 0x70, 0x1C, 0x70, 0x1C, 0x38, 0x38, 0x3E, 0x70, 0x0F, 0xE0, 0x1F, 0xE0,
     0x38, 0xF0, 0x70, 0x78, 0x70, 0x3C, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x38, 0x3C, 0x70,
     0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x3C, 0x70,
     0x38, 0x38, 0x78, 0x38, 0x70, 0x3C, 0x70, 0x3C, 0x70, 0x1C, 0x70, 0x3C, 0x70, 0x3C, 0x78, 0x7C,
     0x3C, 0xFC, 0x1F, 0xFC, 0x00, 0x3C, 0x00, 0x38, 0x00, 0x38, 0x38, 0x70, 0x38, 0x70, 0x3D, 0xE0,
     0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

// 打开屏幕
void lcd_open(void)
{
    int fd;
    // fd = open("/dev/ubuntu_lcd",O_RDWR);
    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        printf("sorry,open fail\n");
    }
    printf("open success\n");
    lcd_fd = fd;
    plcd = mmap(NULL, 480 * 800 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (plcd == MAP_FAILED)
    {
        printf("sorry,mmap open fail\n");
    }
}
// 关闭屏幕
void lcd_close(void)
{
    munmap(plcd, 480 * 800 * 4);
    close(lcd_fd);
}
// 描点
void lcd_draw_point(int x, int y, int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + 800 * y + x) = color;
    }
}
// 画矩形
void lcd_draw_rectanle(int x0, int y0, int w, int h, int color)
{
    int i, j;
    for (i = 0; i < w; i++)
    {
        for (j = 0; j < h; j++)
            lcd_draw_point(x0 + i, y0 + j, color);
    }
}
// 清屏
void lcd_clear_screen(int color)
{
    lcd_draw_rectanle(0, 0, 800, 480, color);
}

void lcd_draw_word(char *ch, int len, int w, int color, int x0, int y0)
{
    int i, j;
    int x, y;
    int flag = w / 8;
    // 遍历整个点阵数组
    for (i = 0; i < len; i++)
    {
        for (j = 7; j >= 0; j--)
        {
            // if (ch[i] & (0x80/pow(2,7-j)))
            if ((ch[i] >> j) & 1)
            {
                x = 7 - j + 8 * (i % flag) + x0;
                y = i / flag + y0;
                lcd_draw_point(x, y, color);
            }
        }
    }
}

void lcd_draw_digit(int value, int x0, int y0, int color)
{
    int i = 0;
    if (value == 0)
    {
        lcd_draw_word(w_digit[0], sizeof(w_digit[0]), 16, color, x0 + 48 - 16 * i, y0);
    }

    while (value)
    {
        int x;
        x = value % 10;
        lcd_draw_word(w_digit[x], sizeof(w_digit[x]), 16, color, x0 + 48 - 16 * i, y0);
        value = value / 10;
        i++;
    }
}

void draw_lcd_font(LcdDevice *lcd, int x, int y, bitmap *font)
{
    if (lcd == NULL)
    {
        return;
    }
    for (int j = 0; j < font->height; j++)
    {
        for (int i = 0; i < font->width; i++)
        {
            int bgrIndex = (j * font->width + i) * 4;
            lcd->lcd_p[j + y + (lcd->area * 480)][i + x] = (font->map[bgrIndex + 3] << 24) | (font->map[bgrIndex + 2] << 16) | (font->map[bgrIndex + 1] << 8) | font->map[bgrIndex];
        }
    }
}
