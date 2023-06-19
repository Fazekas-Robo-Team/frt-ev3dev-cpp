#pragma once
#include "root.hpp"
#include "moz.hpp"
#include <iostream>
#include <algorithm>


using namespace FRT;
using namespace FRT::unit_literals;


int alma3[4]={30,30,60,60};
/*
void hkhaladva(int bdista, int bs0, int bse, int jdista, int js0, int jse,
int kdista, int ks0, int kse, int edist, int es0, int ese)
{
    int deltabs=bse-bs0;
    int bk=left.get_position<deg>().value;
    int bdist=kb-bk+bdista;
    int szamoltbdisth=abs(bdist);

    int sm=1400;

    double btm=0, jtm=0, ktm=0, etm=0;
    btm=(double)abs(bs0-bse)/alma2;
    szamoltbdisth-=abs(bs0*btm)+abs((bse-bs0)/2);
    if(szamoltbdisth<0)
    {
        Logger::error("rossz adat 
        nem tud ilyen távon felgyorsulni");
        while(1);
    }
    if()      
    btm+=sqrt(abs((double)alma2/));


    
}*/

/*
void hkhaladva2(int motkell[4],int dista[4],int  s0[4],int se[4])
{
    int sm=1400;
    int k[4]={left.get_position<deg>().value, right.get_position<deg>().value, emel.get_position<deg>().value, kamp.get_position<deg>().value};
    int dist[4];
    int szamoltdisth[4];
    double tmsz[4][4]={0};
    double alaparany[4]={1};
    for(int i=0; i<4;i++)
    {
        dist[i]=va[i]-k[i]+dista[i];
        szamoltdisth[i]=dist[i]
        tmsz[i][0]=(double)abs(s0[i]-se[i])/alma3[i];
        szamoltdisth[i]-=abs(s0[i]*tmsz[i][0])+abs((se[i]-s0[i])/2);
        if(szamoltdisth[i]<0)
        {
            Logger::error("rossz adat nem tud ilyen távon felgyorsulni");
            while(1);
        }
        if(pow(sm-std::max(s0[i],se[i]),2)/2/alma3[i]+abs((sm-std::max(s0[i],se[i]))*s0[i])*2>szamoltdisth[i])
        {
            tmsz[i][1]=(sqrt((pow(2*std::max(s0[i],se[i]),2)-4*alma3[i]*szamoltdisth[i]))-2*std::max(s0[i],se[i]))/alma3[i];

        }
        else
        {
            tmsz[i][1]=(double)abs(sm-s0[i])/alma3[i]*2;
            //szamoltdisth[i]-=
            tmsz[i][2]=(double)szamoltdisth[i]/sm;
        }
        tmsz[i][3]=tmsz[i][1]+tmsz[i][2]+tmsz[i][0];        
    }
    for(int i=0; i<4;i++)
    {
        alaparany[i]=tmsz[i][3]/std::max(std::max(tmsz[0][3],tmsz[1][3]),std::max(tmsz[2][3], tmsz[3][3]));
    }
}*/

void modkez (int dist, int szog, int s0, int sm, int send, bool mu[2], int distm[2], int ms [2])
{
    szog=-szog;
    int smin=0;

    bool modmegy=1;
    int mirany[2]={1};
    for(int i=0; i<2; i++)
    {
    if(distm[i]<0)
    mirany[i]=-1;
    mva[i]+=distm[i];
    }
    int mp[2]={(int)emel.get_position<deg>().value,(int)kamp.get_position<_deg>().value};

    int bk = left.get_position<deg>().value, bp = bk; // kezdő, és célpozíciók
    int jk = right.get_position<deg>().value, jp = jk;
    //int adiff = bk - jk;
    int jdist =kj-jk +dist - round((szog * axle2 / diam / 2));            //+ vdiff - adiff) / 2);
    int bdist =kb-bk +dist + round((szog * axle2 / diam / 2));            //+ vdiff - adiff
    Logger::info(bdist, jdist);

    int bc = bk + bdist, jc = jk + jdist; // célpozicio (előjeles)
    int bd = abs(bp - bk), jd = abs(jp - jk);       // megtett táv
    int bh = bc - bp, jh = jc - jp;       // hátralévő táv (előjeles)


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
    double bmarany=1, jmarany=1;

    while (balmegy || jobbmegy||modmegy)
    {
        if(mu[0]&&mp[0]>5+mva[0]&&mp[0]<mva[0]-5)
        {
            emel.on(deg(ms[0]*mirany[0]));
            mp[0]=emel.get_position<deg>().value;
        }
        else
        {
        mu[0]=0;
        emel.stop();
        }
        
        if(mu[1]&&mp[1]>5+mva[1]&&mp[1]<mva[1]-5)
        {
            kamp.on(deg(ms[1]*mirany[1]));
            mp[1]=kamp.get_position<deg>().value;
        }
        else
        {
        mu[1]=0;
        kamp.stop();
        }
        modmegy=mu[0]&&mu[1];

         
        //Logger::info(bd-jd, bdist, jdist, bd, jd, speedb, speedj, balmegy, jobbmegy,jarany);
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
                if(speedj < send)
                {
                    speedj = send;
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
}
