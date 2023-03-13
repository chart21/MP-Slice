#include "../protocols/Protocols.h"
class BufferHelper
{
private:
    int num_send_instructions;
    int num_receive_instructions;
public:
BufferHelper()
{
    num_send_instructions = 0;
    num_receive_instructions = 0;
}

template<typename Pr>
void update_Buffers(Pr P)
{
# if SEND_BUFFER > 0
num_send_instructions++;
if( num_send_instructions == SEND_BUFFER)
{
    P.send();
    num_send_instructions = 0;

}
# endif
# if RECEIVE_BUFFER > 0
num_receive_instructions++;
if( num_receive_instructions == RECEIVE_BUFFER)
{
    P.receive();
    num_receive_instructions = 0;
}

# endif

}
};




