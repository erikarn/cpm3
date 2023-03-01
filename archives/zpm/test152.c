/*-------------------------------------------------------------------------*\
 | This program is intended to test BDOS function 152 as implemented in    |
 | Simeon Cran's ZPM3 (or any other BDOS for that matter).                 |
 |                                                                         |
 | The user is prompted for CP/M-style file names, possibly prefixed with  |
 | a drive letter and/or user number (in either order).  Each such name    |
 | is given to the BDOS which fills out a caller-supplied FCB.  If the     |
 | BDOS returns an error then this program reports it, otherwise it prints |
 | out the various fields of the FCB.                                      |
 |                                                                         |
 | Supplying an empty file name terminates the program.                    |
 |                                                                         |
 | Jon Saxton                                                              |
 | 2013-09-09                                                              |
\*-------------------------------------------------------------------------*/

#include <cpm.h>		/* For bdos() function declaration */
#include <stdio.h>
#include <string.h>

typedef struct fcb FCB;	/* Another thing declared in cpm.h */
struct pfncb_
{
    char
        *input;
    FCB
		*output;
};
typedef struct pfncb_ PFNCB;

char instructions[] =
"\nEnter file specifiers, one per line.  This program will pass each specifier"
"\nto BDOS function 152 (Parse File Name) and print out the fields of the FCB"
"\nfilled out by the BDOS.  File specifiers may have a drive/user prefix and\n"
"\nmay contain wildcards.  A password can be supplied too.  Some examples are"
"\n\tabc.def"
"\n\t4k:abc*.d?f"
"\n\tm12:coffee.pot;arabica"
"\n\t8:*.*"
"\n\te:???b?os.spr\n"
"\nOf course file specifiers containing a user number prefix won't work on a"
"\nstandard DRI implementation.  They do work on ZPM3.\n"
"\nAn empty file specifier terminates this program.";
 
int main(int argc, char *argv[])
{
    int
        x,
        curDrv,
        curUsr,
		retVal;
    union rec_
    {
        long
            record;
        unsigned char
            rec[4];
    }
        ranRec;
    FCB
       tfcb;
    PFNCB
       pfncb;
    char
        fnSource[150];   /* Ridiculously long */

    pfncb.input  = fnSource;
    pfncb.output = &tfcb;
    curDrv = bdos(CPMIDRV, 0) + 1;  /* Get current drive */
    curUsr = bdos(CPMSUID, 255);    /* and user number */
    puts(instructions);
    for (;;)
    {
        printf("\nFile specifier: ");
        gets(fnSource);
        if (strlen(fnSource) == 0)
            return 0;

        /* Fill the target FCB with junk */
        memset(&tfcb, 0xB3, sizeof tfcb);

        /* Parse the input string as a file name */
        retVal = bdos(152, &pfncb);
        if (retVal == -1)
        {
            puts("Error!");
            continue;
        }

        /* Print out all the fields of the FCB. */
        printf(" Drive: %d [%c:]\n", tfcb.dr,
               (tfcb.dr ? tfcb.dr : curDrv) + '@');
        printf("  Name: '");
        for (x = 0; x < 8; ++x)
            putchar(tfcb.name[x] & 0x7F);
        putchar('\'');
        putchar('\n');
        printf("  Type: '");
        for (x = 0; x < 3; ++x)
            putchar(tfcb.ft[x] & 0x7F);
        putchar('\'');
        putchar('\n');
        printf("Extent: %d\n", tfcb.ex);

        /* ZPM3 puts user number and drive number in the s1 and s2 fields
           respectively.  Drive 1 = A, drive 2 = B ... drive 16 = P. */
        printf("(user): %d\n", tfcb.fil[0]);
        printf("(disk): %d [%c:]\n", tfcb.fil[1], tfcb.fil[1] + '@');

        printf(" Count: %d\n", tfcb.rc);
        printf("d0-d15: '%-8.8s'", tfcb.dm);
        for (x = 8; x < 16; ++x)
            printf(" %02x", tfcb.dm[x]);
        putchar('\n');
        printf("CurRec: %d\n", tfcb.nr);
        printf("RanRec:");
        ranRec.rec[3] = 0;
        for (x = 0; x < 3; ++x)
        {
            printf(" %02x", tfcb.ranrec[x]);
            ranRec.rec[x] = tfcb.ranrec[x];
        }
        printf(" [%ld]\n", ranRec.record);
    }
}
