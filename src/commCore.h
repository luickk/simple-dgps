#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include "ublox/Message.h"
#include "ublox/message/NavPosllh.h"
#include "ublox/frame/UbloxFrame.h"

using AllInMessages = std::tuple<InNavPosllh>;
using InMessage =ublox::Message<
    comms::option::ReadIterator<const std::uint8_t*>,
    comms::option::Handler<Session> // Dispatch to this object
    >;

using Frame = ublox::Frame<InMessage, AllInMessages>;
using SerialPort = boost::asio::serial_port;

bool configureComm(std::string m_device, SerialPort *m_serial);
void readDataToBuffer(SerialPort *m_serial, std::vector<std::uint8_t> *m_inData);
void proccessBufferData(std::vector<std::uint8_t> *m_inData, Frame *m_frame);