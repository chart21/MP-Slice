#ifndef Network
#define Network

template <typename T>
void send_to(T &data, int playerID);

template <typename T>
void receive_from(T &data, int playerID);

#endif
