//#include <../include/frt/frt.hpp>
//#include "moz.hpp"
#include "root.hpp"
#include "moz.hpp"

using namespace FRT;
using namespace FRT::unit_literals;
double axlerad=axle2/diam;
int axlefok=round(axlerad/pi*180);
double szrad2=fsztav*2/diam;
int fszfok=round(szrad2/pi*180);

void vonalramenes(int col)
{
    bool balt=0, jobbt=0;
    while(!balt&&!jobbt)
    {
        if(ceft.get_reflected_light_intensity()==col)
        {
        kb=left.get_position<deg>().value;
        balt=1;
        }
        if(cight.get_reflected_light_intensity()==col)
        {
        kj=right.get_position<deg>().value;
        jobbt=0;
        }
    }
    kdiff = kb - kj;
    Logger::info(kdiff);
    vdiff = kdiff;
    bva=kb, jva=kj;

}
 void vonalramenes2()
 {
    //az eltérést nézze, és nem a poziciót
    int be=0,je=0;
    int cal=ceft.get_reflected_light_intensity(), cob=cight.get_reflected_light_intensity();
    bool balt=1, jobbt=1, bf=0, jf=0;

    while(balt || jobbt)
    {
        if(cal>40&&balt)
        bf=1;
        if(bf&&cal<20)
        {
        be=left.get_position<deg>().value;
        bf=0;
        balt=0;
        }
        if(cob>40&&jobbt)
        jf=1;
        if(jf&&cob<20)
        {
        je=left.get_position<deg>().value;
        jf=0;
        jobbt=0;
        }
        cob=cight.get_reflected_light_intensity();
        cal=ceft.get_reflected_light_intensity();
    }
    double alpha=atan((double)abs(be-je)/axlefok);
    int corr=round(alpha*axlefok/2*axlefok/fszfok);
    int kp=round(be-(be-je)/2);
    if(je-jva>be-bva)
    {
    kb=kp+corr;
    kj=kp-corr;
    }
    else
    {
    kb=kp-corr;
    kj=kp+corr;
    }
    kdiff = kb - kj;
    Logger::info(corr, kp,alpha, be, je, kdiff,kj, kb);
    vdiff = kdiff;
    bva=kb, jva=kj;

 }

 void vonalkv20(int dist,int s0, int sm, int send)
{
    int smin=30;
    double speed=0;
    double alma=2000;
    int kp=left.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    //int feh=70;
    //int fek=9;
    int id=30;
    int i=0;
    int sd;
    int sd0=ceft.get_reflected_light_intensity();
    double bx=1;
    int fi[8]{-1},fk[8]{-1};
    double kt=1;
    int j=0;
    int k=0;
    double corr=1.05;
    int bp=left.get_position<deg>().value,jp=right.get_position<deg>().value;
    
    if (dist<0)
    {
        while(bp>distgy && speed<sm)
        {
            bp=left.get_position<deg>().value;jp=right.get_position<deg>().value;
            sd=cight.get_reflected_light_intensity();
            Logger::info(sd,bx,kt,left.get_speed<deg>().value, right.get_speed<deg>().value,bp,jp);
            if(sd>sd0)
            {
                bx*=corr;
                fi[j]=i+7;
                j++;
                j=j%8;
            }
            else if(sd<sd0)
            {
                bx/=corr;
                fk[j]=i+7;
                j++;
                j=j%8;
            }
            kt=(double)sd/id;
            //if(sd>feh||sd<fek)
            kt*=bx;
            //left.config.position_coefficient=kt;
            speed=(sqrt(abs(bp-kp)*alma));
            
            left.on(deg(-(speed+s0)/kt));
            right.on(deg(-kt*(speed+s0)));
            sd0=sd;
            if(i==fi[k])
            {
                bx/=corr;
                k++;
                k=k%8;
            }
            if(i==fk[k])
            {
                bx*=corr;
                k++;
                k=k%8;
            }

            i++;
        }

        while(bp>distl&&speed>smin)
        {   
            bp=left.get_position<deg>().value;
            sd=cight.get_reflected_light_intensity();
            Logger::info(sd,bx,kt,left.get_speed<deg>().value, right.get_speed<deg>().value,bp,jp);
            if(sd>sd0)
            {
                bx*=corr;
                fi[j]=i+7;
                j++;
                j=j%8;
            }
            else if(sd<sd0)
            {
                bx/=corr;
                fk[j]=i+7;
                j++;
                j=j%8;
            }
            kt=(double)sd/id;
            //if(sd>feh||sd<fek)
            kt*=bx;
            //left.config.position_coefficient=kt;
            speed=(sqrt(abs(bp-kp)*alma));
            if(speed<send)
            speed=send;
            if(speed>sm)
            speed=sm;
            if(speed<smin)
            speed=smin;
            left.on(-deg(speed/kt));
            right.on(-deg(kt*speed));
            sd0=sd;
            if(i==fi[k])
            {
                bx/=corr; 
                k++;
                k=k%8;
            }
            if(i==fk[k])
            {
                bx*=corr;
                k++;
                k=k%8;
            }

            i++;
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
        
    }

    else
    {
        while(left.get_position<_deg>().value<distgy && speed<sm)
        {   
            speed=(sqrt(abs(kp-left.get_position<_deg>().value)*alma)+s0);
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(left.get_position<deg>().value<distl)
        {   
            speed=(sqrt(abs(left.get_position<_deg>().value-distl)*alma));
            if (speed>(1200))
            speed=(1200);
            if(speed<send)
            speed=send;
            left.on(deg(speed));
            right.on(deg(speed));
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
    }
    kb=left.get_position<deg>().value;
    kj=right.get_position<deg>().value;
}