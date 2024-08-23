#include "font.h"
#include "lcd.h"
#include "truetype.h"

/**
      * @brief  创造一个画板(点阵图)(位图)空间
      * @note   1、这个画板不能指定颜色(默认是黑色数据)、图片数据
                2、你可以设置这个画板的宽和高
      * @param  width：        画板的宽
                height：       画板的高
                byteperpixel： 当前屏幕色深(GEC6818lcd屏幕色深为32位，也就是4个字节)
      * @retval 返回画板结构体指针(bitmap结构体)
    */
bitmap *
createBitmap(u32 width, u32 height, u32 byteperpixel)
{
    bitmap *bm = (bitmap *)malloc(sizeof(bitmap));
    bzero(bm, sizeof(bitmap));
    bm->height = height;
    bm->width = width;
    bm->byteperpixel = byteperpixel;
    bm->map = (u8 *)malloc(width * height * byteperpixel);
    bzero(bm->map, width * height * byteperpixel);
    return bm;
}

/**
 * @brief  关闭画板，释放资源
 * @note   None
 * @param  bm：  指向画板结构体的指针
 * @retval None
 */
void destroyBitmap(bitmap *bm)
{
    bzero(bm->map, bm->height * bm->width * bm->byteperpixel);
    free(bm->map);
    bzero(bm, sizeof(bitmap));
    free(bm);
}
/**
  * @brief  获取当前画板的颜色
  * @note   None
  * @param  bm：指向画板结构体的指针
            x： 画板的宽
            y： 画板的高

  * @retval 成功：返回颜色数据(除了黑色以外的数据)
            失败：返回0(即黑色，证明没有上色)
*/
color getPixel(bitmap *bm, u32 x, u32 y)
{
    if (bm->byteperpixel == 3)
    {
        u8 r = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 0);
        u8 g = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 1);
        u8 b = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 2);
        return getColor(0, r, g, b);
    }
    else if (bm->byteperpixel == 4)
    {
        u8 r = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 0);
        u8 g = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 1);
        u8 b = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 2);
        u8 a = *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 3);
        return getColor(a, r, g, b);
    }
    return 0;
}
/**
  * @brief  给画板上色
  * @note   1、只是单纯的颜色底板
            2、你可以根据此函数例子，自己写一个bmp图片底板
  * @param  bm：指向画板结构体的指针
            x： 画板的宽
            y： 画板的高
            c： 颜色数据
  * @retval None
*/
void setPixel(bitmap *bm, u32 x, u32 y, color c)
{
    if (bm->byteperpixel == 3)
    {
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 0) = getR(c);
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 1) = getG(c);
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 2) = getB(c);
    }
    else if (bm->byteperpixel == 4)
    {
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 0) = getR(c);
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 1) = getG(c);
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 2) = getB(c);
        *(bm->map + y * bm->width * bm->byteperpixel + x * bm->byteperpixel + 3) = getA(c);
    }
}
/**
  * @brief  创造一个画板(点阵图)(位图)空间
  * @note   1、这个画板可以设置底板颜色
            2、也可以设置画板的宽和高
  * @param  width：       画板的宽
            height：      画板的高
            byteperpixel：当前屏幕色深(GEC6818lcd屏幕色深为32位，也就是4个字节)
            c：           颜色数据
  * @retval 返回画板结构体指针(bitmap结构体)
*/
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c)
{
    bitmap *bm = createBitmap(width, height, byteperpixel);
    u32 x, y;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            setPixel(bm, x, y, c);
        }
    }
    return bm;
}

/**
  * @brief  获取字符串长度
  * @note   None
  * @param  ws：字符串
  * @retval 成功：字符串长度('\0'之前的数据)
            失败：None
*/
u32 wstrlen(wchar *ws)
{
    u32 len = 0;
    while (*(ws + len) != '\0')
        len++;
    return len;
}

/**
  * @brief  转换字符串字符格式
  * @note   utf8格式转ucs2格式
  * @param  code：需要转换的字符串
  * @retval 成功：ucs2格式的字符串
            失败：None
*/
wchar *utf8_to_ucs2(char *code)
{
    wchar *ucs2 = (wchar *)malloc((strlen(code) + 2) * sizeof(wchar));
    bzero(ucs2, (strlen(code) + 2) * sizeof(wchar));
    s16 com = 1 << 7;
    u32 x;
    for (x = 0; x <= strlen(code); x++)
    {
        char utf = code[x];
        u32 size = 0;
        u32 i = 0;
        u32 index = (utf & com) != 0;
        u16 binary[16];
        if (index == 0)
        { /// 0xxxxxxx ==> 00000000 0xxxxxxxx
            for (; i < 8; ++i)
            {
                binary[i] = 0;
            }
            for (; i < 16; ++i)
            {
                binary[i] = (utf & 1 << (15 - i)) != 0;
            }
        }
        else if (utf & (1 << 5) == 0)
        { // 110xxxxx 10yyyyyy ==> 00000xxx xxyyyyyy
            for (; i < 5; ++i)
            {
                binary[i] = 0;
            }
            for (; i < 10; ++i)
            {
                binary[i] = (utf & (1 << (9 - i))) != 0;
            }
            x += 1;
            utf = code[x];
            for (; i < 16; ++i)
            {
                binary[i] = (utf & (1 << (15 - i))) != 0;
            }
        }
        else
        { // 1110xxxx 10yyyyyy 10zzzzzz ==> xxxxyyyy yyzzzzzz
            for (; i < 4; ++i)
            {
                binary[i] = (utf & 1 << (3 - i)) != 0;
            }
            x += 1;
            utf = code[x];
            for (; i < 10; ++i)
            {
                binary[i] = (utf & 1 << (9 - i)) != 0;
            }
            x += 1;
            utf = code[x];
            for (; i < 16; ++i)
            {
                binary[i] = (utf & 1 << (15 - i)) != 0;
            }
        }
        wchar ch = 0;
        for (i = 0; i < 16; i++)
        {
            ch <<= 1;
            ch |= binary[i];
        }
        u32 len = wstrlen(ucs2);
        ucs2[len] = ch;
        ucs2[len + 1] = 0;
    }
    return ucs2;
}
/**
  * @brief  将字写到画板上
  * @note   None
  * @param  f：       指向字体处理结构体的指针
            screen：  指向画板结构体的指针
            x：       字在画板的x轴位置
            y：       字在画板的y轴位置
            buf:      要显示的字(字符串)
            c：       字体的颜色
            maxWidth：一行显示字体的最大宽度（例如：字体大小为32，假设设置为64则一行显示两个字体，就换行显示）
  * @retval None
*/
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth)
{
    wchar *wText = utf8_to_ucs2(text);
    u8 *charRaster = NULL;
    s32 bx, by, bw, bh;
    s32 ascent, descent, lineGap;
    s32 sx = 0, sy = 0;
    stbtt_GetFontVMetrics(f->info, &ascent, &descent, &lineGap);

    ascent *= f->scale;
    descent *= f->scale;
    lineGap *= f->scale;

    u32 i;
    for (i = 0; i < wstrlen(wText); i++)
    {
        if (wText[i] == '\n')
        {
            sy += ascent - descent + lineGap;
            sx = 0;
            continue;
        }

        stbtt_GetCodepointBitmapBox(f->info, wText[i], f->scale, f->scale, &bx, &by, &bw, &bh);

        s32 charWidth = bw - bx;
        s32 charHeight = bh - by;
        s32 oy = ascent + by;

        if (maxWidth > 0 && sx + charWidth > maxWidth)
        {
            sy += ascent - descent + lineGap;
            sx = 0;
        }

        charRaster = realloc(charRaster, charWidth * charHeight);

        stbtt_MakeCodepointBitmap(f->info, charRaster, charWidth, charHeight, charWidth, f->scale, f->scale, wText[i]);

        s32 advance;
        stbtt_GetCodepointHMetrics(f->info, wText[i], &advance, 0);
        s32 kerning = stbtt_GetCodepointKernAdvance(f->info, wText[i], wText[i + 1]);
        s32 printLength = advance * f->scale + kerning * f->scale;

        s32 mx;
        for (mx = 0; mx < printLength; mx++)
        {
            if (charWidth + mx < printLength - mx)
            {
                continue;
            }
            break;
        }

        s32 ix, iy;
        for (iy = 0; iy < charHeight; iy++)
        {
            for (ix = 0; ix < charWidth; ix++)
            {
                s32 xpos = x + sx + ix + mx; // + (printLength-charWidth)/2;
                s32 ypos = (y + sy + oy + iy) - 1;
                if (charRaster[ix + iy * charWidth] != 0 && xpos < screen->width && ypos < screen->height)
                {
                    u32 alpha = charRaster[ix + iy * charWidth];
                    u32 invAlpha = 255 - alpha;
                    color bgc = getPixel(screen, xpos, ypos);
                    u8 bgr = getR(bgc);
                    u8 bgg = getG(bgc);
                    u8 bgb = getB(bgc);

                    u8 r = (alpha * getR(c) + invAlpha * bgr) >> 8;
                    u8 g = (alpha * getG(c) + invAlpha * bgg) >> 8;
                    u8 b = (alpha * getB(c) + invAlpha * bgb) >> 8;

                    setPixel(screen, xpos, ypos, getColor(0, r, g, b));
                }
            }
        }

        bzero(charRaster, charWidth * charHeight);

        sx += printLength;
    }
    free(charRaster);
    free(wText);
}
/**
  * @brief  字体大小的设置
  * @note   None
  * @param  f：     操作字库的结构体指针(font结构体)
            pixels：字体大小
  * @retval None
*/
void fontSetSize(font *f, s32 pixels)
{
    f->scale = stbtt_ScaleForPixelHeight(f->info, pixels);
}
/**
  * @brief  初始化字库
  * @note   None
  * @param  fontPath：字库的路径
  * @retval 成功：返回字体处理结构体指针(font结构体)
            失败：返回NULL
 */
font *fontLoad(char *fontPath)
{
    // 打开字体文件并读取
    s32 fd = open(fontPath, O_RDONLY);
    if (fd == -1)
        return NULL;
    u32 bufferSize = lseek(fd, 0, SEEK_END);
    u8 *buffer = (u8 *)malloc(bufferSize);
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, bufferSize);
    close(fd);

    // 从内存读取
    font *f = (font *)malloc(sizeof(font));
    f->info = (stbtt_fontinfo *)malloc(sizeof(stbtt_fontinfo));
    if (!buffer || bufferSize == 0)
        return NULL;
    if (stbtt_InitFont(f->info, buffer, 0) != 1)
        return NULL;
    f->buffer = buffer;
    f->scale = stbtt_ScaleForPixelHeight(f->info, 16);

    // 返回
    return f;
}
/**
 * @brief  关闭字库，释放资源
 * @note   None
 * @param  f：       指向字体处理结构体的指针
 * @retval None
 */
void fontUnload(font *f)
{
    free(f->info);
    free(f->buffer);
    free(f);
}

font *font_init()
{
    font *f = fontLoad("./lib/font/simfang.ttf");
    return f;
}

bitmap *font_set(font *f, Font_t fontp)
{
    // 1.初始化字库
    //  注意要先将ttf文件放入开发板上
    //  font *f = fontLoad("./lib/simfang.ttf"); // 指定字库文件，比如simfang.ttf

    // 2.设置字体的大小
    fontSetSize(f, fontp.fontSize);

    // 3.设置字体输出框的大小
    bitmap *bm;
    bm = createBitmapWithInit(fontp.bgWidth, fontp.bgHeight, 4, fontp.bgColor);

    // 4.把字体输出到输出框中
    fontPrint(f, bm, fontp.offsetX, fontp.offsetY, fontp.str, fontp.fontColor, 0);

    // for(int row = 0; row < bm->width; row++)
    // {
    //     for(int column = 0; column < bm->height; column++)
    //     {
    //         int bgrIndex = (row * bm->width + column) * 4;
    //         lcdBuf[row + fontp.posY][column + fontp.posX] = (bm->map[bgrIndex + 3] << 24) |
    //                                                         (bm->map[bgrIndex + 2] << 16) |
    //                                                         (bm->map[bgrIndex + 1] << 8) |
    //                                                         bm->map[bgrIndex];
    //     }
    // }

    return bm;
}

void font_show(bitmap *bm, int posX, int posY)
{
    for (int row = 0; row < bm->width; row++)
    {
        for (int column = 0; column < bm->height; column++)
        {
            int bgrIndex = (row * bm->width + column) * 4;
            lcdBuf[row + posY][column + posX] = (bm->map[bgrIndex + 3] << 24) |
                                                (bm->map[bgrIndex + 2] << 16) |
                                                (bm->map[bgrIndex + 1] << 8) |
                                                bm->map[bgrIndex];
        }
    }
}

/**
  * @brief  创造一个画板(有bmp图片底色数据)
  * @note   1、这个画板可以设置底板颜色
            2、也可以设置画板的宽和高
  * @param  bmppath：  bmp图片的路径
  * @retval 返回画板结构体指针(bitmap结构体)
*/
bitmap *createBitmapWithInit_bmp(char *bmppath)
{
    // 一、获取bmp图片的数据(第一部分的信息(bmp图片54个字节的头信息数据)和第二部分(bmp的颜色数据(BGR)))
    // 1、打开bmp图片文件
    int bmp_fd = open(bmppath, O_RDONLY);
    if (bmp_fd == -1)
    {
        perror("打开bmp文件失败！\n");
        return (bitmap *)-1;
    }

    // 2、获取第一部分的信息(bmp图片54个字节的头信息数据)
    struct bitmap_header header; // BMP格式头信息结构体哦
    struct bitmap_info info;

    read(bmp_fd, &header, sizeof(header)); // 读取数据存放到其相应的结构体变量中
    read(bmp_fd, &info, sizeof(info));

    int w = info.width;  // 获取bmp图片的宽度
    int h = info.height; // 获取bmp图片的高度

    // 3、获取第二部分(bmp的颜色数据(BGR))
    lseek(bmp_fd, 0, SEEK_SET); // 觉得心里过意不去，写一下
    lseek(bmp_fd, 54, SEEK_SET);

    char bmp_buf[w * h * 3];          // 变长数组，不能赋值
    read(bmp_fd, bmp_buf, w * h * 3); // 读取到的数据是上下翻转颠倒的

    // 4、关闭bmp图片文件
    close(bmp_fd);

    // 二、将bmp图片数据，写入画板中
    bitmap *bm = createBitmap(w, h, 4); // 200*200*4
    u32 i = 0;
    u32 x = 0;
    u32 y = 0;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            *(bm->map + y * w * 4 + x * 4 + 0) = bmp_buf[(h - 1 - y) * w * 3 + x * 3 + 2];
            *(bm->map + y * w * 4 + x * 4 + 1) = bmp_buf[(h - 1 - y) * w * 3 + x * 3 + 1];
            *(bm->map + y * w * 4 + x * 4 + 2) = bmp_buf[(h - 1 - y) * w * 3 + x * 3 + 0];
            *(bm->map + y * w * 4 + x * 4 + 3) = 0x00;
            i++;
        }
    }
    return bm;
}

bitmap *char2rgb(char *text, int pixelSize, int outFrameWidth, int outFrameHeight, int outFrameColor, int fontPosX, int fontPosY, int fontColor)
{
    // 1.初始化字库
    //  注意要先将ttf文件放入开发板上
    font *f = fontLoad("./lib/font/simfang.ttf"); // 指定字库文件，比如simfang.ttf

    // 2.设置字体的大小
    fontSetSize(f, pixelSize);

    // 3.设置字体输出框的大小
    bitmap *bm;
    bm = createBitmapWithInit(outFrameWidth, outFrameHeight, 4, outFrameColor);

    // 4.把字体输出到输出框中
    fontPrint(f, bm, fontPosX, fontPosY, text, fontColor, 0);
    // 5.将bm妥善地放置到LCD上显示出来

    // showbitmap(bm, 200, 200);
    // 关闭字体库
    fontUnload(f);

    return bm;
}

void destroytruetype(bitmap *bm)
{
    // 关闭bitmap
    destroyBitmap(bm);
}
