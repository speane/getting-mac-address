#include <windows.h>

typedef struct
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
} ASTAT;



void getLanNumbers(NCB * ncb, LANA_ENUM * lenum)
{
    memset(ncb, 0, sizeof(NCB));
    ncb->ncb_command = NCBENUM;
    ncb->ncb_buffer = (UCHAR *)lenum;
    ncb->ncb_length = sizeof(LANA_ENUM);
    Netbios(ncb);
}

void reset(NCB * ncb, int lanaNumber)
{
    memset(ncb, 0, sizeof(NCB));
    ncb->ncb_command = NCBRESET;
    ncb->ncb_lana_num = lanaNumber;
    Netbios(ncb);
}

int getAstat(NCB * ncb, int lanaNumber, ASTAT * adapter)
{
    memset(ncb, 0, sizeof (NCB));
    ncb->ncb_command = NCBASTAT;
    ncb->ncb_lana_num = lanaNumber;
    strcpy(ncb->ncb_callname, "*");
    ncb->ncb_buffer = (char *) adapter;
    ncb->ncb_length = sizeof(ASTAT);
    return Netbios(ncb);
}

void printMac(ASTAT adapter)
{
    printf( "MAC ADDRESS: %02x:%02x:%02x:%02x:%02x:%02x\n",
                adapter.adapt.adapter_address[0],
                adapter.adapt.adapter_address[1],
                adapter.adapt.adapter_address[2],
                adapter.adapt.adapter_address[3],
                adapter.adapt.adapter_address[4],
                adapter.adapt.adapter_address[5] );
}

int main (void)
{
    NCB ncb;
    UCHAR uRetCode;
    LANA_ENUM lenum;
    int i;
    ASTAT adapter;

    getLanNumbers(&ncb, &lenum);

    for (i = 0; i < lenum.length; i++) {
        reset(&ncb, lenum.lana[i]);
        uRetCode = getAstat(&ncb, lenum.lana[i], &adapter);

        if (!uRetCode) {
            printMac(adapter);
        }
    }
    getchar();
    return 0;
}
