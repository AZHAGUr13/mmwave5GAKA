/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "five-g-aka-helper.h"
 #include <string>
#include<iostream>
namespace ns3 {

/* ... */

fivegakaHelper::fivegakaHelper()
{
        publicKey ="FA09";
        supi = "0981";
        snid = "BCDE";
        std::cout<<"Constructor called\n";
}

std::string fivegakaHelper::freshKey(std::string r1,std::string r2)
{
        /*fiveGAka f;
        std::string o = f.freshkey(r1,r2);
        std::string SUCI = f.aes(publicKey,supi,o,snid);
        return SUCI;*/
        return "a";
}
}

