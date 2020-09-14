#include <boost/asio.hpp>
#include <iostream>
#include <cassert>

#include "commCore.h"

bool configureComm(std::string m_device, SerialPort *m_serial) 
{
    boost::system::error_code ec;
    m_serial.open(m_device, ec);
    if (ec) 
    {
        std::cerr << "ERROR: Failed to open " << m_device << std::endl;
        return false;
    }

    m_serial.set_option(SerialPort::baud_rate(115200));
    m_serial.set_option(SerialPort::character_size(8));
    m_serial.set_option(SerialPort::parity(SerialPort::parity::none));
    m_serial.set_option(SerialPort::stop_bits(SerialPort::stop_bits::one));
    m_serial.set_option(SerialPort::flow_control(SerialPort::flow_control::none));

    return true;
}

void readDataToBuffer(SerialPort *m_serial, std::vector<std::uint8_t> *m_inData) 
{
    boost::array<std::uint8_t, 512> *m_inputBuf;
    m_serial->async_read_some(
    boost::asio::buffer(m_inputBuf),
    [this](const boost::system::error_code& ec, std::size_t bytesCount)
    {
        if (ec == boost::asio::error::operation_aborted) 
        {
            return;
        }

        if (ec) 
        {
            std::cerr << "ERROR: read failed with message: " << ec.message() << std::endl;
            m_io.stop();
            return;
        }

        auto dataBegIter = m_inputBuf.begin();
        auto dataEndIter = dataBegIter + bytesCount;
        m_inData.insert(m_inData.end(), dataBegIter, dataEndIter);
        processInputData();
        performRead();
    });
}
void proccessBufferData(std::vector<std::uint8_t> *m_inData, Frame *m_frame) 
{
    if (!m_inData.empty()) {
        auto consumed = comms::processAllWithDispatch(&m_inData[0], m_inData.size(), m_frame, *this);
        m_inData.erase(m_inData.begin(), m_inData.begin() + consumed);
    } 
}