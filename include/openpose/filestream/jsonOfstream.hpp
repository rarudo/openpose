#ifndef OPENPOSE_FILESTREAM_JSON_OFSTREAM_HPP
#define OPENPOSE_FILESTREAM_JSON_OFSTREAM_HPP

#include <fstream> // std::ofstream
#include <sstream>
#include <openpose/core/common.hpp>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

namespace op
{
    class OP_API JsonOfstream
    {
    public:
        explicit JsonOfstream(const std::string& filePath, const bool humanReadable = true);

        ~JsonOfstream();

        void objectOpen();

        void objectClose();

        void arrayOpen();

        void arrayClose();
        
        void writeFile();
        
        void sendByUdp(unsigned short port, std::string ipAddr);

        void key(const std::string& string);

        template <typename T>
        inline void plainText(const T& value)
        {
            sStream << value;
        }

        inline void comma()
        {
            sStream << ",";
        }

        void enter();

    private:
        const bool mHumanReadable;
        long long mBracesCounter;
        long long mBracketsCounter;
        std::stringstream sStream;
        std::ofstream mOfstream;

        DELETE_COPY(JsonOfstream);
    };
}

#endif // OPENPOSE_FILESTREAM_JSON_OFSTREAM_HPP
