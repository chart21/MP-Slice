#pragma once
class Share
{
    public:
        virtual Share Xor(Share a, Share b);
        virtual Share* Not(Share a);

};
