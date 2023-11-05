#pragma once
#include "Share.hpp"
#include <stddef.h>
template <class T>
class Protocol
{
typedef T share_type;
    public:
        virtual void prepare_and(const T &a, const T &b) = 0;
        virtual T Xor(const T &a, const T &b) = 0;
        virtual T complete_and(const T &a, const T&b) = 0;
        virtual T* alloc_Share(const size_t length) = 0;
        virtual void receive_from_SRNG(T &a,int player_id,size_t length) = 0;
        virtual void Not(const T &a) = 0;
};
