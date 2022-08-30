
int _doscan(register FILE *stream, const char *format, va_list ap)
{
    int        done = 0;        // �����Ŀ��
    int        nrchars = 0;     // �Ѷ�ȡ�ַ���
    int        conv = 0;        // # ��ת�� 
    int        base;            // ת���Ļ��� 
    unsigned long    val;       // һ������ֵ 
    register char    *str;      // ��ʱָ��
    char       *tmp_string;     // ditto 
    unsigned   width = 0;       // ��ȵ��ֶ� 
    int        flags;           // һЩ��־ 
    int        reverse;         // reverse the checking in [...] 
    int        kind;           
    register int    ic = EOF;   // ������ַ�
#ifndef    NOFLOAT
    long double    ld_val;
#endif

    if (!*format) return 0;     //ָ����Ϊ�գ���������
    while (1)
    {
        if (isspace(*format))   //���ָ������ͷ����û�õĿո�
        {
            while (isspace(*format))
                format++;       //����ָ������Щ�հ׷���
            ic = getc(stream);
            nrchars++;
            while (isspace (ic))    //�����������հ׷��ţ��Ѷ�ȡ���ַ���Ҫ++
            {
                ic = getc(stream);
                nrchars++;
            }
            if (ic != EOF)          //�����������ȡ���ַ����ǳ�ʼֵ��һ
                ungetc(ic,stream);  //����С�Ķ������ķǿ������»�������
            nrchars--;
        }
        if (!*format) break;    // ָ����Ϊ�գ���������

        if (*format != '%')     //���ָ������⵽�Ĳ��ǰٷֺ�
        {
            ic = getc(stream);  //��������ȡһ���ַ�
            nrchars++;
            if (ic != *format++)//���������ָ������ƥ��
                break;          // ��������ȥ
            continue;
        }
        format++;
        
        if (*format == '%')     //���ָ������%
        {
            ic = getc(stream);  //��������ȡһ������
            nrchars++;
            if (ic == '%')      //�����������%
            {
                format++;       //ָ����++
                continue;
            }
            else break;
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
            case 'h': flags |= FL_SHORT; format++; break;
            case 'l': flags |= FL_LONG; format++; break;
            case 'L': flags |= FL_LONGDOUBLE; format++; break;
        }
        
        kind = *format;         //������ָ�����϶�������
        if ((kind != 'c') && (kind != '[') && (kind != 'n'))
        {
            do
            {
                ic = getc(stream);       //����������ͣ�ĳԿո�
                nrchars++;
            } while (isspace(ic));
            if (ic == EOF) break;        // �����������⵽-1������ѭ��
        }
        else if (kind != 'n')
        {                                // ������ %c ���� %[
            ic = getc(stream);
            if (ic == EOF) break;        // ����whileѭ��
            nrchars++;
        }
        switch (kind)
        {
            default:        // ��ʶ���, ���� %q
                return conv || (ic != EOF) ? done : EOF;
                break;
            case 'n':       //����������δ�����
                if (!(flags & FL_NOASSIGN))
                {    // silly, though
                    if (flags & FL_SHORT)
                        *va_arg(ap, short *) = (short) nrchars;     //��ȡ�ɱ�����ĵ�ǰ����������ָ�����Ͳ���ָ��ָ����һ����
                    else if (flags & FL_LONG)
                        *va_arg(ap, long *) = (long) nrchars;
                    else
                        *va_arg(ap, int *) = (int) nrchars;
                }
                break;
            case 'p':        // ָ��
                set_pointer(flags);     //�����ָ������Ϊָ������״
            // ���
            case 'b':        // ��������
            case 'd':        // ʮ������
            case 'i':        // һ������
            case 'o':        // �˽�����
            case 'u':        // �޷�����
            case 'x':        // ʮ��������
            case 'X':        // ͬ��
                if (!(flags & FL_WIDTHSPEC) || width > NUMLEN)
                    width = NUMLEN;
                if (!width) return done;    //���Ϊ����
                //str = o_collect(ic, stream, kind, width, &base);
                str = &base;
                if (str < inp_buf  || (str == inp_buf && (*str == '-' || *str == '+')))
                    return done;
                // ��Ȼ������ֵĳ�����str-inp_buf+1����������û�м�1����Ϊ�����Ѿ����������
                nrchars += str - inp_buf;
                if (!(flags & FL_NOASSIGN))
                {
                    if (kind == 'd' || kind == 'i')
                        val = strtol(inp_buf, &tmp_string, base);   //inp_buf�еĺϷ��ַ�����baseת���������val��ʣ��ı��浽tmp_string��
                    else
                        val = strtoul(inp_buf, &tmp_string, base);  //����ǰ��Ŀհ��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת���������������ֻ��ַ�������ʱ('\0')����ת���������������
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
                // ��ֹ�ַ���
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
                    //MAT 8/9/96û��ƥ����뷵���ַ�
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
                {    // ��ֹ�ַ���
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
    va_start(ap, format);   //��ȡ�ɱ�����б�ĵ�һ�������ĵ�ַ
    retval = _doscan(stdin, format, ap);
    va_end(ap);             //���va_list�ɱ�����б�
    return retval;
}


