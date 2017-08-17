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

        void key(const std::string& string);

        template <typename T>
        inline void plainText(const T& value)
        {
            mOfstream << value;
            sStream << value;

        }

        inline void comma()
        {
            mOfstream << ",";
        }

        void enter();

    private:
        const bool mHumanReadable;
        long long mBracesCounter;
        long long mBracketsCounter;
        std::ofstream mOfstream;
        std::stringstream sStream;

        DELETE_COPY(JsonOfstream);
    };
}

#endif // OPENPOSE_FILESTREAM_JSON_OFSTREAM_HPP
