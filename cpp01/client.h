#ifndef CLIENT_H
#define CLIENT_H


class Client
{
    public:
        Client();
        virtual ~Client();

        void cleanup(int sockfd);
        int init();
        void establish_conn(int sockfd, std::string hostname);
        int send_msg(std::string msg, int sockfd);
        std::string recv_msg(int sockfd);
        void recv_loop(int sockfd);
        void recv_looping(int sockfd);
        void registration(int sockfd);

        void update(int sockfd);


    protected:
    private:
        bool is_reg = false;
};

#endif // CLIENT_H
