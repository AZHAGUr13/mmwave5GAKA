/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef FIVE_G_AKA_HELPER_H
#define FIVE_G_AKA_HELPER_H

#include "ns3/five-g-aka.h"

namespace ns3 {

/* ... */
class fivegakaHelper
{
        std::string publicKey,supi,snid;
public:
        fivegakaHelper();
        std::string freshKey(std::string,std::string);
};
class fivegakaHNHelper
{
public:
        fivegakaHNHelper();
        
        std::string freshKey(std::string,std::string);
};
}

#endif /* FIVE_G_AKA_HELPER_H */

