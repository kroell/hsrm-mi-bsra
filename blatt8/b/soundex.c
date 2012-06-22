/**
 * @brief Soundex implementation in C
 * 
 * Author: Rick Parsons
 * Origin: http://west-penwith.org.uk/misc/soundex.htm
 */

#include <string.h>

#define GROUPS 7

const char * group[GROUPS] =
{ "AEIOUHWY", "BFPV", "CGKJQSXZ", "DT", "L", "MN", "R" };

/* to implement the NARA variation use
#define GROUPS 8
const char * group[GROUPS] =
{ "AEIOUY", "BFPV", "CGKJQSXZ", "DT", "L", "MN", "R", "HW" };
#define HW '7'
*/

const char * digit = { "0123456789" };

/* Returns the soundex equivalent to name */
char * Soundex(char * name)
{
        int i, j, k;
        char prev = digit[GROUPS]; /* value not used for a code */
        static char out[5];
        char c;

        for (i = 0, j = 0; name[i] != (char)NULL && j < 4; i++)
        {
                c = toupper(name[i]);
                /* decode the character */
                for (k = 0; k < GROUPS; k++)
                        if (strchr(group[k], c) != (char *)NULL)
                        {
                                c = digit[k];
                                break;
                        }
                /* if not found then ignore the character */
                if (k == GROUPS)
                        continue;
                /* to implement the NARA variation include
                if (j != 0 && c == HW)
                        continue;
                */
                /* ignore duplicates */
                if (c != prev)
                {
                        prev = c;
                        /* replace first letter */
                        if (j == 0)
                                c = name[0];
                        /* ignore "vowels" */
                        if (c != '0')
                                out[j++] = c;
                }
        }
        /* padd out to 4 chars */
        for ( ; j < 4; j++)
                out[j] = '0';
        /* terminate the string */
        out[4] = (char)NULL;
        return out;
}

