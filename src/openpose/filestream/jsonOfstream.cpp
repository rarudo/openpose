#include <openpose/filestream/jsonOfstream.hpp>
#include <iostream>

namespace op
{
    void enterAndTab(std::ofstream& ofstream, std::stringstream& sStream ,const bool humanReadable, const long long bracesCounter, const long long bracketsCounter)
    {
        try
        {
            if (humanReadable)
            {
                ofstream << "\n";
                sStream << "\n";
                for (auto i = 0ll ; i < bracesCounter + bracketsCounter ; i++)
                    ofstream << "\t";
                    sStream << "\t";
            }
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void send(unsigned short port , std::string ipAddr, std::string message) {

        int destSocket;

        /* sockaddr_in 構造体 */
        struct sockaddr_in destSockAddr;

        /* 各種パラメータ */
        std::string toSendText = message;

        /* sockaddr_in 構造体のセット */
        memset(&destSockAddr, 0, sizeof (destSockAddr));
        destSockAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
        destSockAddr.sin_port = htons(port);
        destSockAddr.sin_family = AF_INET;

        /* ソケット生成 */
        destSocket = socket(AF_INET, SOCK_DGRAM, 0);

        /* パケット送出 */
        printf("sending...\n");
        sendto(destSocket, toSendText.c_str(), strlen(toSendText.c_str()) + 1, 0, (const sockaddr*) &destSockAddr, sizeof (destSockAddr));

        /* ソケットの終了 */
        close(destSocket);
    }
    
 

    JsonOfstream::JsonOfstream(const std::string& filePath, const bool humanReadable) :
        mHumanReadable{humanReadable},
        mBracesCounter{0},
        mBracketsCounter{0},
        mOfstream{filePath}
    {
        try
        {
            if (!filePath.empty() && !mOfstream.is_open())
                error("Json file could not be opened.", __LINE__, __FUNCTION__, __FILE__);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    JsonOfstream::~JsonOfstream()
    {
        try
        {
            enterAndTab(mOfstream, sStream ,mHumanReadable, mBracesCounter, mBracketsCounter);

            if (mBracesCounter != 0 || mBracketsCounter != 0)
            {
                std::string errorMessage = "Json file wronly generated";
                if (mBracesCounter != 0)
                    errorMessage += ", number \"{\" != number \"}\": " + std::to_string(mBracesCounter) + ".";
                else if (mBracketsCounter != 0)
                    errorMessage += ", number \"[\" != number \"]\": " + std::to_string(mBracketsCounter) + ".";
                else
                    errorMessage += ".";
                error(errorMessage, __LINE__, __FUNCTION__, __FILE__);
            }
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::objectOpen()
    {
        try
        {
            mBracesCounter++;
            mOfstream << "{";
            sStream << "{";
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::objectClose()
    {
        try
        {
            mBracesCounter--;
            enterAndTab(mOfstream, sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
            mOfstream << "}";
            sStream << "}";
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::arrayOpen()
    {
        try
        {
            mBracketsCounter++;
            mOfstream << "[";
            sStream << "[";
            enterAndTab(mOfstream, sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::arrayClose()
    {
        try
        {
            mBracketsCounter--;
            enterAndTab(mOfstream, sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
            mOfstream << "]";
            sStream << "]";
            send(8800, "192.168.128.28", sStream.str());

        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::key(const std::string& string)
    {
        try
        {
            enterAndTab(mOfstream, sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
            mOfstream << "\"" + string + "\":";
            sStream << "\"" + string + "\":";
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    void JsonOfstream::enter()
    {
        try
        {
            enterAndTab(mOfstream, sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
}
