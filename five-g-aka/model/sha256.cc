
#include <cstring>
#include <fstream>
#include<iostream>
#include "sha256.h"
 typedef unsigned char u8;
const unsigned int SHA256::sha256_k[64] = //UL = uint32
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
 
void SHA256::transform(const unsigned char *message, unsigned int block_nb)
{
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    unsigned int i;
    unsigned int j;
    for (i = 0; i < (unsigned int) block_nb; i++) {
        sub_block = message + (i << 6);
        for (j = 0; j < 16; j++) {
            SHA2_PACK32(&sub_block[j << 2], &w[j]);
        }
        for (j = 16; j < 64; j++) {
            w[j] =  SHA256_F4(w[j -  2]) + w[j -  7] + SHA256_F3(w[j - 15]) + w[j - 16];
        }
        for (j = 0; j < 8; j++) {
            wv[j] = m_h[j];
        }
        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
                + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }
        for (j = 0; j < 8; j++) {
            m_h[j] += wv[j];
        }
    }
}
 
void SHA256::init()
{
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}
 
void SHA256::update(const unsigned char *message, unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = SHA224_256_BLOCK_SIZE - m_len;
    rem_len = len < tmp_len ? len : tmp_len;
    memcpy(&m_block[m_len], message, rem_len);
    if (m_len + len < SHA224_256_BLOCK_SIZE) {
        m_len += len;
        return;
    }
    new_len = len - rem_len;
    block_nb = new_len / SHA224_256_BLOCK_SIZE;
    shifted_message = message + rem_len;
    transform(m_block, 1);
    transform(shifted_message, block_nb);
    rem_len = new_len % SHA224_256_BLOCK_SIZE;
    memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
    m_len = rem_len;
    m_tot_len += (block_nb + 1) << 6;
}
 
void SHA256::final(unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    unsigned int i;
    block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9)
                     < (m_len % SHA224_256_BLOCK_SIZE)));
    len_b = (m_tot_len + m_len) << 3;
    pm_len = block_nb << 6;
    memset(m_block + m_len, 0, pm_len - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(len_b, m_block + pm_len - 4);
    transform(m_block, block_nb);
    for (i = 0 ; i < 8; i++) {
        SHA2_UNPACK32(m_h[i], &digest[i << 2]);
    }
}
 
void sha256(unsigned char a[],int len,unsigned char oFirstHash[])
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest,0,SHA256::DIGEST_SIZE);
 
    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update( a, len);
    ctx.final(digest);
 
    for (unsigned int i = 0; i < SHA256::DIGEST_SIZE; i++)
        oFirstHash[i] = digest[i];
    
}
void prefix(u8 key[],u8 newKey[],u8 secondKey[])
{
    unsigned char ipad[16];
    unsigned char opad[16];
    
    for(unsigned int i=0;i<16;i++)
    {
        ipad[i]=0x36;
        newKey[i] = ipad[i]^key[i];
        opad[i]=0x5c;
        secondKey[i]  = opad[i]^key[i]; //step 5
    }
}
void calculateRes(u8 FC,u8 p0,u8 l0,u8 fresh[],u8 l1,u8 key[],u8 l2,u8 hres[32])
{
    unsigned char newKey[16],secondKey[16];
    unsigned char oSecondHash[32],nextInput[48];;
    unsigned char text[52],oFirstHash[32];
    prefix(key,newKey,secondKey);
    text[0] = FC;
    text[1] = p0;
    text[2] = l0;
    for(unsigned int i=0;i<16;i++)
    {
       text[i+3] = fresh[i];
    }
    text[19] = l1;
    for(unsigned int i=0;i<16;i++)
    {
        text[i+20] = key[i];
    }
    text[36] = l2;
    for(unsigned int i=0;i<16;i++)
    {
        text[i+37] = newKey[i];
    }
   //  std::cout<<"\n before hash function :\n";
     for(unsigned int i=0;i<53;i++)
    {
   //     std::cout<<int(text[i])<<"  ";

        //nextInput[i] = oFirstHash[i]; 
    }
    sha256(text,52,oFirstHash);
    // Step 4 Over

    //std::cout<<"\n After first hash function :\n";
    for(unsigned int i=0;i<32;i++)
    {
      //  std::cout<<int(oFirstHash[i])<<"  ";

        nextInput[i] = oFirstHash[i]; 
    }
    for(unsigned int i=0;i<16;i++)
    {
        nextInput[i+32] = secondKey[i]; 
    }
    sha256(nextInput,48,oSecondHash);
    //final op is Ohash
    //std::cout<<"\n Res Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
  //      std::cout<<int(oSecondHash[i])<<"  ";
    }

    sha256(oSecondHash,32,hres);
    //std::cout<<"\n H*Res Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
      //  std::cout<<int(hres[i])<<"  ";
    }
}
void oldRes(u8 FC,u8 p0,u8 l0,u8 fresh[],u8 l1,u8 key[],u8 l2,u8 hres[32])
{
    unsigned char newKey[16],secondKey[16];
    unsigned char nextInput[48];;
    unsigned char text[52],oFirstHash[32];
    prefix(key,newKey,secondKey);
    text[0] = FC;
    text[1] = p0;
    text[2] = l0;
    for(unsigned int i=0;i<16;i++)
    {
       text[i+3] = fresh[i];
    }
    text[19] = l1;
    for(unsigned int i=0;i<16;i++)
    {
        text[i+20] = key[i];
    }
    text[36] = l2;
    for(unsigned int i=0;i<16;i++)
    {
        text[i+37] = newKey[i];
    }
   //  std::cout<<"\n before hash function :\n";
     for(unsigned int i=0;i<53;i++)
    {
   //     std::cout<<int(text[i])<<"  ";

        //nextInput[i] = oFirstHash[i]; 
    }
    sha256(text,52,oFirstHash);
    // Step 4 Over

    //std::cout<<"\n After first hash function :\n";
    for(unsigned int i=0;i<32;i++)
    {
      //  std::cout<<int(oFirstHash[i])<<"  ";

        nextInput[i] = oFirstHash[i]; 
    }
    for(unsigned int i=0;i<16;i++)
    {
        nextInput[i+32] = secondKey[i]; 
    }
    sha256(nextInput,48,hres);
    //final op is Ohash
   // std::cout<<"\n Res Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
 //       std::cout<<int(hres[i])<<"  ";
    }
}
void hashRes(u8 input[],u8 output[])
{
    sha256(input,32,output);
    //std::cout<<"\n H*Res Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
        std::cout<<int(output[i])<<"  ";
    }
}

void calculateKausf(u8 FC,u8 p0,u8 l0,u8 fresh[],u8 l1,u8 key[],u8 l2,u8 ausf[32]){
    unsigned char newKey[16],secondKey[16];
    unsigned char nextInput[48];;
    unsigned char text[52],oFirstHash[32];
    prefix(key,newKey,secondKey);
    text[0] = FC;
    text[1] = p0;
    text[2] = l0;
    for(unsigned int i=0;i<16;i++)
    {
       text[i+3] = fresh[i];
    }
    text[19] = l1;
    
    for(unsigned int i=0;i<16;i++)
    {
        text[i+20] = newKey[i];
    }
   //  std::cout<<"\n before hash function :"<<text<<std::endl;
    sha256(text,35,oFirstHash);
    // Step 4 Over

    //std::cout<<"\n After first hash function :\n";
    for(unsigned int i=0;i<32;i++)
    {
  //      std::cout<<int(oFirstHash[i])<<"  ";

        nextInput[i] = oFirstHash[i]; 
    }
    for(unsigned int i=0;i<16;i++)
    {
        nextInput[i+32] = secondKey[i]; 
    }
    sha256(nextInput,48,ausf);
    //final op is Ohash
  //  std::cout<<"\n ausf Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
     //   std::cout<<int(ausf[i])<<"  ";
       
    }
}
void exor(u8 ip[],u8 op[])
{
    for(unsigned int i=0;i<32;i++)
    {
        op[i/4] = op[i/4]^ip[i];  
    }
}
void calculateKseaf(u8 FC,u8 p0,u8 l0,u8 ausf[],u8 seaf[])
{
    unsigned char ipad[32],newKey[16],secondKey[16];
    unsigned char opad[32],oSecondHash[32],nextInput[48];;
    unsigned char text[52],oFirstHash[32];
    for(unsigned int i=0;i<32;i++)
    {
        ipad[i]=0x36;
        newKey[i] = ipad[i]^ausf[i];
        opad[i]=0x5c;
        secondKey[i]  = opad[i]^ausf[i]; //step 5
    }
    text[0] = FC;
    text[1] = p0;
    text[2] = l0;
    for(unsigned int i=0;i<32;i++)
    {
        text[i+3] = newKey[i];
    }
   //  std::cout<<"\n before hash function :"<<text<<std::endl;
    sha256(text,35,oFirstHash);
    // Step 4 Over

  //  std::cout<<"\n After first hash function :\n";
    for(unsigned int i=0;i<32;i++)
    {
     //   std::cout<<int(oFirstHash[i])<<"  ";

        nextInput[i] = oFirstHash[i]; 
    }
    for(unsigned int i=0;i<32;i++)
    {
        nextInput[i+32] = secondKey[i]; 
    }
    sha256(nextInput,48,oSecondHash);
    //final op is Ohash
//    std::cout<<"\n seaf Token  :\n";
    for(unsigned int i=0;i<32;i++)
    {
  //      std::cout<<int(seaf[i])<<"  ";
       
    }
    exor(oSecondHash,seaf);
   // std::cout<<"\n H*seaf Token  :\n";
    for(unsigned int i=0;i<8;i++)
    {
     //   std::cout<<int(seaf[i])<<"  ";
    }
}
