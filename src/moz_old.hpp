//#include <../include/frt/frt.hpp>
#include "root.hpp"
#pragma once
//#define pi 3.1415926535

using namespace FRT;
using namespace FRT::unit_literals;
/*
TachoMotor left(OUTPUT_A, 6.2cm), right(OUTPUT_B, 6.2cm);
ColorSensor ceft(INPUT_1), cight(INPUT_2);
HTColorSensorV2 hol(INPUT_4);*/
    double axle =/*22.8*/20.162;
    double diam=6.2;
    int kb = 0, kj = 0, kdiff = 0, vdiff = 0;
    int bva, jva;

void halad(double dist,double s0,double sm, double send)
{
    double speed=0;
    double alma=3000;
    int_fast64_t kp=left.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    if (dist>0)
    {
        while(left.get_position<_deg>().value<distgy && speed<sm)
        {
            speed=(sqrt(abs(left.get_position<_deg>().value-kp)*alma));
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
        }

        while(left.get_position<_deg>().value<distl&&speed>30)
        {   
 
            speed=sqrt((distl-left.get_position<_deg>().value)*alma)+send;
            if (speed>sm)
                speed=sm;
            left.on(deg(speed));
            right.on(deg(speed));
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
    }

    else
    {
        while(left.get_position<_deg>().value>distgy && speed<(-sm))
        {   
            speed=(-(sqrt(abs(kp-left.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(left.get_position<deg>().value>distl&&!left.is_overloaded()&&speed<send)
        {   
            speed=(-sqrt(abs(left.get_position<_deg>().value-distl)*alma));
            if (speed<(-1200))
            speed=(-1200);
            left.on(deg(speed));
            right.on(deg(speed));
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
    }
}

void haladj(double dist,double s0,double sm,double send)
{
    double speed=0;
    double alma=1000;
    double kp=right.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    if (dist>0)
    {
    while(right.get_position<_deg>().value<distgy && speed<sm&&!left.is_stalled())
    {   
        speed=(sqrt((right.get_position<_deg>().value-kp)*alma)+s0);
        left.on(deg(speed));
        right.on(deg(speed));
    }
    
    while(right.get_position<_deg>().value<distl&&(speed>send&&speed>30)&&!left.is_stalled())
    {   
 
        speed=sqrt((distl-right.get_position<_deg>().value)*alma)+send;
        if (speed>sm)
        speed=sm;  

        left.on(deg(speed));
        right.on(deg(speed));

    }
    if(send==0)
    {
    left.stop();
    right.stop();
    }
    }
    else
    {
        while(right.get_position<_deg>().value>distgy && speed>(-sm)&&!left.is_overloaded())
        {   
            speed=(-(sqrt(abs(kp-right.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(right.get_position<_deg>().value>distl&&speed<send&&speed<(-30)&&!left.is_overloaded())
        {   
 
            speed=(-sqrt(abs(right.get_position<_deg>().value-distl)*alma))+send;

        
            if (speed<(-sm))
            speed=(-sm);
            left.on(deg(speed));
            right.on(deg(speed));
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
    }
}
void valas ()
{
    double brrr=50;
    double brrr2=30;
    double s0=360;
    double s1=60;
    double lr = cight.get_reflected_light_intensity();
    double ll = ceft.get_reflected_light_intensity();
    while(lr<brrr&&ll<brrr)
    {

        //Logger::info(ll);
        left.on(deg(s0));
        right.on(deg(s0));
        lr = cight.get_reflected_light_intensity();
        ll = ceft.get_reflected_light_intensity();

    } 
    Logger::info("brrr");
    if(ll>=brrr)
    {
        left.stop();
        //Logger::info("=r");
        while(lr<brrr)
        {
            lr = cight.get_reflected_light_intensity()+1;
            right.on(deg(2*s1));
            //Logger::info(lr);
        }
    }
    else if(lr>=brrr)
    {
        right.stop();
        while(ll<brrr)
        {
            left.on(deg(2*s1));
            Logger::info(ll);
            ll = ceft.get_reflected_light_intensity();
        }
    }
    Logger::info("brrrrrrr");
    right.stop();
    left.stop();
    double sl=10, sr=10;
    while (ll!=brrr2+-1||lr!=brrr2+-1)
    {
        lr = cight.get_reflected_light_intensity()+1;
        ll = ceft.get_reflected_light_intensity();  
        //Logger::info(sl);
        //Logger::info(sr);  
        if (sl!=0) 
        left.on(deg(sl));
        if (sr!=0)
        right.on(deg(sr)); 
        if(lr<brrr2+1)
            sr=-(s1);
        else if(lr>brrr2-1)
            sr=+(s1);
        else
        {
            right.stop();
            sr=0;
        }
        if(ll<brrr2+1)
            sl=-(s1);
        else if(ll>brrr2-1)
            sl=(s1);
        else
        {
            left.stop();
            sl=0;
        }
    }
    right.stop();
    left.stop();
}
void gyalas(int s1, int s0, int sz)
{

    bool bm=true, jm=true;
    while(bm||jm)
    {
        if(cight.get_reflected_light_intensity()>sz)
        {
            right.on(deg(s1));
            jm=false;
        }
        if(ceft.get_reflected_light_intensity()>sz)
        {
            left.on(deg(s1));
            bm=false;
        }
    }
    bm=true;
    jm=true;
    while(bm||jm)
    {
        if(cight.get_reflected_light_intensity()<sz)
        {
            jm=false;
            right.on(deg(s0));
        }
        if(ceft.get_reflected_light_intensity()<sz)
        {
            left.on(deg(s0));
            bm=false;
        }
    }
    bm=true;
    jm=true;
    while(bm||jm)
    {
        if(cight.get_reflected_light_intensity()>sz)
        {
            right.stop();
            jm=false;
        }
        else    right.on(deg(s0));
        if(ceft.get_reflected_light_intensity()>sz)
        {
            left.stop();
            bm=false;
        }
        else    left.on(deg(s0));


    }  
}

int bl,gr,c[8]={0};
int done=0;
void haladsz(double dist,double sm, int szk)
{
    int szv=done;
    double s0=30;
    double speed=0;
    double alma=2000;
    double kp=right.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    if (dist>0)
    {
        while(right.get_position<_deg>().value<distgy && speed<sm)
        {   
            speed=(sqrt(abs(right.get_position<_deg>().value-kp)*alma));
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  

            if(done<szk+szv)
            {
                if((bl+2<gr&&gr>12)||(gr+8<bl&&bl>20))
                {
                    Logger::info(bl,gr);
                    if(bl<gr)   c[done]++;
                    else    c[done]--;
                }
                else if(c[done]<-5||c[done]>5)
                {
                    Logger::info(c[done]);
                    done++;
                }
            }
        }
    

        while(right.get_position<_deg>().value<distl&&speed>40)
        {   
 
            speed=sqrt(abs(distl-right.get_position<_deg>().value)*alma);
            if (speed>sm)
                speed=sm;
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            
            if(done<szk+szv)
            {
                Logger::info(bl,gr);
                if((bl+2<gr&&gr>10)||(gr+8<bl&&bl>20))
                {
                    
                    if(bl<gr)   c[done]++;
                    else    c[done]--;
                }
            

                else if(c[done]<-3||c[done]>3)
                {
                    Logger::info(c[done]+"ááá");
                    done++;
                }
            }
        }
        left.stop();
        right.stop();
    }

    else
    {
        while(right.get_position<_deg>().value>distgy && speed>(-sm))
        {   
            speed=(-(sqrt(abs(kp-right.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(right.get_position<deg>().value>distl&&speed<(-40))
        {   
            speed=(-sqrt(abs(right.get_position<_deg>().value-distl)*alma));
            if (speed<(-sm))
            speed=(-sm);
            left.on(deg(speed));
            right.on(deg(speed));
        }
        
        left.stop();
        right.stop();
    }
    for(int i=0;i<6;i++)
    {
    if(c[i]<1)
    Logger::info("kék");
    else
    Logger::info("ződ");
    Logger::info(c[i]);
    }
}

void kany(double szog, double s0, double sm, double send, bool mot, int irany)
{
    double dist=0;
    double speed=0;
    double alma=2000;
    double kpr=right.get_position<_deg>().value;
    double kpl=left.get_position<_deg>().value;

    dist=(szog*2*axle*irany/diam);
    if (mot)
    {
        double distgy=(kpl+dist/2);
        double distl=(kpl+dist);
        right.stop();
        if (dist>0)
        {
            while(left.get_position<_deg>().value<distgy && speed<sm)
            {   
                speed=(sqrt((left.get_position<_deg>().value-kpl)*alma)+s0);
                left.on(deg(speed));
            }
    
            while(left.get_position<_deg>().value<distl&&(speed>send+20&&!left.is_overloaded()))
            {   
 
                speed=sqrt((distl-left.get_position<_deg>().value)*alma)+send;
                if (speed>sm)
                speed=sm;  
                left.on(deg(speed));

            }
            if(send==0)            left.stop();
            else             left.on(deg(send));
        }
        else
        {
            dist=dist-4.8779;
            while(left.get_position<_deg>().value>distgy && speed>(-sm))
            {   
                speed=(-(sqrt(abs(kpl-left.get_position<_deg>().value)*alma)+s0));
                left.on(deg(speed));
            }
    
            while(left.get_position<_deg>().value>distl&&speed<send&&!left.is_overloaded())
           {
                speed=(-sqrt(abs(left.get_position<_deg>().value-distl)*alma))+send;

                if (speed<(-sm))
                speed=(-sm);
                left.on(deg(speed));
            }
            if(send==0)            left.stop();
            else            left.on(deg(send));
        }
    }
    else
    {
        double distgy=(kpr+dist/2);
        double distl=(kpr+dist);
        left.stop();
        if (dist>0)
        {
            dist=dist+4.8779;
            while(right.get_position<_deg>().value<distgy && speed<sm)
            {   
                speed=(sqrt((right.get_position<_deg>().value-kpr)*alma)+s0);
                right.on(deg(speed));
            }
    
            while(right.get_position<_deg>().value<distl&&(speed>send+10&&speed>30))
            {   
 
                speed=sqrt((distl-right.get_position<_deg>().value)*alma)+send;
                if (speed>sm)
                speed=sm;  
                right.on(deg(speed));

            }
            if(send==0)            right.stop();
            else            right.on(deg(send));
        }
        else
        {
            while(right.get_position<_deg>().value>distgy && speed>(-sm))
            {   
                speed=(-(sqrt(abs(kpr-right.get_position<_deg>().value)*alma)+s0));
                right.on(deg(speed));
            }
    
            while(right.get_position<_deg>().value>distl&&speed<send&&speed<(-30))
           {
                speed=(-sqrt(abs(right.get_position<_deg>().value-distl)*alma))+send;

                if (speed<(-sm))
                speed=(-sm);
                right.on(deg(speed));
            }
            if(send==0)     right.stop();
            else            right.on(deg(send));
        }
    }
    //right.config.position_coefficient=1;
    //left.config.position_coefficient=1;
}
void ford(double szog)
{
    int seb=600;
    if(szog<0)
    seb=-600;
    double dist=(szog*axle/diam);
    left.set_polarity(TachoMotor::polarities::normal);
    halad(dist, 30, seb, 0);
    left.set_polarity(TachoMotor::polarities::inversed);
}

void falas(int speed, int sz)
{
    bool bm=true, jm=true;
    while(bm||jm)
    {
        if(cight.get_reflected_light_intensity()>sz)
        {
            right.stop();
            jm=false;
        }
        else    right.on(deg(speed));
        if(ceft.get_reflected_light_intensity()>sz)
        {
            left.stop();
            bm=false;
        }
        else    left.on(deg(speed));

       // Logger::info(cight.get_reflected_light_intensity(),cight.get_reflected_light_intensity());
    

    }  
}

void hvalad(double dist,double s0,double sm)
{
    double speed=0;
    double alma=2000;
    int brrr=25;
    int_fast64_t kp=left.get_position<_deg>().value;
    int_fast64_t distgy=(kp+dist/2);
    int_fast64_t distl=(kp+dist);

        while(left.get_position<_deg>().value>distgy && speed>(-sm))
        {   
            speed=(-(sqrt(abs(kp-left.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
            int sz= cight.get_reflected_light_intensity();
            if(sz<brrr)
            left.config.position_coefficient = (double)(sz-brrr)/25;
            if(sz>brrr){}
        }
        
        while(left.get_position<deg>().value>distl&&speed<(-40))
        {   
            speed=(-sqrt(abs(left.get_position<_deg>().value-distl)*alma));
            if (speed<(-1200))
            speed=(-1200);
            left.on(deg(speed));
            right.on(deg(speed));
        }

        left.stop();
        right.stop();

}
        




void fherig()
{
    while(cight.get_reflected_light_intensity()!=50)
    {}
    left.stop();
    right.stop();
}

void haladsze(double dist,double sm, int szk)
{
    int li0=hol.get_blue();
    int szv=done;
    double s0=30;
    double speed=0;
    double alma=2000;
    double kp=right.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    int d=0;
    int li=hol.get_white();
    
    if (dist>0)
    {
        while(right.get_position<_deg>().value<distgy && speed<sm)
        {   
            speed=(sqrt(abs(right.get_position<_deg>().value-kp)*alma));
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            //li=hol.get_white();
            if (bl>li0+1)
            {
            d=1;
            li0=bl;
            }
            else if(bl<li0-1)
            {
                if(d==1)
                {
                    Logger::info(bl,gr);
                    if(bl<gr){   c[done]++; done ++;}
                    else if (gr<bl){    c[done]--;
                    done++;}
                }
                d=-1;
                li0=bl;
            }
        }
    

        while(right.get_position<_deg>().value<distl&&speed>40)
        {   
 
            speed=sqrt(abs(distl-right.get_position<_deg>().value)*alma);
            if (speed>sm)
                speed=sm;
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();
            li0=li;  
            li=hol.get_white();
            
            if(done<szk+szv)
            {
                if (li>li0)
                d=1;
                else if(li<li0)
                {
                    if(d==1)
                    {
                        Logger::info(bl,gr);
                        if(bl<gr)   c[done]++;
                        else    c[done]--;
                        done++;
                    }
                    d=-1;
                }
            }
        }
        left.stop();
        right.stop();
    }

    else
    {
        while(right.get_position<_deg>().value>distgy && speed>(-sm))
        {   
            speed=(-(sqrt(abs(kp-right.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(right.get_position<deg>().value>distl&&speed<(-40))
        {   
            speed=(-sqrt(abs(right.get_position<_deg>().value-distl)*alma));
            if (speed<(-sm))
            speed=(-sm);
            left.on(deg(speed));
            right.on(deg(speed));
        }
        
        left.stop();
        right.stop();
    }
    for(int i=0;i<6;i++)
    {
    if(c[i]<1)
    Logger::info("kék");
    else
    Logger::info("ződ");
    Logger::info(c[i]);
    }
}

void haladsze2(double dist,double sm, int szk)
{
    int szv=done;
    double s0=30;
    double speed=0;
    double alma=2000;
    double kp=right.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    double arany;
    while(right.get_position<_deg>().value<distgy && speed<sm)
        {   
            speed=(sqrt(abs(right.get_position<_deg>().value-kp)*alma));
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            arany=(double)bl/gr;

            if(done<szk+szv)
            {
                if((gr>15&&arany<0.84)||(arany>1.353&&bl>15))
                {
                    Logger::info(bl,gr);
                    if(bl<gr)   c[done]++;
                    else    c[done]--;
                }
                else if(c[done]<-5||c[done]>5)
                {
                    Logger::info(c[done]);
                    done++;
                }
            }
        }
    

        while(right.get_position<_deg>().value<distl&&speed>40)
        {   
 
            speed=sqrt(abs(distl-right.get_position<_deg>().value)*alma);
            if (speed>sm)
                speed=sm;
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            
            if(done<szk+szv)
            {
                arany=(double)bl/gr;
                Logger::info(bl,gr);
                if((gr>15||bl>15)&&(arany>1.353||arany<0.84))
                {
                    
                    if(bl<gr)   c[done]++;
                    else    c[done]--;
                }      

                else if(c[done]<-3||c[done]>3)
                {
                    Logger::info(c[done]);
                    done++;
                }
            }
        }
    left.stop();
    right.stop();
    for(int i=0;i<6;i++)
    {
    if(c[i]<1)
    Logger::info("kék");
    else
    Logger::info("ződ");
    Logger::info(c[i]);
    }
}

void haladsze5(double dist,double sm, int szk, int send, int s0)
{
    jva+=dist;
    bva+=dist;
    int szv=done;
   // double s0=30;
    double speed=0;
    double alma=2000;
    double kp=right.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    double arany;
    int grm=0, blm=0;
    bool vge=false;
    while(right.get_position<_deg>().value<distgy && speed<sm)
        {   
            speed=(sqrt(abs(right.get_position<_deg>().value-kp)*alma));
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            arany=(double)bl/gr;

            if(done<szk+szv)
            {
                Logger::info(bl,gr);
                    if(gr>grm)
                    {
                    grm=gr;
                    vge=false;
                    }
                    else if(gr<grm)
                    {
                    vge=true;
                    grm=gr;
                    }
                    if(bl>blm)
                    {
                    blm=bl;
                    vge=false;
                    }
                    else if(bl<blm)
                    {
                    vge=true;
                    blm=bl;
                    }

                if(arany>1.1&&bl>15&&!vge)
                {
                    //if(c[done]<=0)
                    c[done]--;
                    
                }
                else if(gr>12&&arany<0.95&&!vge)
                {

                    //if(c[done]>=0)
                    c[done]++;
                }
                if((c[done]<-5||c[done]>5)&&vge)
                {
                    Logger::info(c[done]);
                    done++;
                }
            }
        }
    

        while(right.get_position<_deg>().value<distl&&speed>send)
        {   
 
            speed=sqrt(abs(distl-right.get_position<_deg>().value)*alma);
            if (speed>sm)
                speed=sm;
            left.on(deg(speed+s0));
            right.on(deg(speed+s0));
            bl=hol.get_blue();
            gr=hol.get_green();  
            
            if(done<szk+szv)
            {
                arany=(double)bl/gr;
                Logger::info(bl,gr);
                    if(gr>grm)
                    {
                        grm=gr;
                        vge=false;
                    }
                    else if (gr<grm)
                    {
                    vge=true;
                    grm=gr;
                    }
                    if(bl>blm)
                    {
                        blm=bl;
                        vge=false;
                    }
                    else if (bl<blm)
                    {
                    vge=true;
                    blm=bl;
                    }
                if(arany>1.1&&bl>15&&!vge)
                {

                    //if(c[done]<=0)
                    c[done]--;
                    
                }
                else if(gr>12&&arany<0.95&&!vge)
                {

                    //if(c[done]>=0)
                    c[done]++;
                }

                if((c[done]<-5||c[done]>5)&&vge)
                {
                    Logger::info(c[done]);
                    done++;
                }
            }
        }
    if(send==0)
    {
    left.stop();
    right.stop();
    }
    else 
    {
    left.on(deg(send));
    right.on(deg(send));
    }

                    if((c[done]<-5||c[done]>5)&&done<szk+szv)
                {
                    Logger::info(c[done]);
                    done++;
                }
    
    for(int i=0;i<6;i++)
    {
    if(c[i]<1)
    Logger::info("kék");
    else
    Logger::info("ződ");
    Logger::info(c[i]);
    }
}


void vonalkv(int dist,int s0, int sm, int send)
{
    double speed=0;
    double alma=2000;
    int_fast64_t kp=left.get_position<_deg>().value;
    double distgy=(kp+dist/2);
    double distl=(kp+dist);
    //int feh=70;
    //int fek=9;
    int id=30;
    int i=0;
    int sd;
    int sd0=ceft.get_reflected_light_intensity();
    double bx=1;
    int fi[511]{-1},fk[511]{-1};
    double kt=1;
    int j=0;
    int k=0;
    
    if (dist>0)
    {
        while(left.get_position<_deg>().value<distgy && speed<sm)
        {
            sd=ceft.get_reflected_light_intensity();
            //Logger::info(sd,bx,kt);
            if(sd<sd0)
            {
                bx*=1.05;
                fi[j]=i+5;
                j++;
            }
            else if(sd>sd0)
            {
                bx*=0.95238;
                fk[j]=i+5;
                j++;
            }
            kt=(double)id/sd;
            //if(sd>feh||sd<fek)
            kt*=bx;
            //left.config.position_coefficient=kt;
            speed=(sqrt(abs(left.get_position<_deg>().value-kp)*alma));
            left.on(deg((speed+s0)/kt));
            right.on(deg(kt*(speed+s0)));
            sd0=sd;
            if(i==fi[k])
            {
                bx*=0.95238;
                k++;
            }
            if(i==fk[k])
            {
                bx*=1.05;
                k++;
            }

            i++;
        }

        while(left.get_position<_deg>().value<distl&&speed>30)
        {   
            sd=ceft.get_reflected_light_intensity();
            //Logger::info(sd,bx,kt);
            if(sd<sd0)
            {
                bx*=1.05;
                fi[j]=i+5;
                j++;
            }
            else if(sd>sd0)
            {
                bx=bx/1.05;
                fk[j]=i+5;
                j++;
            }
            kt=(double)id/sd;
            //if(sd>feh||sd<fek)
            kt*=bx;
            //left.config.position_coefficient=kt;
            speed=(sqrt(abs(left.get_position<_deg>().value-kp)*alma));
            left.on(deg((speed+s0)/kt));
            right.on(deg(kt*(speed+s0)));
            sd0=sd;
            if(i==fi[k])
            {
            bx=bx/1.05;
            k++;
            }
            if(i==fk[k])
            {
                bx*=1.05;
                k++;
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
        while(left.get_position<_deg>().value>distgy && speed<(-sm))
        {   
            speed=(-(sqrt(abs(kp-left.get_position<_deg>().value)*alma)+s0));
            left.on(deg(speed));
            right.on(deg(speed));
        }
    
        while(left.get_position<deg>().value>distl&&!left.is_overloaded()&&speed<send)
        {   
            speed=(-sqrt(abs(left.get_position<_deg>().value-distl)*alma));
            if (speed<(-1200))
            speed=(-1200);
            left.on(deg(speed));
            right.on(deg(speed));
        }
        if(send==0)
        {
        left.stop();
        right.stop();
        }
    }
    
}