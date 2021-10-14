#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_

namespace Communications {

class Segment
{
private:
    typedef struct segmentFormat {

        const bool bitArray[7][18];
        //session message ID
        //char* sessionMsgID;
        //Flags
        
    }SEGMENTFORMAT;

    // const bool bitArray[7][18];

public:
    Segment(/* args */);
    ~Segment();
};

Segment::Segment(/* args */)
{
    //bitArray[][] = false;
}

Segment::~Segment()
{
}



}


#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_ */