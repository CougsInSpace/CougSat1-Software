#include <stdlib.h>
#include <stdio.h>
typedef unsigned char  u8;
typedef unsigned int  u32;

//Remake the program to use big endian. So Vibes 
class Packet
{
private:
    const static u32 Maxsize = 1024 +2;
    u8 Data[Maxsize]; //Payload is 1024bytes max
    u32 length;    //Length is 10bits
    u32 sender;    
    u32 recipient; //Combined Sender and recipient IDs are 3 bytes each
public:
    Packet(u32 sender,u32 recipient,u32 length);
    Packet();
    ~Packet();
    void SetSender(u32 S);
    u32 GetSender();

    void SetRecipient(u32 R);
    u32 GetRecipient();

    void SetLength(u32 L);
    u32 GetLength();

    void OverwriteData(u8 Input,u32 Index);
    u8 GetData(u32 Index);
    void IncrementData(u8 Inout); //Work on it.
    //What form should I expect ascending descending data to be in?
};

Packet::Packet(u32 sender,u32 recipient,u32 length){
    for(int i =0;i<Maxsize;i++){
        Data[i]=0;
    }
}

Packet::Packet()
{
    for(int i =0;i<Maxsize;i++){
        Data[i]=0;
    }
}

/*
Packet::Packet(FrameSource F)
{
    for(int i=0;i<Maxsize & F.loadPayloadData(i);i++)
    {
        Data[i]=F.loadPayloadData(i);
    }
}*/

Packet::~Packet()
{
}

void Packet::SetSender(u32 S){
    if (S>=8){return;}

Data[0] = Data[0] && 31;//Zero out sender in data
S = S <<5;//Push up s to the right position
Data[0] = Data[0] | S;
}//Big endian 

u32 Packet::GetSender(){
u32 S = 0;
S = (Data[0] >> 5) & 0x07;
return S;
} //Big Endian

void Packet::SetRecipient(u32 R){
///check recipient value. 
if(R > 7){
return;
}
Data[0] = Data[0] & 227;
R = R << 2;
Data[0] = Data[0] & R;
}//big endian 

u32 Packet::GetRecipient(){
    u32 R = 0;
    R = (Data[0] >>2) & 0x07;//Get three bits out. 
    return R;
}//big endian

u32 Packet::GetLength(){
u32 Product = 0;
Product = (Data[0]) & 0x03;//Grab lowest two bits. 
Product <<= 8;//Push most important bits up. 
Product |= Data[1];//Grab 8 lowest bits. 
return Product;
}//big endian

void Packet::SetLength(u32 L){
if(L > 1024){
    return;//Data can only be 1024 at most 
}
length = L;
Data[1] = 0;//zero out lowest 8 bits. 
Data[0] = Data[0] & 252;//zero out the 2 most important bits.

Data[1] = Data[1] | L;
L = L >> 8;
Data[0] = Data[0] & L;
//Big endian 
}

void Packet::OverwriteData(u8 Input, u32 Index){
if(Index > length -1){
return;///Error out of bounds
    }
Data[Index + 2] = 0; 
Data[Index + 2] = Input;
}

u8 Packet::GetData(u32 Index){
if(Index >length - 1){
    return 0;
}
return Data[Index + 2];
}

class Testpacket
{
private:
    Packet P;
public:
    Testpacket(/* args */);
    ~Testpacket();
    void Test(u32 recipient,u32 sender,u32 length);
};

Testpacket::Testpacket(/* args */)
{
}

Testpacket::~Testpacket()
{
}

void Testpacket::Test(u32 recipient,u32 sender,u32 length){
    P.SetLength(length);
    P.SetRecipient(recipient);
    P.SetSender(sender);
    u8 C = P.GetData(-2);
    printf("Recipient %d Sender %d Length %d",P.GetRecipient(),P.GetSender(),P.GetLength());
}

int main(){
Testpacket T;
T.Test(2,3,19);

int x = 0x03;
}