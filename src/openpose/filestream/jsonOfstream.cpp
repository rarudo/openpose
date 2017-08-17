#include <openpose/filestream/jsonOfstream.hpp>
#include <iostream>

namespace op
{
    void enterAndTab(std::stringstream& sStream ,const bool humanReadable, const long long bracesCounter, const long long bracketsCounter)
    {
        try
        {
            if (humanReadable)
            {
                sStream << "\n";
                for (auto i = 0ll ; i < bracesCounter + bracketsCounter ; i++)
                    sStream << "\t";
            }
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
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
            enterAndTab(sStream ,mHumanReadable, mBracesCounter, mBracketsCounter);

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
            enterAndTab(sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
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
            sStream << "[";
            enterAndTab(sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
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
            enterAndTab(sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
            sStream << "]";

        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
    
    void JsonOfstream::writeFile()
    {
        mOfstream << sStream.str();
    }

    void JsonOfstream::sendByUdp(unsigned short port, std::string ipAddr) 
    {

        int destSocket;

        struct sockaddr_in destSockAddr;

        std::string toSendText = sStream.str();

        //ソケットのパラメーターを設定
        memset(&destSockAddr, 0, sizeof (destSockAddr));
        destSockAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
        destSockAddr.sin_port = htons(port);
        destSockAddr.sin_family = AF_INET;

        //ソケット作成
        destSocket = socket(AF_INET, SOCK_DGRAM, 0);

        //送信
        sendto(destSocket, toSendText.c_str(), strlen(toSendText.c_str()) + 1, 0, (const sockaddr*) &destSockAddr, sizeof (destSockAddr));

        //終了
        close(destSocket);
    }
    

    void JsonOfstream::key(const std::string& string)
    {
        try
        {
            enterAndTab(sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
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
            enterAndTab(sStream, mHumanReadable, mBracesCounter, mBracketsCounter);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
}
