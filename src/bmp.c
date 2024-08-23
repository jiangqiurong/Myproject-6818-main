#include "bmp.h"
#include "lcd.h"

// 显示图片界面
int bmp_show(const char *bmpFile, int posX, int posY)
{
    // 打开bmp文件
    int bmpFd = open(bmpFile, O_RDONLY);
    if (bmpFd == -1)
    {
        log_error("open bmp error");
        return -1;
    }

    struct tagBITMAPFILEHEADER bfh;
    read(bmpFd, &bfh, sizeof(bfh));
    struct tagBITMAPINFOHEADER bih;
    read(bmpFd, &bih, sizeof(bih));

    int bmpWidth = bih.biWidth;
    int bmpHeight = bih.biHeight;

    if (bmpWidth % 4 != 0)
    {
        log_error("image width is not a multiple of 4");
        return -1;
    }

    char bmpBuf[bmpWidth * bmpHeight * 3];
    memset(bmpBuf, 0, bmpWidth * bmpHeight * 3);

    int tempBuf[bmpHeight][bmpWidth];
    memset(*tempBuf, 0, bmpWidth * bmpHeight * 3);

    // 从bmp中读取数据
    read(bmpFd, bmpBuf, bmpWidth * bmpHeight * 3);

    for (int i = 0, j = 0; i < bmpWidth * bmpHeight; i++, j += 3)
    {
        (*tempBuf)[i] = 0x00 << 24 | bmpBuf[j + 2] << 16 | bmpBuf[j + 1] << 8 | bmpBuf[j];
    }

    // 把处理后的图像数据存到lcd虚拟内存
    for (int row = 0; row < bmpHeight; row++)
    {
        for (int column = 0; column < bmpWidth; column++)
        {
            lcdBuf[row + posY][column + posX] = tempBuf[bmpHeight - row][column];
        }
    }

    close(bmpFd);

    log_debug("show %s completed", bmpFile);
}

// 按倍率显示图片
int bmp_showScale(const char *bmpFile, int posX, int posY, int scale)
{
    // 打开bmp文件
    int bmpFd = open(bmpFile, O_RDONLY);
    if (bmpFd == -1)
    {
        log_error("open bmp error");
        return -1;
    }

    struct tagBITMAPFILEHEADER bfh;
    read(bmpFd, &bfh, sizeof(bfh));
    struct tagBITMAPINFOHEADER bih;
    read(bmpFd, &bih, sizeof(bih));

    int bmpWidth = bih.biWidth;
    int bmpHeight = bih.biHeight;

    if (bih.biWidth % 4 != 0)
    {
        log_error("image width is not a multiple of 4");
        return -1;
    }

    char bmpBuf[bmpWidth * bmpHeight * 3];
    memset(bmpBuf, 0, bmpWidth * bmpHeight * 3);

    int tempBuf[bmpHeight][bmpWidth];
    memset(*tempBuf, 0, bmpWidth * bmpHeight * 3);

    // 从bmp中读取数据
    read(bmpFd, bmpBuf, bmpWidth * bmpHeight * 3);

    for (int i = 0, j = 0; i < bmpWidth * bmpHeight; i++, j += 3)
    {
        (*tempBuf)[i] = 0x00 << 24 | bmpBuf[j + 2] << 16 | bmpBuf[j + 1] << 8 | bmpBuf[j];
    }

    int scaleWidth, scaleHeight;
    scaleWidth = bmpWidth / scale;
    scaleHeight = bmpHeight / scale;

    // 把处理后的图像数据存到lcd虚拟内存
    for (int row = 0; row < scaleHeight; row++)
    {
        for (int column = 0; column < scaleWidth; column++)
        {
            lcdBuf[row + posY][column + posX] = tempBuf[(scaleHeight - row) * scale][column * scale];
        }
    }

    close(bmpFd);

    log_debug("show scaled %s completed", bmpFile);
}
// 显示bmp图片
void bmp_display(const char *filename, int x0, int y0)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        // printf("failed to open %s\n", filename);
        return;
    }

    int width, height;
    short depth;

    // width
    lseek(fd, 0x12, SEEK_SET);
    read(fd, &width, 4);

    // height
    lseek(fd, 0x16, SEEK_SET);
    read(fd, &height, 4);

    // depth
    lseek(fd, 0x1c, SEEK_SET);
    read(fd, &depth, 2);

    // printf("%d * %d \n", width, height);
    // printf("depth: %d\n", depth);

    if (!(depth == 24 || depth == 32))
    {
        printf("Sorry, Non-standard Bmp\n");
        return;
    }

    //
    int total_bytes;      // 图片中整个像素数组的字节数
    int line_bytes_valid; // 每一行所占的像素字节数
    int line_bytes;       // 一行所占的字节数
    int laizi = 0;        // 每一行末尾赖子数量

    line_bytes_valid = abs(width) * (depth / 8);

    if (line_bytes_valid % 4)
    {
        laizi = 4 - line_bytes_valid % 4;
    }
    line_bytes = line_bytes_valid + laizi;

    total_bytes = line_bytes * abs(height);

    unsigned char *pixel = malloc(total_bytes);
    lseek(fd, 54, SEEK_SET);
    read(fd, pixel, total_bytes);

    int i = 0;
    unsigned char a, r, g, b;
    int color;

    int x, y;

    for (y = 0; y < abs(height); y++)
    {
        // 这一行的第x点的解析
        for (x = 0; x < abs(width); x++)
        {
            b = pixel[i++];
            g = pixel[i++];
            r = pixel[i++];

            if (depth == 32)
            {
                a = pixel[i++];
            }
            else
            {
                a = 0;
            }
            color = (a << 24) | (r << 16) | (g << 8) | (b);
            lcd_draw_point(x0 + x, y0 + abs(height) - 1 - y, color);
        }
        i = i + laizi;
    }

    free(pixel);

    close(fd);
}

// 图片渐变
int bmp_show_gradually(const char *bmpFile, int posX, int posY, int delay)
{
    // 打开bmp文件
    int bmpFd = open(bmpFile, O_RDONLY);
    if (bmpFd == -1)
    {
        log_error("open bmp error");
        return -1;
    }

    struct tagBITMAPFILEHEADER bfh;
    read(bmpFd, &bfh, sizeof(bfh));
    struct tagBITMAPINFOHEADER bih;
    read(bmpFd, &bih, sizeof(bih));

    int bmpWidth = bih.biWidth;
    int bmpHeight = bih.biHeight;

    if (bmpWidth % 4 != 0)
    {
        log_error("image width is not a multiple of 4");
        close(bmpFd); // 记得关闭文件
        return -1;
    }

    char bmpBuf[bmpWidth * bmpHeight * 3];
    memset(bmpBuf, 0, bmpWidth * bmpHeight * 3);

    int tempBuf[bmpHeight][bmpWidth];
    memset(*tempBuf, 0, bmpWidth * bmpHeight * 3);

    // 从bmp中读取数据
    read(bmpFd, bmpBuf, bmpWidth * bmpHeight * 3);

    for (int i = 0, j = 0; i < bmpWidth * bmpHeight; i++, j += 3)
    {
        (*tempBuf)[i] = 0x00 << 24 | bmpBuf[j + 2] << 16 | bmpBuf[j + 1] << 8 | bmpBuf[j];
    }

    // 把处理后的图像数据按行逐渐存到lcd虚拟内存
    for (int row = 0; row < bmpHeight; row++)
    {
        for (int column = 0; column < bmpWidth; column++)
        {
            lcdBuf[row + posY][column + posX] = tempBuf[bmpHeight - row - 1][column];
        }

        // 每显示一行，暂停一会儿实现渐进效果
        usleep(delay * 1000); // delay in milliseconds
    }

    close(bmpFd);

    log_debug("show %s completed", bmpFile);
    return 0;
}
