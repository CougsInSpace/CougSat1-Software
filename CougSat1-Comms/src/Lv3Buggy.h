#include <stdlib.h>
#include <stdio.h>
typedef unsigned char  u8;
typedef unsigned int  u32;
class Packet
{
private:
    u8 Data[1026]; //Payload is 1024bytes max
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
    for(int i =0;i<1026;i++){
        Data[i]=0;
    }
}

Packet::Packet()
{
    for(int i =0;i<1028;i++){
        Data[i]=0;
    }
}

Packet::~Packet()
{
}

void Packet::SetSender(u32 S){
    if (S<8){
sender = S;     //[7|6|5|4|3|2|1|0] bit ordering
u8 T = Data[0];
T = T >> 3;    //Clear up the sender region
T = T << 3;
T = T|S;       //Copy S into Sender region
Data[0] = T;      //Overwrite 1st byte of Data
    
    }
}

u32 Packet::GetSender(){
u8 T = Data[0];
u32 S = 0;
T = T >> 5;

S = S|T;

return S;
}

void Packet::SetRecipient(u32 R){
///Store sender

if(R > 7){
return;
}
recipient = R;

u8 T = Data[0];
u8 tempsender = 0;
T = T << 5;
T = T >> 5;
tempsender = T; //Store just the sender bits

T = Data[0];
T = T >> 6;
T = T << 3;//Make space for recipient. 
T = T | R;//Place recipient

T = T << 3;//Make space for sender. 
T = T | tempsender; //Place sender

Data[0] = T;

}

u32 Packet::GetRecipient(){
    u8 T = Data[0];
    T = T << 2; //Cut 2 greatest length bits
    T = T >> 5; //Cut sender and place recipient to lowest bit placement

    u32 R = 0;
    R = R|T;
    return R;
}

u32 Packet::GetLength(){
u32 Product = 0;
u8 T = Data[0];

T = T>>6; //Leave only the greatest 2 bits.

Product = T | Product; //Place 2 greatest bits of length into product 
Product = Product << 8; //Push up the greatest two bits of length by 8 bits
Product = Product | Data[1];//OR the first 8 bits of length to product

return Product;
}

void Packet::SetLength(u32 L){
if(L > 1024){
    return;//Data can only be 1024 at most 
}
length = L;

u32 T = L; 
Data[1] = 0; 
Data[1] = Data[1] | L;//OR the lower 8 bits of Length to Data[1]

L = L >> 8; // only greatest two bits remain
L = L << 6; //Place greatest two bits into bits 6 and 7
Data[0] = Data[0] << 2; 
Data[0] = Data[0] >> 2;//Remove old greatest two bits

Data[0] = Data[0] | L;
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
}