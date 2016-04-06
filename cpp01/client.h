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


        /*struct client_t
        {
            int & sockfd;
            client_t(int & _sockfd):sockfd(_sockfd){}
            void operator()() // overloading ()
            {
                Client::recv_loop(sockfd); // needs to be implemented
            }
        };*/  //optional variable list */;

        void update();


    protected:
    private:
        bool is_reg = false;
        int state = 1;
};

#endif // CLIENT_H
