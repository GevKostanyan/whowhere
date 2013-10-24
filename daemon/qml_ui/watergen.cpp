/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QDebug.h>
#include <math.h>
#include "watergen.h"

#define WATER_COLOR_R 33
#define WATER_COLOR_G 88
#define WATER_COLOR_B 255
#define NORMAL_CALCULATION_DISTANCE 8

const int winct[9][2] = {
{  2 , 2  },
{  1 , 4  },
{  2 , 4  },
{  4 , 3  },
{  6 , 2  },
{  4 , 5  },
{  2 , 8  },
{  4 , 7  },
{  7 , 5  },
};

WaterGen::WaterGen() {
        // Generate cosinetable for waves
    for (int f=0; f<WATERGEN_TABLE_LENGTH; f++) {
        cosineTable[f] = 65000- 2*(int)abs(( cosf( (float)f*3.14159f / (float)WATERGEN_TABLE_LENGTH ) )*65000.0f );
    };

    waveHeightMap = 0;
    initializedWidth = 0;
    initializedHeight = 0;
    waveComponents = 0;
    waveComponentAmount = 0;
    reinitWaves( 9 );

};


WaterGen::~WaterGen() {
    if (waveHeightMap) delete [] waveHeightMap;
    waveHeightMap = 0;
    reinitWaves( 0 );
};


void WaterGen::reinitWaves( int amount, bool static_waves ) {
    if (waveComponents) delete waveComponents;
    waveComponents = 0;
    waveComponentAmount = amount;
    if (amount<1) return;

    waveComponents = new SWaveComponent[ waveComponentAmount ];

    for (int w=0; w<waveComponentAmount; w++) {
      SWaveComponent *wc = waveComponents+w;
      if (static_waves) {
        wc->xm = winct[w][0];
        wc->ym = winct[w][1];
      } else {
        int total = 2+w;
        int part = (((rand() & 255) * total) >> 8);
        wc->xm = 1+part;
        wc->ym = 1+(total - part);
      }
      wc->amplitude = 65000/( (abs(wc->xm ) + abs(wc->ym) -3)*2 );
      wc->movingSpeed = 8000+12*wc->amplitude;
      wc->currentPos = (rand() & 255)<<8;
    };

    // output the waves
    if (!static_waves) {
        /*
        qDebug() << "const int winct[10][2] = {";
        for (int w=0; w<waveComponentAmount; w++) {
            qDebug() << "{ " << waveComponents[w].xm << "," << waveComponents[w].ym << " },";
        };
        qDebug() << "}";
        */
    }
};


inline unsigned long resample_alpha( int x, int y, unsigned long *source ) {
    source += (((y>>8)&255)<<8) + ((x>>8)&255);
    x = ((x&255));
    y = ((y&255));
    return ((( ((source[0]>>24)*(255^x) + (source[1]>>24)*x) * (255^y) ) +
             ( ((source[256]>>24)*(255^x) + (source[257]>>24)*x) * (y) ) ) >> 16);

}

unsigned long resample( int x, int y, unsigned long *source ) {
    unsigned long *pos = source + (((y>>8)&255)<<8) + ((x>>8)&255);
    x = ((x&255)>>1);
    y = ((y&255)>>1);
    int negofsx = 128-x;

    return  (
            (((( (((((pos[0]&0x00FF00FF)*(negofsx)) + ((pos[1]&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * (128-y))  +
               ( (((((pos[256]&0x00FF00FF)*(negofsx)) + ((pos[257]&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * y)) >> 7) & 0x00FF00FF) |
            ((((( ((((((pos[0]>>8)&0x00FF00FF)*(negofsx)) + (((pos[1]>>8)&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * (128-y))  +
                ( ((((((pos[256]>>8)&0x00FF00FF)*(negofsx)) + (((pos[257]>>8)&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * y)) >> 7) & 0x00FF00FF) << 8));
}

unsigned long resample_loop( int x, int y, unsigned long *source ) {
    unsigned long c1, c2, c3, c4;
    c1 = source[(((y>>8)&255)<<8) + ((x>>8)&255)];
    c2 = source[((((y>>8)+1)&255)<<8) + ((x>>8)&255)];
    c3 = source[(((y>>8)&255)<<8) + (((x>>8)+1)&255)];
    c4 = source[((((y>>8)+1)&255)<<8) + (((x>>8)+1)&255)];

    x = ((x&255)>>1);
    y = ((y&255)>>1);
    int negofsx = 128-x;

    return  (
            (((( (((((c1&0x00FF00FF)*(negofsx)) + ((c3&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * (128-y))  +
               ( (((((c2&0x00FF00FF)*(negofsx)) + ((c4&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * y)) >> 7) & 0x00FF00FF) |
            ((((( ((((((c1>>8)&0x00FF00FF)*(negofsx)) + (((c3>>8)&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * (128-y))  +
                ( ((((((c2>>8)&0x00FF00FF)*(negofsx)) + (((c4>>8)&0x00FF00FF)*(x)) ) >> 7)&0x00FF00FF) * y)) >> 7) & 0x00FF00FF) << 8));
}


void WaterGen::generate(QImage &targetImage, QImage &reflection, const float timePassed) {
    if (initializedWidth!=targetImage.width() ||
            initializedHeight!=targetImage.height() ||
            waveHeightMap == 0) {
        if (waveHeightMap) delete [] waveHeightMap;
        initializedWidth = targetImage.width();
        initializedHeight = targetImage.height();
        waveHeightMap = new int[ initializedWidth * initializedHeight ];
    };


    // generate waveHeight
    memset( waveHeightMap, 0, 4*initializedWidth * initializedHeight );

    int x, y;
    int mx, my;
    int mx_inc, my_inc;
    int spos;
    int ftime = (int)(timePassed * 65536.0f );
    int a;
    int *h;

    for (int w=0; w<waveComponentAmount; w++) {
        SWaveComponent *wc = waveComponents + w;
        wc->currentPos+=((ftime * wc->movingSpeed)>>16);

        // run the wave here.
        spos = wc->currentPos;
        a = (wc->amplitude>>6);

        mx_inc = (wc->xm<<16) / initializedWidth;
        my_inc = (wc->ym<<16) / initializedHeight;

        h = waveHeightMap;
        my = 0;
        for (y=0; y<initializedHeight; y++) {
            mx = 0;
            for (x=0; x<initializedWidth; x++) {
                *h+=((cosineTable[ ((spos+mx+my)>>6) & WATERGEN_TABLE_AND ]*a)>>12);
                h++;
                mx+=mx_inc;
            };
            my+=my_inc;
        }
    };


    int waveand = (targetImage.width() - 1);

    unsigned long tecol, mask;
    unsigned long *refl = (unsigned long*)reflection.bits();

    for (y=0; y<initializedHeight; y++) {
        unsigned long *t_pix = (unsigned long*)targetImage.bits() + y*targetImage.bytesPerLine()/4;
        for (x=0; x<initializedWidth; x++) {

            a = ((x-NORMAL_CALCULATION_DISTANCE)&waveand);
            spos = ((x+NORMAL_CALCULATION_DISTANCE)&waveand);
            mx = -waveHeightMap[ a + ((y-NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth ] +
                 -waveHeightMap[ a + y*initializedWidth ] +
                 -waveHeightMap[ a + ((y+NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth ] +
                 waveHeightMap[ spos + ((y-NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth ] +
                 waveHeightMap[ spos + y*initializedWidth ] +
                 waveHeightMap[ spos + ((y+NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth ];


            a = ((y-NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth;
            spos = ((y+NORMAL_CALCULATION_DISTANCE)&waveand)*initializedWidth;
            my = -waveHeightMap[ a + ((x-NORMAL_CALCULATION_DISTANCE)&waveand) ] +
                 -waveHeightMap[ a + x ] +
                 -waveHeightMap[ a + ((x+NORMAL_CALCULATION_DISTANCE)&waveand) ] +
                 waveHeightMap[ spos + ((x-NORMAL_CALCULATION_DISTANCE)&waveand) ] +
                 waveHeightMap[ spos + x ] +
                 waveHeightMap[ spos + ((x+NORMAL_CALCULATION_DISTANCE)&waveand) ];


            a = resample_alpha( 32768+(mx>>1), 32768+(my>>1), refl );
            tecol = refl[ (((y + (my>>9))&255)<<8) | ((x+(mx>>9))&255) ];

            spos = ((a-128)<<1) + 196;
            if (spos<128) spos = 128;
            if (spos>255) spos = 255;

            tecol = ((((tecol&0x00FF00FF) * spos)>>8)&0x00FF00FF) |
                    ((((((tecol>>8)&0x00FF00FF) * spos)>>8)&0x00FF00FF)<<8);

            a-=(128);
            if (a>0) {
                tecol = ((tecol&0xFEFEFEFE)>>1) + (a|(a<<8)|(a<<16));
                mask = (tecol&0x80808080);
                tecol |= (mask-(mask>>7));
                tecol = ((tecol&0x7F7F7F7F)<<1);
            }

            *t_pix = tecol | 0xFF000000;
            t_pix++;
        };
    };

};



