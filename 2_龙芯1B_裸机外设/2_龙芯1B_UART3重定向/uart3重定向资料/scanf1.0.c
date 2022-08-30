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
    int             done = 0;       // �����Ŀ��(�Ѿ����뼸������)
    int             nrchars = 0;    // �Ѷ�ȡ�ַ���
    int             base;           // ת���Ļ���
    unsigned long   val=0;          // һ������ֵ
    register char   *str;           // ��ʱָ��
    unsigned        width = 0;      // ��ȵ��ֶ�
    int             flags;          // һЩ��־
    int             kind;           // ��������
    register int    ic = 0;         // �����ݻ��������ȡ�����ַ�


    if (!*format) return 0;     //ָ����Ϊ�գ���������
    while (1)
    {
        if (isspace(*format))   //���ָ������ͷ����û�õĿո�
        {
            while (isspace(*format))
                format++;       //����ָ������Щ�հ׷���
            ic = buf_S[nrchars];
            nrchars++;
            while (isspace (ic))    //�����������հ׷��ţ��Ѷ�ȡ���ַ���Ҫ++
            {
                ic = buf_S[nrchars];
                nrchars++;
            }
            if (ic != 0)          //�����������ȡ���ַ����ǳ�ʼֵ0
                nrchars--;
        }
        if (!*format) break;    // ָ����Ϊ�գ���������

        if (*format != '%')     //���ָ������⵽�Ĳ��ǰٷֺ�
        {
            break;          // ��������ȥ
        }
        else if (*format == '%')     //���ָ������%
        {
            //��������ȡһ������
            ic = buf_S[nrchars];
            nrchars++;
            format++;
        }
        
        flags = 0;
        if (*format == '*')
        {
            format++;
            flags |= FL_NOASSIGN;//δ�����־
        }
        if (isdigit (*format))  //�ж�ָ����ָ��������֣�������
        {
            flags |= FL_WIDTHSPEC;
            for (width = 0; isdigit (*format);)
                width = width * 10 + *format++ - '0';
        }

        switch (*format)        //����˵����h,l,L
        {
            case 'h': flags |= FL_SHORT;        format++; break;
            case 'l': flags |= FL_LONG;         format++; break;
            case 'L': flags |= FL_LONGDOUBLE;   format++; break;
        }
        
        kind = *format;         //������ָ�����϶�������
        switch (kind)
        {
            case 'n':       //����������δ�����
                if (!(flags & FL_NOASSIGN))
                {
                    if (flags & FL_SHORT)
                        *va_arg(ap, short *) = (short) nrchars;     //��ȡ�ɱ�����ĵ�ǰ����������ָ�����Ͳ���ָ��ָ����һ����
                    else if (flags & FL_LONG)
                        *va_arg(ap, long *) = (long) nrchars;
                    else
                        *va_arg(ap, int *) = (int) nrchars;
                }
                break;

            case 'o':        // �˽�������
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
                
            case 'd':        // ʮ��������
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
            case 'X':        // ʮ����������
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
    va_start(ap, format);   //��ȡ�ɱ�����б�ĵ�һ�������ĵ�ַ
    retval = _doscan(format, ap);
    va_end(ap);             //���va_list�ɱ�����б�
    clean_buf();
    return retval;
}


