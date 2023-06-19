/*#include <../include/frt/frt.hpp>
#include "szolvas.hpp"
#include "vn.hpp"*/
#include "root.hpp"
#include "mod.hpp"
#include "moz_old.hpp"
#include "moz.hpp"
#include "szolvas.hpp"
#include "vn.hpp"

using namespace FRT;
using namespace FRT::unit_literals;

int main () 

{

    std::ios_base::sync_with_stdio(false);
    left.set_polarity(TachoMotor::polarities::inversed);
    /*while (1);
    {
        Logger::info(cight.get_reflected_light_intensity(),cight.get_reflected_light_intensity());
        sleep(100ms);
    }
    */
    //meggy20(0, 360,    Logger::info(hol.getblue(),hol.getgreen()); 30, 1000);
    /*
    rekab();
    for(int i=0; i<4; i++)
    {
    meggy20(2000,0,30,1000,0);
    Logger::error("egy");
    sleep(2s);
    meggy20(-2000,0,30, 1000,0);
    Logger::error("ketto");
    sleep(2s);
    meggy20(200,90,30,1000,0);
    Logger::error("harom");
    sleep(2s);
    meggy20(-200,-90,30,1000,0);
    Logger::error("meh");
    sleep(2s);
    meggy20(0,180, 30,1000,0);
    sleep(4s);
    Logger::error("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    meggy20(0,-180,30,1000,0);
    sleep(2s);
    }

    */
/*
Logger::info(hol.get_blue());
szolvas102(600);
fekez22(200,0,600,0);
//   haladsze5(60,100,1,100,100);
//   szolvas101();
//   haladsze5(800,600,2);

sleep(500ms);
Logger::info(left.get_position<deg>().value,right.get_position<deg>().value);
Logger::info(szinek[0],szinek[1]);
*/
/*
meggy20(200,0,30,1200,400);
vonalramenes2();
fekez22(500,0,400,0);
*/
/*
vonalkv20(500,30,600,0);
meggy20(0,0,30,1200,0);
*/

//   meggy20(515,0,30,300,0);
/*
szolvas102(400);
Logger::info(left.get_position<deg>().value,right.get_position<deg>().value);
fekez22(285,0,1200,0);*/
/*
szolvas103(600);
fekez22(270,0,1200,0);
*/
//meggy20(-300,0,40,1200,0);
//meggy20(0,180,30,1200,0);
//meggy20(800,90,40,1200,300);
//meggy20(200,0,300,1200,300);
//meggy20(200,-90,300,1200,0);

//meggy20(0,360,30,1200,0);
//meggy21(400,0,30,1200,600);
//meggy21(400,0,400,1200,0);
//vonalkv20(-1500,30,1300,0);
//meggy21(0,-720,30,1200,0);

szolvas103(400);
meggy21(270,0,400,1200,0);
meggy21(-100,0,30,1200,0);
meggy21(0,-120,30,1200,0);
meggy21(800,120,30,1200,0);
sleep(1000ms);

Logger::info(left.get_position<deg>().value,right.get_position<deg>().value,szinek[0],szinek[1]);



}
