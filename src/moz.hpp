#pragma once
//#include <../include/frt/frt.hpp>
//#include "moz_old.hpp"
#include "root.hpp"
#include "moz_old.hpp"

using namespace FRT;
using namespace FRT::unit_literals;
/*
TachoMotor left(OUTPUT_A, 6.2cm), right(OUTPUT_B, 6.2cm);
// ColorSensor cight(INPUT_2);
HTColorSensorV2 hol(INPUT_3);*/
    double axle2 =41;//40.7;///*22.8*/20.162*2;
 //   double diam=6.2;

//int kb = 0, kj = 0, kdiff = 0, vdiff = 0;
int alma = 30;
//int bva, jva;
int va[4];

void rekab()
{
    kb = left.get_position<deg>().value;
    kj = right.get_position<deg>().value;
    kdiff = kb - kj;
    Logger::info(kdiff);
    vdiff = kdiff;
    bva=kb, jva=kj;
    va[0]=kb;
    va[1]=kj;
    va[2]=emel.get_position<deg>().value;
    va[3]=kamp.get_position<deg>().value;

}

int alma2 = 1000;

void meggy20(int dist, int szog, int s0, int sm, int send)
{
    szog=-szog;
    int bk = left.get_position<deg>().value, bp = bk; // kezdő, és célpozíciók
    int jk = right.get_position<deg>().value, jp = jk;
    //int adiff = bk - jk;
    int jdist =kj-jk +dist - round((szog * axle2 / diam / 2));            //+ vdiff - adiff) / 2);
    int bdist =kb-bk +dist + round((szog * axle2 / diam / 2));            //+ vdiff - adiff
    Logger::info(bdist, jdist);

    int bc = bk + bdist, jc = jk + jdist; // célpozicio (előjeles)
    int bd = abs(bp - bk), jd = abs(jp - jk);       // megtett táv
    int bh = bc - bp, jh = jc - jp;       // hátralévő táv (előjeles)
    Logger::info(bc, jc);

    bool jfeleninnen = 1, bfeleninnen = 1;
    int jir = 0, bir = 0;
    if (jdist < 0)
        jir = -1;
    else if (jdist > 0)
        jir = 1;
    if (bdist < 0)
        bir = -1;
    else if (bdist > 0)
        bir = 1;
    bdist = abs(bdist);
    jdist = abs(jdist);


    double barany = 1, jarany = 1;
    if (bdist < jdist)
        barany =(double) bdist / jdist;
    if (jdist < bdist)
        jarany = (double) jdist / bdist;

    bool balmegy = 1, jobbmegy = 1;    
    int speedb = 0, speedj = 0;
    int corrb=1, corrj=1;
    double bmarany=1, jmarany=1;

    while (balmegy || jobbmegy)
    {
 
        //Logger::info(bd-jd, bdist, jdist, bd, jd, speedb, speedj, balmegy, jobbmegy,jarany);
        bp = left.get_position<deg>().value;
        jp = right.get_position<deg>().value;
        bd = abs (bp-bk);
        jd = abs (jp-jk);
        bh = abs (bc - bp);
        jh = abs (jc - jp);
        if(bd>bh)
        bfeleninnen=false;
        if(jd>jh)
        jfeleninnen=false;
        
        corrb=1;
        corrj=1;

        if(bd>bdist-5)
        {
        balmegy=0;
        if(bd>bdist+5)
        {
        balmegy=1;
        corrb=-1;
        }
        }
        if(jd>jdist-5)
        {
        jobbmegy=0;
        if(jd>jdist+5)
        {
        corrj=-1;
        jobbmegy=1;
        }
        }

        if(bdist&&jdist)
        {
            jmarany=(double)jd/jdist;
            bmarany=(double)bd/bdist;
        }







        if ((balmegy||send!=0)||bh>6)
        {
            if (bfeleninnen)
            {
                speedb = round(sqrt(bd * alma2));

                if (speedb > sm)
                    speedb =sm;
                speedb*=barany;
                if (speedb < s0)
                    speedb = s0;
                
            }
            else 
            {
                speedb = round(sqrt(bh * alma2));

                if (speedb > sm)
                    speedb =sm;
                speedb*=barany;
                if (speedb<s0)
                {
                    speedb = send;
                    balmegy=0;
                }
                if(speedb < send)
                {
                    speedb = send;
                }
            }
            if(bmarany>jmarany)
            speedb*=0.875;

            left.on(deg(speedb*bir*corrb));
        }
        else if(send==0)
        left.stop();
        if ((jobbmegy||send!=0)||jh>6)
        {
            if (jfeleninnen)
            {
                speedj = round(sqrt(jd * alma2));

                if (speedj > sm)
                    speedj =sm;
                speedj*=jarany;
                if (speedj < s0)
                    speedj = s0;
                
            }
            else 
            {
                speedj = round(sqrt(jh * alma2));

                if (speedj > sm)
                    speedj =sm;
                speedj*=jarany;
                if (speedj<s0)
                {
                    speedj = send;
                    jobbmegy=0;
                }
                if(speedj < send)
                {
                    speedj = send;
                }
            }
            if(jmarany>bmarany)
            speedj*=0.875;

            right.on(deg(speedj*jir*corrj));
        }
        else if(send==0)
        right.stop();
        
    }
    Logger::info(bp, jp);
    kj+=dist-round((szog * axle2 / diam) / 2);
    kb+=dist+round((szog * axle2 / diam) / 2);
    Logger::info(kj,kb);
//        vdiff+=(szog*axle2/diam);
}

void fekez22(int dist, int szog, int sm, int send)
{
    szog=-szog;
    int bk = left.get_position<deg>().value, bp = bk; // kezdő, és célpozíciók
    int jk = right.get_position<deg>().value, jp = jk;
    //int adiff = bk - jk;
    int jdist =kj-jk +dist - round((szog * axle2 / diam / 2));            //+ vdiff - adiff) / 2);
    int bdist =kb-bk +dist + round((szog * axle2 / diam / 2));            //+ vdiff - adiff
    Logger::info(bdist, jdist);

    int bc = bk + bdist, jc = jk + jdist; // célpozicio (előjeles)
    int bd = abs(bp - bk), jd = abs(jp - jk);       // megtett táv
    int bh = bc - bp, jh = jc - jp;       // hátralévő táv (előjeles)


    int jir = 0, bir = 0;
    if (jdist < 0)
        jir = -1;
    else if (jdist > 0)
        jir = 1;
    if (bdist < 0)
        bir = -1;
    else if (bdist > 0)
        bir = 1;
    bdist = abs(bdist);
    jdist = abs(jdist);


    double barany = 1, jarany = 1;
    if (bdist < jdist)
        barany =(double) bdist / jdist;
    if (jdist < bdist)
        jarany = (double) jdist / bdist;

    bool balmegy = 1, jobbmegy = 1;    
    int speedb = 0, speedj = 0;
    int corrb=1, corrj=1;
    double bmarany=1, jmarany=1;

    while (balmegy || jobbmegy)
    {
        //Logger::info(bd-jd, bdist, jdist, bd, jd, speedb, speedj, balmegy, jobbmegy,jarany);
        bp = left.get_position<deg>().value;
        jp = right.get_position<deg>().value;
        bd = abs (bp-bk);
        jd = abs (jp-jk);
        bh = abs (bc - bp);
        jh = abs (jc - jp);
        
        corrb=1;
        corrj=1;

        if(bd>bdist-5)
        {
        balmegy=0;
        if(bd>bdist+5)
        {
        balmegy=1;
        corrb=-1;
        }
        }
        if(jd>jdist-5)
        {
        jobbmegy=0;
        if(jd>jdist+5)
        {
        corrj=-1;
        jobbmegy=1;
        }
        }

        if(bdist&&jdist)
        {
            jmarany=(double)jd/jdist;
            bmarany=(double)bd/bdist;
        }







        if ((balmegy||send!=0)||bh>6)
        {

                speedb = round(sqrt(bh * alma2));

                if (speedb > sm)
                    speedb =sm;
                speedb*=barany;
                if(speedb < send)
                {
                    if(send==0)
                    balmegy=0;
                    speedb = send;
                }
            
            if(bmarany>jmarany)
            speedb*=0.875;

            left.on(deg(speedb*bir*corrb));
        }
        else if(send==0)
        left.stop();
        if ((jobbmegy||send!=0)||jh>6)
        {
                speedj = round(sqrt(jh * alma2));

                if (speedj > sm)
                    speedj =sm;
                speedj*=jarany;

                if(speedj < send)
                {
                    if(send==0)
                    jobbmegy=0;
                    speedj = send;
                }
            if(jmarany>bmarany)
            speedj*=0.875;

            right.on(deg(speedj*jir*corrj));
        }
        else if(send==0)
        right.stop();
        
    }
    Logger::info(bp, jp);
    kj+=dist-round((szog * axle2 / diam) / 2);
    kb+=dist+round((szog * axle2 / diam) / 2);
  //  Logger::info(kj,kb);
//        vdiff+=(szog*axle2/diam);
}


void meggy21(int dist, int szog, int s0, int sm, int send)
{
    szog=-szog;
    int smin=30;
    int bk = left.get_position<deg>().value, bp = bk; // kezdő, és célpozíciók
    int jk = right.get_position<deg>().value, jp = jk;
    //int adiff = bk - jk;
    int jdist =kj-jk +dist - round((szog * axle2 / diam / 2));            //+ vdiff - adiff) / 2);
    int bdist =kb-bk +dist + round((szog * axle2 / diam / 2));            //+ vdiff - adiff
    Logger::info(bdist, jdist);

    int bc = bk + bdist, jc = jk + jdist; // célpozicio (előjeles)
    int bd = abs(bp - bk), jd = abs(jp - jk);       //abs megtett táv
    int bh = abs(bc - bp), jh = abs(jc - jp);                 //abs hátralévő táv 

        
    int jir = 0, bir = 0;
    if (jdist < 0)
        jir = -1;
    else if (jdist > 0)
        jir = 1;
    if (bdist < 0)
        bir = -1;
    else if (bdist > 0)
        bir = 1;
    bdist = abs(bdist);
    jdist = abs(jdist);


    double barany = 1, jarany = 1;
    if (bdist < jdist)
        barany =(double) bdist / jdist;
    if (jdist < bdist)
        jarany = (double) jdist / bdist;

    bool jfeleninnen = 1, bfeleninnen = 1;
    bool balmegy = 1, jobbmegy = 1;    
    int speedb = 0, speedj = 0;
    double bmarany=1, jmarany=1;

    while (balmegy || jobbmegy)
    {
 
        Logger::info(bd-jd, bdist, jdist, bd, jd, speedb, speedj, balmegy, jobbmegy,jarany);
        bp = left.get_position<deg>().value;
        jp = right.get_position<deg>().value;
        bd = abs (bp-bk);       //megtett táv   (abs)
        jd = abs (jp-jk);
        bh = abs (bc - bp);     //hátralévő táv (abs)
        jh = abs (jc - jp);
        if(bd>bh)
        bfeleninnen=false;
        if(jd>jh)
        jfeleninnen=false;

        if(bp>bc-5&&bp<bc+5)
        {
            balmegy=false;
        }
        if(jp>jc-5&&jp<jc+5)
        {
            jobbmegy=false;
        }

        if(bdist&&jdist)
        {
            jmarany=(double)jd/jdist;
            bmarany=(double)bd/bdist;
        }

        if (balmegy)
        {
            if (bfeleninnen)
            {
                speedb = round(sqrt(bd * alma2));

                if (speedb > sm)
                    speedb =sm;
                speedb*=barany;
                if (speedb < s0)
                    speedb = s0;
                
            }
            else 
            {
                speedb = round(sqrt(bh * alma2));

                if (speedb > sm)
                    speedb =sm;
                if(speedb < send)
                {
                    speedb = send;
                }
                speedb*=barany;
                if(speedb < smin)
                {
                    speedb = smin;
                    balmegy=0;
                }
            }
            if(bmarany>jmarany)
            speedb*=0.875;

            left.on(deg(speedb*bir));
        }
        else if(send==0)
        left.stop();
        else 
        left.on(deg(send));
        if (jobbmegy)
        {
            if (jfeleninnen)
            {
                speedj = round(sqrt(jd * alma2));

                if (speedj > sm)
                    speedj =sm;
                speedj*=jarany;
                if (speedj < s0)
                    speedj = s0;
                
            }
            else 
            {
                speedj = round(sqrt(jh * alma2));

                if (speedj > sm)
                    speedj =sm;
                if(speedj < send)
                {
                    speedj = send;
                }
                speedj*=jarany;
                if (speedj<smin)
                {
                    speedj = smin;
                    jobbmegy=0;
                }
            }
            if(jmarany>bmarany)
            speedj*=0.875;

            right.on(deg(speedj*jir));
        }
        else if(send==0)
        right.stop();
        else
        right.on(deg(send));
    } 
    if(send)
    {
    left.on(deg(send));
    right.on(deg(send));
    }
    Logger::info(bp, jp);
    kj+=dist-round((szog * axle2 / diam) / 2);
    kb+=dist+round((szog * axle2 / diam) / 2);
    Logger::info(kj,kb);
}
