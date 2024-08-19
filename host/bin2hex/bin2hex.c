/*
  bin2hex.c Copyright 2017 Greg Sydney-Smith

  History:
  2017-11-23. gss. Initial version.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  DEVELOPERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>


int bin2hex(int addr, char *fn1, char *fn2) {
    int n, i, chk;
    FILE *fp1, *fp2;
    char buff[16];

    if ((fp1=fopen(fn1,"rb"))==NULL) {
        printf("Can't open %s\n",fn1);
        return 1;
    }

    if ((fp2=fopen(fn2,"w"))==NULL) {
        printf("Can't create %s\n",fn2);
        return 1;
    }

    while ((n=fread(buff,1,16,fp1)) > 0) {
        fprintf(fp2,":%02X%04X00",n,addr);
        for (chk=n+(addr/256)+(addr%256), i=0; i<n; i++) {
            fprintf(fp2,"%02X",buff[i]&0xFF);
            chk+=buff[i];
        }
        fprintf(fp2,"%02X\n",(0x100 - (chk&0xFF))&0xFF);
        addr+=n;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    int addr;

    switch (argc) {
    case 4: sscanf(argv[1],"%x",&addr);
            bin2hex(addr,argv[2],argv[3]);
            return 0;
    }
    printf("Usage: bin2hex addr file.bin file.hex\n");
    printf("eg   : bin2hex 0x80 boot.bin boot.hex\n");
    return 1;
}

