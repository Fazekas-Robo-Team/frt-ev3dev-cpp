//#include <../include/frt/frt.hpp>
#include "root.hpp"
#include "moz.hpp"

using namespace FRT;
using namespace FRT::unit_literals;

int szinek[6]={0};        //-1=>gr      1=>bl

int haladsze6(int dist)
{
    //int kp=left.get_position<deg>().value;
    int gr=0, bl=0;
    while(left.get_position<deg>().value<kb+dist)
    {
        Logger::info(hol.get_blue(), hol.get_green());
        gr+=hol.get_green();
        bl+=hol.get_blue();
    }
    kb+=dist;
    kj+=dist;
    Logger::info(bl,gr);
    if(gr>bl)
    return(-1);
    else 
    return (1);
}

int szolvas101()
{
    meggy20(400,0,30,600,100);
    haladsze5(20,100,1,100,100);
    Logger::info(hol.get_blue(),hol.get_green());
    meggy20(20,0,100,100,100);
    haladsze5(20,100,1,100,100);
    Logger::info(hol.get_blue(),hol.get_green());
    return(0);
}
void szolvas102(int s)
{
   meggy21(370,0,30,1200,s);//350
   szinek[0]=haladsze6(60); 
   meggy21(35,0,s,s,s); //20
   szinek[1]=haladsze6(70);  //60
   //meggy20(0,0,s,s,0);
   //fekez22(60,0,s,0);
   Logger::info(szinek[0],szinek[1]);
}

//mozadalek:               Logger::info(bp,jp,hol.get_green(),hol.get_blue());

void szolvas103(int s)
{
    meggy21(380,0,30,s,s);
    szinek[0]=haladsze6(50);
    meggy21(40,0,s,s,s);
    szinek[1]=haladsze6(60);
    Logger::info(szinek[0],szinek[1]);

}


