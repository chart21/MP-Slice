#pragma once
#include "../networking/buffers.h"
void send_()
{
for (int t = 0; t < 2; t++)
{
    /* if(sending_args[t].elements_to_send[sending_args[t].send_rounds] > 0) */
    /* { */
    sending_args[t].total_rounds += 1;
    sending_args[t].send_rounds += 1;
    sending_args[t].elements_to_send.push_back(0);
    /* } */
}
}

void receive_()
{
for (int t = 0; t < 2; t++)
{
    /* if(receiving_args[t].elements_to_rec[receiving_args[t].rec_rounds] > 0) */
    /* { */
    receiving_args[t].total_rounds += 1;
    receiving_args[t].rec_rounds +=1;
    receiving_args[t].elements_to_rec.push_back(0);
    /* } */
}
sockets_received.push_back(0);
}

#if PRE == 1
void pre_send_to_(int player_index)
{
sending_args_pre[player_index].elements_to_send[0] += 1;
/* sending_args_pre[player_index].elements_to_send[sending_args_pre[player_index].send_rounds] += 1; */
}

void pre_receive_from_(int player_index)
{
/* receiving_args_pre[player_index].elements_to_rec[receiving_args_pre[player_index].rec_rounds -1] += 1; */
receiving_args_pre[player_index].elements_to_rec[0] += 1;
}
#endif

void send_to_(int player_index)
{
sending_args[player_index].elements_to_send[sending_args[player_index].send_rounds] += 1;
}

void receive_from_(int player_index)
{
receiving_args[player_index].elements_to_rec[receiving_args[player_index].rec_rounds -1] += 1;
}

void communicate_()
{
    send_();
    receive_();
}

void finalize_(std::string* ips)
{
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not receive from itself
    receiving_args[t].player_count = num_players;
    receiving_args[t].received_elements = new DATATYPE*[receiving_args[t].rec_rounds]; //every thread gets its own pointer array for receiving elements
    
    receiving_args[t].player_id = player_id;
    receiving_args[t].connected_to = t+offset;
    receiving_args[t].ip = ips[t];
    receiving_args[t].hostname = (char*)"hostname";
    receiving_args[t].port = (int) base_port + player_id * (num_players-1) + t; //e.g. P0 receives on base port from P1, P2 on base port + num_players from P0 6000,6002
    //init_srng(t, (t+offset) + player_id); 
}
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not send to itself
    sending_args[t].sent_elements = new DATATYPE*[sending_args[t].send_rounds];
    /* sending_args[t].elements_to_send[0] = 0; //input sharing with SRNGs */ 
    sending_args[t].player_id = player_id;
    sending_args[t].player_count = num_players;
    sending_args[t].connected_to = t+offset;
    sending_args[t].port = (int) base_port + (t+offset) * (num_players -1) + player_id - 1 + offset; //e.g. P0 sends on base port + num_players  for P1, P2 on base port + num_players for P0 (6001,6000)
    sending_args[t].sent_elements[0] = NEW(DATATYPE[sending_args[t].elements_to_send[0]]); // Allocate memory for first round
       
}
rounds = 0;
sending_rounds = 0;
rb = 0;
sb = 0;

}
void init(){
    for(int t=0;t<(num_players-1);t++) {
sending_args[t].elements_to_send[0] = 0;
receiving_args[t].elements_to_rec[0] = 0;
}
}

void finalize_(std::string* ips, receiver_args* ra, sender_args* sa)
{
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not receive from itself
    ra[t].player_count = num_players;

    ra[t].received_elements = new DATATYPE*[ra[t].rec_rounds]; //every thread gets its own pointer array for receiving elements
    
    ra[t].player_id = player_id;
    ra[t].connected_to = t+offset;
    ra[t].ip = ips[t];
    ra[t].hostname = (char*)"hostname";
    ra[t].port = (int) base_port + player_id * (num_players-1) + t; //e.g. P0 receives on base port from P1, P2 on base port + num_players from P0 6000,6002
}
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not send to itself
#if PRE == 1 // only in actual preprocessing phase
    sa[t].send_rounds = 1;
#endif

    sa[t].sent_elements = new DATATYPE*[sa[t].send_rounds];
    /* sending_args[t].elements_to_send[0] = 0; //input sharing with SRNGs */ 
    sa[t].player_id = player_id;
    sa[t].player_count = num_players;
    sa[t].connected_to = t+offset;
    sa[t].port = (int) base_port + (t+offset) * (num_players -1) + player_id - 1 + offset; //e.g. P0 sends on base port + num_players  for P1, P2 on base port + num_players for P0 (6001,6000)
    sa[t].sent_elements[0] = NEW(DATATYPE[sa[t].elements_to_send[0]]); // Allocate memory for first round
    share_buffer[t] = 0;  
}
rounds = 0;
sending_rounds = 0;
rb = 0;
sb = 0;
}




