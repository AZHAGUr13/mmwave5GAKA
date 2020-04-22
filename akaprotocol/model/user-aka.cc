/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "user-aka.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include "ns3/netanim-module.h"
#include "ns3/custom-data-tag.h"
#include "ns3/net-device.h"
#include "ns3/myheader.h"
#include "ns3/sha256.h"
#include "vector"
#include<string>
#include<map>
#include<iostream>
#include "ns3/log.h"
#include "ns3/simulator.h"
typedef unsigned char u8;
void Acomple(u8 r1[],u8 fresh[])
{
        for( unsigned int i=0; i<16; i++)
         {
              fresh[i]=0xff-r1[i];            
         }

}/* Acomplement function end*/  
/* addition function*/
void  Aadd(u8 r1[],u8 r2[],u8 addition[])
{
          
        
  for( unsigned int i=0; i<16; i++)
  {
    addition[i] = (r1[i]+r2[i]) % 32;
  }             
        
	
}/* Addtion function end */
void Arotr(u8 r[], unsigned int sumDigit,u8 rotation[])
{
        unsigned int k=0;
        for( unsigned int i=sumDigit; i<16; i++)
        {
          rotation[k++]=r[i];
        }
                
        for(unsigned int i=0; i<sumDigit; i++)
          rotation[k++]=r[i];
        

	
}
void Afreshkey(u8 r1[], u8 r2[],u8 fresh[])
{
        unsigned int sumDigit = 0;
        u8 rotation[16],adddition[16];
       // std::cout<<"\nIndividual Addition\n";
        for(unsigned int i=0; i<16; i++)
        {
               sumDigit +=  r1[i];
                
         //       std::cout<<sumDigit<<" "<<int(r1[i])<<std::endl;
        }
       
        sumDigit = sumDigit % 16;
        //std::cout<<"\n Mod:"<<sumDigit;
        Arotr(r2, sumDigit,rotation );
        //std::cout<<"\nRotation :\n";
        for(unsigned int i=0;i<16;i++)
        {
     //     std::cout<<int(rotation[i])<<" ";
        }
        Aadd(r1,rotation,adddition);
       // std::cout<<"\nAddition :\n";
        for(unsigned int i=0;i<16;i++)
        {
         // std::cout<<int(adddition[i])<<" ";
        }
         Acomple(adddition,fresh);
        //std::cout<<"\ncompl :\n";
        for(unsigned int i=0;i<16;i++)
        {
          //std::cout<<int(fresh[i])<<" ";
        }
        
}
            typedef unsigned char u8;
u8 AOP[16] = {0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65,
             0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};
            /*------- Insert your value of OP here -------*/
void AComputeOPc( u8 op_c[16] );
void ARijndaelKeySchedule( u8 key[16] );
void ARijndaelEncrypt( u8 input[16], u8 output[16] );
void AComputeOPc( u8 op_c[16] )
{
  u8 i;
  
  ARijndaelEncrypt( AOP, op_c );
  for (i=0; i<16; i++)
    op_c[i] ^= AOP[i];

  return;
} /* end of function ComputeOPc */
/*-------------------- Rijndael round subkeys ---------------------*/
u8 AroundKeys[11][4][4];

/*--------------------- Rijndael S box table ----------------------*/
u8 AS[256] = {
 99,124,119,123,242,107,111,197, 48,  1,103, 43,254,215,171,118,
202,130,201,125,250, 89, 71,240,173,212,162,175,156,164,114,192,
183,253,147, 38, 54, 63,247,204, 52,165,229,241,113,216, 49, 21,
  4,199, 35,195, 24,150,  5,154,  7, 18,128,226,235, 39,178,117,
  9,131, 44, 26, 27,110, 90,160, 82, 59,214,179, 41,227, 47,132,
 83,209,  0,237, 32,252,177, 91,106,203,190, 57, 74, 76, 88,207,
208,239,170,251, 67, 77, 51,133, 69,249,  2,127, 80, 60,159,168,
 81,163, 64,143,146,157, 56,245,188,182,218, 33, 16,255,243,210,
205, 12, 19,236, 95,151, 68, 23,196,167,126, 61,100, 93, 25,115,
 96,129, 79,220, 34, 42,144,136, 70,238,184, 20,222, 94, 11,219,
224, 50, 58, 10, 73,  6, 36, 92,194,211,172, 98,145,149,228,121,
231,200, 55,109,141,213, 78,169,108, 86,244,234,101,122,174,  8,
186,120, 37, 46, 28,166,180,198,232,221,116, 31, 75,189,139,138,
112, 62,181,102, 72,  3,246, 14, 97, 53, 87,185,134,193, 29,158,
225,248,152, 17,105,217,142,148,155, 30,135,233,206, 85, 40,223,
140,161,137, 13,191,230, 66,104, 65,153, 45, 15,176, 84,187, 22,
};

/*------- This array does the multiplication by x in GF(2^8) ------*/
u8 AXtime[256] = {
  0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
 96, 98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,
128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,
160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,
192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,
224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,
 27, 25, 31, 29, 19, 17, 23, 21, 11,  9, 15, 13,  3,  1,  7,  5,
 59, 57, 63, 61, 51, 49, 55, 53, 43, 41, 47, 45, 35, 33, 39, 37,
 91, 89, 95, 93, 83, 81, 87, 85, 75, 73, 79, 77, 67, 65, 71, 69,
123,121,127,125,115,113,119,117,107,105,111,109, 99, 97,103,101,
155,153,159,157,147,145,151,149,139,137,143,141,131,129,135,133,
187,185,191,189,179,177,183,181,171,169,175,173,163,161,167,165,
219,217,223,221,211,209,215,213,203,201,207,205,195,193,199,197,
251,249,255,253,243,241,247,245,235,233,239,237,227,225,231,229
};

void ARijndaelKeySchedule( u8 key[16] )
{
  u8 roundConst;
  int i, j;

  /* first round key equals key */
  for (i=0; i<16; i++)
    AroundKeys[0][i & 0x03][i>>2] = key[i];

  roundConst = 1;

  /* now calculate round keys */
  for (i=1; i<11; i++)
  {
    AroundKeys[i][0][0] = AS[AroundKeys[i-1][1][3]]
                         ^ AroundKeys[i-1][0][0] ^ roundConst;
    AroundKeys[i][1][0] = AS[AroundKeys[i-1][2][3]]
                         ^ AroundKeys[i-1][1][0];
    AroundKeys[i][2][0] = AS[AroundKeys[i-1][3][3]]
                         ^ AroundKeys[i-1][2][0];
    AroundKeys[i][3][0] = AS[AroundKeys[i-1][0][3]]
                         ^ AroundKeys[i-1][3][0];

    for (j=0; j<4; j++)
    {
      AroundKeys[i][j][1] = AroundKeys[i-1][j][1] ^ AroundKeys[i][j][0];
      AroundKeys[i][j][2] = AroundKeys[i-1][j][2] ^ AroundKeys[i][j][1];
      AroundKeys[i][j][3] = AroundKeys[i-1][j][3] ^ AroundKeys[i][j][2];
      }

    /* update round constant */
    roundConst = AXtime[roundConst];
  }

  return;
} /* end of function RijndaelKeySchedule */


/* Round key addition function */
void AKeyAdd(u8 state[4][4], u8 roundKeys[11][4][4], int round)
{
  int i, j;

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      state[i][j] ^= roundKeys[round][i][j];

  return;
}
/* Byte substitution transformation */
int AByteSub(u8 state[4][4])
{
  int i, j;

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      state[i][j] = AS[state[i][j]];
  
  return 0;
}


/* Row shift transformation */
void AShiftRow(u8 state[4][4])
{
  u8 temp;

  /* left rotate row 1 by 1 */
  temp = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = temp;

  /* left rotate row 2 by 2 */
  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;
  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  /* left rotate row 3 by 3 */
  temp = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = temp;

  return;
}


/* MixColumn transformation*/
void AMixColumn(u8 state[4][4])
{
  u8 temp, tmp, tmp0;
  int i;

  /* do one column at a time */
  for (i=0; i<4;i++)
  {
    temp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
    tmp0 = state[0][i];

    /* Xtime array does multiply by x in GF2^8 */
    tmp = AXtime[state[0][i] ^ state[1][i]];
    state[0][i] ^= temp ^ tmp;

    tmp = AXtime[state[1][i] ^ state[2][i]];
    state[1][i] ^= temp ^ tmp;

    tmp = AXtime[state[2][i] ^ state[3][i]];
    state[2][i] ^= temp ^ tmp;

    tmp = AXtime[state[3][i] ^ tmp0];
    state[3][i] ^= temp ^ tmp;
  }

  return;
}


/*-------------------------------------------------------------------
 *  Rijndael encryption function.  Takes 16-byte input and creates 
 *  16-byte output (using round keys already derived from 16-byte 
 *  key).
 *-----------------------------------------------------------------*/

void ARijndaelEncrypt( u8 input[16], u8 output[16] )
{
  u8 state[4][4];
  int i, r;

  /* initialise state array from input byte string */
  for (i=0; i<16; i++)
    state[i & 0x3][i>>2] = input[i];

  /* Aadd first round_key */
  AKeyAdd(state, AroundKeys, 0);
  
  /* do lots of full rounds */
  for (r=1; r<=9; r++)
  {
    AByteSub(state);
    AShiftRow(state);
    AMixColumn(state);
    AKeyAdd(state, AroundKeys, r);
  }

  /* final round */
  AByteSub(state);
  AShiftRow(state);
  AKeyAdd(state, AroundKeys, r);

  /* produce output byte string from state array */
  for (i=0; i<16; i++)
  {
    output[i] = state[i & 0x3][i>>2];
  }

  return;
} /* end of function RijndaelEncrypt */

void Af1    ( u8 k[16], u8 rand[16], u8 rand2[16], u8 amf[2], 
             u8 mac_a[8] )
{
  u8 op_c[16];
  u8 temp[16];
  u8 in1[16];
  u8 out1[16];
  u8 rijndaelInput[16];
  u8 i;

  ARijndaelKeySchedule( k );

  AComputeOPc( op_c );

  for (i=0; i<16; i++)
    rijndaelInput[i] = rand[i] ^ op_c[i];
  ARijndaelEncrypt( rijndaelInput, temp );
for (i=0; i<16; i++)
  {
    in1[i]    = rand2[i];
    
  }
  /*for (i=0; i<2; i++)
  {
    in1[i+6]  = amf[i];
    in1[i+14] = amf[i];
  }*/

  /* XOR op_c and in1, rotate by r1=64, and XOR *
   * on the constant c1 (which is all zeroes)   */

  for (i=0; i<16; i++)
    rijndaelInput[(i+8) % 16] = in1[i] ^ op_c[i];

  /* XOR on the value temp computed before */

  for (i=0; i<16; i++)
    rijndaelInput[i] ^= temp[i];
  
  ARijndaelEncrypt( rijndaelInput, out1 );
  for (i=0; i<16; i++)
    out1[i] ^= op_c[i];

  for (i=0; i<8; i++)
    mac_a[i] = out1[i];

  return;
} /* end of function f1 */


namespace ns3 {

/* ... */
NS_LOG_COMPONENT_DEFINE("olduser");


NS_OBJECT_ENSURE_REGISTERED (olduser);


  
/*-------------------------------------------------------------------
 *                            Algorithm f5*
 *-------------------------------------------------------------------
 *
 *  Takes key K and random challenge RAND, and returns resynch
 *  anonymity key AK.
 *
 *-----------------------------------------------------------------*/

/*-------------------------------------------------------------------
 *  Function to compute OPc from OP and K.  Assumes key schedule has
    already been performed.
 *-----------------------------------------------------------------*/

            /*------- Insert your value of OP here -------*/
olduser::~olduser ()
{
}


olduser::olduser ()
{
  for(unsigned int i=0;i<16;i++)
     {
        R2[i] =0x56;
        SQN[i] = 0x55; 
        supi[i]=0xfa;
        key[i] = 0x2f;
     }
  nodes.Create(1);
  uePositionAlloc = CreateObject<ListPositionAllocator> ();
  uePositionAlloc->Add (Vector (30.0, rand()%50,rand()%50));
  uemobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  uemobility.SetPositionAllocator (uePositionAlloc);
  uemobility.Install (nodes);
}
olduser::olduser (u8 t)
{
  for(unsigned int i=0;i<16;i++)
     {
        R2[i] = t;
        SQN[i] = t; 
        supi[i] = 0xfa;
        key[i] = 0x2f;
     }
  nodes.Create(1);
  uePositionAlloc = CreateObject<ListPositionAllocator> ();
  uePositionAlloc->Add (Vector (30.0, 0.0, 0.0));
  uemobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  uemobility.SetPositionAllocator (uePositionAlloc);
  uemobility.Install (nodes);
}
NodeContainer olduser::getNode()
{
  return nodes;
}
void 
olduser::setString(u8 a[],u8 suciSN[])
{
  

  snid = a[0];
  // calculate SUCI NULL Encryption
  for(unsigned int i=0;i<16;i++)
      suci[i] = supi[i];
    
    // printing sUci
    for(unsigned int i=0;i<16;i++)
    {
      
       suciSN[i] = suci[i];
   //   std::cout<<int(suci[i])<<" "<<int(suciSN[i])<<std::endl;
    }
}
bool 
olduser::calculation(u8 xamc[],u8 output[])
{
  // calculate fresh key from two random numbers
 // unsigned int k = R1[15];
  u8 mac_u[8],amf[2];
 
   //calculate fresh key from two random numbers
 
 // std::cout<<"\nFresh Token At ue:\n";
        for(unsigned int i=0;i<16;i++)
        {
 //         std::cout<<int(fresh[i])<<" ";
        }
    u8 p0 = snid; // SN name
    u8 l0 = 0x08; //SN lenght
    u8 l1 = 0x80; //fresh key length
    u8 l2 = 0x80;
    u8 ausf[32],ek[32];
/* K ausf Token calculation */
    u8 FC =  0x6a;
    calculateKausf(FC,p0,l0,R2,l1,key,l2,ausf); // key is used as B not a part input text
 //   std::cout<<"\nAUSF Token At UE:\n";
    for(unsigned int i=0;i<32;i++)
    {
 //     std::cout<<int(ausf[i])<<" ";
    }
/* K seaf Token Calculation */

   FC = 0x6c;
  calculateKseaf(FC,p0,l0,ausf,ek);
 // std::cout<<"\nSEAF Token At UE:\n";
    for(unsigned int i=0;i<32;i++)
    {
 //     std::cout<<int(ek[i])<<" ";
    }
/* EK Token calculation */
/* Mac token calculation Verification */
  Af1(key,R2,SQN,amf,mac_u);
  
  //std::cout<<"\nMAC Token At UE:\n";
        for(unsigned int i=0;i<8;i++)
        {
   //       std::cout<<int(mac_u[i])<<" "<<(int(xamc[i]))<<std::endl;
          if((int(mac_u[i])) != (int(xamc[i])) )
          {
              for(unsigned int j=0; j<16; j++)
              {
                 resUE[j]= 0x00;
              }
              return false;
          }
            
        }
         FC = 0x6b;
    
     oldRes(FC,p0,l0,R2,l1,key,l2,resUE);
        for(unsigned int i=0;i<32;i++)
        {
            output[i] = resUE[i];
        }
        return true;
}
TypeId 
olduser::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::olduser")
    .SetParent<Object> ()
    .AddConstructor<Node> ()
    ;
    return tid;
}

		
}
