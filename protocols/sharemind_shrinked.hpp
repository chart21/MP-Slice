class Sharemind
{
bool input_srngs;
    public:
Sharemind(bool use_srngs) {input_srngs = use_srngs;}

bool share_SRNG(bool a)
{
    bool s[3]; 
    s[pprev] = getRandomVal(pprev); //sample SRNG with previous player
    s[pnext] = getRandomVal(pnext); //sample SRNG with next player
    s[2] = XOR(s[pprev],s[pnext]); //last share always belongs to player itself
    s[2] = XOR(a,s[2]);
    return s[2];
}

bool receive_share_SRNG(int player)
{
    return getRandomVal(player); // sample SRNG with Player
}

bool public_val(bool a)
{
    return a;
}

bool Not(bool a)
{
   return NOT(a);
}

bool Xor(bool a, bool b)
{
   return XOR(a, b);
}

// create a new sharing of a using SRNGs
void reshare(bool a, bool u[])
{
u[pprev] = getRandomVal(pprev);
u[pnext] = getRandomVal(pnext);
u[2] = XOR(u[pprev],u[pnext]);
u[2] = XOR(a,u[2]);
 
}

void prepare_and(bool &a, bool &b)
{
bool u[3];
bool v[3];
reshare(a,u);
reshare(b,v);
a = u[2];
b = v[2];
sending_args[pnext].sent_elements[sending_rounds][sb] = u[2]; //send share to next player
sending_args[pprev].sent_elements[sending_rounds][sb] = v[2]; //send share to prev player
sb+=1;
}

bool complete_and(bool a, bool b)
{
bool w = AND(a,b);
bool u_p = receiving_args[pprev].received_elements[rounds-1][rb];
bool v_n = receiving_args[pnext].received_elements[rounds-1][rb];
rb+=1;
bool v_i = b; 
w = XOR (w,   XOR ( AND(u_p,v_i) , AND(u_p,v_n) ));
return w;
}

void prepare_reveal_to_all(bool a)
{
    for(int t = 0; t < num_players-1; t++) 
        sending_args[t].sent_elements[sending_rounds][sb] = a; //send share to the other players
    sb += 1;
}    


}

bool complete_Reveal(bool a)
{
bool result = a;
for(int t = 0; t < num_players-1; t++) 
    result = XOR(result,receiving_args[t].received_elements[rounds-1][rb]); //Reconstruct share with received shares from the other players
rb+=1;
return result;
}

bool share_without_SRNG(bool a)
{
    bool s[3]; //last share always belongs to player itself
    s[pprev] = getRandomVal(num_players -1); //sample RNG (not SRNG)
    s[pnext] = getRandomVal(num_players -1);
    s[2] = XOR(s[pprev],s[pnext]);
    s[2] = XOR(a,s[2]);
    sending_args[pprev].sent_elements[sending_rounds][sb] = s[pprev]; // send share to previous player
    sending_args[pnext].sent_elements[sending_rounds][sb] = s[pnext];
    sb+=1;
    return s[2];
}

}
