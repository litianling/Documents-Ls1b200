/*
 * scanf.c
 *
 * created: 2021/10/26
 *  author: 
 */


#include "bsp.h"
#include "printf.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>


#define FL_NOASSIGN         (1U <<  1U)  //2
#define FL_WIDTHSPEC        (1U <<  3U)  //8
#define NR_CHARS            (1U <<  6U)  //64
#define FL_SHORT            (1U <<  7U)  //128
#define FL_LONG             (1U <<  8U)  //256
#define FL_LONGDOUBLE       (1U <<  9U)  //512

#define buf_size    50
unsigned char buf_S[buf_size] = {0};
// import float.h for DBL_MAX
#include <float.h>

void clean_buf()
{
    int i=0;
    for(;i<buf_size;i++)
        buf_S[i]=0;
}


int _doscan(const char *format, va_list ap)
{
    int             done = 0;       // 完成项目数(已经输入几个变量)
    int             nrchars = 0;    // 已读取字符数
    int             base;           // 转换的基础
    unsigned long   val=0;          // 一个整数值
    register char   *str;           // 临时指针
    unsigned        width = 0;      // 宽度的字段
    int             flags;          // 一些标志
    int             kind;           // 数据类型
    register int    ic = 0;         // 从数据缓存数组读取到的字符


    if (!*format) return 0;     //指令流为空，立即结束
    while (1)
    {
        if (isspace(*format))   //如果指令流开头就是没用的空格
        {
            while (isspace(*format))
                format++;       //跳过指令流那些空白符号
            ic = buf_S[nrchars];
            nrchars++;
            while (isspace (ic))    //数据流跳过空白符号，已读取的字符数要++
            {
                ic = buf_S[nrchars];
                nrchars++;
            }
            if (ic != 0)          //如果数据流读取的字符不是初始值0
                nrchars--;
        }
        if (!*format) break;    // 指令流为空，立即结束

        if (*format != '%')     //如果指令流检测到的不是百分号
        {
            break;          // 出错跳出去
        }
        else if (*format == '%')     //如果指令流是%
        {
            //数据流读取一个数据
            ic = buf_S[nrchars];
            nrchars++;
            format++;
        }
        
        flags = 0;
        if (*format == '*')
        {
            format++;
            flags |= FL_NOASSIGN;//未分配标志
        }
        if (isdigit (*format))  //判断指令流指向的是数字，分配宽度
        {
            flags |= FL_WIDTHSPEC;
            for (width = 0; isdigit (*format);)
                width = width * 10 + *format++ - '0';
        }

        switch (*format)        //特殊说明符h,l,L
        {
            case 'h': flags |= FL_SHORT;        format++; break;
            case 'l': flags |= FL_LONG;         format++; break;
            case 'L': flags |= FL_LONGDOUBLE;   format++; break;
        }
        
        kind = *format;         //接下来指令流肯定是类型
        switch (kind)
        {
            case 'n':       //数据类型是未分配的
                if (!(flags & FL_NOASSIGN))
                {
                    if (flags & FL_SHORT)
                        *va_arg(ap, short *) = (short) nrchars;     //获取可变参数的当前参数，返回指定类型并将指针指向下一参数
                    else if (flags & FL_LONG)
                        *va_arg(ap, long *) = (long) nrchars;
                    else
                        *va_arg(ap, int *) = (int) nrchars;
                }
                break;

            case 'o':        // 八进制整数
                while(ic>=48 && ic<=55)
                {
                    val = val*8 + (ic-48);
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                if (flags & FL_LONG)
                    *va_arg(ap, unsigned long *) = (unsigned long) val;
                else if (flags & FL_SHORT)
                    *va_arg(ap, unsigned short *) = (unsigned short) val;
                else
                    *va_arg(ap, unsigned *) = (unsigned) val;
                break;
                
            case 'd':        // 十进制整数
                while(isdigit(ic)) 
                {
                    val = val*10 + (ic-48);
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                if (flags & FL_LONG)
                    *va_arg(ap, unsigned long *) = (unsigned long) val;
                else if (flags & FL_SHORT)
                    *va_arg(ap, unsigned short *) = (unsigned short) val;
                else
                    *va_arg(ap, unsigned *) = (unsigned) val;
                break;

            case 'x':
            case 'X':        // 十六进制整数
                while(isdigit (ic) || ( ic>64 && ic<71 ) || ( ic>96 && ic<103 ))
                {
                    if(isdigit (ic))
                        val = val*16 + (ic-48);
                    else if( ic>64 && ic<71 )
                        val = val*16 + (ic-65+10);
                    else
                        val = val*16 + (ic-97+10);
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                if (flags & FL_LONG)
                    *va_arg(ap, unsigned long *) = (unsigned long) val;
                else if (flags & FL_SHORT)
                    *va_arg(ap, unsigned short *) = (unsigned short) val;
                else
                    *va_arg(ap, unsigned *) = (unsigned) val;
                break;
                
            case 'c':
                while(isspace(ic))
                {
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                *va_arg(ap, char *) = (char) ic;
                break;

            case 's':
                str = va_arg(ap, char *);
                while(isspace(ic))
                {
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                while (ic!=0 && ic!=10 && ic!=13)
                {
                    *str++ = (char) ic;
                    ic = buf_S[nrchars];
                    nrchars++;
                }
                break;

            default:  done--;
                break;
        }
        done++;
        if (!(flags & FL_NOASSIGN) && kind != 'n') done++;
            format++;
    }
    return  done ;
}


int scanf(const char *format, ...)
{
    uart3_initialize(115200,8,'N',1);
    uart3_read(buf_S,buf_size);
    va_list ap;
    int retval;
    va_start(ap, format);   //获取可变参数列表的第一个参数的地址
    retval = _doscan(format, ap);
    va_end(ap);             //清空va_list可变参数列表
    clean_buf();
    return retval;
}


