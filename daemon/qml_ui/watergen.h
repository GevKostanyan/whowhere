/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef __C_WATERGEN__
#define __C_WATERGEN__

#include <QImage>

#define WATERGEN_TABLE_LENGTH 1024
#define WATERGEN_TABLE_AND 1023
#define WATERGEN_TABLE_BITS 10


class WaterGen {
public:
    WaterGen();
    ~WaterGen();


    void reinitWaves( int amount, bool static_waves = true );
    int getNofWaves() { return waveComponentAmount; }

    void generate( QImage &targetImage,
                   QImage &reflection,
                   const float timePassed );




protected:
    int cosineTable[ WATERGEN_TABLE_LENGTH ];

    int initializedWidth;
    int initializedHeight;
    int *waveHeightMap;


    struct SWaveComponent {
        int xm;
        int ym;
        int amplitude;
        int movingSpeed;
        int currentPos;
    };

    SWaveComponent *waveComponents;
    int waveComponentAmount;
};


#endif
