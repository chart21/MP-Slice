#include "Share.hpp"
#include "../arch/DATATYPE.h"
#include <stddef.h>
#pragma once
class Protocol
{
Share share_type;
    public:
        virtual void prepare_and(Share &a, Share &b) = 0;
        virtual Share Xor(Share a, Share b) = 0;
        virtual Share complete_and(Share a, Share b) = 0;
        virtual Share* alloc_Share(const size_t length) = 0;
        virtual Share receive_from_SRNG(Share a[],int player_id,size_t length) = 0;
        virtual Share& Not(Share &a) = 0;
        virtual Share public_val(DATATYPE a) = 0;
        virtual void communicate() = 0;
        virtual void prepare_reveal_to_all(Share a) = 0;
        virtual DATATYPE complete_Reveal(Share a) = 0;
};
