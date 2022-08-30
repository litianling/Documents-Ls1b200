
int _doscan(register FILE *stream, const char *format, va_list ap)
{
    int        done = 0;        // 完成项目数
    int        nrchars = 0;     // 已读取字符数
    int        conv = 0;        // # 的转换 
    int        base;            // 转换的基础 
    unsigned long    val;       // 一个整数值 
    register char    *str;      // 临时指针
    char       *tmp_string;     // ditto 
    unsigned   width = 0;       // 宽度的字段 
    int        flags;           // 一些标志 
    int        reverse;         // reverse the checking in [...] 
    int        kind;           
    register int    ic = EOF;   // 输入的字符
#ifndef    NOFLOAT
    long double    ld_val;
#endif

    if (!*format) return 0;     //指令流为空，立即结束
    while (1)
    {
        if (isspace(*format))   //如果指令流开头就是没用的空格
        {
            while (isspace(*format))
                format++;       //跳过指令流那些空白符号
            ic = getc(stream);
            nrchars++;
            while (isspace (ic))    //数据流跳过空白符号，已读取的字符数要++
            {
                ic = getc(stream);
                nrchars++;
            }
            if (ic != EOF)          //如果数据流读取的字符不是初始值负一
                ungetc(ic,stream);  //将不小心读出来的非空数据吐回数据流
            nrchars--;
        }
        if (!*format) break;    // 指令流为空，立即结束

        if (*format != '%')     //如果指令流检测到的不是百分号
        {
            ic = getc(stream);  //数据流读取一个字符
            nrchars++;
            if (ic != *format++)//如果数据流指令流不匹配
                break;          // 出错跳出去
            continue;
        }
        format++;
        
        if (*format == '%')     //如果指令流是%
        {
            ic = getc(stream);  //数据流读取一个数据
            nrchars++;
            if (ic == '%')      //如果数据流是%
            {
                format++;       //指令流++
                continue;
            }
            else break;
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
            case 'h': flags |= FL_SHORT; format++; break;
            case 'l': flags |= FL_LONG; format++; break;
            case 'L': flags |= FL_LONGDOUBLE; format++; break;
        }
        
        kind = *format;         //接下来指令流肯定是类型
        if ((kind != 'c') && (kind != '[') && (kind != 'n'))
        {
            do
            {
                ic = getc(stream);       //让数据流不停的吃空格
                nrchars++;
            } while (isspace(ic));
            if (ic == EOF) break;        // 如果数据流检测到-1跳出大循环
        }
        else if (kind != 'n')
        {                                // 类型是 %c 或者 %[
            ic = getc(stream);
            if (ic == EOF) break;        // 跳出while循环
            nrchars++;
        }
        switch (kind)
        {
            default:        // 不识别的, 就像 %q
                return conv || (ic != EOF) ? done : EOF;
                break;
            case 'n':       //数据类型是未分配的
                if (!(flags & FL_NOASSIGN))
                {    // silly, though
                    if (flags & FL_SHORT)
                        *va_arg(ap, short *) = (short) nrchars;     //获取可变参数的当前参数，返回指定类型并将指针指向下一参数
                    else if (flags & FL_LONG)
                        *va_arg(ap, long *) = (long) nrchars;
                    else
                        *va_arg(ap, int *) = (int) nrchars;
                }
                break;
            case 'p':        // 指针
                set_pointer(flags);     //将鼠标指针设置为指定的形状
            // 清空
            case 'b':        // 二进制数
            case 'd':        // 十进制数
            case 'i':        // 一般整数
            case 'o':        // 八进制数
            case 'u':        // 无符号数
            case 'x':        // 十六进制数
            case 'X':        // 同上
                if (!(flags & FL_WIDTHSPEC) || width > NUMLEN)
                    width = NUMLEN;
                if (!width) return done;    //宽度为结束
                //str = o_collect(ic, stream, kind, width, &base);
                str = &base;
                if (str < inp_buf  || (str == inp_buf && (*str == '-' || *str == '+')))
                    return done;
                // 虽然这个数字的长度是str-inp_buf+1，但是我们没有加1，因为我们已经计算过它了
                nrchars += str - inp_buf;
                if (!(flags & FL_NOASSIGN))
                {
                    if (kind == 'd' || kind == 'i')
                        val = strtol(inp_buf, &tmp_string, base);   //inp_buf中的合法字符按照base转化后输出到val，剩余的保存到tmp_string中
                    else
                        val = strtoul(inp_buf, &tmp_string, base);  //跳过前面的空白字符，直到遇上数字或正负符号才开始做转换，再遇到非数字或字符串结束时('\0')结束转换，并将结果返回
                    if (flags & FL_LONG)
                        *va_arg(ap, unsigned long *) = (unsigned long) val;
                    else if (flags & FL_SHORT)
                        *va_arg(ap, unsigned short *) = (unsigned short) val;
                    else
                        *va_arg(ap, unsigned *) = (unsigned) val;
                }
                break;
            case 'c':
                if (!(flags & FL_WIDTHSPEC))
                    width = 1;
                if (!(flags & FL_NOASSIGN))
                    str = va_arg(ap, char *);
                if (!width) return done;
                while (width && ic != EOF)
                {
                    if (!(flags & FL_NOASSIGN))
                        *str++ = (char) ic;
                    if (--width)
                    {
                        ic = getc(stream);
                        nrchars++;
                    }
                }
                if (width)
                {
                    if (ic != EOF) //ungetc(ic,stream);
                        nrchars--;
                }
                break;
            case 's':
                if (!(flags & FL_WIDTHSPEC))
                    width = 0xffff;
                if (!(flags & FL_NOASSIGN))
                    str = va_arg(ap, char *);
                if (!width) return done;
                while (width && ic != EOF && !isspace(ic))
                {
                    if (!(flags & FL_NOASSIGN))
                        *str++ = (char) ic;
                    if (--width)
                    {
                        ic = getc(stream);
                        nrchars++;
                    }
                }
                // 终止字符串
                if (!(flags & FL_NOASSIGN))
                    *str = '\0';
                if (width)
                {
                 if (ic != EOF)  ungetc(ic,stream);
                    nrchars--;
                }
                break;
            case '[':
                if (!(flags & FL_WIDTHSPEC))
                    width = 0xffff;
                if (!width) return done;
                if ( *++format == '^' )
                {
                    reverse = 1;
                    format++;
                }
                else
                    reverse = 0;
                for (str = Xtable; str < &Xtable[NR_CHARS] ; str++)
                    *str = 0;
                if (*format == ']') Xtable[*format++] = 1;
                while (*format && *format != ']')
                {
                    Xtable[*format++] = 1;
                    if (*format == '-')
                    {
                        format++;
                        if (*format && *format != ']' && *(format) >= *(format -2))
                        {
                            int c;
                            for( c = *(format -2) + 1 ; c <= *format ; c++)
                                Xtable[c] = 1;
                            format++;
                        }
                        else Xtable['-'] = 1;
                    }
                }
                if (!*format) return done;
                if (!(Xtable[ic] ^ reverse))
                {
                    //MAT 8/9/96没有匹配必须返回字符
                    ungetc(ic, stream);
                    return done;
                }
                if (!(flags & FL_NOASSIGN))
                    str = va_arg(ap, char *);
                do
                {
                    if (!(flags & FL_NOASSIGN))
                        *str++ = (char) ic;
                    if (--width)
                    {
                        ic = getc(stream);
                        nrchars++;
                    }
                } while (width && ic != EOF && (Xtable[ic] ^ reverse));
                if (width)
                {
                    if (ic != EOF)  ungetc(ic, stream);
                        nrchars--;
                }
                if (!(flags & FL_NOASSIGN))
                {    // 终止字符串
                    *str = '\0';
                }
                break;
                
#ifndef    NOFLOAT
            case 'e':
            case 'E':
            case 'f':
            case 'g':
            case 'G':
                if (!(flags & FL_WIDTHSPEC) || width > NUMLEN)
                    width = NUMLEN;
                if (!width) return done;
                str = f_collect(ic, stream, width);
                if (str < inp_buf || (str == inp_buf
                 && (*str == '-' || *str == '+'))) return done;
                //Although the length of the number is str-inp_buf+1,we don't add the 1 since we counted it already
                nrchars += str - inp_buf;
                if (!(flags & FL_NOASSIGN))
                {
                    ld_val = strtod(inp_buf, &tmp_string);
                    if (flags & FL_LONGDOUBLE)
                        *va_arg(ap, long double *) = (long double) ld_val;
                    else if (flags & FL_LONG)
                        *va_arg(ap, double *) = (double) ld_val;
                    else
                        *va_arg(ap, float *) = (float) ld_val;
                }
                break;
#endif
            }
        conv++;
        if (!(flags & FL_NOASSIGN) && kind != 'n') done++;
            format++;
    }
    return conv || (ic != EOF) ? done : EOF;
}


int scanf(const char *format, ...)
{
    va_list ap;
    int retval;
    va_start(ap, format);   //获取可变参数列表的第一个参数的地址
    retval = _doscan(stdin, format, ap);
    va_end(ap);             //清空va_list可变参数列表
    return retval;
}


